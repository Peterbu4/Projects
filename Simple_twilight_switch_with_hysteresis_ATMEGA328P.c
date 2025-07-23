// That code also requires appropriatly defined functions to operate LCD display using I2C which should be included in "LCDI2C.h" header file!
#include <avr/io.h>
#include <util/delay.h>
#include "LCDI2C.h"

LCD_I2C Lcd = LCD_I2C(0x27); // LCD I2C Address

// Makra
#ifndef _BV
#define _BV(bit)	(1<<(bit))
#endif

#ifndef sbi
#define sbi(reg,bit)	reg |= (_BV(bit))
#endif

#ifndef cbi
#define cbi(reg,bit)	reg &= ~(_BV(bit))
#endif

#ifndef tbi
#define tbi(reg,bit)	reg ^=(_BV(bit))
#endif

// ADC Initialization
void ADC_init() {
    // Setting reference voltage on AVCC (5V)
    ADMUX = (1 << REFS0);
    
    // Setting prescaler on 128 (16MHz/128 = 125kHz)
    ADCSRA = (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);
    
    // Turning on ADC converter
    ADCSRA |= (1 << ADEN);
}

// Measurement from ADC0 channel (PC0)
uint16_t adcChannel0() {
    // Wybór kanału ADC0 (PC0)
    ADMUX = (ADMUX & 0xF0) | (0 << MUX0);
    
    // Start conversion
    ADCSRA |= (1 << ADSC);
    
    // Waiting until the conversion is over
    while (ADCSRA & (1 << ADSC));
    
    return ADC;
}

// Measurement from ADC1 channel (PC1)
uint16_t adcChannel1() {
    // Wybór kanału ADC1 (PC1)
    ADMUX = (ADMUX & 0xF0) | (1 << MUX0);
    
    // Start conversion
    ADCSRA |= (1 << ADSC);
    
    // Waiting until the conversion is over
    while (ADCSRA & (1 << ADSC));
    
    return ADC;
}

// Function used to display the results
void update_display() {
    char line1[16], line2[16];
    
    // Reding the value from two ADC channels
    uint16_t adc0_value = adcChannel0();
    uint16_t adc1_value = adcChannel1();
    
    // First line - ADC value
    Lcd.goTo(0, 0);
    sprintf(line1, "ADC0:%04d ADC1:%04d", adc0_value, adc1_value);
    
    Lcd.writeText(line1);
    
    // Second line - calculated voltage
    uint16_t voltage0_mV = (uint16_t)((float)adc0_value * (5000 / 1023));
    uint16_t voltage1_mV = (uint16_t)((float)adc1_value * (5000 / 1023));
    
    Lcd.goTo(0, 1);
    sprintf(line2, "%02d.%02dV %02d.%02dV", 
           voltage0_mV / 1000, (voltage0_mV % 1000) / 10,
           voltage1_mV / 1000, (voltage1_mV % 1000) / 10);
    
    Lcd.writeText(line2);
}

void set {
    uint16_t value = adcChannel1();
    uint16_t v = adcChannel0();
    // Simple hysteresis
    if (v + 25 < value) {sbi(PORTD, PD0)};
    if (v - 25 > value) {cbi(PORTD, PD0)};
}

int main() {
    // LED diod port
    DDRD = 0xFF;
    sbi(PORTD, PD0);
    
    // Initializing LCD and ADC
    Lcd.backLightOn();
    ADC_init();
    
    while(1) {
	set();
        update_display();
        _delay_ms(200); // Delay between measurements
    }
    
    return 0;
}
