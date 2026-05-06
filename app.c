/*
 * app.c
 *
 * Created: 2026-05-02 오후 2:00:22
 *  Author: jymen
 */ 
#include "app.h"


app_state_t status;
uint32_t millis_cnt;
bool isOn;

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
	UART_Open(F_CPU, 9600); // F_CPU=10MHz, Baud=9600
	I2C_Open(F_CPU, 100000ul);
	TIMER_OpenMillis(F_CPU, 64);
	
	_delay_ms(100);
		
	sei();

	printf("Hello VL53LX!\r\n");
	
	 if (!VL53_Init()) 
	 {
		 printf("[ERR] Init failed. Halt.\r\n");
		 while (1);
	 }
	 
	 _delay_ms(100);
	 
	 millis_cnt = millis();
}

void APP_Run(void)
{
	if(millis() - millis_cnt > 100)
	{		
		millis_cnt = millis();
		
		uint16_t distance = VL53_ReadDistance();
		printf("distance = %d\r\n", distance);
		
		if(isOn == true)
		{
			isOn = false;
		}
		else
		{
			isOn = true;
		}
	}
	
	GPIO_Write(&PORTA, 1, isOn);
	
	switch(status)
	{
		case IDLE:
		break;

		case SLEEP:
		break;

		case ACTIVE:
		break;

		case ERROR:
		break;
	}
}