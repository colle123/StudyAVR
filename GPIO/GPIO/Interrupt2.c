/*
 * Interrupt2.c
 *
 * Created: 2022-06-20 오전 10:39:37
 *  Author: PKNU
 */ 

#define F_CPU 7432800UL
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

volatile unsigned char count = 0;


char FND1[] = {0x3f, 0x06, 0x5b, 0x4f, 0x66, 0x6d, 0x7d, 0x27, 0x7f, 0x67, 0x77, 0x7c, 0x39, 0x5e, 0x79, 0x71}; // 0 1 2 3 4 5 6 7 8 9 A B C D E F
char FND2[] = {0x70, 0xB0, 0xD0, 0xE0}; // 0111, 1011, 1101, 1110 // 애노드라 0이 들어가야 켜짐

int SW_count[4];

SIGNAL(INT4_vect){
	
	if(PORTA == 0X00) 
		PORTA = 0XFF;
    else
		PORTA = 0X00;
}

int main(){
	
	DDRE = 0x00;
	DDRB = 0xF0; // PORTB 의 출력 1111 FND의 C0~C3까지 연결
	DDRA = 0xff;
	EIMSK = (1 << INT4); 
	EICRA = (1 << ISC01) | (0 << ISC01); //EICRA = 0x03; // 0b 0000 0011 = INT의 상승에지에서 인터럽트를 발생시킴.
	sei();
	
	while(1){
		
			count++;

			SW_count[3] = (count) % 10;
			SW_count[2] = (count / 10) % 10;
			SW_count[1] = (count / 100) % 10;
			SW_count[0] = (count / 1000) % 10;

			
			for(int j = 0 ; j < 4 ; j++){
				
				PORTB = FND2[j];
				PORTA = FND1[SW_count[j]];
				_delay_ms(10);

			}
		
	};
	
	return 0;
}