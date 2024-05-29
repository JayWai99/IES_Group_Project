#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stddef.h>     
#include "ultrasonic.h"
#include "bit.h"


#define ULTRASONIC_TRIG_PIN PB1 // Ultra Sonic Trigger line connected to pin 7 at port D, pin 4
#define ULTRASONIC_ECHO_PIN PB0 // Ultra Sonic Echo line connected to pin 6 at port D, pin 3

unsigned long t_rise, t_fall, t1OVF = 0;

// This function should initialize the ultrasonic sensor. This should mostly involve initializing the pins and other register values.
void setup_ultrasonic(void) {
    // Set the ultrasonic trigger as output
    BIT_SET(DDRB, ULTRASONIC_TRIG_PIN);
    // Set the ultrasonic echo as input
    BIT_CLEAR(DDRB, ULTRASONIC_ECHO_PIN);

    BIT_SET(TCCR1B, ICNC1);

    BIT_SET(TCCR1B, ICES1);

    BIT_SET(TCCR1B, CS10);

    BIT_SET(TIMSK1, ICIE1);

    BIT_SET(TIMSK1, TOIE1);
}

bool is_echo_rising(void) {
    return BIT_READ(TCCR1B, ICES1);
}

// This function should cause the ultrasonic sensor to emit a pulse and then return the time it takes for the pulse to return in us.
double read_ultrasonic(void) {
        BIT_SET(PORTB, ULTRASONIC_TRIG_PIN);
        _delay_us(10);
        BIT_CLEAR(PORTB, ULTRASONIC_TRIG_PIN);

        

        while (is_echo_rising()) 
        ;

        // size_t count; // The round trip time of the echo signal, in tens of microseconds
        // for (count = 0; is_ultrasonic_echo_high(); count++) {
        //     _delay_us(10);
        // }

        // uint32_t rtt = count * 10; // The round trip time of the echo signal, in microseconds

        double t_echo = t_fall / (double) F_CPU;

        return t_echo;
}

ISR(TIMER1_OVF_vect){
    t1OVF++;
}

ISR(TIMER1_CAPT_vect){
    if (is_echo_rising()){
        t_rise =  (t1OVF << 16) + ICR1;
        t1OVF = 0;
        TCNT1 = 0;    
    }
    else{
        t_fall = (t1OVF << 16) + ICR1;
    }
    BIT_FLIP(TCCR1B, ICES1); 
}