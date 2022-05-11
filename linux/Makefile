.PHONY: all burn clean tags

TARGET = main

dirs = *.c lib/*.c

SRCS := $(wildcard $(dirs))
OBJECT_FILES := $(SRCS:%.c=%.o)

PORT=/dev/ttyUSB0
MCU=atmega328p

CC=avr-gcc
CFLAGS=-g -Wall -mcall-prologues -mmcu=$(MCU) -Os
LDFLAGS=-Wl,-gc-sections -Wl,-relax

all: $(TARGET).hex

%.hex: %.obj
	avr-objcopy -R .eeprom -O ihex $< $@

%.obj: $(OBJECT_FILES)
	$(CC) $(CFLAGS) $(OBJECT_FILES) $(LDFLAGS) -o $@

burn: $(TARGET).hex
	sudo avrdude -p $(MCU) -c arduino -P $(PORT) -b 57600 -D -U flash:w:$(TARGET).hex

clean:
	@echo "Cleaning files"
	rm -f *.o *.obj *.hex

tags:
	@echo "Updating tags"
	ctags -R .
