/* HEADER */
#include "led.h"

/* LIBRARIES */
#include <avr/io.h>         // For Arduino I/O ports
#include <stddef.h>         // For size_t

#include "usart.h"          // For USART functions

/* MACROS */
#define MAX 255             // Maximum size of OCR0A in decimal
#define F_CLK 16000000      // CPU clock frequency

#define LED_LO 0.25         // Duty cycle for LED low    brightness
#define LED_MD 0.50         // Duty cycle for LED medium brightness
#define LED_HI 0.75         // Duty cycle for LED high   brightness

/* PINS */
#define PIN_LED PIND5       // LED connected to pin 5 via pin D5

/* CONSTANTS */
static const int prescaler[] = {1, 8, 64, 256, 1024};

/* VARIABLES */
static float duty_cycle;

/* FUNCTIONS */
void set_prescaler_led(int i);

/* MAIN */
void setup_led(void)
{
    // LED pin output mode
    DDRD |= (1 << PIN_LED);

    // Timer/counter fast PWM TOP
    // Requires setting WGMx[0-2] to 111
    TCCR0B |=  (1 << WGM02);
    TCCR0A |=  (1 << WGM01);
    TCCR0A |=  (1 << WGM00);

    // Output compare toggle OCxA on compare match
    // Requires setting COMxA[0-1] to 01
    TCCR0A &= ~(1 << COM0A1);
    TCCR0A |=  (1 << COM0A0);
}

void set_status_led(bool enabled)
{
    if (enabled)
    {
        // Output compare non-inverting mode
        // Requires setting COMxB[0-1] to 10
        TCCR0A |=  (1 << COM0B1);
        TCCR0A &= ~(1 << COM0B0);
    }
    else
    {
        // Output compare disabled
        // Requires setting COMxB[0-1] to 00
        TCCR0A &= ~(1 << COM0B1);
        TCCR0A &= ~(1 << COM0B0);
    }
}

void set_brightness_led(brightness_t brightness)
{
    switch (brightness)
    {
        case BRIGHTNESS_LOW:
            duty_cycle = LED_LO;
            break;
        case BRIGHTNESS_MEDIUM:
            duty_cycle = LED_MD;
            break;
        case BRIGHTNESS_HIGH:
            duty_cycle = LED_HI;
            break;
        default: // This should never happen, but it's good to have a default case just in case.
            LOG_DEBUG_VARIABLE("Invalid brightness level", (double) brightness);
            break;
    }

    OCR0B = duty_cycle * OCR0A;
}

// Valid frequencies are between 62 Hz and 60,000 Hz.
void set_frequency_led(float frequency)
{
    float top = 0;

    for (size_t i = 0; i < sizeof(prescaler)/sizeof(prescaler[0]); i++)
    {
        top = F_CLK / (prescaler[i] * frequency);

        if (top <= MAX)
        {
            OCR0A = top;
            OCR0B = duty_cycle * top;
            set_prescaler_led(i);
            return;
        }
    }

    LOG_DEBUG_VARIABLE("Invalid frequency", (double) frequency);
}

void set_prescaler_led(int i)
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
