/*
 * UART.c
 *
 * Created: 2022-06-21 오전 9:35:50
 *  Author: PKNU
 */ 

#define F_CPU 7372800UL
#include <avr/io.h>

void putch(unsigned char data){
	
	while((UCSR0A & 0x20) == 0); // 전송준비가 될때까지 대기
	UDR0 = data;                 // 데이터를 UDR0에 쓰면 전송
	UCSR0A |= 0x20;
}

int main(){
	
	unsigned char text[] = "Hello World! \r\n";
	unsigned char i = 0;
	
	DDRE =0xfe; // RX(입력 0), TX(출력 1) 0x 1111 1110
	
	UCSR0A = 0x00;
	UCSR0B = 0x18; // Rx, Tx Enable 0001 1000
    UCSR0C = 0x06; // 비동기 방식, No Parity bit, 1 Stop bit
	UBRR0H = 0x00;
	UBRR0L = 3; // 115200 bps 0x 0000 0111
	
	while(text[i]!='\0'){ // 문자열 데이터가 '\0'인 경우, 문자열이 끝임
		
		putch(text[i++]); // 저장된 문자열 출력
		
	}
	
	return 0;
}