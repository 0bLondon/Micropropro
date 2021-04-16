from machine import Pin
from rp2 import PIO, StateMachine, asm_pio
from time import sleep
import math

MAX_OUT = 65526
MIN_OUT = 0

DOMAIN = 10
def sigmoid(x):
    return 1 / (1 + math.exp(-x))

def pwm_val(frac):
    x = sigmoid(((frac-.5)/.5)*DOMAIN)
    return int(x * (MAX_OUT-MIN_OUT)) + MIN_OUT

@asm_pio(sideset_init=PIO.OUT_LOW)
def pwm_prog():
    # fmt: off
    pull(noblock) .side(0)
    mov(x, osr)  # Keep most recent pull data stashed in X, for recycling by noblock
    mov(y, isr)  # ISR must be preloaded with PWM count max
    label("pwmloop")
    jmp(x_not_y, "skip")
    nop()         .side(1)
    label("skip")
    jmp(y_dec, "pwmloop")
    # fmt: on


max_count=(1 << 16) - 1

pin = 25
sm = StateMachine(0, pwm_prog, freq=10000000, sideset_base=Pin(pin))
sm.put(max_count)
sm.exec("pull()")
sm.exec("mov(isr, osr)")
sm.active(1)

m = 256
total_time = 5
step_time = total_time/m
while True:
    print("loopy")
    for i in range(m+1):
        value = pwm_val(i/m)
        sm.put(value)
        sleep(step_time)
    for i in range(m, -1, -1):
        value = pwm_val(i/m)
        sm.put(value)
        sleep(step_time)