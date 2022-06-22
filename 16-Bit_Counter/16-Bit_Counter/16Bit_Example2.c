/*
 * _16Bit_Example2.c
 *
 * Created: 2022-06-22 오전 11:43:55
 *  Author: PKNU
 */ 

#define F_CPU 7372800UL
#include <avr/io.h>
#include <avr/interrupt.h>

unsigned char FND_DATA_TBL[] = { 0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7C, 0x07, 0x7F, 0x67, 0x77, 0x7C, 0x39, 0x5E, 0x79, 0x71, 0x08, 0x80};
volatile unsigned char time_s = 0; // 초를 세는 변수

int main(){
	
	DDRA = 0xFF;
	
	TCCR1A = 0x00;
	TCCR1B = (1 << CS12) | (1 << CS10);
	
	OCR1A = 7200;
	TIMSK = (1 << OCIE1A);
	TIFR = (1 << OCF1A);
	
	sei();
	
	while(1){
		
		PORTA = FND_DATA_TBL[time_s];
	}
	
	return 0;
}

SIGNAL(TIMER1_COMPA_vect){
	
	cli();
	
	OCR1A = 7200;
		if(time_s >= 17)
			time_s = 0;
		else
			time_s++;
			
	sei();
}