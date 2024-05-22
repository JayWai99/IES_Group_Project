#ifndef BUZZER_H
#define BUZZER_H

#include "usart.h"

typedef enum {
    VOLUME_LOW = 1,
    VOLUME_MEDIUM,
    VOLUME_HIGH
} volume_t;

static volume_t next_volume(volume_t volume) {
    switch (volume) {
        case VOLUME_LOW:
            return VOLUME_MEDIUM;
        case VOLUME_MEDIUM:
            return VOLUME_HIGH;
        case VOLUME_HIGH:
            return VOLUME_LOW;
        default: // This should never happen, but it's good to have a default case just in case.
            LOG_DEBUG_VARIABLE("Invalid volume level", (double) volume);
            return VOLUME_LOW;
    }
}

// Buzzer starts with T/C2 output compare module in non-inverting fast PWM mode by default
void setup_buzzer(void);

void set_status_buzzer(bool enabled);

void set_volume_buzzer(volume_t volume);

// Valid frequencies are between 62 Hz and 20,000 Hz.
void set_frequency_buzzer(float frequency);

#endif // BUZZER_H
