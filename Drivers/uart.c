/*
 * uart.c
 *
 * Created: 2026-05-02 오후 2:51:59
 *  Author: jymen
 */ 
#include "uart.h"


// putchar 대체 함수
int UART_putchar(char c, FILE *stream)
{
	if (c == '\n') {
		UART_WriteChar('\r'); // CRLF 처리
	}
	UART_WriteChar(c);
	return 0;
}

// FILE 객체 생성
FILE uart_output = FDEV_SETUP_STREAM(UART_putchar, NULL, _FDEV_SETUP_WRITE);

void UART_InitStdout(void)
{
	stdout = &uart_output; // printf 출력 리다이렉트
}

void UART_Init(void) 
{
	USART0.CTRLA = 0x00; // 인터럽트 비활성화 (필요시 수정)
	UART_InitStdout();
}

void UART_Open(uint32_t f_cpu, uint32_t baudrate) {
	// Baudrate 계산
	uint16_t baud_setting = ((uint16_t) (F_CPU * 64 / (16 * (float)baudrate)));
	
	USART0.BAUD = baud_setting;

	// 핀 방향 설정
	PORTB.DIRSET = PIN2_bm; // PB2 TX 출력
	PORTB.DIRCLR = PIN3_bm; // PB3 RX 입력	
	USART0.CTRLB = 0 << USART_MPCM_bp     /* Multi-processor Communication Mode: disabled */
		     	 | 0 << USART_ODME_bp     /* Open Drain Mode Enable: disabled */
				 | 1 << USART_RXEN_bp     /* Reciever enable: enabled */
				 | USART_RXMODE_NORMAL_gc /* Normal mode */
				 | 0 << USART_SFDEN_bp    /* Start Frame Detection Enable: disabled */
				 | 1 << USART_TXEN_bp;    /* Transmitter Enable: enabled */
	USART0.CTRLC = USART_CHSIZE_8BIT_gc; // 8비트, No parity, 1 stop
}


void UART_WriteChar(char c) 
{
	while (!(USART0.STATUS & USART_DREIF_bm)); // Data Register Empty
	USART0.TXDATAL = c;
}

void UART_WriteBuffer(const char *data, uint8_t length) 
{
	for (uint8_t i = 0; i < length; i++) 
	{
		UART_WriteChar(data[i]);
	}
}

char UART_ReadChar(void) 
{
	while (!(USART0.STATUS & USART_RXCIF_bm)); // RX Complete
	return USART0.RXDATAL;
}

bool UART_DataAvailable(void) 
{
	return (USART0.STATUS & USART_RXCIF_bm);
}
