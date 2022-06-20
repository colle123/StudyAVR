/*
 * LCDTest.c
 *
 * Created: 2022-06-20 오후 2:55:15
 *  Author: PKNU
 */ 

#define F_CPU 7372800UL
#include <stdio.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/delay.h>
#include "Lcd.h"

#define STOP 0
#define GO 1
int state = GO;
Byte *str = " Interrupt !!!";

SIGNAL(INT4_vect){
	
	if(state == GO){
		Lcd_Clear();
		Lcd_Pos(0, 0);
		Lcd_STR(str);
		state = STOP;
	}
	
	else state = GO;
}

int main(){
	
	DDRE = 0x00; // E 입력 0000 0000
	EIMSK = (1 << INT4); // 외부 인터럽트 4, 5번 사용
	EICRB = (1 << ISC41); // 인터럽트 4, 5번 모두 하강에지에서 동작하도록 설계
	
	Byte *str1 = " GuguDan";
	char Temp[20];
	
	DDRA = 0xFF;

	Lcd_Clear();
	LcdInit_4bit();


	sei();
	 
	while(1){
		
			for(int i = 2 ; i < 10 ; i++){
				
				for(int j = 1 ; j < 10 ; j++){
				
					if(state == GO){
						Lcd_Clear();
						Lcd_Pos(0, 0);
						Lcd_STR(str1);
					
						Lcd_Pos(1, 0);
						sprintf(Temp, "%d * %d = %d", i, j, i*j );
						Lcd_STR(Temp);
						_delay_ms(1000);
					}
					
					else if(state == STOP){
						
							while(state == STOP){
							
							sei();
							
							}
					}						
				}
		}		
	}
	
	return 0;
}