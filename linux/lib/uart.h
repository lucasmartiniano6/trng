#ifndef UART_SEEN
#define UART_SEEN

#ifndef F_CPU
#define F_CPU 16000000UL // 16 MHz clock speed
#endif

//UART MACROS
#define BAUD 9600       // define baud bps
#define BAUDRATE (((F_CPU)/(BAUD*8UL)) -1)           // set baud rate value for UBRR

// function to initialize UART
void uart_init(void);

// function to send data
void uart_transmit (uint8_t data);

#endif 
