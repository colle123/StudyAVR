/*
 * PWM_LED2.c
 *
 * Created: 2022-06-22 오후 4:35:33
 *  Author: PKNU
 */ 

#define F_CPU 7372800UL
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

volatile int Light = 0;
volatile unsigned Light_Flag = 1;

int main(){
	
	DDRB = 0x80; // 출력포트 B 0x 1000 0000	
	DDRE = 0x00; // 입력포트 E 0x 0000 0000
	// PC PWM모드, 8분주, PWM 주기 : F_CPU(7372800)/256/2/8 = 1.8KHz, 업카운트시 Clear, 다운카운트시 Set 으로 설정 0110 0010
	TCCR2 = (1 << WGM20) | (1 << COM21) | (0 << COM20) | (0 << CS22) | (1 << CS21) | (1 << CS20);; 
	
	TCNT2 = 0x00;
	
	// 0xFF 인터럽트 4, 5, 6, 7을 상승엣지에서 동작하도록 설정
	EICRB = (1 << ISC71) | (1 << ISC70) | (1 << ISC61) | (1 << ISC60) | (1 << ISC51) | (1 << ISC50) | (1 << ISC41) | (1 << ISC40);

	EIMSK = (1 << INT7) | (1 << INT6) | (1 << INT5) | (1 << INT4); //0xF0 인터럽트 4, 5, 6, 7을 허용
	EIFR = (1 << INTF7) | (1 << INTF6) | (1 << INTF5) | (1 << INTF4);// 0xF0 인터럽트 4, 5, 6, 7 Flag 를 Clear
	
	sei();
	
	while(1){
		
		if(Light_Flag)
		{
			OCR2 = Light; // Light 값에 따라 밝기 제어
			Light_Flag = 0;
		}
	}
}

SIGNAL(INT4_vect)  // 인터럽트 서비스 루틴
{
	cli();
	Light = 0;
	Light_Flag = 1;
	sei();
}

SIGNAL(INT5_vect)
{
	cli();
	Light -= 51;  // LED 밝기 감소
	if(Light < 0)
	Light = 0;
	Light_Flag = 1;
	sei();
}

SIGNAL(INT6_vect)
{
	cli();
	Light += 51;  // LED 밝기 증가
	if(Light > 255)
	Light = 255;
	Light_Flag = 1;
	sei();
}

SIGNAL(INT7_vect)
{
	cli();
	Light = 255;
	Light_Flag = 1;
	sei();
}