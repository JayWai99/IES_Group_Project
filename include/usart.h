#ifndef USART_H
#define USART_H

#include <stdint.h>

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

#ifdef DEBUG
#define LOG_DEBUG(msg) (usart_tx_string("[DEBUG] " msg "\n"))
#define LOG_DEBUG_VARIABLE(msg, value) ({ \
    usart_tx_string("[DEBUG] " msg ": "); \
    usart_tx_double(value, 32, 16); \
    usart_tx_string("\n"); \
})
#else
#define LOG_DEBUG(msg) ({})
#define LOG_DEBUG_VARIABLE(msg, value) ({})
#endif

#define LOG_INFO(msg) (usart_tx_string("[INFO ] " msg "\n"))
#define LOG_INFO_VARIABLE(msg, value) ({ \
    usart_tx_string("[INFO ] " msg ": "); \
    usart_tx_double(value, 32, 16); \
    usart_tx_string("\n"); \
})
#define LOG_WARN(msg) (usart_tx_string("[WARN ] " msg "\n"))
#define LOG_WARN_VARIABLE(msg, value) ({ \
    usart_tx_string("[WARN ] " msg ": "); \
    usart_tx_double(value, 32, 16); \
    usart_tx_string("\n"); \
})
#define LOG_ERROR(msg) (usart_tx_string("[ERROR] " msg "\n"))
#define LOG_ERROR_VARIABLE(msg, value) ({ \
    usart_tx_string("[ERROR] " msg ": "); \
    usart_tx_double(value, 32, 16); \
    usart_tx_string("\n"); \
})

#endif // USART_H