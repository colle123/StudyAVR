/*
 * PWM_LED.c
 *
 * Created: 2022-06-22 오후 4:24:33
 *  Author: PKNU
 */ 

#define F_CPU 7372800UL
#include <avr/io.h>
#include <util/delay.h>

int main(){
	
	unsigned char Light = 0;
	
	DDRA = 0x10; // 포트 B 를 출력포트로 설정 0x 0001 0000
	
	
	TCCR0 = (1 << WGM00) | (1 << COM01) | (1 << COM00) | (1 << CS02) | (0 << CS01) | (1 << CS00); //0x77 분주비 128
	TCNT0 = 0x00;
	
	while(1){
		
		for(Light = 0 ; Light < 255 ; Light++)
		{
			OCR0 = Light;
			_delay_ms(5);			
		}
		for(Light = 255 ; 0 < Light	; Light--)
		{
			OCR0 = Light;
			_delay_ms(5);			
		}
	}
}