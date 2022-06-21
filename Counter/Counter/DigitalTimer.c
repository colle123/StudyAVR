/*
 * DigitalTimer.c
 *
 * Created: 2022-06-21 오후 4:12:25
 *  Author: PKNU
 */ 

#define F_CPU 7372800UL
#include <avr/io.h>
#include <avr/interrupt.h>

unsigned char FND_DATA_TBL[] = { 0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7C, 0x07, 0x7F, 0x67, 0x77, 0x7C, 0x39, 0x5E, 0x79, 0x71, 0x08, 0x80};
volatile unsigned char time_s = 0; // 초를 세는 변수
unsigned char timer0Cnt = 0;

int main(){
	
	DDRA = 0xFF;  // 포트A 를 출력포트를 설정
	
	TCCR0 = 0x07; // 프리스케일러(분주비) 1024
	OCR0 = 72;   // 0.01초 마다 한 번씩 인터럽트 발생
	TIMSK = 0x02; // 출력 비교 인터럽트 활성화
	TIFR |= 1 << OCF0;
	
	sei();
	
	while(1){
		
		PORTA = FND_DATA_TBL[time_s];
	}
	
	return 0;
	
}

SIGNAL(TIMER0_COMP_vect){
	
	cli();
	OCR0 += 72;    // 0.01초 후에 인터럽트 발생
	timer0Cnt++;    // timer0Cnt 변수 1 증가
	
	// 0.01s * 100 1s
	if(timer0Cnt == 100){ // 1초를 얻기 위한 카운트 횟수
		
		if(time_s >= 16) // time_s 변수는 16까지만 증가
			time_s = 0; 
			
		else {time_s++;} // 16이 되면 0으로 초기화 
			timer0Cnt = 0;
	}
	
	sei();
}
