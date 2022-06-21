/*
 * TimerCounter2.c
 *
 * Created: 2022-06-21 오후 3:58:18
 *  Author: PKNU
 */ 

#define F_CPU 7372800UL
#include <avr/io.h>
#include <avr/interrupt.h>

volatile unsigned char LED_Data = 0x01;
unsigned char timer2Cnt = 0, Shift_Flag = 0;

SIGNAL(TIMER2_OVF_vect);

int main(){
	
	DDRC = 0x0F;  // 포트 C 를 출력포트로 설정
	
	TCCR2 = 0x05; // 프리스케일러를 1024로 설정
	
	// 256 - 72 = 184 -> 0.01초 마다 한 번씩 Interrupt 발생
	TCNT0 = 184; TIMSK = 0x40;
	TIFR |= 1 << TOV2;
	
	sei();
	
	while(1){
		
		PORTC = LED_Data;
	}
	
	return 0;
}

SIGNAL(TIMER2_OVF_vect){
	
	cli();
	TCNT2 = 184;
	timer2Cnt++;
	
	if(timer2Cnt == 50){
		
		if(Shift_Flag == 0){
			
			// LED0 ~ LED3을 이동
			LED_Data <<= 1;
			if(LED_Data == 0x08)
				Shift_Flag = 1;		
		}
		
		else {
			
			// LED3 ~ LED0으로 이동
			LED_Data >>= 1;
			
			if(LED_Data == 0x01)
				Shift_Flag = 0;
		}
		
		timer2Cnt = 0;
	}
	
	sei();
}