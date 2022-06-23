/*
 * Motor_Example3.c
 *
 * Created: 2022-06-23 오후 3:17:35
 *  Author: PKNU
 */ 

#define  F_CPU 7372800UL
#include <avr/io.h>
#include <util/delay.h>

unsigned char Step[8] = {0x90, 0x80, 0xC0, 0x40, 0x60, 0x20, 0x30, 0x10};

int main(){
	
	unsigned char i, t = 0;

	DDRD = 0xF0;
	
	while(1){
		
		for( i = 0 ; i < 24 ; i++ ){
			PORTD = Step[t];
			t++;
			if(t > 7) t = 0;
			_delay_ms(10);
		}
		_delay_ms(1000);
	}
}