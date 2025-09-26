/*
 * uart.h
 *
 * Created: 9/24/2025 11:43:11 PM
 *  Author: Nico
 */ 


// uart.h
#ifndef UART_H_
#define UART_H_

void UART_init(unsigned int ubrr);
void UART_sendChar(char data);
void UART_sendString(const char *str);

#endif
