/* LIBRARIES */
#include <avr/io.h>
#include <util/delay.h>     // For the delay function
#include <stdlib.h>
#include <stddef.h>         // Standard Definition library for type definitions
#include <math.h>           // For mathematical operations
#include <avr/interrupt.h>  // For interrupt routine

#include "bit.h"            // For bit set and bit clear functions
#include "usart.h"          // For USART

/* MACROS */
#define F_CPU 16000000UL    // Gives "util/delay.h" the MCU's clock cycles
#define __DELAY_BACKWARD_COMPATIBLE__


/* FUNCTIONS */


/* GLOBAL VARIABLES */


/* INTERRUPT SERVICE ROUTINE */

/* MAIN */
int main0(void)
{
    //
    usart_init(8);  // 103-9600 bps; 8-115200


    sei();
}