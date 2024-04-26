/* HEADER */
#include "buzzer.h"

/* LIBRARIES */
#include <avr/io.h>         // For Arduino I/O ports
#include <stddef.h>         // For size_t

#include "usart.h"          // For USART functions

/* MACROS */
#define MAX 255             // Maximum size of OCR0A in decimal
#define F_CLK 16000000      // CPU clock frequency

#define BUZ_LO 0.10         // Duty cycle for buzzer low    volume
#define BUZ_MD 0.40         // Duty cycle for buzzer medium volume
#define BUZ_HI 0.90         // Duty cycle for buzzer high   volume

/* PINS */
#define PIN_BUZ PIND3       // Buzzer connected to pin 3 via pin D3

/* CONSTANTS */
static const int prescaler[] = {1, 8, 32, 64, 128, 256, 1024};

/* VARIABLES */
static float duty_cycle;

/* FUNCTIONS */
void set_prescaler_buz(int i);

/* MAIN */
void setup_buzzer(void)
{
    // Buzzer pin output mode
    DDRD |= (1 << PIN_BUZ);

    // Timer/counter fast PWM TOP
    // Requires setting WGMx[0-2] to 111
    TCCR2B |=  (1 << WGM22);
    TCCR2A |=  (1 << WGM21);
    TCCR2A |=  (1 << WGM20);

    // Output compare toggle OCxA on compare match
    // Requires setting COMxA[0-1] to 01
    TCCR2A &= ~(1 << COM2A1);
    TCCR2A |=  (1 << COM2A0);
    
    duty_cycle = BUZ_LO;
}

void set_status_buzzer(bool enabled)
{
    if (enabled)
    {
        // Output compare non-inverting mode
        // Requires setting COMxB[0-1] to 10
        TCCR2A |=  (1 << COM2B1);
        TCCR2A &= ~(1 << COM2B0);
    }
    else
    {
        // Output compare disabled
        // Requires setting COMxB[0-1] to 00
        TCCR2A &= ~(1 << COM2B1);
        TCCR2A &= ~(1 << COM2B0);
    }
}

void set_volume_buzzer(volume_t volume)
{
    switch (volume)
    {
        case VOLUME_LOW:
            duty_cycle = BUZ_LO;
            break;
        case VOLUME_MEDIUM:
            duty_cycle = BUZ_MD;
            break;
        case VOLUME_HIGH:
            duty_cycle = BUZ_HI;
            break;
        default: // This should never happen, but it's good to have a default case just in case.
            LOG_DEBUG_VARIABLE("Invalid volume level", (double) volume);
            break;
    }

    OCR2B = duty_cycle * OCR2A;
}

// Valid frequencies are between 62 Hz and 20,000 Hz.
void set_frequency_buzzer(float frequency)
{
    float top = 0;

    for (size_t i = 0; i < sizeof(prescaler)/sizeof(prescaler[0]); i++)
    {
        top = F_CLK / (prescaler[i] * frequency);

        if (top <= MAX)
        {
            OCR2A = top;
            OCR2B = duty_cycle * top;
            set_prescaler_buz(i);
            return;
        }
    }

    LOG_DEBUG_VARIABLE("Invalid frequency", (double) frequency);
}

void set_prescaler_buz(int i)
{
    switch (i)
    {
        case 0:
            TCCR2B &= ~(1 << CS22);
            TCCR2B &= ~(1 << CS21);
            TCCR2B |=  (1 << CS20);
            break;
        case 1:
            TCCR2B &= ~(1 << CS22);
            TCCR2B |=  (1 << CS21);
            TCCR2B &= ~(1 << CS20);
            break;
        case 2:
            TCCR2B &= ~(1 << CS22);
            TCCR2B |=  (1 << CS21);
            TCCR2B |=  (1 << CS20);
            break;
        case 3:
            TCCR2B |=  (1 << CS22);
            TCCR2B &= ~(1 << CS21);
            TCCR2B &= ~(1 << CS20);
            break;
        case 4:
            TCCR2B |=  (1 << CS22);
            TCCR2B &= ~(1 << CS21);
            TCCR2B |=  (1 << CS20);
            break;
        case 5:
            TCCR2B |=  (1 << CS22);
            TCCR2B |=  (1 << CS21);
            TCCR2B &= ~(1 << CS20);
            break;
        case 6:
            TCCR2B |=  (1 << CS22);
            TCCR2B |=  (1 << CS21);
            TCCR2B |=  (1 << CS20);
            break;
        default:
            TCCR2B &= ~(1 << CS22);
            TCCR2B &= ~(1 << CS21);
            TCCR2B &= ~(1 << CS20);
            break;
    }
}
