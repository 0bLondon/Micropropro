#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "hardware/adc.h"
#include "hardware/regs/rosc.h"
#include "hardware/regs/addressmap.h"

uint32_t rng(void){
    int k , random, added_random_bits = 0;
    volatile uint32_t *rnd_reg=(uint32_t *)(ROSC_BASE + ROSC_RANDOMBIT_OFFSET);
    for (k=0;k<32;++k){
        added_random_bits = added_random_bits << 1;
        uint16_t adc_bit = (0x00000001 & adc_read());
        added_random_bits = added_random_bits + (0x00000001 & adc_bit);
    }
    for(k=0;k<32;k++){
        random =random << 1;
        random=random + (0x00000001 & (*rnd_reg));
    }
    random  = random ^ added_random_bits;
    return random;
}

int main(){

    stdio_init_all();

    sleep_ms(1000);
    adc_init();

    // Make sure GPIO is high-impedance, no pullups etc
    adc_gpio_init(26);
    // Select ADC input 0 (GPIO26)
    adc_select_input(0);
    int i = 0;
    while(i > -1){
        uint32_t rand = rng();
        for(int j = 0; j < 32; j++){
            int bit = 1 & rand;
            rand = rand >> 1;
            printf("%u\n",bit);
        }
        sleep_ms(10);
        i++;
    }
}