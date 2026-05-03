/*
 * app.c
 *
 * Created: 2026-05-02 오후 2:00:22
 *  Author: jymen
 */ 
#include "app.h"

void APP_Init(void)
{
//	CLOCK_Set(CLOCK_SOURCE_OSC20M, 1);



//	CLOCK_Set();
	GPIO_Init();
	I2C_Init();
	UART_Init();


	GPIO_Open(&PORTA, 1, GPIO_DIR_OUTPUT, NULL);
	I2C_Open(F_CPU, 100000UL); // F_CPU=20MHz, SCL=100kHz
	UART_Open(F_CPU, 9600); // F_CPU=20MHz, Baud=9600
	_delay_ms(100);

	sei();

	UART_WriteBuffer("Hello VL53LX!\r\n", 15);
}

void APP_Run(void)
{
	GPIO_Write(&PORTA, 1, true);

//	uint8_t txData[2] = {0x01, 0x02};
//	I2C_Write(0x29, txData, 2); // VL53LX 같은 센서에 쓰기

//	uint8_t rxData[4];
//	I2C_Read(0x29, rxData, 4); // 데이터 읽기

	if (UART_DataAvailable()) 
	{
		char c = UART_ReadChar();
		UART_WriteChar(c); 
	}
}