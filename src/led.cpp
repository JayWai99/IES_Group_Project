/* HEADER */
#include "led.h"

/* LIBRARIES */
#include <avr/io.h>         // For Arduino I/O ports

/* PINS */
#define PIN_LED PIND5       // LED connected to pin 5 via pin D5

/* CONTENTS */
void setup_led(void)
{
    // LED pin output mode
    DDRD  |=  (1 << PIN_LED);
    
    // Turn off LED by default
    PORTD &= ~(1 << PIN_LED);
}

void set_status_led(bool enabled)
{
    if (enabled)
    {
        PORTD |=  (1 << PIN_LED);
    }
    else
    {
        PORTD &= ~(1 << PIN_LED);
    }
}