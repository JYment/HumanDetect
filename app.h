/*
 * app.h
 *
 * Created: 2026-05-02 오후 2:00:29
 *  Author: jymen
 */ 


#ifndef APP_H_
#define APP_H_

#include "clock.h"
#include "power.h"
#include "wdt.h"
#include "timer.h"
#include "gpio.h"
#include "i2c.h"
#include "uart.h"
#include "vl53l0x.h"
#include "DFPlayer.h"


typedef enum {
	IDLE	= 0,
	SLEEP	= 1,
	ACTIVE	= 2,
	ERROR	= 3
} app_state_t;



void APP_Init(void);
void APP_Run(void);


#endif /* APP_H_ */