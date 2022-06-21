/*
 * TimerCounter_4FND.c
 *
 * Created: 2022-06-21 오후 5:13:19
 *  Author: PKNU
 */ 
#define  F_CPU 7432800UL
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#define STOP 0
#define GO 1
int state = STOP;

char FND1[] = {0x3f, 0x06, 0x5b, 0x4f, 0x66, 0x6d, 0x7d, 0x27, 0x7f, 0x67}; // 0x77, 0x7c, 0x39, 0x5e, 0x79, 0x71}; // 0 1 2 3 4 5 6 7 8 9 A B C D E F
char FND2[] = {0x70, 0xB0, 0xD0, 0xE0}; // 0111, 1011, 1101, 1110 // 애노드라 0이 들어가야 켜짐

int SW_count[4];
int count = 0;
int Stop_count = 0;

unsigned char timer0Cnt = 0;

SIGNAL(INT4_vect){
	
	if(state == STOP){
		state = GO;
	}
	else if(state == GO){
		
		state = STOP;
		Stop_count = count;
	}
}

SIGNAL(INT5_vect){
	
	count = 0;
	Stop_count = 0;
}

int FND_Count(int count){

	SW_count[0] = (count) % 10;
	SW_count[1] = (count / 10) % 10;
	SW_count[2] = (count / 100) % 10;
	SW_count[3] = (count / 1000) % 10;

	for(int j = 0 ; j < 4 ; j++){
		
		PORTB = FND2[j];
		PORTA = FND1[SW_count[j]];
		_delay_ms(5);

	}
	
	if(count >= 9999){
		
		count = 0;
		Stop_count = 0;
	}
	
};

SIGNAL(TIMER2_COMP_vect){
	
	cli();	    // 전체 인터럽트를 금지
	OCR2 += 72; // 0.01초 후에 인터럽트 발생
	timer0Cnt++; // timer0Cnt 변수를 1 증가
	
	if(timer0Cnt == 100){
		
		// 0.01 * 50 = 0.5s 0.5초를 얻기 위한 카운트 횟수
		if(state == GO) // Time_STOP 이 0인 경우에만
		{
			count++;
		}
		timer0Cnt = 0;
	}
	sei();
}

int main(){

	DDRA = 0xFF; // PORTA 의 출력 1111 1111 FND의 A~H 까지 연결
	DDRB = 0xF0; // PORTB 의 출력 1111 FND의 C0~C3까지 연결
	DDRE = 0x00; // PORTE 의 입력 0000 0000 E 의 입력은 스위치와 연결
	
	TCCR2 = 0x05;  // 프리스케일러 1024
	OCR2 = 72;
	TIMSK = 0x80;
	TIFR |= 1 << OCF2;
	
	EIMSK = (1 << INT4) | (1 << INT5); // 외부 인터럽트 4, 5번 사용
	EICRB = (1 << ISC41) | (1 << ISC51); // 인터럽트 4, 5번 모두 하강에지에서 동작하도록 설계
	EIFR = 0x10;   // 인터럽트 4 플래그를 클리어
	sei();

	while(1){
		
		if(state == GO){
			FND_Count(count);
		}
		else if(state == STOP){
			FND_Count(Stop_count);
		}
		
	}

	return 0;
}
