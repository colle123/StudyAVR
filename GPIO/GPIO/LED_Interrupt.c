/*
 * LED_Interrupt.c
 *
 * Created: 2022-06-20 오전 11:33:22
 *  Author: PKNU
 */ 

#include <avr/io.h> // AVR 입출력에 대한 헤더 파일
#include <avr/interrupt.h> // AVR 인터럽트에 대한 헤더파일
#include <util/delay.h> // Delay 함수사용을 위한 헤더파일

volatile unsigned char Shift_flag = 1;

SIGNAL(INT5_vect){
	
	cli();
	Shift_flag = 1;
	sei();
	
}

SIGNAL(INT7_vect){
	
	cli();
	Shift_flag = 2;
	sei();
	
}

int main() {
	
	unsigned char LED_Data = 0x01; // LED_Data를 저장
	
	DDRC = 0x0F; // 0b 0000 1111 포트 C 를 출력포트로 설정
	DDRE = 0x00; // 0b 0000 0000 포트 E 를 입력포트로 설정
	
	EICRB = 0xC8; // 0b 1100 1000 인터럽트 7을 상승에지, 인터럽트 5를 하강에지로 동작
	
	EIMSK = 0xA0; // 0b 1010 0000 인터럽트 5, 7번을 사용.
	EIFR = 0xA0; // 0b 1010 0000 인터럽트 5, 7번 플래그를 클리어
	
	sei(); // 전체 인터럽트를 허용
	
	while(1){
		
		// 포트 C 로 변수 LED_Data에 있는 데이터를 출력
		
		PORTC = LED_Data;
		
		if(Shift_flag == 1){ // LED 0 ~ LED 3으로 이동
			
			if(LED_Data == 0x08) LED_Data = 0x01;
			
			// LED_Data 값을 왼쪽으로 쉬프트			
			else LED_Data <<= 1;
		}
		
		else if(Shift_flag == 2) // LED3 ~ LED1 으로 이동
		{
			if(LED_Data == 0x01) LED_Data = 0x08;
			else LED_Data >>= 1; // LED_Data 값을 오른쪽으로 쉬프트
		}
		_delay_ms(500);
	}
	
	
}