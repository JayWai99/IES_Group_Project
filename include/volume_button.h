#ifndef VOLUME_BUTTON_H
#define VOLUME_BUTTON_H

#include <stdbool.h>

void setup_volume_button(void);

bool is_pressed_volume_button(void);
bool is_volume_button_pressed_debounced(void);

#endif // VOLUME_BUTTON_H