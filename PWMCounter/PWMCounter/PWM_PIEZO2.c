/*
 * PWM_PIEZO2.c
 *
 * Created: 2022-06-23 오전 10:11:07
 *  Author: PKNU
 */ 

#define F_CPU 7372800UL
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

unsigned int DoReMi[8] = {261, 293, 329, 349, 391, 440, 493, 523};
volatile unsigned char sound_flag = 1;

int main(){	
	
	DDRE = 0x08; // 0x 0000 1000 => 포트E PE3를 출력, 나머지는 입력포트로 설정, PE3을 Buzzer에 연결
	DDRB = 0x00; //
	
	// 0x00 = WGM3(1:0) = "00" Normal, TOP = 0xFF
	TCCR3A |= 0x00; 
	
	// 0x19 = 0x 0001 1001, WGM3(3:2) = "11", CS3(2:0) = "001" 1분주 사용
	TCCR3B |= 0x19; // (1 << WGM13) | (1 << WGM12) | (1 << CS10);
	TCCR3C = 0x00;    // WGM3(3:0) = "1110", Fast PWM, 모드 14
	TCNT3 = 0x0000;   // 타이머1 카운터 초기화
	
	// 0xFF = 0x 1111 1111 인터럽트 0, 1, 2, 3를 상승엣지에서 동작하도록 설정
	//EICRA = (1 << ISC31) | (1 << ISC30) | (1 << ISC21) | (1 << ISC20) | (1 << ISC11) | (1 << ISC10) | (1 << ISC01) | (1 << ISC00);
	// 0xFF = 0x 1111 1111 인터럽트 4, 5, 6, 7를 상승엣지에서 동작하도록 설정
	//EICRB = (1 << ISC71) | (1 << ISC70) | (1 << ISC61) | (1 << ISC60) | (1 << ISC51) | (1 << ISC50) | (1 << ISC41) | (1 << ISC40);
	// 0xFF = 0x 1111 1111 인터럽트 0, 1, 2, 3, 4, 5, 6, 7 을 동작하도록 설정
	//EIMSK = (1 << INT7) | (1 << INT6) | (1 << INT5) | (1 << INT4) | (1 << INT3) | (1 << INT2) | (1 << INT1) | (1 << INT0);
	// 0xFF = 0x 1111 1111 인터럽트 0, 1, 2, 3, 4, 5, 6, 7 플래그를 클리어
	//EIFR = (1 << INTF7) | (1 << INTF6) | (1 << INTF5) | (1 << INTF4) | (1 << INTF3) | (1 << INTF2) | (1 << INTF1) | (1 << INTF0);
	
	EICRA = 0xff;
	EICRB = 0xff;  
	EIMSK = 0xff;  
	EIFR = 0xff;
	
	sei();
	
	while(1){
		
		if(sound_flag){
			
			_delay_ms(500);
			TCCR3A = 0x00; // 부저 소리를 끈다.
			sound_flag = 0;
		}
	}
}

SIGNAL(INT0_vect){
	
	cli();
	ICR3 = 7372800 / DoReMi[0] / 6; // 도의 음향을 연주한다
	TCCR3A = 0x40;				// PE4로 출력 0x 0100 0000
	sound_flag = 1;
	sei();
}

SIGNAL(INT1_vect){
	
	cli();
	ICR3 = 7372800 / DoReMi[1] / 6; // 레의 음향을 연주한다
	TCCR3A = 0x40;				// PE4로 출력
	sound_flag = 1;
	sei();
}

SIGNAL(INT2_vect){
	
	cli();
	ICR3 = 7372800 / DoReMi[2] / 6; // 미의 음향을 연주한다
	TCCR3A = 0x40;				// PE4로 출력
	sound_flag = 1;
	sei();
}

SIGNAL(INT3_vect){
	
	cli();
	ICR3 = 7372800 / DoReMi[3] / 6; // 파의 음향을 연주한다
	TCCR3A = 0x40;				// PE4로 출력
	sound_flag = 1;
	sei();
}

SIGNAL(INT4_vect){
	
	cli();
	ICR3 = 7372800 / DoReMi[4] / 6; // 솔의 음향을 연주한다
	TCCR3A = 0x40;				// PE4로 출력
	sound_flag = 1;
	sei();
}

SIGNAL(INT5_vect){
	
	cli();
	ICR3 = 7372800 / DoReMi[5] / 6; // 라의 음향을 연주한다
	TCCR3A = 0x40;				// PE4로 출력
	sound_flag = 1;
	sei();
}

SIGNAL(INT6_vect){
	
	cli();
	ICR3 = 7372800 / DoReMi[6] / 6; // 시의 음향을 연주한다
	TCCR3A = 0x40;				// PE4로 출력
	sound_flag = 1;
	sei();
}

SIGNAL(INT7_vect){
	
	cli();
	ICR3 = 7372800 / DoReMi[7] / 6; // 도의 음향을 연주한다
	TCCR3A = 0x40;				// PE4로 출력
	sound_flag = 1;
	sei();
}
