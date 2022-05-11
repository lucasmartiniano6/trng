/*
Author: Lucas Martiniano
Date: 28-10-2020

AVR IMPLEMENTATION FOR HARDWARE-BASED TRUE RANDOM NUMBER GENERATOR (AVALANCHE NOISE)
*/

#ifndef F_CPU
#define F_CPU 16000000UL // 16 MHz clock speed
#endif

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

//UART MACROS
#define BAUD 250000                                  // define baud bps
#define BAUDRATE (((F_CPU)/(BAUD*8UL)) -1)           // set baud rate value for UBRR

//ADC MACROS
#define BUFFSIZE 2          //each buffer index holds one byte
#define DELAY_US 5          //delay in microseconds(us) before each adc measurement

// function to initialize UART
void uart_init (void)
{
	UBRR0H = (BAUDRATE>>8);                      // shift the register right by 8 bits
	UBRR0L = BAUDRATE;                           // set baud rate
	UCSR0B|= (1<<TXEN0);						// enable transmitter
	UCSR0C|= (1<<UMSEL01)|(1<<UCSZ00)|(1<<UCSZ01);   // 8bit data format

	UCSR0A |= (1<<U2X0); //set double speed mode

}

// function to send data
void uart_transmit (uint8_t data)
{
	while (!( UCSR0A & (1<<UDRE0)));                // wait while register is free
	UDR0 = data;                                   // load data in the register
}

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

//keeps filling a byte with the bits passed
void fill_byte(uint8_t recv_byte, unsigned int cnt, uint8_t *final_byte, unsigned int cntfinal){
  int roll = 0;
  for(int i=cntfinal; i<(cntfinal+cnt); i++){
      int bit = (recv_byte & (1<<roll)) != 0;
      *final_byte = *final_byte | (bit<<i);
      roll++;
  }
}

int main(void)
{

	uart_init();
	adc_init();

  uint8_t buffer[BUFFSIZE];          // holds the measured bytes
  unsigned short int bufCounter = 0; // index counter of buffer[]

  uint8_t tempbyteA, tempbyteB;      // temporary bytes (MAX 4 bits - due to the neuman() method)
  unsigned int cnta, cntb = 0;       // counter of how many bits passed through neuman() and were set in the new byte (not discarded)

  uint8_t final_byte;                // this byte is being recursively filled with the tempByte values until we get the necessary 8 bits 
  unsigned int cntfinal;             // counter that keeps track and points to which part of the final_byte we are, so we can keep filling it
  while(1)
	{
    final_byte = 0x00;
    cntfinal = 0;

    while(cntfinal<8){
      tempbyteA = neuman(adc_read(0), &cnta);
       _delay_us(DELAY_US);
      tempbyteB = neuman(adc_read(0), &cntb);

      if (tempbyteA && tempbyteB){ //both not NULL
        if (cnta>cntb){ //get the bigger neuman() byte counter - MAX of 4bits
          fill_byte((tempbyteA^tempbyteB), cnta, &final_byte, cntfinal);
          cntfinal += cnta;
        }
        else{
          fill_byte((tempbyteA^tempbyteB), cntb, &final_byte, cntfinal);
          cntfinal += cntb;
        }
      }
    }


    //final_byte now has 8bits
    buffer[bufCounter] = final_byte; //add ADC measurement to buffer

    if (bufCounter == BUFFSIZE-1){ //buffer is full

      //start to XOR the whole buffer with itself until it ends up with a single byte
      uint8_t xoredByte = buffer[0];
      for (unsigned short int i=1; i<BUFFSIZE; i++){
        xoredByte = xoredByte ^ buffer[i];
      }

      uart_transmit(xoredByte);

      bufCounter = 0; //reset counter
    }
    else{
      bufCounter++;
    }

	}

}
