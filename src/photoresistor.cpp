#include "photoresistor.h"
#include "bit.h"

#include <stdint.h>
#include <avr/interrupt.h>

#define PHOTORESISTOR_PIN PINC0
#define PHOTORESISTOR_CHANNEL ADC_CHANNEL_0

typedef enum {
    ADC_REFERENCE_VOLTAGE_AREF = 0b00,
    ADC_REFERENCE_VOLTAGE_AVCC = 0b01,
    ADC_REFERENCE_VOLTAGE_INTERNAL = 0b11
} adc_reference_voltage_t;

void set_adc_reference_voltage(adc_reference_voltage_t reference_voltage) {
    ADMUX |= (reference_voltage & 0b11) << REFS0;
}

typedef enum {
    ADC_CHANNEL_0 = 0b0000,
    ADC_CHANNEL_1 = 0b0001,
    ADC_CHANNEL_2 = 0b0010,
    ADC_CHANNEL_3 = 0b0011,
    ADC_CHANNEL_4 = 0b0100,
    ADC_CHANNEL_5 = 0b0101,
    ADC_CHANNEL_6 = 0b0110,
    ADC_CHANNEL_7 = 0b0111,
    ADC_CHANNEL_8 = 0b1000,
    ADC_CHANNEL_BANDGAP = 0b1110,
    ADC_CHANNEL_GND = 0b1111
} adc_channel_t;

void set_adc_channel(adc_channel_t channel) {
    ADMUX |= (channel & 0b1111) << MUX0;
}

void set_adc_left_aligned(bool left_aligned) {
    BIT_ASSIGN(ADMUX, ADLAR, left_aligned);
}

typedef enum {
    ADC_PRESCALER_2 = 0b001,
    ADC_PRESCALER_4 = 0b010,
    ADC_PRESCALER_8 = 0b011,
    ADC_PRESCALER_16 = 0b100,
    ADC_PRESCALER_32 = 0b101,
    ADC_PRESCALER_64 = 0b110,
    ADC_PRESCALER_128 = 0b111
} adc_prescaler_t;

void set_adc_prescaler(adc_prescaler_t prescaler) {
    ADCSRA |= (prescaler & 0b111) << ADPS0;
}

typedef enum {
    ADC_AUTO_TRIGGER_SOURCE_FREE_RUNNING = 0b000,
    ADC_AUTO_TRIGGER_SOURCE_ANALOG_COMPARATOR = 0b001,
    ADC_AUTO_TRIGGER_SOURCE_EXTERNAL_INTERRUPT_REQUEST0 = 0b010,
    ADC_AUTO_TRIGGER_SOURCE_TIMER_COUNTER0_COMPARE_MATCH_A = 0b011,
    ADC_AUTO_TRIGGER_SOURCE_TIMER_COUNTER0_OVERFLOW = 0b100,
    ADC_AUTO_TRIGGER_SOURCE_TIMER_COUNTER_COMPARE_MATCH_B = 0b101,
    ADC_AUTO_TRIGGER_SOURCE_TIMER_COUNTER1_OVERFLOW = 0b110,
    ADC_AUTO_TRIGGER_SOURCE_TIMER_COUNTER1_CAPTURE_EVENT = 0b111
} adc_auto_trigger_source_t;

void set_adc_auto_trigger_source(adc_auto_trigger_source_t auto_trigger_source) {
    ADCSRB |= (auto_trigger_source & 0b111) << ADTS0;
}

void set_adc_auto_trigger(bool auto_trigger) {
    BIT_ASSIGN(ADCSRA, ADATE, auto_trigger);
}

void set_adc_enabled(bool enabled) {
    BIT_ASSIGN(ADCSRA, ADEN, enabled);
}

void set_adc_interrupt(bool enabled) {
    BIT_ASSIGN(ADCSRA, ADIE, enabled);
}

void start_adc_conversion(void) {
    BIT_SET(ADCSRA, ADSC);
}

void setup_adc(adc_channel_t channel, adc_reference_voltage_t reference_voltage, adc_prescaler_t prescaler, bool interrupt) {
    set_adc_channel(channel);
    set_adc_reference_voltage(reference_voltage);
    set_adc_prescaler(prescaler);
    set_adc_left_aligned(false);
    set_adc_auto_trigger(false);
    set_adc_interrupt(interrupt);
    set_adc_enabled(true);
}

void setup_adc_auto_trigger(adc_auto_trigger_source_t auto_trigger_source) {
    set_adc_auto_trigger(true);
    set_adc_auto_trigger_source(auto_trigger_source);
}

static volatile uint16_t photoresistor_value;

ISR(ADC_vect) {
    // Read the ADC value and save so that the last know value can be read
    photoresistor_value = ADC;
}

// This function should initialize the photoresistor. This should mostly involve initializing the pins and other register values.
void setup_photoresistor(void) {
    photoresistor_value = 0;

    BIT_CLEAR(DDRC, PHOTORESISTOR_PIN); // Set the photoresistor pin as input

    setup_adc(PHOTORESISTOR_CHANNEL, ADC_REFERENCE_VOLTAGE_AVCC, ADC_PRESCALER_128, true);
    setup_adc_auto_trigger(ADC_AUTO_TRIGGER_SOURCE_FREE_RUNNING);
    sei();
    start_adc_conversion();
}

// This function should read the current value of the photoresistor and return it, scaled to a value between 0 and 1. Where 0 is the darkest and 1 is the brightest.
float read_photoresistor(void) {
    return (float) photoresistor_value / 1023.0f;
}