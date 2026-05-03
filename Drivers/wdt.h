/*
 * wdt.h
 *
 * Created: 2026-05-03 오전 10:19:03
 *  Author: loveh
 */ 


#ifndef WDT_H_
#define WDT_H_

#include "def.h"


typedef enum {
	WDT_PERIOD_8MS   = WDT_PERIOD_8CLK_gc,
	WDT_PERIOD_16MS  = WDT_PERIOD_16CLK_gc,
	WDT_PERIOD_32MS  = WDT_PERIOD_32CLK_gc,
	WDT_PERIOD_64MS  = WDT_PERIOD_64CLK_gc,
	WDT_PERIOD_128MS = WDT_PERIOD_128CLK_gc,
	WDT_PERIOD_256MS = WDT_PERIOD_256CLK_gc,
	WDT_PERIOD_512MS = WDT_PERIOD_512CLK_gc,
	WDT_PERIOD_1S    = WDT_PERIOD_1KCLK_gc,
	WDT_PERIOD_2S    = WDT_PERIOD_2KCLK_gc,
	WDT_PERIOD_4S    = WDT_PERIOD_4KCLK_gc,
	WDT_PERIOD_8S    = WDT_PERIOD_8KCLK_gc
} wdt_period_t;

void WDT_enable(wdt_period_t period);
void WDT_disable(void);
void WDT_reset(void);

#endif /* WDT_H_ */