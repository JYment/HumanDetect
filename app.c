/*
 * app.c
 *
 * Created: 2026-05-02 오후 2:00:22
 *  Author: jymen
 */ 
#include "app.h"

gpio_input_opt_t g_optPA1 = {
	.pullup = false,
	.ext_int = false,
	.isc_mode = PORT_ISC_INTDISABLE_gc
};

app_state_t status;
uint32_t millis_cnt;
uint16_t distanceArr[30];
uint8_t distanceCnt;
uint32_t distacneTotal;
bool isOn_PA5;

ISR(PORTA_PORT_vect)
{	
	if(GPIO_Read(&PORTA, 5)) 
	{
//		isOn_PA5 = true;
		PORTA.INTFLAGS = PIN5_bm;
	}
}


void APP_Init(void)
{
	clock_init(CLOCK_SOURCE_INTERNAL);
	clock_set_prescaler(2);
	GPIO_Init();
	I2C_Init();
	UART_Init();
	
	GPIO_Open(&PORTA, 2, GPIO_DIR_OUTPUT, NULL);			// Power 5V Enable
	GPIO_Open(&PORTC, 0, GPIO_DIR_OUTPUT, NULL);			// LED
	GPIO_Open(&PORTA, 5, GPIO_DIR_INPUT, NULL);				// SR501_L
	GPIO_Open(&PORTA, 1, GPIO_DIR_INPUT, NULL);
	UART_Open(F_CPU, 9600);								// F_CPU=10MHz, Baud=9600
	
	I2C_Open(F_CPU, 100000ul);
	TIMER_OpenMillis(F_CPU, 64);
	GPIO_Write(&PORTA, 2, true);
				
	sei();

	if (!VL53_Init())
	{
		while (1);
	}
	
	_delay_ms(2000);
	DF_SetVolume(30);
	
	millis_cnt = millis();

	distanceCnt = 0;
}

void APP_Run(void)
{
	uint16_t avgDistance;

	GPIO_Write(&PORTC, 0, isOn_PA5);	
	isOn_PA5 = GPIO_Read(&PORTA, 5);
	
	if(millis() - millis_cnt > 100)
	{
		millis_cnt = millis();
		uint16_t distance = VL53_ReadDistance();

		if(isOn_PA5 == true)
		{
			if(distance > 25)
			{
				distanceArr[distanceCnt] = distance;
				distacneTotal += distance;
				distanceCnt++;
			}
		}
	}	

	if(distanceCnt == 20)
	{
		avgDistance = distacneTotal / distanceCnt;
		distacneTotal = 0;
		distanceCnt = 0;
		if(avgDistance <= 1000)
		{
			DF_Play(1);
			_delay_ms(6000);
		}
		for(uint8_t i=0; i<20; i++)
			distanceArr[i] = 0;
	}
	
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