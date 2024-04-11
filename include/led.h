#ifndef LED_H
#define LED_H

#include <stdbool.h>

typedef enum {
    BRIGHTNESS_LOW = 1,
    BRIGHTNESS_MEDIUM,
    BRIGHTNESS_HIGH
} brightness_t;

void setup_led(void);

void set_status_led(bool enabled);

void set_brightness_led(brightness_t brightness, float top);

void set_frequency_led(float frequency);

#endif // LED_H
