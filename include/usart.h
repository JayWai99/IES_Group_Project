#ifndef USART_H
#define USART_H

#include <stdint.h>

// struct usart
// {
//     uint8_t *tx_buffer;
//     size_t tx_buffer_size;
// };
// typedef struct usart usart_t;

uint16_t usart_ubrr(uint32_t fosc, uint32_t baud);

void usart_init(uint16_t ubrr);
void usart_transmit(uint8_t byte);
void usart_tx_string(const char* pStr);
void usart_tx_uint8(uint8_t x);
void usart_tx_uint16(uint16_t x);
void usart_tx_uint32(uint32_t x);
void usart_tx_int8(int8_t x);
void usart_tx_int16(int16_t x);
void usart_tx_int32(int32_t x);
void usart_tx_float(float x, uint8_t width, uint8_t precision);
void usart_tx_double(double x, uint8_t width, uint8_t precision);
void usart_teleplot(const char* variable_name, double value);

#endif // USART_H