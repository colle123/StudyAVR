/*
 * UART_Echo.c
 *
 * Created: 2022-06-21 오전 10:53:48
 *  Author: PKNU
 */ 

#define F_CPU 7372800UL
#include <avr/io.h> // AVR 입출력에 관한 헤더 파일

void putch(unsigned char data){
	
	while((UCSR0A & 0x20) == 0 ); // 전송 준비가 될 때까지 대기
	UDR0 = data;				  // 데이터를 UDR0에 쓰면 전송
	UCSR0A |= 0x20;
}

unsigned char getch(){
	
	unsigned char data; 
	while((UCSR0A & 0x80) == 0); // 데이터를 받을 때까지 대기
	data = UDR0;				 // 수신된 데이터는 UDR0에 저장
	UCSR0A |= 0x80;
	return data;				 // 읽어온 문자를 반환
}

int main(){
	
	unsigned char text[] = "\r\nWelcome! edgeiLAB\r\n USART 0 Test Program. \r\n";
	
	unsigned char echo[] = "ECHO >> ";
	unsigned char i = 0;
	
	DDRE = 0xfe;    // Rx(입력 0), Tx(출력, 1)
	
	UCSR0A = 0x00;
	UCSR0B = 0x18;  // Rx, Tx 
	UCSR0C = 0x06;
	
	UBRR0H = 0x00;
	UBRR0L = 3;
	
	while(text[i] != '\0'){
		
		putch(text[i++]);
	}
	
	i = 0;
	
	while(echo[i] != '\0'){
		
		putch(echo[i++]);
	}
	
	while(1){
		
		putch(getch());
	}
}
