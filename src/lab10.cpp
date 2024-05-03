/* MACROS */
#define F_CPU 16000000UL    // Define the CPU frequency
#define BAUD 9600           // Define the baud rate
#define MAX 255
#define duty_cycle 0.5

/* LIBRARIES */
#include <avr/io.h>
#include <stdlib.h>
#include <stddef.h>         // Standard Definition library for type definitions
#include <math.h>           // For mathematical operations
#include <avr/interrupt.h>  // For interrupt routine

#include "bit.h"            // For bit set and bit clear functions
#include "usart.h"          // For USART
#include "ultrasonic.h"     // For Ultrasonic sensor
#include "power_button.h"     // For LED Button
#include "volume_button.h"  // For Volume Button
#include "photoresistor.h"  // For Photoresistor
#include "led.h"            // For LED
#include "buzzer.h"         // For Buzzer

#define PIN_PWM_A PINB1
#define PIN_PWM_B PINB2
#define PIN_ACP PIND6
#define PIN_ACN PIND7

/* GLOBAL VARIABLES */
int prescaler_value = 2;
float frequency = 1000;

/* FUNCTIONS */
void setup_pwm(void){
    BIT_SET(DDRB, PIN_PWM_A);
    BIT_SET(DDRB, PIN_PWM_B);

    BIT_SET(TCCR1A, WGM10);
    BIT_SET(TCCR1B, WGM12);

    BIT_SET(TCCR1A, COM1A0);
    BIT_CLEAR(TCCR1A, COM1A1);    
}

void set_prescaler(int i){
        switch (i)
    {
        case 0:
            TCCR1B &= ~(1 << CS12);
            TCCR1B &= ~(1 << CS11);
            TCCR1B |=  (1 << CS10);
            break;
        case 1:
            TCCR1B &= ~(1 << CS12);
            TCCR1B |=  (1 << CS11);
            TCCR1B &= ~(1 << CS10);
            break;
        case 2:
            TCCR1B &= ~(1 << CS12);
            TCCR1B |=  (1 << CS11);
            TCCR1B |=  (1 << CS10);
            break;
        case 3:
            TCCR1B |=  (1 << CS12);
            TCCR1B &= ~(1 << CS11);
            TCCR1B &= ~(1 << CS10);
            break;
        case 4:
            TCCR1B |=  (1 << CS12);
            TCCR1B &= ~(1 << CS11);
            TCCR1B |=  (1 << CS10);
            break;
        case 5:
            TCCR1B |=  (1 << CS12);
            TCCR1B |=  (1 << CS11);
            TCCR1B &= ~(1 << CS10);
            break;
        case 6:
            TCCR1B |=  (1 << CS12);
            TCCR1B |=  (1 << CS11);
            TCCR1B |=  (1 << CS10);
            break;
        default:
            TCCR1B &= ~(1 << CS12);
            TCCR1B &= ~(1 << CS11);
            TCCR1B &= ~(1 << CS10);
            break;
    }
}

void setup_ac(void){
    BIT_CLEAR(PORTD, PIN_ACP);
    BIT_CLEAR(PORTD, PIN_ACN);
}

void setup_ic(void){
    BIT_SET(TCCR1B, ICES1);
}

void setup_lab10(void) {
    usart_init(usart_ubrr(F_CPU, BAUD));
    
    setup_pwm();

    set_prescaler(prescaler_value);
    
    setup_ac();

    setup_ic();

}

bool read_state(void){
    return BIT_READ(PINB, PIN_PWM_B);
}

void pwm_output(void){
    float top = 0;

    for (prescaler_value = 0; prescaler_value < 7; prescaler_value++)
    {
        top = F_CPU / (prescaler_value * frequency);

        if (top <= MAX){
            OCR1A = top;
            OCR1B = duty_cycle * top;
            usart_teleplot("pwm output", (double) read_state());
            return;
        }
    }
    

}


/* INTERRUPT SERVICE ROUTINE */

/* MAIN */
int main(void) {
    setup_lab10();

    while (true) {
        pwm_output(); 
    }
}