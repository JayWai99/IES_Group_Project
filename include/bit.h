#ifndef BIT_H
#define BIT_H

#define BIT_READ(reg, n) ((reg >> n) & 1)
#define BIT_SET(reg, n) (reg |= 1 << n)
#define BIT_CLEAR(reg, n) (reg &= ~(1 << n))
#define BIT_FLIP(reg, n) (reg ^= (1 << n))
#define BIT_ASSIGN(reg, n, value) (reg = (reg & ~(1 << n)) | ((value << n) & (1 << n))

#endif // BIT_H