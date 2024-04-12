/* LIBRARIES */
#include <avr/io.h>         // For Arduino I/O ports
#include <util/delay.h>     // For delay

/* MACROS */
#define CLK_FREQ 16000000   // CLK frequency (Hz)
#define PWM_DUTY 90         // PWM duty cycle (%)(fixed value for test purposes)
#define PWM_FREQ 65         // PWM frequency (Hz)(fixed value for test purposes)

/* PINS */
#define PIN_PWM PIND5       // PWM connected to pin 5 via pin D5

/* CONSTANTS */
const int prescaler[] = {1, 8, 64, 256, 1024};

/* VARIABLES */


/* FUNCTIONS */
void setup_pwm(void);

/* MAIN */
int main(void)
{
    setup_pwm();

    while(1)
    {
        //
        //_delay_ms(5000);
        //
    }
}

void setup_pwm(void)
{
    // Calculate PWM parameters
    float period = 1/PWM_FREQ;
    float period_on  = period *    (PWM_DUTY/100) ;
    float period_off = period * (1-(PWM_DUTY/100));

    // PWM pin to output mode
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
    TCCR0B |=  (1 << CS02);
    TCCR0B &= ~(1 << CS01);
    TCCR0B |=  (1 << CS00);

    // for (size_t i = 0; i < count; i++)
    // {
    //     /* code */
    // }
    

    // Define TOP as PWM period

    // OCR0A defines TOP value
    // OCR0B defines duty cycle value
    OCR0A = 240;
    OCR0B = 240*(PWM_DUTY/100);

    // OCR0A defines TOP value
    // OCR0B defines duty cycle value
    //OCR0A = CLK_FREQ / (PWM_FREQ * );
    //OCR0B = 1/PWM_FREQ;

    // Calculate 
}
