/* HEADER */
#include "photoresistor.h"

/* LIBRARIES */
#include <avr/io.h>         // For Arduino I/O ports
#include <avr/interrupt.h>  // For Arduino interrupts
#include <util/delay.h>     // For Utility delay
#include <stddef.h>         // For size_t

#include "led.h"            // For LED

#include "usart.h"          // For USART functions

/* PINS */
#define PIN_AC_POS PIND6       // AN0 connected to pin  6 via pin C6
#define PIN_AC_NEG PIND7       // AN1 connected to pin  7 via pin C7

/* CONTENTS */
void setup_photoresistor(void)
{
    // AC pins input mode
    DDRD   &= ~(1 << PIN_AC_POS);
    DDRD   &= ~(1 << PIN_AC_NEG);

    // Analogue comparator positive input AIN0 PIND6
    ACSR   &= ~(1 << ACBG);
    
    // Analogue comparator negative input AIN1 PIND7
    ADCSRB &= ~(1 << ACME);
}

bool read_status_photoresistor(void)
{
    return (1 & (ACSR >> ACO));
}