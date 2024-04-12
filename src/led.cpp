/* HEADER */
#include "led.h"

/* LIBRARIES */
#include <avr/io.h>         // For Arduino I/O ports

#include "bit.h"            // For bit manipulation functions
#include "usart.h"          // For USART functions

/* MACROS */
#define PIN_LED PIND5       // LED connected to pin 5 via pin D5

// This function initialises the LED by setting the associated DDR to output mode
void setup_led(void)
{
    BIT_SET(DDRD, PIN_LED);    // LED pin to output mode
}

// This function should set the LED to be 
void set_status_led(bool enabled) {
    if (enabled) {
        // Enable the LED to flash with the current brightness and frequency.
    } else {
        // Disable the LED from flashing. The current brightness and frequency should not be discarded.
    }
}

// This function sets the brightness of the LED
void set_brightness_led(brightness_t brightness, float top)
{
    switch (brightness)
    {
        case BRIGHTNESS_LOW:
            OCR1B = top*(LED_LO/100);
            break;
        case BRIGHTNESS_MEDIUM:
            OCR1B = top*(LED_MD/100);
            break;
        case BRIGHTNESS_HIGH:
            OCR1B = top*(LED_HI/100);
            break;
        default: // This should never happen, but it's good to have a default case just in case.
            LOG_DEBUG_VARIABLE("Invalid brightness level", (double) brightness);
            break;
    }
}

// This function should set the frequency of the LED.
void set_frequency_led(float frequency) {

}
