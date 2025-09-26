/*
 * main.c
 *
 * Created: 9/24/2025 11:03:04 PM
 *  Author: Nico
 */ 
#define F_CPU 16000000UL
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "uart/uart.h"

#define BAUD 115200UL
#define UBRR_VAL ((F_CPU)/(8UL*BAUD) - 1UL) // nota: ahora con 8 en vez de 16


volatile char key_to_send = 0;
volatile uint8_t prevB = 0x1F; // <-- agregado
volatile uint8_t prevC = 0x01; // <-- agregado

// --- ISR para PB0–PB4 ---
ISR(PCINT0_vect) {
	uint8_t estadoB = PINB & 0x1F;
	uint8_t cambioB = prevB ^ estadoB;

	if ((cambioB & (1 << PB0)) && !(estadoB & (1 << PB0))) key_to_send = 'A';
	else if ((cambioB & (1 << PB1)) && !(estadoB & (1 << PB1))) key_to_send = 'B';
	else if ((cambioB & (1 << PB2)) && !(estadoB & (1 << PB2))) key_to_send = 'U';
	else if ((cambioB & (1 << PB3)) && !(estadoB & (1 << PB3))) key_to_send = 'D';
	else if ((cambioB & (1 << PB4)) && !(estadoB & (1 << PB4))) key_to_send = 'R';

	prevB = estadoB;
}

// --- ISR para PC0 (Izquierda) ---
ISR(PCINT1_vect) {
	uint8_t estadoC = PINC & 0x01;
	uint8_t cambioC = prevC ^ estadoC;
	if ((cambioC & (1 << PC0)) && !(estadoC & (1 << PC0))) key_to_send = 'L';
	prevC = estadoC;
}

int main(void) {
	// Configuraciones de pins (igual que antes)
	DDRB &= ~((1 << PB0) | (1 << PB1) | (1 << PB2) | (1 << PB3) | (1 << PB4));
	PORTB |= (1 << PB0) | (1 << PB1) | (1 << PB2) | (1 << PB3) | (1 << PB4);
	DDRC &= ~(1 << PC0);
	PORTC |= (1 << PC0);

	// Habilita PCINTs (igual que antes)
	PCICR |= (1 << PCIE0) | (1 << PCIE1);
	PCMSK0 |= (1<<PCINT0)|(1<<PCINT1)|(1<<PCINT2)|(1<<PCINT3)|(1<<PCINT4);
	PCMSK1 |= (1<<PCINT8);

	// UART init con UBRR calculado
	UART_init((unsigned int)UBRR_VAL);

	sei();

	// Prueba básica: delay corto al inicio para abrir la terminal
	_delay_ms(200);

	while (1) {
		if (key_to_send) {
			UART_sendChar(key_to_send);
			// opcional: enviar salto de línea para que sea legible
			UART_sendChar('\r');
			UART_sendChar('\n');

			// debounce y evita envíos duplicados
			_delay_ms(50);
			key_to_send = 0;
		}
	}
}
