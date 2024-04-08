#ifndef ULTRASONIC_H
#define ULTRASONIC_H

#include <stdint.h>

void setup_ultrasonic(void);

uint32_t read_ultrasonic(void);

#endif // ULTRASONIC_H