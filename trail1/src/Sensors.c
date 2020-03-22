/*
 * Sensors.c
 *
 * Created: 3/22/2020 5:05:30 PM
 *  Author: IEEE R&D
 */ 

#include <avr/io.h>
#include "Sensors.h"

void Tr_ON(void){
	DDRD |= (1 << PD7); // direction
	PORTD |= (1 << PD7); //enable the Transmitters
}
void Tr_OFF(void){
	PORTD &= ~(1 << PD7); //disable the Transmitters
}

void ADC_init()
{
	ADMUX = (1<<REFS0); // AVCC with external capacitor at AREF pin
	ADCSRA |= (1 << ADEN); // Enable ADC
	ADCSRA |= (1<<ADEN)|(1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0); // pr-scaler = 128
}
uint16_t ADC_read(uint8_t n)
{
	/*
	n is the number of the channel hence corresponding sensor
	n from 0 to 7 to make sure we have to AND it with 0b00000111
	*/
	n &= 0b00000111;  // AND operation with 7
	
	ADMUX = (ADMUX & 0xF8)|n;     // clears the bottom 3 bits before ORing
	
	ADCSRA |= (1<<ADSC);// start single conversion 
	
	while(ADCSRA & (1<<ADSC));// wait for conversion to complete ADSC becomes '0' again till then, run loop continuously
	
	return (ADC); //the result of conversion
}

uint16_t Front_Left(void) {
	int read1,read2; //read1 With transmitters off and read2 With transmitters on
	//REC 3 is connected to	AD3	
	read1=ADC_read(3); //noise signal
	Tr_ON();
	read2=ADC_read(3); //total signal
	Tr_OFF();
	return (read2-read1);  //total signal-noise signal = actual signal
}
uint16_t Front_Right(void) {
	int read1,read2; //read1 With transmitters off and read2 With transmitters on
	//REC 1 is connected to	AD7
	read1=ADC_read(7); 
	Tr_ON();
	read2=ADC_read(7);
	Tr_OFF();
	return (read2-read1);
}
uint16_t Left(void) {
	int read1,read2; //read1 With transmitters off and read2 With transmitters on
	//REC 4 is connected to	AD4
	read1=ADC_read(4);
	Tr_ON();
	read2=ADC_read(4);
	Tr_OFF();
	return (read2-read1);
}
uint16_t Right(void) {
	int read1,read2; //read1 With transmitters off and read2 With transmitters on
	//REC 2 is connected to	AD6
	read1=ADC_read(6);
	Tr_ON();
	read2=ADC_read(6);
	Tr_OFF();
	return (read2-read1);
}