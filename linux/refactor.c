#include <stdint.h>

void sendByte(uint8_t final, uint8_t buffer[BUFFSIZE]){

  //start to XOR the whole buffer with itself until it ends up with a single byte
  uint8_t xoredByte = buffer[0];
  for (unsigned short int i=1; i<BUFFSIZE; i++){
    xoredByte = xoredByte ^ buffer[i];
  }

  uart_transmit(xoredByte);

}

int main(void)
{

	uart_init();
	adc_init();

  uint8_t buffer[BUFFSIZE];          // holds the measured bytes
  unsigned short int bufCounter = 0; // index counter of buffer[]

  uint8_t tempbyteA, tempbyteB;      // temporary bytes (MAX 4 bits - due to the neuman() method)
  uint8_t final_byte;                // this byte is being filled with the tempByte values until we get the necessary 8 bits 

 while(1)
{
  __delay_us(DELAY_US);
  tempbyteA = neuman(adc_read(0));
   _delay_us(DELAY_US);
  tempbyteB = neuman(adc_read(0));
  //neuman() returns MAX of 4 bits

  if (tempbyteA && tempbyteB){ //both not NULL

    final_byte = 0x00;

    final_byte = final_byte | (tempbyteB<<4);
    final_byte = final_byte | tempbyteA;
    //final_byte now has 8bits

    buffer[bufCounter] = final_byte; //add ADC measurement to buffer

    if (bufCounter == BUFFSIZE-1){
      sendByte(final_byte, buffer);
      bufCounter = 0;
    }
    else{
      ++bufCounter;
    }
  }

}//end while()

}//end main()
