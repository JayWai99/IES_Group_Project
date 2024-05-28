#ifndef PWM_H
#define PWM_H

#include <stdbool.h>

// LED starts with T/C0 output compare module in non-inverting fast PWM mode by default
void pwm_setup(void);

void pwm_set_status(bool enabled);

// Valid duty cycles are percentages represented as values between 0 and 1
void pwm_set_duty_cycle(float duty_cycle);

// Valid frequencies are between 62 Hz and 60,000 Hz.
void pwm_set_frequency(float frequency);

#endif // PWM_H
