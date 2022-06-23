/*
 * PWM_PIEZO.c
 *
 * Created: 2022-06-22 오후 5:09:27
 *  Author: PKNU
 */ 

#define F_CPU 7372800UL
#include <avr/io.h>
#include <util/delay.h>

unsigned int DoReMi[8] = {523, 587, 659, 698, 783, 880, 987, 1046};
	
int main(){
	
	unsigned char piano = 0;
	
	DDRB = 0x80; // 0x 1000 0000 PE7(OC1C핀) PWM 출력
	
	// 0x0A = 0x 0000 1100, COM1C(1:0) = "10", OC1C핀 사용, WGM(1:0) = "10"
	TCCR1A |= 0x0A; // (1 << COM1C1) | (1 << COM1C0);
	// 0x19 = 0x 0001 1001, WGM3(3:2) = "11", CS3(2:0) = "001" 1분주 사용 
	TCCR1B |= 0x19; // (1 << WGM13) | (1 << WGM12) | (1 << CS10);   
	TCCR1C = 0x00;    // WGM3(3:0) = "1110", Fast PWM, 모드 14
	TCNT1 = 0x0000;   // 타이머1 카운터 초기화
	
	while(1){
		
		ICR1 = 7372800 / DoReMi[piano];
		OCR1C = ICR1/32; // Duty 조절 
		piano++;
		if(8 < piano) piano = 0;
		_delay_ms(1000);
	}	
}
