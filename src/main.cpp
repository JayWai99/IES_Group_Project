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
#include <avr/delay.h>

#include "bit.h"            // For bit set and bit clear functions
#include "usart.h"          // For USART
#include "pwm.h"            // For PWM on TC0


/* FUNCTIONS */
void setup(void) {
    usart_init(usart_ubrr(F_CPU, BAUD));
    
    LOG_INFO("Running Setup");

    LOG_DEBUG("Setting up LED");
    pwm_setup();

    LOG_DEBUG("Setting up Timer1");
    BIT_CLEAR(DDRB, PINB0); // Set pin B0 as input
    BIT_SET(TCCR1B, CS10); // Set prescaler to 1
    BIT_SET(TCCR1B, ICNC1); // Enable noise canceler
    BIT_SET(TCCR1B, ICES1); // Set input capture edge to rising edge
    BIT_SET(TIMSK1, ICIE1); // Enable input capture interrupt
    sei();
}


/* GLOBAL VARIABLES */
volatile uint16_t overflow_counter = 0;
volatile uint32_t last_rising_edge = 0;
volatile uint32_t last_falling_edge = 0;

/* INTERRUPT SERVICE ROUTINE */
ISR(TIMER1_OVF_vect) {
    overflow_counter += 1;
}

ISR(TIMER1_CAPT_vect) {
    if (BIT_READ(TCCR1B, ICES1)) {
        last_rising_edge = (overflow_counter << 16) + ICR1;
        overflow_counter = 0;
        TCNT1 = 0;
    } else {
        last_falling_edge = (overflow_counter << 16) + ICR1;
    }


    BIT_FLIP(TCCR1B, ICES1); // Toggle input capture edge
}

/* MAIN */
int main(void) {
    setup();

    uint8_t duty_cycle = 10;

    pwm_set_frequency(1000
    
    );
    pwm_set_status(true);
    while (true) {
        pwm_set_duty_cycle(duty_cycle / 100.0f);
        duty_cycle = (duty_cycle + 10) % 100;

        usart_teleplot("real", duty_cycle / 100.0f);

        double period = last_rising_edge / 16000000.0f;
        double t_on = last_falling_edge / 16000000.0f;
        usart_teleplot("period", period);
        usart_teleplot("duty_cycle", t_on / period);

        _delay_ms(100);
    }
}