/*
 * app.h
 *
 * Created: 2026-05-02 오후 2:00:29
 *  Author: jymen
 */ 


#ifndef APP_H_
#define APP_H_

#include "clock.h"
#include "gpio.h"
#include "i2c.h"
#include "uart.h"


void APP_Init(void);
void APP_Run(void);


#endif /* APP_H_ */