
#define F_CPU 8000000UL

#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include "Sensors.h"

volatile uint16_t Left_Slots;
volatile uint16_t Right_Slots;


int main (void)
{
	 
	ADC_init(); // initialize the ADC
	
	
	sei(); // Enable the interrupt
	
	
	MCUCR |= (1<<ISC10) | (1<<ISC00);  // make INT 1 and  INT0 on toggle condition
	GICR |= (1<<INT1) | (1<<INT0);		//enable the interrupt 1 and 0 
	
	//PWM0
	TCCR0 |=(1<<WGM01) | (1<<WGM00); // enable fast PWM
	TCCR0 |=(1<<COM01); // Clear OC0 on compare match, set OC0 at BOTTOM.
	TCCR0 |=(1<<CS01) | (1<<CS02);  // set PRE-scaling with /1024
	
	/*
	//PWM1
	TCCR1A |=(1<<COM1A1); // Clear OC1A on compare match (Set output to low level)
	TCCR1A |=(1<<WGM12) | (1<<WGM10); //Fast PWM, 8-bit
	TCCR1A |=(1<<CS10) | (1<<CS12);  // set PRE-scaling with /1024
	*/
	
	
	while(1){}
}
	
ISR(INT0_vect){
	Right_Slots++;
}

ISR(INT1_vect){
	Left_Slots++;
}

float Calculte_Distance(uint16_t Slots){
	// if 20 slots then one cycle
	//one cycle means 2*3pi
	float cycles = Slots/20;
	float Distance = cycles*2*3*22/7;
	return Distance;
}

void set_Left_Speed(uint8_t speed){
	/*
	speed in range 0 -> 255
	*/
	OCR1A = speed;
}
void set_Right_Speed(uint8_t speed){
	/*
	speed in range 0 -> 255
	*/
	OCR0 = speed;
}

void Stop_Motor(){
	DDRB |= (1<<PB0)|(1<<PB1)|(1<<PB2)|(1<<PB4);
	PORTB &= ~ ((1<<PB0)|(1<<PB1)|(1<<PB2)|(1<<PB4));
}
void Move_Forword(uint8_t speed_L,uint8_t speed_R ){
	//1001
	DDRB |= (1<<PB0)|(1<<PB1)|(1<<PB2)|(1<<PB4);
	PORTB |= ((1<<PB0)|(1<<PB4));
	set_Left_Speed(speed_L);
	set_Right_Speed(speed_R);
}
void Move_Right(uint8_t speed){
	Move_Forword(speed,0);
}
void Move_Left(uint8_t speed){
	Move_Forword(0,speed);
}
void U_Turn(uint8_t speed){
	//1010
	while( /* untill 90 degree*/){
		Move_Left(speed);
		Move_Right(speed);
	}
}

