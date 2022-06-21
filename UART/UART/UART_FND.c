/*
 * UART_FND.c
 *
 * Created: 2022-06-21 오전 11:17:28
 *  Author: PKNU
 */ 

#define F_CPU 7372800UL
#include <avr/io.h> // AVR 입출력에 관한 헤더 파일
#include <avr/interrupt.h>

volatile unsigned char TX_flag = 0;
volatile char TX_data = 0; // 스위치 전송 값 저장 변수

// 7-Segment에 표시할 글자의 입력 데이터를 저장

unsigned char FND_DATA_TBL[] = {0x3f, 0x06, 0x5b, 0x4f, 0x66, 0x6d, 0x7c, 0x07, 0x7f, 0x67, 0x77, 0x7c, 0x39, 0x5e, 0x79, 0x71, 0x08, 0x80};

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
	
	unsigned char RX_data = 0; 
	
	DDRE = 0x0e;
	DDRA = 0xff;
	
	UCSR0A = 0x00;
	UCSR0B = 0x18;  // Rx, Tx
	UCSR0C = 0x06;
	UBRR0H = 0x00;
	UBRR0L = 3;
	
	while (1){
		
		RX_data = getch();
		
		if((RX_data >= 0x30) && (RX_data <= 0x39)){
			
			PORTA = FND_DATA_TBL[RX_data - 0x30]; // 포트 A에 입력된 값의 FND Table을 출력    * 아스키코드 '0'은 0x30임
		}
		
		if((RX_data >= 0x61) && (RX_data <= 0x65)){
			
			PORTA = FND_DATA_TBL[RX_data];
		}
	}
}
