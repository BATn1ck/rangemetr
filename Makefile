.PHONY: load compile default build clean

CC = avr-gcc
LD = avr-ld
GET_HEX = avr-objcopy
DUDE = avrdude
DUDE_FLAGS = -b 9600 -p atmega8 -P usb -v -c usbasp -U flash:w:main.hex:i
CCFLAGS = -DF_CPU=8000000UL -g -Os -mmcu=atmega8 -Wall
HEX_FLAGS = -j .text -j .bss -j .data -O ihex main.elf main.hex

compile:
	$(CC) $(CCFLAGS) -c lcd.c -o lcd.o
	$(CC) $(CCFLAGS) lcd.o main.c -o main.elf
	$(GET_HEX) $(HEX_FLAGS)	

load:
	$(DUDE) $(DUDE_FLAGS)

build: compile load

default: compile

clean:
	rm -fv *.o *.elf *.hex
