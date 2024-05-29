#include <avr/io.h>
#include <util/delay.h>
#include "bit.h"

#define VOL_BUTTON_PIN PINB4 
#define DEBOUNCE_DELAY 100

static bool last_state = false;

// This function should initialize the volume button. This should mostly involve initializing the pins and other register values.
void setup_volume_button(void) {
    // Set the push button as output and initialise it to logic high
    BIT_SET(DDRB, VOL_BUTTON_PIN);
    BIT_SET(PORTB, VOL_BUTTON_PIN);

    // Set the push button as input
    BIT_CLEAR(DDRB, VOL_BUTTON_PIN);
}

bool volume_button_read(void){
    return BIT_READ(PINB, VOL_BUTTON_PIN) == 0;
}

// This function should return true if the led button is pressed and false otherwise.
// This function should be debounced. If the button status isn't changed from the last known value it should return immediately.
// Otherwise, it should wait so the output can be debounced (verified) and then return the new button status.
bool is_pressed_volume_button(void) {
    bool state = volume_button_read();
    if (state == last_state) {
        last_state = state;
        return false;
    }
    last_state = state;

    if (state) {
        _delay_us(DEBOUNCE_DELAY);
        return volume_button_read();
    }
    return false;
}
