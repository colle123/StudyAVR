/*
 * PWM_PIEZO_UART_.c
 *
 * Created: 2022-06-23 오전 11:26:25
 *  Author: PKNU
 */ 

#define F_CPU 7372800UL
#include <avr/io.h>  //AVR 입출력에 대한 헤더 파일
#include <avr/interrupt.h> //AVR 인터럽트에 대한 헤더파일
#include <util/delay.h>  //delay 함수사용을 위한 헤더파일

//피아노 음계에 해당하는 PWM 주파수
unsigned int DoReMi[8] = {523, 587, 659, 698, 783, 880, 987, 1046};
unsigned char Output[8] = {'C', 'D', 'E', 'F', 'G', 'A', 'B', 'C'};
volatile unsigned char sound_flag = 1;

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
	
	UCSR0A = 0x00;
	UCSR0B = 0x18;  // Rx, Tx
	UCSR0C = 0x06;
	UBRR0H = 0x00;
	UBRR0L = 3;
	
    DDRE = 0x08;   //포트E PE3을 출력 나머지는 입력포트로 설정한다.
	
	//Buzzer를 PE3에 연결
	
	TCCR3A = 0x00;   //WGM3(1:0) = "00"
	TCCR3B = 0x19;   //WGM3(3:2) = "11" ,  CS3(2:0) = "001"  1분주 사용
	TCCR3C = 0x00;   //WGM3(3:0) = "1110", Fast PWM, 모드 14
	
    TCNT3 = 0x0000;    //타이머3 카운터 초기화
	 
	while (1){
		
		_delay_ms(250);
		TCCR3A = 0x00;
		RX_data = getch();
		
		if((RX_data >= 0x30) && (RX_data <= 0x37)){
			
			int INT_data = RX_data - 0x30;
			
			ICR3 = 7372800 / DoReMi[INT_data] / 2;
			TCCR3A = 0x40;  //PE4로 출력
			putch(Output[INT_data]);

		}		
		
	}
}