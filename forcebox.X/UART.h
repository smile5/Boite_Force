
#ifndef UART_H
#define UART_H

#include <xc.h>

void UART_init(void);
uint8_t UART_read(void);
void UART_write(uint8_t txData);
void UART_write_text(char *uartText);
void UART_write_word(short txData);
void UART_write_long(long txData);
#endif	/* UART_H */
