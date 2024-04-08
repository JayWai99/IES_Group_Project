#include "led_button.h"

#define LED_BUTTON_PIN 0 // CHANGE THIS

// This function should initialize the led button. This should mostly involve initializing the pins and other register values.
void setup_led_button(void) {

}

// This function should return true if the led button is pressed and false otherwise.
// This function should be debounced. If the button status isn't changed from the last known value it should return immediately.
// Otherwise, it should wait so the output can be debounced (verified) and then return the new button status.
bool is_pressed_led_button(void) {
    return false;
}