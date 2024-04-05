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
#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdint.h>
#define F_CPU 16000000 

/* 
FUNCIONES
-------------------------------------------------
*/
void setup(void);

/*
VARIABLES
-------------------------------------------------
*/
uint8_t valor = 0 ; 

int main(void)
{
    setup(); 
    while (1) 
    {
		PORTD = valor; 
    }
}

void setup(void){
	cli ();
	// Entradas y salidas
	DDRD = 0xFF;
	DDRC |= (1 << DDC0 )|(1 << DDC1 );
	// Pull-up
	PORTC |= (1 << PORTC0)|(1 << PORTC1) ;	
	// Iniciar apagado
	PORTD = 0;
	// Habilitar RX y Tx 
	UCSR0B = 0; 
	// Interrupciones 
	PCMSK1 |= (1 << PCINT9)|(1 << PCINT8);
	PCICR |= (1 << PCIE1);
	sei ();
}

ISR (PCINT1_vect){
	
	if (PINC==(1<<PINC0))
	{
		valor++;
	}
	else if (PINC== (1<<PINC1))
	{
		valor--;
	}	
	else {
		valor=valor;
	}
	
	PCIFR |= (1<< PCIF1);
}