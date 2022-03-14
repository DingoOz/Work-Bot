// avr-gcc -mmcu=atmega32u4 -Os -DF_CPU=8000000UL blink.c -o blink.o
// avr-objcopy -O ihex blink.o blink.hex

#include <avr/io.h>
#include "cpu_speed.h"
#include <util/delay.h>

int main(){
    set_clock_speed(CPU_8MHz);

    DDRB =  DDRB | 0b00010000;
    DDRD =  DDRD | 0b00010000;
    DDRD =  DDRD | 0b00100000;

    DDRF = DDRF & 0b11011111;
    while(1){
		PORTD = PORTD & 0b11011111;
		PORTB = PORTB | 0b00010000;
		_delay_ms(500);
		PORTB = PORTB & 0b11101111;
		PORTD = PORTD | 0b00010000;
		_delay_ms(500);
		PORTD = PORTD & 0b11101111;
		PORTD = PORTD | 0b00100000;
		_delay_ms(500);
    }
return 0;
}