#include <avr/io.h>
#include <util/delay.h>
#include <util/twi.h>
#include "usart.h"
#include "myTWI.h"

#define SLA_W (0x22 << 1)
#define SLA_R ((0x22 << 1) | 0x01)

void twi_print_twsr(){
    usart_tx_reg(TWSR);
    usart_transmit('\n');
}

int main(void){
    usart_init(103);
    twi_init_master();

    unsigned char data = 0;

    while(1){
        twi_start();

        twi_write(SLA_W);

        twi_write(data++);

        twi_start();

        twi_write(SLA_R);

        uint8_t receiveData = twi_read_nack();

        twi_stop();

        usart_tx_float(receiveData, 3, 0);
    }
}