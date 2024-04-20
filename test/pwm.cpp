/* LIBRARIES */
#include <avr/io.h>         // For Arduino I/O ports
#define CLK_FREQ 16000000   // CLK frequency (Hz)
#define __DELAY_BACKWARD_COMPATIBLE__
#include <util/delay.h>     // For delay
#include "usart.h"

/* MACROS */
//#define PWM_DUTY 90         // PWM duty cycle (%)(fixed value for test purposes)
//#define PWM_FREQ 65         // PWM frequency (Hz)(fixed value for test purposes)

/* PINS */
#define PIN_TC0 PIND5       // PWM connected to pin 5 via pin D5
#define PIN_PWM PIND6       // PWM connected to pin 6 via pin D6
#define BAUD_RATE 9600      //baud rate

/* CONSTANTS */
const int prescaler[] = {1, 8, 64, 256, 1024};

/* VARIABLES */


/* FUNCTIONS */
void setup_pwm(void);
void setup_usart(void);

/* MAIN */
int main(void)
{
    setup_pwm();
    setup_usart();
    _delay_ms(500);

    while(1)
    {
        // For test purposes, change duty cycle to 25%, then 50%, then 75%
        //OCR0B = OCR0A*(0/100);
        //_delay_ms(1000);
        float top = 240;
        OCR0B = top*((float)25/(float)100);
        usart_teleplot("OCR0A", OCR0A);
        usart_teleplot("OCR0B", OCR0B);
        _delay_ms(1000);
        OCR0B = top*((float)50/(float)100);
        usart_teleplot("OCR0A", OCR0A);
        usart_teleplot("OCR0B", OCR0B);
        _delay_ms(1000);
        OCR0B = top*((float)75/(float)100);
        usart_teleplot("OCR0A", OCR0A);
        usart_teleplot("OCR0B", OCR0B);
        _delay_ms(1000);
        //OCR0B = OCR0A*(100/100);
        //_delay_ms(1000);
    }
}

void setup_pwm(void)
{
    // Calculate PWM parameters
    // float period = 1/PWM_FREQ;
    // float period_on  = period *    (PWM_DUTY/100) ;
    // float period_off = period * (1-(PWM_DUTY/100));

    // T/C0 pins to output mode
    DDRD   |=  (1 << PIN_TC0);
    DDRD   |=  (1 << PIN_PWM);

    // Timer/counter to fast PWM MAX
    // Requires setting WGMx to 011
    // TCCR1B &= ~(1 << WGM12);
    // TCCR1A |=  (1 << WGM11);
    // TCCR1A |=  (1 << WGM10);

    // Select prescaler for
    // Prescaler for 

    // Timer/counter to fast PWM TOP
    // Requires setting WGMx[0-2] to 111
    TCCR0B |=  (1 << WGM02);
    TCCR0A |=  (1 << WGM01);
    TCCR0A |=  (1 << WGM00);

    // Output compare to toggle OCxA on compare match
    // Requires setting COMxA[0-1] to 01
    TCCR0A &= ~(1 << COM0A1);
    TCCR0A |=  (1 << COM0A0);
    // Output compare to non-inverting mode
    // Requires setting COMxB[0-1] to 10
    TCCR0A |=  (1 << COM0B1);
    TCCR0A &= ~(1 << COM0B0);

    // Select prescaler for
    // Prescaler for 

    // Clock source to F_CLK / 1024 from prescaler
    // Requires setting CSx[0-2] to 101
    // Clock source to F_CLK / 256 from prescaler for test
    // Requires setting CSx[0-2] to 100
    TCCR0B |=  (1 << CS02);
    TCCR0B &= ~(1 << CS01);
    TCCR0B &= ~(1 << CS01);

    // for (size_t i = 0; i < count; i++)
    // {
    //     /* code */
    // }
    

    // Define TOP as PWM period
    int top = 240;
    // OCR0A defines TOP value
    // OCR0B defines duty cycle value
    OCR0A = top;
    //OCR0B = top*(PWM_DUTY/100);

    // OCR0A defines TOP value
    // OCR0B defines duty cycle value
    //OCR0A = CLK_FREQ / (PWM_FREQ * );
    //OCR0B = 1/PWM_FREQ;

    // Calculate
}

void setup_usart(void)
{
    usart_init(usart_ubrr(F_CPU, BAUD_RATE));
}