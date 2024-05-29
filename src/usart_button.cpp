#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdbool.h>
#include "bit.h"

#define USART_BUTTON_PIN PD2 
#define DEBOUNCE_DELAY 100

bool usart_state = false;
static bool last_usart_state = false;

// This function should initialize the usart button. This should mostly involve initializing the pins and other register values.
void setup_usart_button(void) {
    // Set the push button as output and initialise it to logic high
    BIT_SET(PORTD, USART_BUTTON_PIN);

    // Set the push button as input
    BIT_CLEAR(DDRD, USART_BUTTON_PIN);

    BIT_SET(PCICR, PCIE2);

    BIT_SET(PCMSK2, PCINT18);
}

bool usart_button_read(void){
    return BIT_READ(PIND, USART_BUTTON_PIN) == 0;
}

// bool read_interrupt(void){
//     return BIT_READ(EIMSK, INT0);
// }

// This function should return true if the led button is pressed and false otherwise.
// This function should be debounced. If the button status isn't changed from the last known value it should return immediately.
// Otherwise, it should wait so the output can be debounced (verified) and then return the new button status.
bool is_pressed_usart_button(void) {
    if (usart_state == last_usart_state){
        last_usart_state = usart_state;
        return false;
    }
    last_usart_state = usart_state;
    
    if (usart_state) {
        _delay_us(DEBOUNCE_DELAY);
        return usart_button_read();            
    }
    return false;
}


ISR(PCINT2_vect){
    if (usart_button_read()){
        usart_state = !usart_state;
    }
    


}