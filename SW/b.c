#define F_CPU 1000000
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

unsigned char stick=0;
unsigned char mtick=0;
unsigned char htick=0;


void tim1(){

	TCCR1A = (1<<FOC1A);
	TCCR1B =(1<<CS10)|(1<<CS12)|(1<<WGM12);
	TCNT1=0;
	OCR1A=1000;
	SREG|=(1<<7);

	TIMSK= (1<<OCIE1A);

}




ISR ( TIMER1_COMPA_vect){
	stick++;
	if (stick==60){
		stick=0;
		mtick++;


	}
	if (mtick==60){

		mtick=0;
		stick=0;
		htick++;
	}
	if (htick==24){
		stick=0;
		htick=0;
		mtick=0;
	}
}

void int0 (){

	DDRD  =DDRD & (~(1<<2));
	PORTD =PORTD |(1<<2);
	GICR =GICR|(1<<INT0);
		MCUCR  = MCUCR |(1<<ISC01);
		MCUCR &= ~(1<<ISC00);
		SREG|=(1<<7);
}
ISR (INT0_vect){
	stick=0;
	mtick=0;
	htick=0;
}

void int1(){
	GICR |=(1<<INT1);
		MCUCR|=(1<<ISC11)|(1<<ISC10);
		SREG|=(1<<7);
		DDRD&=~(1<<3);
}
ISR (INT1_vect){
	TIMSK&=~(1<<OCIE1A);
}

void int2(){SREG|=(1<<7);
DDRA &=~(1<<2);
GICR|=(1<<INT2);
MCUCSR &=~(1<<ISC2);
PORTA|=(1<<2);}

ISR (INT2_vect){
	TIMSK|= (1<<OCIE1A);
}
int main (){

	DDRA = 0b00011111;
	DDRC = 0x0f;
	PORTC = 0x00;


	int0();
	int1();
	int2();
	tim1();
	while(1){
		PORTA=(1<<0);
		PORTC=stick%10;
		_delay_ms(5);
		PORTA=(1<<1);
		PORTC=stick/10;
		_delay_ms(5);
		PORTA=(1<<2);
		PORTC=mtick%10;
		_delay_ms(5);
		PORTA=(1<<3);
		PORTC=mtick/10;
		_delay_ms(5);
		PORTA=(1<<4);
		PORTC=htick%10;
		_delay_ms(5);
		PORTA=(1<<5);
		PORTC=htick/10;
		_delay_ms(5);

	}

}
