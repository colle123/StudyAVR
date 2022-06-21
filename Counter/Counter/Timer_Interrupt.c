/*
 * Timer_Interrupt.c
 *
 * Created: 2022-06-21 오후 4:57:38
 *  Author: PKNU
 */ 

#define F_CPU 7372800UL
#include <avr/io.h>
#include <avr/interrupt.h>

unsigned char FND_DATA_TBL[] = { 0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7C, 0x07, 0x7F, 0x67, 0x77, 0x7C, 0x39, 0x5E, 0x79, 0x71, 0x08, 0x80};
volatile unsigned char time_s = 0; // 초를 세는 변수
volatile unsigned char Time_STOP = 0;
unsigned char timer0Cnt = 0;

int main(){
	
	DDRA = 0xFF; // 포트 A 를 출력포트로 설정
	DDRE = 0x00; // 포트 E 를 입력포트로 설정
	
	TCCR2 = 0x05;  // 프리스케일러 1024
	OCR2 = 72;
	TIMSK = 0x80;
	TIFR |= 1 << OCF2;
	
	EICRB = 0x03;  // 인터럽트 4를 상승엣지에서 동작하도록 설정
	EIMSK = 0x10;  // 인터럽트 4를 허용
	EIFR = 0x10;   // 인터럽트 4 플래그를 클리어
	
	sei();
	
	while(1){
		
		PORTA = FND_DATA_TBL[time_s]; // 포트A에 FND Table 값을 출력
	}
	
	return 0;
}

SIGNAL(TIMER2_COMP_vect){
	
	cli();	    // 전체 인터럽트를 금지
	OCR2 += 72; // 0.01초 후에 인터럽트 발생
	timer0Cnt++; // timer0Cnt 변수를 1 증가
	
	if(timer0Cnt == 100){
		
		// 0.01 * 50 = 0.5s 0.5초를 얻기 위한 카운트 횟수
		if(Time_STOP == 0) // Time_STOP 이 0인 경우에만
		{
			if(time_s >= 16)
				time_s = 0;
			else
				time_s++;
		}
		timer0Cnt = 0;
	}
	sei();
}

SIGNAL(INT4_vect)
{
	cli();
	
	Time_STOP = ~Time_STOP;
		
	sei();
}