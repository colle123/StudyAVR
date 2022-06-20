/*
 * FND_Interrupt2.c
 *
 * Created: 2022-06-20 오후 12:22:27
 *  Author: PKNU
 */ 

#include <avr/io.h> // AVR 입출력에 대한 헤더 파일
#include <avr/interrupt.h> // AVR 인터럽트에 대한 헤더파일
#include <util/delay.h> // Delay 함수사용을 위한 헤더파일

volatile unsigned char Time_STOP = 0;
volatile unsigned char cnt = 0;

SIGNAL(INT4_vect){
	
	cli();
	
	Time_STOP = 1;
	cnt = 0;
	
	sei();
}

SIGNAL(INT6_vect){
	
	cli();
	
	Time_STOP = 0;
	
	sei();
}

int main(){
	
	unsigned char FND_DATA_TBL[] = {0x3F, 0x06, 0x5b, 0x4f, 0x66, 0x6d, 0x7c, 0x07, 0x7f, 0x67, 0x77, 0x7c, 0x39, 0x5e, 0x79, 0x71, 0x08, 0x80};
	
	DDRA = 0xff; // A 출력 1111 1111
	DDRE = 0x00; // E 입력 0000 0000
	
	EICRB = 0x32; // 인터럽트 4를 하강엣지, 6을 상승엣지에서 동작하도록 설정
	EIMSK = 0x50; // 인터럽트 4, 6 허용
	EIFR = 0x50; // 인터럽트 4, 6 플래그를 클리어
	sei();
	
	while(1){
		
		PORTA = FND_DATA_TBL[cnt];
		if(Time_STOP == 0){
			
			cnt++;
			if(cnt > 17) cnt = 0;
		}
		
		_delay_ms(1000);
		
	}
}
