/*
 * input1.c
 *
 * Created: 2022-06-17 오후 3:20:33
 *  Author: PKNU
 */ 

#define F_CPU 7432800UL
#include <avr/io.h>
#include <util/delay.h>

int main(){

	DDRD = 0x00; // 포트 A 입력으로 설정( 0x 0000 0000)
	DDRA = 0xFF; // 포트 B 출력으로 설정( 0x 0000 0000)

	while(1){
	
		PORTA = PIND;
	}

	return 0;
}