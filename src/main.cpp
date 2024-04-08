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

#include "bit.h"            // For bit set and bit clear functions
#include "usart.h"          // For USART
#include "ultrasonic.h"     // For Ultrasonic sensor
#include "led_button.h"     // For LED Button
#include "volume_button.h"  // For Volume Button
#include "photoresistor.h"  // For Photoresistor
#include "led.h"            // For LED
#include "buzzer.h"         // For Buzzer


/* FUNCTIONS */
void setup(void) {
    usart_init(usart_ubrr(F_CPU, BAUD));
    
    LOG_INFO("Running Setup");

    LOG_DEBUG("Setting up Ultrasonic Sensor");
    setup_ultrasonic();
    
    LOG_DEBUG("Setting up Buzzer");
    setup_buzzer();

    LOG_DEBUG("Setting up LED");
    setup_led();

    LOG_DEBUG("Setting up LED Button");
    setup_led_button();

    LOG_DEBUG("Setting up Volume Button");
    setup_volume_button();

    LOG_DEBUG("Setting up Photoresistor");
    setup_photoresistor();
}


/* GLOBAL VARIABLES */


/* INTERRUPT SERVICE ROUTINE */

/* MAIN */
int main(void) {
    setup();

    bool led_enabled = false; // Initialize the LED status to be off
    volume_t buzzer_volume = VOLUME_LOW; // Initialize the buzzer volume to be low

    while (true) {
        uint32_t ultrasonic_duration = read_ultrasonic();

        // Poll for Events
        bool led_button_pressed = is_pressed_led_button();
        bool volume_button_pressed = is_pressed_volume_button();
        float light_level = read_photoresistor();

        if (led_button_pressed) {
            led_enabled = !led_enabled;
            set_status_led(led_enabled);
        }

        if (volume_button_pressed) {
            buzzer_volume = next_volume(buzzer_volume);
            set_volume_buzzer(buzzer_volume);
        }

        if (light_level < 0.33) {
            set_brightness_led(BRIGHTNESS_LOW);
        } else if (light_level < 0.66) {
            set_brightness_led(BRIGHTNESS_MEDIUM);
        } else {
            set_brightness_led(BRIGHTNESS_HIGH);
        }

        // These frequency scales will need to be adjusted based on the actual values of the ultrasonic sensor
        set_frequency_led(1.0f / ultrasonic_duration);
        set_frequency_buzzer(1.0f / ultrasonic_duration);
    }
}