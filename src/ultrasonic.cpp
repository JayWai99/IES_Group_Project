#include <avr/io.h>
#include <util/delay.h>
#include <stddef.h>     
#include "ultrasonic.h"
#include "bit.h"

#define ULTRASONIC_TRIG_PIN PIND4 // Ultra Sonic Trigger line connected to pin 4 at port D, pin 4
#define ULTRASONIC_ECHO_PIN PIND3 // Ultra Sonic Echo line connected to pin 3 at port D, pin 3

// 343 m/s = 343 * 100 cm/s = 34300 cm/s
// 34300 cm/s = 34300 / 1000 cm/ms = 34.3 cm/ms
// 34.3 cm/ms = 34.3 / 1000 cm/us = 0.0343 cm/us
// #define SPEED_OF_SOUND_CM_US (343.0 * 100.0 / 1000.0 / 1000.0) // Speed of sound in cm/us

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
        // double distance = ((double) rtt) / 2;
        // double distance_cm = distance * SPEED_OF_SOUND_CM_US;
        return rtt;
}