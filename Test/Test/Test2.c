/*
 * Test2.c
 *
 * Created: 2022-06-23 오후 4:03:38
 *  Author: PKNU
 */ 

#define F_CPU 7372800UL
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "Lcd.h"

unsigned int Open_Sound[4] = {1000, 1300, 1600, 1800}; // Open 소리
unsigned int Error_Sound[4] = {800, 1700, 800, 1700};  // Error 소리

volatile char Password[] = "1234";                     // 초기 비밀번호 설정 = "1234"
volatile char Error[] = " Error number !!";			   // Error 발생시 출력
volatile char Open[] = " Open";						   // Open 발생시 출력
volatile char Output[30];							   // LCD 화면(1, 0)에 출력되는 문장
volatile char Input[5];								   // 실제 Keypad 를 통해서 입력된 값이 저장되는 배열
volatile char Encrypt_Input[5];                        // LCD 화면(1, 0)에는 출력되는 '*'가 저장되는 배열
volatile int Key_Num = 0;							   // Password Array(Input, Encrypt_Input 등)에서 사용되는 Index 값 
volatile unsigned char key = 0;							  

volatile int FLAG = 1;    
volatile int State = 1;         

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

// Input, Encrypt_Input Array 를 Initialize 해줌
void Key_init(){
	
	for(int i = 0 ; i < sizeof(Input) ; i++){
		
		Input[i] = '\0';
		Encrypt_Input[i] = '\0';
	}
	
	for(int i = 0 ; i < sizeof(Output) ; i++){
		
		Output[i] = '\0';
	}
	
	Key_Num = 0;
}

// keypad 에서 값을 읽어와 Input, Encrypt_Input Array 에 넣어주는 함수
void Insert_Password(){
	
	if((key >= 0x30) && (key <= 0x39)){
			
		if(Key_Num == 0){
				
			Input[0] = key;
			Input[1] = '\0';
			Encrypt_Input[0] = '*';
			Encrypt_Input[1] = '\0';
		}
			
		if(Key_Num == 1){
				
			Input[1] = key;
			Input[2] = '\0';
			Encrypt_Input[1] = '*';
			Encrypt_Input[2] = '\0';
		}
			
		if(Key_Num == 2){
				
			Input[2] = key;
			Input[3] = '\0';
			Encrypt_Input[2] = '*';
			Encrypt_Input[3] = '\0';
		}
			
		if(Key_Num == 3){
				
			Input[3] = key;
			Input[4] = '\0';
			Encrypt_Input[3] = '*';
			Encrypt_Input[4] = '\0';
		}
			
		Key_Num++;
		
	}
}

// Open 시에 Motor 가 동작하게 하는 함수
void Motor(){
	
	for(int i = 0 ; i < 16 ; i++ ){
		PORTD = 0x30;
		_delay_ms(10);
		PORTD = 0x90;
		_delay_ms(10);
		PORTD = 0xC0;
		_delay_ms(10);
		PORTD = 0x60;
		_delay_ms(10);
	}
}

// Open, Error 발생시 PIEZO Buzzer 작동
void Sound(int sound[]){
	
	unsigned char Door_Num = 0;
	
	DDRB = 0x80; // 0x 1000 0000 PE7(OC1C핀) PWM 출력
	
	// 0x0A = 0x 0000 1100, COM1C(1:0) = "10", OC1C핀 사용, WGM(1:0) = "10"
	TCCR1A |= 0x0A; // (1 << COM1C1) | (1 << COM1C0);
	// 0x19 = 0x 0001 1001, WGM3(3:2) = "11", CS3(2:0) = "001" 1분주 사용
	TCCR1B |= 0x19; // (1 << WGM13) | (1 << WGM12) | (1 << CS10);
	TCCR1C = 0x00;    // WGM3(3:0) = "1110", Fast PWM, 모드 14
	TCNT1 = 0x0000;   // 타이머1 카운터 초기화
	
	while(Door_Num < 4){
		
		ICR1 = 7372800 / sound[Door_Num];
		OCR1C = ICR1/8; // Duty 조절
		Door_Num++;
		_delay_ms(300);
	}
	
	DDRB = 0x00; 
	
}

int main(void)
{
	
	DDRA = 0xFF;
	DDRC = 0xFF; // 포트 C 0b 1111 1111를 출력으로 사용
	DDRD = 0xF0; // 포트 D 0b 1111 를 출력으로 사용
	DDRE = 0x0E; // 포트 E 0b 0000 1110을 입, 출력으로 사용
	DDRF = 0xFF; // 포트 F 0b 1111 1111를 출력으로 사용
	DDRG = 0x00;

	UCSR0A = 0x00;
	UCSR0B = 0x18;  // Rx, Tx
	UCSR0C = 0x06;
	UBRR0H = 0x00;
	UBRR0L = 3;
	
	EICRB = 0x0C;
	EIMSK = 0x20;
	EIFR = 0x20;
	
	sei();
	
	Key_init();	
	LcdInit_4bit();
	
	while(1){
		
		if(FLAG == 0){
			
			Key_init();
			LcdInit_4bit();
			FLAG = 1;
		}
		
		if(FLAG == 1){
			
			if(State == 1){	
			key = getch();
			putch(key);
			}
			
			Insert_Password();
			
			Lcd_Clear();
			Lcd_Pos(0, 0);
			Lcd_STR(" Password Input");
			
			sprintf(Output, " Key : %s", Encrypt_Input);
			Lcd_Pos(1, 0);
			Lcd_STR(Output);
			_delay_ms(300);
			
			// Input 값과 초기 지정된 Password 값이 일치하고, Password 4자리 입력했으면 Open
			if(!strcmp(Password, Input) & Key_Num == 4){
				
				Lcd_Clear();
				Lcd_Pos(0, 0);
				Lcd_STR(Open);
			
				unsigned int i = 0;
				
				Sound(Open_Sound);
				Motor();
				
				_delay_ms(2500);
				FLAG = 0;
				
			}
			
			// Password 가 4자리 입력했지만 Password 와 다르면 Error 발생
			else if(strcmp(Password, Input) & Key_Num == 4){
				
				Lcd_Clear();
				Lcd_Pos(0, 0);
				Lcd_STR(Error);
				
				Sound(Error_Sound);
				
				_delay_ms(2500);
				FLAG = 0;
			}
			
			else if(Key_Num >= 4){
				
				Lcd_Clear();
				Lcd_Pos(0, 0);
				Lcd_STR(Error);
				
				Sound(Error_Sound);
				
				_delay_ms(2500);
				FLAG = 0;
			}
		}
	}
	
	return 0;

}