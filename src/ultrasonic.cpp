#include <avr/io.h>
#include <util/delay.h>
#include <stddef.h>     
#include "ultrasonic.h"
#include "bit.h"

#define ULTRASONIC_TRIG_PIN PIND7 // Ultra Sonic Trigger line connected to pin 7 at port D, pin 4
#define ULTRASONIC_ECHO_PIN PIND6 // Ultra Sonic Echo line connected to pin 6 at port D, pin 3

// This function should initialize the ultrasonic sensor. This should mostly involve initializing the pins and other register values.
void setup_ultrasonic(void) {
    // Set the ultrasonic trigger as output
    BIT_SET(DDRD, ULTRASONIC_TRIG_PIN);
    // Set the ultrasonic echo as input
    BIT_CLEAR(DDRD, ULTRASONIC_ECHO_PIN);
}

bool is_ultrasonic_echo_high(void) {
    return BIT_READ(PIND, ULTRASONIC_ECHO_PIN) == 1;
}

// This function should cause the ultrasonic sensor to emit a pulse and then return the time it takes for the pulse to return in us.
uint32_t read_ultrasonic(void) {
        BIT_SET(PORTD, ULTRASONIC_TRIG_PIN);
        _delay_us(10);
        BIT_CLEAR(PORTD, ULTRASONIC_TRIG_PIN);

        while (!is_ultrasonic_echo_high()) {
        }

        size_t count; // The round trip time of the echo signal, in tens of microseconds
        for (count = 0; is_ultrasonic_echo_high(); count++) {
            _delay_us(10);
        }

        uint32_t rtt = count * 10; // The round trip time of the echo signal, in microseconds
        return rtt;
}