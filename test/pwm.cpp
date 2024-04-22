/* DIRECTIVES */
#define CLK_FREQ 16000000   // CLK frequency (Hz)
#define __DELAY_BACKWARD_COMPATIBLE__

/* LIBRARIES */
#include <avr/io.h>         // For Arduino I/O ports
#include <util/delay.h>     // For delay
#include <stddef.h>         // For size_t

#include "usart.h"
#include "led.h"
#include "buzzer.h"

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
    setup_buzzer();

    set_brightness_led(BRIGHTNESS_LOW);
    set_volume_buzzer(VOLUME_LOW);

    bool led_enabled = true;
    bool buzzer_enabled = true;
    set_status_led(led_enabled);
    set_status_buz(buzzer_enabled);

    while(1)
    {
        for (size_t i = 65; i < 2065; i = i + 100)
        {
            led_enabled = !led_enabled;
            buzzer_enabled = !buzzer_enabled;
            set_status_led(led_enabled);
            set_status_buz(buzzer_enabled);

            set_frequency_led(i);
            set_frequency_buzzer(i);
            usart_teleplot("F", i);

            set_brightness_led(BRIGHTNESS_LOW);
            set_volume_buzzer(VOLUME_LOW);
            print_variables();
            _delay_ms(2000);
            set_brightness_led(BRIGHTNESS_MEDIUM);
            set_volume_buzzer(VOLUME_MEDIUM);
            print_variables();
            _delay_ms(2000);
            set_brightness_led(BRIGHTNESS_HIGH);
            set_volume_buzzer(VOLUME_HIGH);
            print_variables();
            _delay_ms(2000);
        }
    }
}

void print_variables(void)
{
    usart_teleplot("OCR0A", OCR0A);
    usart_teleplot("OCR0B", OCR0B);
    usart_teleplot("CS", (TCCR0B >> CS20) & 0b111);
    usart_teleplot("OCR2A", OCR2A);
    usart_teleplot("OCR2B", OCR2B);
    usart_teleplot("CS", (TCCR2B >> CS20) & 0b111);
}

void setup_usart(void)
{
    usart_init(usart_ubrr(F_CPU, BAUD_RATE));
}