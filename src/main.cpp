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
#include "power_button.h"     // For LED Button
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

    LOG_DEBUG("Setting up POWER Button");
    setup_power_button();

    LOG_DEBUG("Setting up Volume Button");
    setup_volume_button();

    LOG_DEBUG("Setting up Photoresistor");
    setup_photoresistor();
}

float bound(float value, float min, float max) {
    return fmaxf(fminf(value, max), min);
}


/* GLOBAL VARIABLES */


/* INTERRUPT SERVICE ROUTINE */

/* MAIN */
int main(void) {
    setup();

    bool power_status = false; // Initialize the LED status to be off
    volume_t buzzer_volume = VOLUME_LOW; // Initialize the buzzer volume to be low

    while (true) {
        uint32_t ultrasonic_duration = read_ultrasonic();

        // Poll for Events
        bool power_button_pressed = is_pressed_power_button();
        bool volume_button_pressed = is_pressed_volume_button();
        float light_level = read_photoresistor();

        if (power_button_pressed) {
            LOG_DEBUG_VARIABLE("Power Button Pressed", power_status);
            power_status = !power_status;
            set_status_led(power_status);
            set_status_buzzer(power_status);
        }

        if (volume_button_pressed) {
            LOG_DEBUG_VARIABLE("Volume Button Pressed", buzzer_volume);
            buzzer_volume = next_volume(buzzer_volume);
            set_volume_buzzer(buzzer_volume);
        }

        if (light_level < 0.15) {
            set_brightness_led(BRIGHTNESS_HIGH);
        } else if (light_level < 0.6) {
            set_brightness_led(BRIGHTNESS_MEDIUM);
        } else {
            set_brightness_led(BRIGHTNESS_LOW);
        }

        // These frequency scales will need to be adjusted based on the actual values of the ultrasonic sensor
        float bounded_range = (bound(ultrasonic_duration, 200, 4000) - 200) / (4000 - 200);
        float led_frequency = 20060 - (bounded_range * 20000);
        float buzzer_frequency = 1500 - (bounded_range * 500);

        LOG_DEBUG_VARIABLE("Volume", buzzer_volume);

        set_frequency_led(led_frequency);
        set_frequency_buzzer(buzzer_frequency);
    }
}