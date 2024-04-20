#ifndef LED_H
#define LED_H

#include <stdbool.h>

typedef enum {
    BRIGHTNESS_LOW = 1,
    BRIGHTNESS_MEDIUM,
    BRIGHTNESS_HIGH
} brightness_t;

// LED starts with T/C0 output compare module in non-inverting fast PWM mode by default
void setup_led(void);

void set_status_led(bool enabled);

void set_brightness_led(brightness_t brightness);

// This function should set the frequency of the LED.
// Valid frequencies are between 62 Hz and 60,000 Hz.
void set_frequency_led(float frequency);

#endif // LED_H
