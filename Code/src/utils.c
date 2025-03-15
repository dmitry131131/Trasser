#define F_CPU 1000000UL

#include "utils.h"
#include "custom_delay.h"

void small_outside_blink() {
    Outside_LED_On();
    delay_x_0_1_sec(1);
    Outside_LED_Off();
}