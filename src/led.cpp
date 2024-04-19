/* HEADER */
#include "led.h"

/* LIBRARIES */
#include <avr/io.h>         // For Arduino I/O ports
#include <stddef.h>         // For size_t

#include "bit.h"            // For bit manipulation functions
#include "usart.h"          // For USART functions

/* MACROS */
#define MAX 255             // Maximum size of OCR0A in decimal
#define F_CLK 16000000      // CPU clock frequency

#define LED_LO 0.25         // Duty cycle for LED low    brightness
#define LED_MD 0.50         // Duty cycle for LED medium brightness
#define LED_HI 0.75         // Duty cycle for LED high   brightness

/* PINS */
#define PIN_LED PIND5       // LED connected to pin 5 via pin D5
#define PIN_IDK PIND6       // LED connected to pin 6 via pin D6

/* FUNCTIONS */
float find_top(float frequency);
void set_prescaler(int i);

/* CONSTANTS */
static const int prescalers[] = {1, 8, 64, 256, 1024};

/* VARIABLES */
static float duty_cycle;

// This function initialises the LED by setting the associated DDR to output mode
void setup_led(void)
{
    duty_cycle = 0;
    // LED pin to output mode
    DDRD |= (1 << PIN_LED);
    DDRD |= (1 << PIN_IDK);

    // Timer/counter to fast PWM TOP
    // Requires setting WGMx[0-2] to 111
    TCCR0B |=  (1 << WGM02);
    TCCR0A |=  (1 << WGM01);
    TCCR0A |=  (1 << WGM00);

    // Output compare to toggle OCxA on compare match
    // Requires setting COMxA[0-1] to 01
    TCCR0A &= ~(1 << COM0A1);
    TCCR0A |=  (1 << COM0A0);
}

// This function should set the LED to be 
void set_status_led(bool enabled)
{
    if (enabled)
    {
        // Output compare to non-inverting mode
        // Requires setting COMxB[0-1] to 10
        TCCR0A |=  (1 << COM0B1);
        TCCR0A &= ~(1 << COM0B0);
    }
    else
    {
        // Output compare to non-inverting mode
        // Requires setting COMxB[0-1] to 10
        TCCR0A &= ~(1 << COM0B1);
        TCCR0A &= ~(1 << COM0B0);
    }
}

// This function sets the brightness of the LED
void set_brightness_led(brightness_t brightness)
{
    switch (brightness)
    {
        case BRIGHTNESS_LOW:
            duty_cycle = LED_LO;
            // OCR0B = OCR0A*(LED_LO/100.0);
            break;
        case BRIGHTNESS_MEDIUM:
            duty_cycle = LED_MD;
            // OCR0B = OCR0A*(LED_MD/100.0);
            break;
        case BRIGHTNESS_HIGH:
            duty_cycle = LED_HI;
            // OCR0B = OCR0A*(LED_HI/100.0);
            break;
        default: // This should never happen, but it's good to have a default case just in case.
            LOG_DEBUG_VARIABLE("Invalid brightness level", (double) brightness);
            break;
    }

    OCR0B = duty_cycle * OCR0A;
}

// This function should set the frequency of the LED.
// Valid frequencies are between 62 Hz and 60,000 Hz.
void set_frequency_led(float frequency)
{
    OCR0A = find_top(frequency);
    OCR0B = duty_cycle * OCR0A;
}

// Find top
float find_top(float frequency)
{
    float top = 0;

    for (size_t i = 0; i < sizeof(prescalers)/sizeof(prescalers[0]); i++)
    {
        top = F_CLK / (prescalers[i] * frequency);

        if (top <= MAX)
        {
            set_prescaler(i);
            return top;
        }
    }

    LOG_DEBUG_VARIABLE("Invalid frequency", (double) frequency);
    return 0;
}

// Sets the prescaler
void set_prescaler(int i)
{
    switch (i)
    {
        case 0:
            TCCR0B &= ~(1 << CS02);
            TCCR0B &= ~(1 << CS01);
            TCCR0B |=  (1 << CS00);
            break;
        case 1:
            TCCR0B &= ~(1 << CS02);
            TCCR0B |=  (1 << CS01);
            TCCR0B &= ~(1 << CS00);
            break;
        case 2:
            TCCR0B &= ~(1 << CS02);
            TCCR0B |=  (1 << CS01);
            TCCR0B |=  (1 << CS00);
            break;
        case 3:
            TCCR0B |=  (1 << CS02);
            TCCR0B &= ~(1 << CS01);
            TCCR0B &= ~(1 << CS00);
            break;
        case 4:
            TCCR0B |=  (1 << CS02);
            TCCR0B &= ~(1 << CS01);
            TCCR0B |=  (1 << CS00);
            break;
        default:
            TCCR0B &= ~(1 << CS02);
            TCCR0B &= ~(1 << CS01);
            TCCR0B &= ~(1 << CS00);
            break;
    }
}
