#include <avr/io.h>
#include "adc.h"


//function to initialize ADC
void adc_init(){
	ADCSRA = 0;	// clear ADCSRA register

	ADMUX |= (1 << REFS0);  // set reference voltage
	ADMUX |= (1 << ADLAR);  // left align ADC value to 8 bits from ADCH register

	// sampling rate is [ADC clock] / [prescaler] / [conversion clock cycles]
	// Arduino Uno ADC clock is 16 MHz and a conversion takes 13 clock cycles
	ADCSRA |= (1 << ADPS2) | (0 << ADPS1) | (1 << ADPS0); //prescaler 32

	ADCSRA |= (1 << ADEN);  // enable ADC

}

//adc running on single conversion mode
uint8_t adc_read(uint8_t ch){
	  // select the corresponding channel 0~7
	  // ANDing with 7? will always keep the value
	  // of ch between 0 and 7
	  ch &= 0b00000111;  // AND operation with 7
	  ADMUX = (ADMUX & 0xF8)|ch; // clears the bottom 3 bits before ORing

	  ADCSRA |= (1<<ADSC); // start single conversion

	  // wait for conversion to complete
	  // ADSC becomes 0? again
	  // till then, run loop continuously
	  while(ADCSRA & (1<<ADSC));

	  return (ADCH);
}

//von neuman byte correction
uint8_t neuman(uint8_t bRecv, unsigned int* cnt){
    // 00 and 11 are discarded

    uint8_t finalByte = 0x00;
    unsigned short int counterFinal = 0;

    for(unsigned short int i=0; i<=6; i+=2){
        unsigned short int bitA = (bRecv & (1<<i)  ) != 0;
        unsigned short int bitB = (bRecv & (1<<(i+1))) != 0;

        if (bitA != bitB){
            finalByte |= (bitB << counterFinal);
            counterFinal++;
        }
    }

    *cnt = counterFinal;
    return bRecv;
}

