#include "usart.h"

#include <avr/io.h> // Include here if avr/io.h is needed for USART functions
#include <stdlib.h>
#include "bit.h"

uint16_t usart_ubrr(uint32_t fosc, uint32_t baud) {
    return fosc / 16 / baud - 1;
}

void usart_init(uint16_t ubrr) {
    /*Set baud rate */
    UBRR0 = ubrr;
    // Enable receiver and transmitter */
    // BIT_SET(UCSR0A, U2X0);
    UCSR0B = (1 << RXEN0) | (1 << TXEN0);
    /* Set frame format: 8data, 2stop bit */
    UCSR0C = (1 << USBS0) | (3 << UCSZ00);
    BIT_SET(UCSR0C, UPM01);
}

void usart_transmit(uint8_t byte) {
    /* Wait for empty transmit buffer */
    while (!(UCSR0A & (1 << UDRE0))) {
    }
    /* Put data into buffer, sends the data */
    UDR0 = byte;
}

void usart_tx_string(const char* pStr) {
    while (*pStr != '\0') {
        usart_transmit(*pStr);
        pStr++;
    }
}

void usart_tx_uint8(uint8_t x) {
    char buffer[4];
    buffer[3] = '\0';

    if (x == 0) {
        usart_transmit('0');
        return;
    }

    size_t index = 3;
    while (x > 0 && index > 0) {
        buffer[--index] = x % 10 + '0';
        x /= 10;
    }

    usart_tx_string(&buffer[index]);
}

void usart_tx_uint16(uint16_t x) {
    char buffer[6];
    buffer[5] = '\0';

    if (x == 0) {
        usart_transmit('0');
        return;
    }

    size_t index = 5;
    while (x > 0 && index > 0) {
        buffer[--index] = x % 10 + '0';
        x /= 10;
    }

    usart_tx_string(&buffer[index]);
}

void usart_tx_uint32(uint32_t x) {
    char buffer[11];
    buffer[10] = '\0';

    if (x == 0) {
        usart_transmit('0');
        return;
    }

    size_t index = 10;
    while (x > 0 && index > 0) {
        buffer[--index] = x % 10 + '0';
        x /= 10;
    }

    usart_tx_string(&buffer[index]);
}

void usart_tx_int8(int8_t x) {
    if (x < 0) {
        usart_transmit('-');
        x = -x;
    }

    usart_tx_uint8(x);
}

void usart_tx_int16(int16_t x) {
    if (x < 0) {
        usart_transmit('-');
        x = -x;
    }

    usart_tx_uint16(x);
}

void usart_tx_int32(int32_t x) {
    if (x < 0) {
        usart_transmit('-');
        x = -x;
    }

    usart_tx_uint32(x);
}

void usart_tx_float(float x, uint8_t width, uint8_t precision) {
    char buffer[width + 1];
    
    dtostrf(x, width, precision, buffer);
    buffer[width] = '\0';
    
    usart_tx_string(buffer);
}

void usart_tx_double(double x, uint8_t width, uint8_t precision) {
    char buffer[width + 1];
    
    dtostrf(x, width, precision, buffer);
    buffer[width] = '\0';
    
    usart_tx_string(buffer);
}

void usart_teleplot(const char* variable_name, double value) {
    // Format: '>' + variable_name + ':' + value + '\n'
    usart_transmit('>');
    usart_tx_string(variable_name);
    usart_transmit(':');
    usart_tx_double(value, 32, 16);
    usart_transmit('\n');
}