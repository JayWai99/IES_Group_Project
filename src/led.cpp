/* HEADER */
#include "led.h"

/* LIBRARIES */
#include <avr/io.h>         // For Arduino I/O ports

#include "bit.h"            // For bit manipulation functions
#include "usart.h"          // For USART functions

/* MACROS */
#define MAX 255             // Maximum size of OCR0A in decimal
#define F_CLK 16000000      // CPU clock frequency

#define LED_LO 25           // Duty cycle for LED low    brightness
#define LED_MD 50           // Duty cycle for LED medium brightness
#define LED_HI 75           // Duty cycle for LED high   brightness

/* PINS */
#define PIN_LED PIND5       // LED connected to pin 5 via pin D5

/* CONSTANTS */
static const int prescaler[] = {1, 8, 64, 256, 1024};

/* VARIABLES */


// This function initialises the LED by setting the associated DDR to output mode
void setup_led(void)
{
    // LED pin to output mode
    DDRD |= (1 << PIN_LED);

    // Timer/counter to fast PWM TOP
    // Requires setting WGMx[0-2] to 111
    TCCR0B |=  (1 << WGM02);
    TCCR0A |=  (1 << WGM01);
    TCCR0A |=  (1 << WGM00);

    // Output compare to toggle OCxA on compare match
    // Requires setting COMxA[0-1] to 01
    TCCR0A &= ~(1 << COM0A1);
    TCCR0A |=  (1 << COM0A0);
    // Output compare to non-inverting mode
    // Requires setting COMxB[0-1] to 10
    TCCR0A |=  (1 << COM0B1);
    TCCR0A &= ~(1 << COM0B0);
}

// This function should set the LED to be 
void set_status_led(bool enabled)
{
    if (enabled)
    {
        // Enable the LED to flash with the current brightness and frequency.
    }
    else
    {
        // Disable the LED from flashing. The current brightness and frequency should not be discarded.
    }
}

// This function sets the brightness of the LED
void set_brightness_led(brightness_t brightness)
{
    switch (brightness)
    {
        case BRIGHTNESS_LOW:
            OCR0B = OCR0A*((float)LED_LO/(float)100);
            break;
        case BRIGHTNESS_MEDIUM:
            OCR0B = OCR0A*((float)LED_MD/(float)100);
            break;
        case BRIGHTNESS_HIGH:
            OCR0B = OCR0A*((float)LED_HI/(float)100);
            break;
        default: // This should never happen, but it's good to have a default case just in case.
            LOG_DEBUG_VARIABLE("Invalid brightness level", (double) brightness);
            break;
    }
}

// This function should set the frequency of the LED.
void set_frequency_led(float frequency) {

}
