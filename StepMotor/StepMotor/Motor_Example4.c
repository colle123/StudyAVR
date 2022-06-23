/*
 * Motor_Example4.c
 *
 * Created: 2022-06-23 오후 3:35:44
 *  Author: PKNU
 */ 

#define  F_CPU 7372800UL
#include <avr/io.h>
#include <avr/interrupt.h>

#define DIR_L 0
#define DIR_R 1

unsigned char timer0Cnt, mot_cnt = 0;
volatile unsigned char dir = DIR_R; // 처음 방향은 우측부터, 1-2상 여자 값을 사용
unsigned char Step[8] = {0x90, 0x80, 0xC0, 0x40, 0x60, 0x20, 0x30, 0x10};

SIGNAL(TIMER0_OVF_vect);  // Timer0 Overflow0 ISP
SIGNAL(TIMER1_OVF_vect);  // Timer1 Overflow1 ISP

int main(){
	
	DDRD = 0xF0;
	
	TCCR0 = 0x07; // 0x 0000 0111 (1 << CS02) | (1 << CS01) | (1 << CS00), 1024분주
	TCNT0 = 184;
	TCCR1A = 0; // WGM(1:0) = "00"
	TCCR1B = 0x04; // WGM(3:2) = "00", (1 << CS12)
	
	// (1/(7372800 / 256)) * 634 => 21.99ms => 22ms => 45kHz
	
	// 65536 - 634 = 64902 = 0xFD86
	TCNT1H = 0xFD; // 0X 1111 1101 (1 << TCNT115) | (1 << TCNT114) | (1 << TCNT113) | (1 << TCNT112) | (1 << TCNT111) | (1 << TCNT110) | (1 << TCNT18);
	TCNT1L = 0x86; // 0x 1000 0110 (1 << TCNT17) | (1 << TCNT12) | (1 << TCNT11);
	TIMSK = 0x05; // 0x 0000 0101 (1 << TOIE1) | (1 << TOIE0);
	TIFR = 0x05; // 0x 0000 0101 (1 << TOV1) | (1 << TOV0);
	
	sei();
	
	while(1){ }
}

SIGNAL(TIMER0_OVF_vect){
	
	cli();
	
	TCNT0 = 184;
	timer0Cnt++;
	if(timer0Cnt == 200){
		
		dir^=1;
		timer0Cnt = 0;
	}
	
	sei();
}

SIGNAL(TIMER1_OVF_vect){
	
	cli();
	TCNT1H = 0xFB;
	TCNT1L = 0x0D;
	
	PORTD = Step[mot_cnt];
	if(dir == DIR_R){
		
		if(mot_cnt++==7) mot_cnt = 0;
	}
	else if(mot_cnt --== 0) mot_cnt = 7;
	sei();
}