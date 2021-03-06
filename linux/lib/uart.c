#include <avr/io.h>
#include "uart.h"

// function to initialize UART
void uart_init (void)
{
        UBRR0H = (BAUDRATE>>8);                      // shift the register right by 8 bits
        UBRR0L = BAUDRATE;                           // set baud rate
        UCSR0B|= (1<<TXEN0);                                            // enable transmitter
        UCSR0C|= (1<<UMSEL01)|(1<<UCSZ00)|(1<<UCSZ01);   // 8bit data format

        UCSR0A |= (1<<U2X0); //set double speed mode

}

// function to send data
void uart_transmit (uint8_t data)
{
        while (!( UCSR0A & (1<<UDRE0)));                // wait while register is free
        UDR0 = data;                                   // load data in the register
}
