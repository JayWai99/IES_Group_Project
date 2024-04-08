#include "led.h"
#include "usart.h"

#define LED_PIN 0 // CHANGE THIS

// This function should initialize the led. This should mostly involve initializing the pins and other register values.
void setup_led(void) {

}

// This function should set the LED to be 
void set_status_led(bool enabled) {
    if (enabled) {
        // Enable the LED to flash with the current brightness and frequency.
    } else {
        // Disable the LED from flashing. The current brightness and frequency should not be discarded.
    }
}

// This function should set the brightness of the LED.
void set_brightness_led(brightness_t brightness) {
    switch (brightness) {
        case BRIGHTNESS_LOW:
            break;
        case BRIGHTNESS_MEDIUM:
            break;
        case BRIGHTNESS_HIGH:
            break;
        default: // This should never happen, but it's good to have a default case just in case.
            LOG_DEBUG_VARIABLE("Invalid brightness level", (double) brightness);
            break;
    }
}

// This function should set the frequency of the LED.
void set_frequency_led(float frequency) {

}
