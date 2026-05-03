/*
 * app.c
 *
 * Created: 2026-05-02 오후 2:00:22
 *  Author: jymen
 */ 
#include "app.h"


app_state_t status;



ISR(PORTA_PORT_vect)
{
	if (VPORTA.INTFLAGS & (1 << 2))
	{
		VPORTA.INTFLAGS = (1 << 2); // 플래그 클리어
	}
}


void APP_Init(void)
{
	clock_init(CLOCK_SOURCE_INTERNAL);
	clock_set_prescaler(2);

	GPIO_Init();
	I2C_Init();
	UART_Init();


	GPIO_Open(&PORTA, 1, GPIO_DIR_OUTPUT, NULL);
	I2C_Open(F_CPU, 100000UL); // F_CPU=20MHz, SCL=100kHz
	UART_Open(F_CPU, 9600); // F_CPU=20MHz, Baud=9600

	sei();

	WDT_enable(WDT_PERIOD_2S);
	POWER_set_mode(POWER_MODE_PDOWN);
	UART_WriteBuffer("Hello VL53LX!\r\n", 15);
}

void APP_Run(void)
{
	WDT_reset();
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
	
		
	switch(status)
	{
		case IDLE:
		// TIMER 동작... 10초 지나면 SLEEP으로 이동
		break;

		case SLEEP:
		POWER_enter();
		break;

		case ACTIVE:
		break;

		case ERROR:
		break;
	}
}