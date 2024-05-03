/* MACROS */
#define F_CPU 16000000UL    // Define the CPU frequency
#define BAUD 9600           // Define the baud rate
#define DEBUG 1             // Define the debug mode

/* LIBRARIES */
#include <avr/io.h>
#include <stdlib.h>
#include <stddef.h>         // Standard Definition library for type definitions
#include <math.h>           // For mathematical operations
#include <avr/interrupt.h>  // For interrupt routine
#include <util/delay.h>     // For delay functions

#include "bit.h"            // For bit set and bit clear functions
#include "usart.h"          // For USART
#include "led.h"            // For LED

#define MOTOR_IN1_PIN PIND4
#define MOTOR_IN2_PIN PIND3

/* FUNCTIONS */
void setup(void) {
    usart_init(usart_ubrr(F_CPU, BAUD));
    
    LOG_INFO("Running Setup");

    // Set up the motor pins
    BIT_SET(DDRD, MOTOR_IN1_PIN);
    BIT_SET(DDRD, MOTOR_IN2_PIN);

    BIT_CLEAR(PORTD, MOTOR_IN1_PIN);
    BIT_SET(PORTD, MOTOR_IN2_PIN);

    LOG_DEBUG("Setting up LED");
    setup_led();
}

float bound(float value, float min, float max) {
    return fmaxf(fminf(value, max), min);
}


/* GLOBAL VARIABLES */


/* INTERRUPT SERVICE ROUTINE */

/* MAIN */
int main(void) {
    setup();

    set_brightness_led(BRIGHTNESS_HIGH);
    set_status_led(true);
    set_frequency_led(200.0);

    const brightness_t brightnesses[] = {BRIGHTNESS_LOW, BRIGHTNESS_MEDIUM, BRIGHTNESS_HIGH};

    while (true) {
        LOG_DEBUG("Waiting for input");
        for (size_t i = 0; i < sizeof(brightnesses) / sizeof(brightness_t); i++) {
            set_brightness_led(brightnesses[i]);
            _delay_ms(5000);
        }
    }
}