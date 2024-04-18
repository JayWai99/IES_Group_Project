/* MACROS */
#define F_CPU 16000000UL    // Define the CPU frequency
#define BAUD 9600           // Define the baud rate
#define DEBUG 1             // Define the debug mode
#define __DELAY_BACKWARD_COMPATIBLE__

/* LIBRARIES */
#include <avr/io.h>
#include <stdlib.h>
#include <stddef.h>         // Standard Definition library for type definitions
#include <math.h>           // For mathematical operations
#include <avr/interrupt.h>  // For interrupt routine
#include <util/delay.h>     // For delay functions

#include "bit.h"            // For bit set and bit clear functions
#include "usart.h"          // For USART

// 343 m/s = 343 * 100 cm/s = 34300 cm/s
// 34300 cm/s = 34300 / 1000 cm/ms = 34.3 cm/ms
// 34.3 cm/ms = 34.3 / 1000 cm/us = 0.0343 cm/us
#define SPEED_OF_SOUND_CM_US (343.0 * 100.0 / 1000.0 / 1000.0) // Speed of sound in cm/us
// 13503.9 in/s = 13503.9 / 1000 in/ms = 13.5039 in/ms
// 13.5039 in/ms = 13.5039 / 1000 in/us = 0.0135039 in/us
#define SPEED_OF_SOUND_IN_US (13503.9 / 1000 / 1000) // Speed of sound in in/us



#define ULTRASONIC_TRIGGER_PIN PD7
#define ULTRASONIC_ECHO_PIN PD2
#define BUTTON_PIN PD3

/* FUNCTIONS */
void setup(void) {
    usart_init(usart_ubrr(F_CPU, BAUD));
    
    LOG_INFO("Running Setup");

    LOG_DEBUG("Setting up ultrasonic sensor");
    // Set trigger pin as output
    BIT_SET(DDRD, ULTRASONIC_TRIGGER_PIN);
    // Set echo pin as input
    BIT_CLEAR(DDRD, ULTRASONIC_ECHO_PIN);

    BIT_SET(EIMSK, INT0); // Enable INT0
    BIT_SET(EICRA, ISC00); // Trigger on any logical change

    LOG_INFO("Setup 1MHz Interrupt");
    // Set Timer0 to CTC mode (0b010)
    BIT_CLEAR(TCCR0B, WGM02);
    BIT_SET(TCCR0A, WGM01);
    BIT_CLEAR(TCCR0A, WGM00);
    // // Set prescaler to 1 (0b001)
    BIT_CLEAR(TCCR0B, CS02);
    BIT_SET(TCCR0B, CS01);
    BIT_CLEAR(TCCR0B, CS00);
    // Set TOP value for 1MHz (16MHz / 16 = 1MHz)
    OCR0A = 20;
    TCNT0 = 0; // Reset Timer0 counter
    // Enable Timer0 interrupt
    BIT_SET(TIMSK0, OCIE0A);


    LOG_DEBUG("Setting up button");
    // Set button pin as input
    BIT_CLEAR(DDRD, BUTTON_PIN);
    // Enable pull-up resistor
    BIT_SET(PORTD, BUTTON_PIN);

    BIT_SET(EIMSK, INT1); // Enable INT1
    BIT_SET(EICRA, ISC10); // Trigger on any logical change

    sei(); // Enable global interrupts
}

bool is_pressed_power_button(void) {
    return BIT_READ(PIND, BUTTON_PIN) == 0;
}

bool is_pressed_power_button_debounced(void) {
    if (is_pressed_power_button()) {
        _delay_us(20);
        return is_pressed_power_button();
    }
    return false;
}

/* GLOBAL VARIABLES */
volatile static uint32_t duration = 0;
volatile static bool is_echoing = false;
volatile static bool is_button_pressed = false;
volatile static bool button_state = false;

ISR(TIMER0_COMPA_vect) {
    duration++;
}

/* INTERRUPT SERVICE ROUTINE */
ISR(INT0_vect) {
    if (BIT_READ(PIND, ULTRASONIC_ECHO_PIN)) {
        is_echoing = true;
    } else {
        is_echoing = false;
    }
}

ISR(INT1_vect) {
    if (is_pressed_power_button_debounced()) {
        
        button_state = !button_state;
    } else {
        is_button_pressed = false;
    }
}

/* MAIN */
int main(void) {
    setup();

    button_state = false;

    while (true) {
        if (button_state) {
            continue;
        }

        duration = 0;
        is_echoing = true;
        TCNT0 = 0; // Reset Timer0 counter

        // Send a 10us pulse to the trigger pin
        BIT_SET(PORTD, ULTRASONIC_TRIGGER_PIN);
        _delay_us(10);
        BIT_CLEAR(PORTD, ULTRASONIC_TRIGGER_PIN);

        BIT_SET(TCCR0B, CS00); // Start Timer0
        // Wait for the echo pin to go low
        while (is_echoing);
        BIT_CLEAR(TCCR0B, CS00); // Stop Timer0

        double rtt = duration * 10.0; // Round trip time in microseconds
        double distance = rtt / 2;


        // print the distance in cm
        usart_teleplot("metric", distance * SPEED_OF_SOUND_CM_US);
        // print the distance in inches
        usart_teleplot("imperial", distance * SPEED_OF_SOUND_IN_US);
    }
}
