/*
 * UART_Gugudan.c
 *
 * Created: 2022-06-21 오전 11:42:12
 *  Author: PKNU
 */ 

#define F_CPU 7372800UL
#include <avr/io.h> // AVR 입출력에 관한 헤더 파일
#include <avr/interrupt.h>

volatile unsigned char TX_flag = 0;
volatile char TX_data = 0; // 스위치 전송 값 저장 변수

// 7-Segment에 표시할 글자의 입력 데이터를 저장

void putch(unsigned char data){
	
	while((UCSR0A & 0x20) == 0 ); // 전송 준비가 될 때까지 대기
	UDR0 = data;				  // 데이터를 UDR0에 쓰면 전송
	UCSR0A |= 0x20;
}

void putch_Str(char *str){
	
	unsigned char i = 0;
	while(str[i]!='\0')
	putch(str[i++]);
	
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
	unsigned char TX_data[20];
	
	DDRE = 0x0e;
	DDRA = 0xff;
	
	UCSR0A = 0x00;
	UCSR0B = 0x18;  // Rx, Tx
	UCSR0C = 0x06;
	UBRR0H = 0x00;
	UBRR0L = 3;
	
	while (1){
		
		RX_data = getch();
		
		if((RX_data >= 0x31) && (RX_data <= 0x39)){
			
			putch_Str("\r\n");	
			putch_Str(" < Gugudan ");
			putch(RX_data);
			putch_Str("dan Print !! > \r\n");
			
			for(int i = 2 ; i < 10 ; i++)
			{
				int INT_data = RX_data - 0x30;
				sprintf(TX_data, "     %d * %d = %d", INT_data, i, INT_data*i); // INT형 data 들을 sprintf 함수를 이용해서 문자로 변경함.
				putch_Str("\r\n");
				putch_Str(TX_data); // 출력
			}
			
			putch_Str("\r\n");
			
	    }
	}
}
