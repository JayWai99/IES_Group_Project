/* DIRECTIVES */
#define CLK_FREQ 16000000   // CLK frequency (Hz)
#define __DELAY_BACKWARD_COMPATIBLE__

/* LIBRARIES */
#include <avr/io.h>         // For Arduino I/O ports
#include <util/delay.h>     // For delay
#include <stddef.h>         // For size_t

#include "usart.h"
#include "led.h"

/* MACROS */

/* PINS */
#define BAUD_RATE 9600      // Baud rate

/* CONSTANTS */

/* VARIABLES */

/* FUNCTIONS */
void setup_usart(void);
void print_variables(void);

/* MAIN */
int main(void)
{
    setup_usart();
    _delay_ms(500);

    setup_led();

    set_brightness_led(BRIGHTNESS_LOW);
    
    bool led_enabled = true;
    set_status_led(led_enabled);

    while(1)
    {
        for (size_t i = 65; i < 60065; i = i + 10000)
        {
            led_enabled = !led_enabled;
            set_status_led(led_enabled);

            set_frequency_led(i);
            usart_teleplot("F", i);

            set_brightness_led(BRIGHTNESS_LOW);
            print_variables();
            _delay_ms(2000);
            set_brightness_led(BRIGHTNESS_MEDIUM);
            print_variables();
            _delay_ms(2000);
            set_brightness_led(BRIGHTNESS_HIGH);
            print_variables();
            _delay_ms(2000);
        }
    }
}

void print_variables(void)
{
    usart_teleplot("OCR0A", OCR0A);
    usart_teleplot("OCR0B", OCR0B);
    usart_teleplot("CS", (TCCR0B >> CS00) & 0b111);
}

void setup_usart(void)
{
    usart_init(usart_ubrr(F_CPU, BAUD_RATE));
}