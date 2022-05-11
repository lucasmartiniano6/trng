/*
Author: Lucas Martiniano
Date: 28-10-2020

AVR IMPLEMENTATION FOR HARDWARE-BASED TRUE RANDOM NUMBER GENERATOR (AVALANCHE NOISE)
*/

#ifndef F_CPU
#define F_CPU 16000000UL // 16 MHz clock speed
#endif

//do i need to include these avr/ here as well?
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#include "lib/uart.h"
#include "lib/adc.h"

int main(void){
  uart_init();
  adc_init();
  
  while(1){
    uart_transmit(adc_read(0));
    _delay_ms(1000);
  }
  return 0;
}
