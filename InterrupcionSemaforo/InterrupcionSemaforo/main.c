/*
 * InterrupcionSemaforo.c
 *
 * Created: 21/12/2024 10:48:41 a.m.
 * Author : LSL
 *
 * Semáforo: 5s verde (3s estable y 2s parpadeando), 2s naranja (intermitente) y 5s rojo (3s estable y 2s parpadeando)
 * Si pulsa el botón se queda parpadeando el verde 6s
 */ 

#define F_CPU 16000000UL
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

void intermitencia(int *pointPin, int *CountStop);

int main(void)
{
	cli(); //Se desactivan todas las interrupciones globales
	
	int stop = 0, Pin;
    //Output pin
	DDRB |= (1 << 4); //Orange led
	DDRB |= (1 << 1); //Green led
	DDRB |= (1 << 7); //Red led
	//Input pin
	DDRD &=~ (1 << 4); //Button
	//PORTD |= (1 << PORTD4); //Pull-up activado
	
	//------------------- INTERRUPCIÓN ------------------------
	PCICR  |= (1 << PCIE2); //PCIE2:?Pin Change Interrupt Enable 2
	PCMSK2 |= (1 << PCINT20); //Pin Change Enable Mask
	//PCIFR  |= (1 << PCIF2);
	sei(); //Se activan todas las interrupciones globales
	
    while (1) 
    {
		int *PointStop = &stop, *PointPin = &Pin;
	
		Pin = 1; // Green on
		PORTB |= (1 << *PointPin);
		_delay_ms(800);
		stop = 4;
		intermitencia(PointPin, PointStop);
		
		Pin = 4; // Orange on
		PORTB |= (1 << *PointPin);
		stop = 8;
		intermitencia(PointPin, PointStop);
		
		Pin = 7; // Red on
		PORTB |= (1 << *PointPin);
		_delay_ms(800);
		stop = 4;
		intermitencia(PointPin, PointStop);
		
		Pin = 4; // Orange on
		PORTB |= (1 << *PointPin);
		stop = 8;
		intermitencia(PointPin, PointStop);
    }
}

void intermitencia(int *pointPin, int *CountStop){
	
	for(int i = 0; i <= *CountStop; i++){
		
		PORTB |= (1 << *pointPin);
		_delay_ms(100);
		PORTB &=~ (1 << *pointPin);
		_delay_ms(100);
	}
			
	*CountStop = 0;
	PORTB &=~ (1 << *pointPin);
}

ISR(PCINT2_vect){ //Solicitud de interrupción externa 1 (CONSULTAR: https://ece--classes-usc-edu.translate.goog/ee459/library/documents/avr_intr_vectors/?_x_tr_sl=en&_x_tr_tl=es&_x_tr_hl=es&_x_tr_pto=tc)
	
	if(PIND & (1 << PIND4)){
		
		for(int j = 0; j <= 10; j++){
		PORTB |= (1 << PORTB1);
		_delay_ms(100);
		PORTB &=~ (1 << PORTB1);
		_delay_ms(100);
		}
	}
	
}