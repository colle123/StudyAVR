/*
 * Test1.c
 *
 * Created: 2022-06-17 오전 11:45:14
 *  Author: PKNU
 */ 

#include <avr/io.h>
#include <util/delay.h>

int main(){
	
	DDRA = 0xFF;
	//DDRB = 0xFF; // portA를 출력으로 0b 1111 1111(F면 모든 Bit가 출력으로 잡힘)
	//DDRC = 0xFF; // portA를 출력으로 0b 1111 1111(F면 모든 Bit가 출력으로 잡힘)
	//DDRD = 0xFF; // portA를 출력으로 0b 1111 1111(F면 모든 Bit가 출력으로 잡힘)
	//DDRE = 0xFF; // portA를 출력으로 0b 1111 1111(F면 모든 Bit가 출력으로 잡힘)
	//DDRF = 0xFF; // portA를 출력으로 0b 1111 1111(F면 모든 Bit가 출력으로 잡힘)
	//DDRG = 0xFF; // portA를 출력으로 0b 1111 1111(F면 모든 Bit가 출력으로 잡힘)
	
	while(1){
		
		for(int i = 0 ; i < 2 ; i++){
			
			PORTA = 0xAA; // PORTA의 각 비트에 5V 출력 1010 1010 12 12 123456789ABC
			_delay_ms(500);
			PORTA = 0x55;
			_delay_ms(500);
		}
		
		//PORTB = 0xFF; // PORTA의 각 비트에 5V 출력
		//PORTC = 0xFF; // PORTA의 각 비트에 5V 출력
		//PORTD = 0xFF; // PORTA의 각 비트에 5V 출력
		//PORTE = 0xFF; // PORTA의 각 비트에 5V 출력
		//PORTF = 0xFF; // PORTA의 각 비트에 5V 출력
		//PORTG = 0xFF; // PORTA의 각 비트에 5V 출력
		
	}
	
	return 0;
}