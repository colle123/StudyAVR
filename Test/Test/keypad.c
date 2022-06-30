/*
 * keyT.c
 *
 * Created: 2022-06-23 오후 5:23:44
 *  Author: Cking
 */ 

#define F_CPU 7372800UL
#include <avr/io.h>
#include <util/delay.h>

void init_keypad()
{
	DDRA = 0xF8; //0b 1111 1000
	PORTA = 0x07; //0b 0000 0111
}

unsigned char keyscan()
{
	PORTA = 0x08; //0b 0000 1000
	_delay_ms(1);
	if((PINA & 0x07) == 0x01) return '1';
	else if((PINA & 0x07) == 0x02) return '2';
	else if((PINA & 0x07) == 0x04) return '3';
	_delay_ms(10);
	
	PORTA = 0x10;		// 0x10
	_delay_ms(1);
	if((PINA & 0x07) == 0x01) return '4';
	else if((PINA & 0x07) == 0x02) return '5';
	else if((PINA & 0x07) == 0x04) return '6';
	_delay_ms(10);
	
	PORTA = 0x20;		// 0x20
	_delay_ms(1);
	if((PINA & 0x07) == 0x01) return '7';
	else if((PINA & 0x07) == 0x02) return '8';
	else if((PINA & 0x07) == 0x04) return '9';
	_delay_ms(10);
	
	PORTA = 0x40;		// 0x40
	_delay_ms(1);
	if((PINA & 0x07) == 0x01) return '*';
	else if((PINA & 0x07) == 0x02) return '0';
	else if((PINA & 0x07) == 0x04) return '#';
	_delay_ms(10);
	
	return 0;
}

int main()
{
	DDRC = 0xFf;
	init_keypad();
	
	while(1){
		unsigned char key = keyscan();
		if(key == '1') PORTC = 0x01;
		else if(key == '2') PORTC = 0x02;
		else if(key == '3') PORTC = 0x03;
		else if(key == '4') PORTC = 0x04;
		else if(key == '5') PORTC = 0x05;
		else if(key == '6') PORTC = 0x06;
		else if(key == '7') PORTC = 0x07;
		else if(key == '8') PORTC = 0x08;
		else if(key == '9') PORTC = 0x09;
		else if(key == '*') PORTC = 0x0a;
		else if(key == '0') PORTC = 0x0b;
		else if(key == '#') PORTC = 0x0c;
	}
	return 0;
}