/*
 * LabADC.c
 *
 * Created: 4/04/2024 21:13:03
 * Author : Josué Ortíz 
 */ 

/* 
INCLUDES Y DEFINICIONES
-------------------------------------------------
*/
#define F_CPU 16000000000
#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdint.h>
#include <util/delay.h>


/* 
FUNCIONES
-------------------------------------------------
*/
void setup(void);
void initADC(void);
void initTimer1(void);

/*
VARIABLES
-------------------------------------------------
*/
uint8_t valor = 0 ; 
uint8_t contador = 0; 
const uint8_t tabla[]={0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x67, 0x77, 0x7C, 0x39, 0x5E, 0x79, 0x71}; 

int main(void)
{
    setup(); 
    while (1) 
    {
		PORTB |= (1<<PORTB0); 
		PORTD = tabla[(valor & 0x0F)]; 
		_delay_us(5);
		PORTB &= ~(1<<PORTB0); 
		PORTB |= (1<<PORTB1); 
		PORTD = tabla[valor>>4];
		_delay_us(5);
		PORTB &= ~(1<<PORTB1);
		PORTB |= (1<<PORTB2); 
		PORTD = contador;
		_delay_us(5);
		PORTB &= ~(1<<PORTB2); 
    }
}

void setup(void){
	cli ();
	// ENTRADAS Y SALIDAS
	DDRB = 0x0F;
	DDRD = 0xFF;
	DDRC &= ~((1 << DDC0 )|(1 << DDC1 ));
	// Pull-up
	PORTC |= (1 << PORTC0)|(1 << PORTC1) ;	
	// INICIAR APAGADO
	PORTD = 0;
	PORTB = 0; 
	//HBILITAR RX Y TX 
	UCSR0B = 0; 
	// INTERRUPCIONES 
	PCMSK1 |= (1 << PCINT9)|(1 << PCINT8);
	PCICR |= (1 << PCIE1);
	initADC();
	initTimer1();
	sei ();
}

void initADC(void){
	ADMUX = 0;
	ADMUX |= (1<<REFS0)|(1<<ADLAR)|(1<<MUX2)|(1<<MUX1);
	
	ADCSRA = 0;
	// HABILITAR INTERRUPCION
	ADCSRA |= (1<<ADIE);
	//PRESCALER 128
	ADCSRA |= (1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0);
	//HABILITAR
	ADCSRA |= (1<<ADEN);
}

void initTimer1(void){
	TCCR1B |= (1<<CS12)| (1<<CS10); 
	TCNT1 = 62411; 	
	TIMSK1 |= (1<<TOIE1);
}

ISR (PCINT1_vect){
	
	if (PINC==(1<<PINC0))
	{
		contador++;
	}
	else if (PINC== (1<<PINC1))
	{
		contador--;
	}	
	else {
		contador=contador;
	}
	
	PCIFR |= (1<< PCIF1);
}

ISR (ADC_vect){
	valor= ADCH;
	if (contador<=valor)
	{
		PORTB |= (1<<PORTB3);
	} 
	else
	{
		PORTB &= ~(1<<PORTB3);
	}
	ADCSRA |= (1<<ADIF);	
}

ISR (TIMER1_OVF_vect){
	ADCSRA |= (1<<ADSC);
	TCNT1 = 62411;
	TIFR1 |= (1<<TOV1);
}