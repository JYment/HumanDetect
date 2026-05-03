/*
 * uart.h
 *
 * Created: 2026-05-02 오후 2:52:06
 *  Author: jymen
 */ 

#ifndef UART_H_
#define UART_H_

#include "def.h"

// 초기화 함수
void UART_Init(void);

// Open 함수 (보레이트 설정)
void UART_Open(uint32_t f_cpu, uint32_t baudrate);

// Write 함수 (문자/버퍼 전송)
void UART_WriteChar(char c);
void UART_WriteBuffer(const char *data, uint8_t length);

// Read 함수 (문자 수신)
char UART_ReadChar(void);
bool UART_DataAvailable(void);

#endif /* UART_H_ */
