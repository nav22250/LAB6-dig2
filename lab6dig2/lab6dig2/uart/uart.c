/*
 * uart.c
 *
 * Created: 9/24/2025 11:43:01 PM
 *  Author: Nico
 */ 
#include <avr/io.h>
#include "uart.h"

void UART_init(unsigned int ubrr) {
	UCSR0A = (1 << U2X0); // habilitar doble velocidad
	UBRR0H = (unsigned char)(ubrr >> 8);
	UBRR0L = (unsigned char)(ubrr);
	UCSR0B = (1 << TXEN0);                   // habilita TX
	UCSR0C = (1 << UCSZ01) | (1 << UCSZ00); // 8N1
}

void UART_sendChar(char data) {
	while (!(UCSR0A & (1 << UDRE0)));  // esperar buffer libre
	UDR0 = data;
}

void UART_sendString(const char *str) {
	while (*str) {
		UART_sendChar(*str++);
	}
}
