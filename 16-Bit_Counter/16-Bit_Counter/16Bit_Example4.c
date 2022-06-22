/*
 * _16Bit_Example3.c
 *
 * Created: 2022-06-22 오후 12:14:19
 *  Author: PKNU
 */ 

#define F_CPU 7372800UL
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

unsigned char FND_DATA_TBL[] = { 0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7C, 0x07, 0x7F, 0x67, 0x77, 0x7C, 0x39, 0x5E, 0x79, 0x71, 0x08, 0x80};
unsigned char FND2[] = {0x70, 0xB0, 0xD0, 0xE0}; 
	
unsigned int SW_count[4];

volatile unsigned char Stop_time = 0;	
volatile unsigned char time_s = 0;
volatile unsigned char FND_flag = 0, edge_flag = 0;

int FND_Count(char count){

	SW_count[0] = (count) % 10;
	SW_count[1] = (count / 10) % 10;
	SW_count[2] = (count / 100) % 10;
	SW_count[3] = (count / 1000) % 10;

	for(int j = 0 ; j < 4 ; j++){
		
		PORTB = FND2[j];
		PORTA = FND_DATA_TBL[SW_count[j]];
		_delay_ms(5);

	}

};

int main(){
	
	DDRA = 0xFF;
	DDRB = 0xF0;
	DDRE = 0x00;
	
	TCCR3A = 0x00;
	TCCR3B = (1 << ICES3) | (1 << CS32) | (1 << CS30); // 0x45, 프리스케일러 1024, 상승(양)에지 캡쳐 트리거 설정
	ETIMSK = (1 << TICIE3);
	ETIFR = (1 << ICF3);
	
	sei();
	
	while(1) {
		
		FND_Count(Stop_time); // 포트 A 에 FND Table 값을 출력
		
		if(FND_flag) {
			
			if(time_s > 9999)  // 
				time_s = 9999;
				
			FND_Count(time_s); // 포트 A 에 FND Table 값 출력
			FND_flag = 0;
		}
	}
	
	return 0;
}

SIGNAL(TIMER3_CAPT_vect) {
	
	cli();
	
	unsigned int count_check;
	
	// 스위치가 눌릴 시간 측정을 위해
	// 상승에지에서 하강에지까지의 시간을 계산
	
	if(edge_flag == 0) { // 상승 에지(스위치를 누르면)
		
		TCCR3B = (1 << CS32) | (1 << CS30); // 프리스케일러 1024, 하강(음)에지 캡처 트리거 설정
		TCNT3 = 0;	                        // TCNT3 레지스터를 0으로 초기화
		ICR3 = 0;							// ICR3 레지스터를 0으로 초기화
		edge_flag = 1;
	}
	
	else // 하강 에지(스위치를 떼면), 프리스케일러 1024, 상승(양)에지 캡처 트리거 설정
	{
		TCCR3B = (1 << ICES3) | (1 << CS32) | (1 << CS30);
		count_check = ICR3;
		
		time_s = count_check / 720; // 누를 시간을 0.1초 단위로 변경
		Stop_time = time_s;
		
		FND_flag = 1; 
		edge_flag = 0;
		
	}
	
	sei();
}