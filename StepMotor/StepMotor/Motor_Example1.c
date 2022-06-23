/*
 * Motor_Example1.c
 *
 * Created: 2022-06-23 오후 3:04:45
 *  Author: PKNU
 */ 

#define  F_CPU 7372800UL
#include <avr/io.h>
#include <util/delay.h>

int main(){
	
	DDRB = 0x20;
	DDRD = 0xF0;
	PORTB &= ~0x20;
	while(1){
		
		PORTD = 0x10;
		_delay_ms(10);
		PORTD = 0x20;
		_delay_ms(10);
		PORTD = 0x40;
		_delay_ms(10);
		PORTD = 0x80;
		_delay_ms(10);
	}
}