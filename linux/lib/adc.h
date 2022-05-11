#ifndef ADC_SEEN
#define ADC_SEEN

//ADC MACROS
#define BUFFSIZE 2 //each buffer index holds one byte
#define DELAY_US 5 //delay in microseconds(us) before each adc measurement

//function to initialize ADC
void adc_init();

//adc running on single conversion mode
uint8_t adc_read(uint8_t ch);

//von neuman byte correction
uint8_t neuman(uint8_t bRecv, unsigned int* cnt);


#endif
