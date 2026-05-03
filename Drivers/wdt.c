/*
 * wdt.c
 *
 * Created: 2026-05-03 오전 10:19:11
 *  Author: loveh
 */ 

#include "wdt.h"

void WDT_enable(wdt_period_t period)
{
	// WDT 활성화 (PERIOD + ENABLE)
	_PROTECTED_WRITE(WDT.CTRLA, period | (1 << 0)); // ENABLE = bit0
}

void WDT_disable(void)
{
	_PROTECTED_WRITE(WDT.CTRLA, 0x00);
}

void WDT_reset(void)
{
	__asm__ __volatile__("wdr");
}