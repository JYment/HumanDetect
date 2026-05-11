/*
 * gpio.c
 *
 * Created: 2026-05-02 오후 1:56:14
 *  Author: jymen
 */ 
#include "gpio.h"

// 내부 헬퍼: PINnCTRL 레지스터 포인터 반환
static volatile uint8_t* GPIO_GetPinCtrl(volatile PORT_t *port, uint8_t pin) 
{
	switch(pin) 
	{
		case 0: return &port->PIN0CTRL;
		case 1: return &port->PIN1CTRL;
		case 2: return &port->PIN2CTRL;
		case 3: return &port->PIN3CTRL;
		case 4: return &port->PIN4CTRL;
		case 5: return &port->PIN5CTRL;
		case 6: return &port->PIN6CTRL;
		case 7: return &port->PIN7CTRL;
		default: return NULL;
	}
}

void GPIO_Init(void) 
{
	// 필요하다면 포트 초기화 코드 작성
}

void GPIO_Open(volatile PORT_t *port, uint8_t pin, gpio_dir_t dir, gpio_input_opt_t *opt) 
{
	if (dir == GPIO_DIR_OUTPUT) 
	{
		port->DIRSET = (1 << pin);
	} 
	else 
	{
		port->DIRCLR = (1 << pin);

		volatile uint8_t* pinctrl = GPIO_GetPinCtrl(port, pin);

		if (pinctrl && opt != NULL) 
		{
			uint8_t ctrl = 0;
			if (opt->pullup) 
			{
				ctrl |= PORT_PULLUPEN_bm;
			}
			if (opt->ext_int) 
			{
				ctrl |= opt->isc_mode; // 사용자가 원하는 ISC 모드 지정
			}
			else
			{
				ctrl |= PORT_ISC_INPUT_DISABLE_gc;
			}
			*pinctrl = ctrl;
		}
	}
}

void GPIO_Write(volatile PORT_t *port, uint8_t pin, bool value) 
{
	if (value) 
	{
		port->OUTSET = (1 << pin);
	} 
	else 
	{
		port->OUTCLR = (1 << pin);
	}
}

bool GPIO_Read(volatile PORT_t *port, uint8_t pin) 
{
	return (port->IN & (1 << pin)) != 0;
}
