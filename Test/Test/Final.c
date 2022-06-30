/*
 * Final.c
 *
 * Created: 2022-06-24 오후 3:08:19
 *  Author: PKNU
 */ 

#define F_CPU 7372800UL
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "Lcd.h"

unsigned int Open_Sound[4] = {1000, 1300, 1600, 1800}; // Open 소리
unsigned int Error_Sound[4] = {800, 1700, 800, 1700};  // Error 소리

volatile char Password[] = "1234";                     // 초기 비밀번호 설정 = "1234"
volatile char Error[] = " Error number !!";			   // Error 발생시 출력
volatile char Interrupt_Button[] = " State Change";    // Interrupt 발생시 State 가 변경되면서 LCD 화면(0, 0)에 출력되는 Array
volatile char Open[] = " Open";						   // Open 발생시 출력
volatile char Password_Output[30];                     // FLAG = 2일때 LCD 화면(1, 0)에 출력되는 Array
volatile char Output[30];							   // FLAG = 1일때 LCD 화면(1, 0)에 출력되는 Array
volatile char Input[5];								   // 실제 Keypad 를 통해서 입력된 값이 저장되는 Array
volatile char Encrypt_Input[5];                        // LCD 화면(1, 0)에는 출력되는 '*'가 저장되는 Array
volatile int Key_Num = 0;							   // Password Array(Input, Encrypt_Input 등)에서 사용되는 Index 값
volatile int Password_Num = 0;
volatile unsigned char key;                            // Keyscan() Func 의 Return value 를 저장
volatile unsigned char password;

volatile int FLAG = 1;                                 // FLAG = 0 : 초기화 / FLAG = 1 : Key 입력상태 / FLAG = 2 : Password 입력상태
volatile int State = 0;

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

// keypad 초기화 시켜주는 함수
void init_keypad()
{
	DDRA = 0xF8; //0b 1111 1000
	PORTA = 0x07; //0b 0000 0111
}

// keypad 에서 입력된 값을 읽어와 Return 해주는 함수
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

// Input, Encrypt_Input Array Initialize
void Key_init(){
	
	for(int i = 0 ; i < sizeof(Input) ; i++){
		
		Input[i] = '\0';
		Encrypt_Input[i] = '\0';
	}
	
	Key_Num = 0;
}

// Password[] Array Initialize
void Password_init(){
		
	for(int i = 0 ; i < sizeof(Password) ; i++){
			
		Password[i] = '\0';
			
	}
}

// keypad 또는 Keyboard 에서 값을 읽어와 Input, Encrypt_Input Array 에 넣어주는 함수
void Insert_Key(){

	if(State == 0){

		key = keyscan();
		
		if(key == '#'){
			
			FLAG = 2;
			Password_init();
		}
		
		if((key >= '0') & (key <= '9')){
			
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
	
	if(State == 1){
		
		if(key == 0x23){
			
			FLAG = 2;
			Password_init();
		}
		
		if((key >= 0x30) & (key <= 0x39)){
			
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
}

// keypad 또는 Keyboard 에서 값을 읽어와 Password Array 에 넣어주는 함수
void Insert_Password(){

	if(State == 0){

		password = keyscan();
		
		if((password >= '0') & (password <= '9')){
			
			if(Password_Num == 0){
				
				Password[0] = password;
				Password[1] = '\0';

			}
			
			if(Password_Num == 1){
				
				Password[1] = password;
				Password[2] = '\0';

			}
			
			if(Password_Num == 2){
				
				Password[2] = password;
				Password[3] = '\0';

			}
			
			if(Password_Num == 3){
				
				Password[3] = password;
				Password[4] = '\0';

			}
			
			Password_Num++;
		}
	}
	
	if(State == 1){
		
		if((password >= 0x30) & (password <= 0x39)){
			
			if(Password_Num == 0){
				
				Password[0] = password;
				Password[1] = '\0';
				
			}
			
			if(Password_Num == 1){
				
				Password[1] = password;
				Password[2] = '\0';
				
			}
			
			if(Password_Num == 2){
				
				Password[2] = password;
				Password[3] = '\0';

			}
			
			if(Password_Num == 3){
				
				Password[3] = password;
				Password[4] = '\0';
				
			}
			
			Password_Num++;
		}
	}
}

// Open 시에 Motor 가 동작하게 하는 함수
void Motor(){
	
	for(int i = 0 ; i < 12 ; i++ ){
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

// Password 수정
void Modify_Password(){
	
	Lcd_Clear();
	Lcd_Pos(0, 0);
	Lcd_STR(" Modify Password");

	Lcd_Pos(1, 0);
	sprintf(Password_Output, " Pw : %s", Password);
	
	Insert_Password();	
	
}

// 입력한 Input 과 기입력된 Password 를 비교하는 작업
void Compare_Password(){
	
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

// Switch 동작시 Interrupt 발생되면 할 동작
SIGNAL(INT4_vect){
	
	cli();
	
	if(State == 0) State = 1;
	
	else if(State == 1) State = 0;
	
	Lcd_Clear();
	Lcd_Pos(0, 0);
	Lcd_STR(Interrupt_Button);
	_delay_ms(1000);
	
	Key_init();
	
	sei();
}

int main(void)
{
	
	DDRC = 0xFF; // 포트 C 0b 1111 1111를 출력으로 사용
	DDRD = 0xF0; // 포트 D 0b 1111 를 출력으로 사용
	DDRE = 0x0E; // 포트 E 0b 0000 1110을 입, 출력으로 사용
	DDRF = 0xFF; // 포트 F 0b 1111 1111를 출력으로 사용
	DDRG = 0x00;
	
	UCSR0A = 0x00;
	UCSR0B = 0x18;  // (1 << RXEN0) | (1 << TXEN0)
	UCSR0C = 0x06;  // (1 << UCSZ01) | (1 << UCSZ00)
	UBRR0H = 0x00;
	UBRR0L = 3; // (1 << UBRR01) | (1 << UBRR00)   
	
	EICRB = 0x03; // (1 << ISC41) | (1 << ISC40) 인터럽트 4번을 상승엣지에서 사용
	EIMSK = 0x10; // (1 << INT4) 인터럽트 4번 사용
	EIFR = 0x10; // (1 << INTF4) 인터럽트 4번 FLAG 허용
	
	sei();
	
	Key_init();
	init_keypad();
	LcdInit_4bit();
	
	while(1){
		
		// FLAG == 0, Key 입력이 4개 이상되면 초기화 시켜줌		
		if(FLAG == 0){
			
			Key_init();
			LcdInit_4bit();
			FLAG = 1;
		}
		
		// FLAG == 1, 일반적인 Key 입력상태
		if(FLAG == 1){									
			
			// State = 1, KeyBoard를 이용해서 입력을 받는 상태, 0이면 Keypad 를 이용해서 입력받음.
			if(State == 1){
				
				key = getch();
				putch(key);
			}
			
			Insert_Key();

			Lcd_Clear();
			Lcd_Pos(0, 0);
			Lcd_STR(" Password Input");
				
			sprintf(Output, " Key : %s", Encrypt_Input); // char Output[30]; " Key : ****";
			Lcd_Pos(1, 0);
			Lcd_STR(Output);
			_delay_ms(200);
			
			Compare_Password();
			
		}
		
		// FLAG == 2, Key입력에서 '#'을 누르면 비밀번호 변경상태로 변경
		if(FLAG == 2){
			
			Modify_Password();
			
			if(Password_Num <= 4){
				
				Password_Num = 0;
				FLAG = 0;
			}
		}
	}
	
	return 0;

}