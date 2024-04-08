#ifndef POWER_BUTTON_H
#define POWER_BUTTON_H

#include <stdbool.h>

void setup_power_button(void);

bool is_pressed_power_button(void);
bool is_pressed_power_button_debounced(void);
bool power_button_state(bool button_state);

#endif // LED_BUTTON_H