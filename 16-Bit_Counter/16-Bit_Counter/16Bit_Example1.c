/*
 * _16Bit_Example1.c
 *
 * Created: 2022-06-22 오전 10:15:56
 *  Author: PKNU
 */ 

#define F_CPU 7372800UL
#include <avr/io.h>
#include <avr/interrupt.h>

volatile unsigned char LED_Data = 0x00;
unsigned char timer = 0;

int main(){
	
	DDRC = 0x0F; // PORTC의 출력을 0000 1111로 설정
	
	TCCR1A = 0x00;
	TCCR1B = (1 << CS12);
	
	TCNT1 = 36736;
	TIMSK = (1 << TOIE1);
	TIFR = (1 << TOV1);
	
	sei();
	
	while(1){
		
		PORTC = LED_Data;
	}
	
	return 0;
}

SIGNAL(TIMER1_OVF_vect){
	
	cli();
	timer++;	
	
	if(timer){	
	LED_Data++;

	if(LED_Data > 0x0F)
		LED_Data = 0;
		
		TCNT1 = 36736;
		timer = 0;
	}
	
	sei();
}