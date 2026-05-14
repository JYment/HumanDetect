/*
 * app.c
 *
 * Created: 2026-05-02 오후 2:00:22
 *  Author: jymen
 */ 
#include "app.h"

gpio_input_opt_t g_optPC3 = {
	.pullup = true,
	.ext_int = true,
	.isc_mode = PORT_ISC_RISING_gc
};

gpio_input_opt_t g_optPA4 = {
	.pullup = false,
	.ext_int = true,
	.isc_mode = PORT_ISC_RISING_gc
};

gpio_input_opt_t g_optPA5 = {
	.pullup = false,
	.ext_int = true,
	.isc_mode = PORT_ISC_RISING_gc
};

app_state_t status;
uint32_t millis_cnt;
bool isOn_PA4;
bool isOn_PA5;
bool isOn_PC3;


ISR(PORTA_PORT_vect)
{	
	if(GPIO_Read(&PORTA, 4)) 
	{
		if(isOn_PA4 == true)
		{
			isOn_PA4 = false;
		}
		else
		{
			isOn_PA4 = true;
		}
		PORTA.INTFLAGS = PIN4_bm;
	}
	
	if(GPIO_Read(&PORTA, 5))
	{
		if(isOn_PA5 == true)
		{
			isOn_PA5 = false;
		}
		else
		{
			isOn_PA5 = true;
		}
		PORTA.INTFLAGS = PIN5_bm;
	}
	
	if(GPIO_Read(&PORTC, 3))
	{
		isOn_PC3 = true;
		
			
		PORTC.INTFLAGS = PIN3_bm;		
	}	
}


void APP_Init(void)
{
	clock_init(CLOCK_SOURCE_INTERNAL);
	clock_set_prescaler(2);
	GPIO_Init();
	I2C_Init();
	UART_Init();
	
	GPIO_Open(&PORTA, 2, GPIO_DIR_OUTPUT, NULL);		// Power 5V Enable
	GPIO_Open(&PORTC, 1, GPIO_DIR_OUTPUT, NULL);		// LED
	GPIO_Open(&PORTC, 0, GPIO_DIR_OUTPUT, NULL);		// LED
	GPIO_Open(&PORTC, 3, GPIO_DIR_INPUT, &g_optPC3);	// SW
	GPIO_Open(&PORTA, 4, GPIO_DIR_INPUT, &g_optPA4);	// SR501_R
	GPIO_Open(&PORTA, 5, GPIO_DIR_INPUT, &g_optPA5);	// SR501_L
	UART_Open(F_CPU, 9600);								// F_CPU=10MHz, Baud=9600
	
	I2C_Open(F_CPU, 100000ul);
	TIMER_OpenMillis(F_CPU, 64);
	GPIO_Write(&PORTA, 2, true);
			
	sei();

	if (!VL53_Init())
	{
		while (1);
	}
	
	_delay_ms(100);	
	 
	_delay_ms(2000);
	DF_SetVolume(30);   
	DF_Play(1);
	
	millis_cnt = millis();
}

void APP_Run(void)
{
	uint8_t resp[10];
	uint8_t cmd = DF_ReadResponse(resp);
	if(cmd == 0x3D)
	{
		uint16_t track = (resp[5] << 8) | resp[6];
		DF_Next();
	}
	
	if(millis() - millis_cnt > 5000)
	{
		millis_cnt = millis();
		uint16_t distance = VL53_ReadDistance();
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