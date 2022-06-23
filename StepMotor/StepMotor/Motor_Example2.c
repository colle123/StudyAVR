/*
 * Motor_Example2.c
 *
 * Created: 2022-06-23 오후 3:11:44
 *  Author: PKNU
 */ 

#define  F_CPU 7372800UL
#include <avr/io.h>
#include <util/delay.h>

int main(){
	
	unsigned char i;

	DDRD = 0xF0;
	
	while(1){
		
		for( i = 0 ; i < 12 ; i++ ){
		PORTD = 0x30;
		_delay_ms(10);
		PORTD = 0x90;
		_delay_ms(10);
		PORTD = 0xC0;
		_delay_ms(10);
		PORTD = 0x60;
		_delay_ms(10);
		}
		_delay_ms(1000);
		
		for( i = 0 ; i < 12 ; i++ ){
		
		PORTD = 0x10;
		_delay_ms(10);
		PORTD = 0x20;
		_delay_ms(10);
		PORTD = 0x40;
		_delay_ms(10);
		PORTD = 0x80;
		_delay_ms(10);
		
		}
		_delay_ms(1000);
	}
}
