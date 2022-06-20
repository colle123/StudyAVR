/*
 * LED2.c
 *
 * Created: 2022-06-17 오후 1:54:40
 *  Author: PKNU
 */ 

#define F_CPU 7432800
#include <avr/io.h>
#include <util/delay.h>

int main(){
	
	
	DDRA=0xFF;
	DDRB=0xFF;

	int i;
	while(1){

		for(i=0;i<8;i++){ 

			PORTA= ~(0x01<<i); 
			PORTB= (0x80>>i);
			
			_delay_ms(200); 
		}

	}
	
	return 0;
}