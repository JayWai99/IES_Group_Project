/* LIBRARIES */
#include <avr/io.h>         // For Arduino I/O ports

/* FUNCTIONS */
void setup_pwm(void);

/* MACROS */
#define PWM_DUTY 50         // PWM duty cycle (%)(fixed value for test purposes)
#define PWM_FREQ 10         // PWM frequency (Hz)(fixed value for test purposes)

/* PINS */
#define PIN_PWM PINB2       // PWM connected to pin 10 via pin B2

/* MAIN */
int main(void)
{
    setup_pwm();


    while(1)
    {
        //
    }
}

void setup_pwm(void)
{
    // Calculate PWM parameters
    float period = 1/PWM_FREQ;
    float period_on  = period *    (PWM_DUTY/100) ;
    float period_off = period * (1-(PWM_DUTY/100));

    // PWM pin to output mode and initialise low
    DDRB   |=  (1 << PIN_PWM);
    PORTB  &= ~(1 << PIN_PWM);

    // Timer/counter to fast PWM MAX
    // Requires setting WGMx to 011
    // TCCR1B &= ~(1 << WGM12);
    // TCCR1A |=  (1 << WGM11);
    // TCCR1A |=  (1 << WGM10);

    // Select prescaler for
    // Prescaler for 

    // Timer/counter to fast PWM TOP
    // Requires setting WGMx[0-2] to 111
    TCCR1B |=  (1 << WGM12);
    TCCR1A |=  (1 << WGM11);
    TCCR1A |=  (1 << WGM10);

    // Output compare to toggle OCxA on compare match
    // Requires setting COMxA[0-1] to 01 
    TCCR1A &= ~(1 << COM1A1);
    TCCR1A |=  (1 << COM1A0);
    // Output compare to non-inverting mode
    // Requires setting COMxB[0-1] to 10
    TCCR1A |=  (1 << COM1B1);
    TCCR1A &= ~(1 << COM1B0);

    // Select prescaler for
    // Prescaler for 

    // Define TOP as PWM period
    //TCCR1B |=  ();

    // Calculate 
}