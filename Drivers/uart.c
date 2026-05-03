/*
 * uart.c
 *
 * Created: 2026-05-02 오후 2:51:59
 *  Author: jymen
 */ 
#include "uart.h"

void UART_Init(void) 
{
	// 기본 초기화: USART0 Enable
	USART0.CTRLA = 0x00; // 인터럽트 비활성화 (필요시 수정)
	USART0.CTRLB = USART_TXEN_bm | USART_RXEN_bm; // 송신/수신 Enable
	USART0.CTRLC = USART_CHSIZE_8BIT_gc; // 8비트 데이터
}

void UART_Open(uint32_t f_cpu, uint32_t baudrate) {
	// Baudrate 계산
	uint16_t baud_setting = (uint16_t)((f_cpu * 64) / (16 * baudrate) + 0.5);
	USART0.BAUD = baud_setting;

	// 핀 방향 설정
	PORTB.DIRSET = PIN2_bm; // PB2 TX 출력
	PORTB.DIRCLR = PIN3_bm; // PB3 RX 입력

	// Enable TX/RX
	USART0.CTRLB = USART_TXEN_bm | USART_RXEN_bm;
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
