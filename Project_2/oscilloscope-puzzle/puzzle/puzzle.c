#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "pico/stdlib.h"
#include "pico/time.h"
#include "hardware/gpio.h"
#include "hardware/adc.h"

#define LED_PIN 25
#define GP4 4
#define GP12 12
#define GP18 18
#define GP19 19
#define GP20 20
#define GP21 21
#define GP6 6
#define GP8 8
#define GP14 14
#define SEQ_LENGTH 10

int PHASE_1_SEQ[] = {1, 0, 0, 0, 0, 0, 0, 0, 0, 0};
int PHASE_2_SEQ[] = {1, 0, 1, 0, 0, 0, 0, 0, 0, 0};
int PHASE_3_SEQ[] = {1, 0, 1, 0, 1, 0, 0, 0, 0, 0};
int PHASE_4_SEQ[] = {1, 0, 1, 0, 1, 0, 1, 0, 1, 0};

int seq_pos = 0;
int *phase_led = PHASE_1_SEQ;

static char *alpha[] = {
    ".-",   //A
    "-...", //B
    "-.-.", //C
    "-..",  //D
    ".",    //E
    "..-.", //F
    "--.",  //G
    "....", //H
    "..",   //I
    ".---", //J
    "-.-",  //K
    ".-..", //L
    "--",   //M
    "-.",   //N
    "---",  //O
    ".--.", //P
    "--.-", //Q
    ".-.",  //R
    "...",  //S
    "-",    //T
    "..-",  //U
    "...-", //V
    ".--",  //W
    "-..-", //X
    "-.--", //Y
    "--..", //Z
};

static char *num[] = {
    "-----", //0
    ".----", //1
    "..---", //2
    "...--", //3
    "....-", //4
    ".....", //5
    "-....", //6
    "--...", //7
    "---..", //8
    "----.", //9
};

bool morse_to_signal(int c, int pin){
    if(c == '-'){
        gpio_put(pin, 1);
        sleep_ms(20);
        gpio_put(pin, 0);
    }
    else if(c == '.'){
        gpio_put(pin, 1);
        sleep_ms(1);
        gpio_put(pin, 0);        
    }
    sleep_ms(20);
    return true;
}

bool letter_to_out(int out, int pin){
    char *encoding = alpha[out];
    while (*encoding != '\0'){
        morse_to_signal(*encoding, pin);
        encoding++;
    }
    return true;
}
bool num_to_out(int out, int pin){
    char *encoding = num[out];
    while (*encoding != '\0'){
        morse_to_signal(*encoding, pin);
        encoding++;
    }
    return true;
}

void encode_morse(const char *s, int length, int pin){
    for(int i  = 0; i < length;++i){
        char ch = s[i];
        if(isalpha(ch)){
            ch = toupper(ch);
            letter_to_out(ch - 'A', pin);    
        } 
        else if(isdigit(ch)){
            num_to_out(ch - '0', pin);
        }
        sleep_ms(50);
    }
}
bool LEDCallback(struct repeating_timer *t) {
    gpio_put(LED_PIN, phase_led[seq_pos % SEQ_LENGTH]);
    if (seq_pos % SEQ_LENGTH == 0) seq_pos = 0;
    seq_pos++;
    return true;
}

bool gp12_down = false;
bool check_g12_down(struct repeating_timer *t){
    gp12_down = (gpio_get(GP12) == 0);
    return true;
}
void phase1(){
    struct repeating_timer p1_timer;

    gpio_init(GP4);
    gpio_set_dir(GP4, GPIO_OUT);

    gpio_init(GP12);
    gpio_set_dir(GP12, GPIO_IN);
    gpio_set_pulls(GP12,true, false);
    add_repeating_timer_ms(-100, check_g12_down, NULL, &p1_timer);
    
    while(!gp12_down) {
        encode_morse("GP12 DOWN", 9, GP4);
        sleep_ms(1000);
    }
    gpio_set_pulls(GP12,false, false);
    cancel_repeating_timer(&p1_timer);
}


bool encoded_5 = false;
bool check_encode_5(struct repeating_timer *t){
    encoded_5 = ((gpio_get(GP18) == 1) 
             && (gpio_get(GP19) == 0) 
             && (gpio_get(GP20) == 1)
             && (gpio_get(GP21) == 0))
             || ((gpio_get(GP18) == 0) 
             && (gpio_get(GP19) == 1) 
             && (gpio_get(GP20) == 0)
             && (gpio_get(GP21) == 1));
    return true;
}
void phase2(){
    struct repeating_timer p2_timer;

    gpio_init(GP6);
    gpio_init(GP8);
    gpio_set_dir(GP6, GPIO_OUT);
    gpio_set_dir(GP8, GPIO_OUT);

    gpio_init(GP18);
    gpio_set_dir(GP18, GPIO_IN);
    gpio_set_pulls(GP18,true, false);
    gpio_init(GP19);
    gpio_set_dir(GP19, GPIO_IN);
    gpio_set_pulls(GP19,true, false);
    gpio_init(GP20);
    gpio_set_dir(GP20, GPIO_IN);
    gpio_set_pulls(GP20,true, false);
    gpio_init(GP21);
    gpio_set_dir(GP21, GPIO_IN);
    gpio_set_pulls(GP21,true, false);

    add_repeating_timer_ms(-100, check_encode_5, NULL, &p2_timer);

    while(!encoded_5) {  
        encode_morse("4 PINS HIGH", 12, GP6);
        encode_morse("5 BASE 2", 13, GP8);
        sleep_ms(1000);
    }
    gpio_set_pulls(GP18,false, false);
    gpio_set_pulls(GP19,false, false);
    gpio_set_pulls(GP20,false, false);
    gpio_set_pulls(GP21,false, false);
    cancel_repeating_timer(&p2_timer);
}

bool voltage_match = false;
const float conversion_factor = 3.3f / (1 << 12);
uint16_t result;
float voltage;
uint16_t raw_vals[10];
int val_cnt = 0;
bool check_voltage(struct repeating_timer *t){
    result = adc_read();
    raw_vals[val_cnt % 10] = result;
    if(val_cnt % 10 == 0){
        float sum = 0;
        for (int i = 0; i < 10; i++){
            sum += raw_vals[i];
        }
        voltage = sum * conversion_factor / 10;
        voltage_match = (voltage > 2.3f) && (voltage < 2.7f);
        val_cnt = 0;
    }
    val_cnt++;
    return true;
}
void phase3(){
    struct repeating_timer p3_timer;
    adc_gpio_init(26);
    // Select ADC input 0 (GPIO26)
    adc_select_input(0);
    add_repeating_timer_ms(-1, check_voltage, NULL, &p3_timer);
    
    gpio_init(GP14);
    gpio_set_dir(GP14,GPIO_OUT);
    while(!voltage_match) {
        
        encode_morse("HALF 1010 V", 12, GP14);
        encode_morse("IN ADC 0", 9, GP4);
        sleep_ms(1000);
    }
    cancel_repeating_timer(&p3_timer);
}

void solved(){
    printf("PUZZLE SOLVED! Secret key: aae2de846b8538d5a13fd0db88d86bb7d80fb8e63157ead35966723365976cb2\n");
    while(1);
}

struct repeating_timer ledtimer;
int main(){
    stdio_init_all();

    adc_init();
    
    gpio_init(LED_PIN);
    gpio_set_dir(LED_PIN, GPIO_OUT);
    add_repeating_timer_ms(-100, LEDCallback, NULL, &ledtimer);

    phase1();
    phase_led = PHASE_2_SEQ;
    phase2();
    phase_led = PHASE_3_SEQ;
    phase3();
    phase_led = PHASE_4_SEQ;
    solved();
}