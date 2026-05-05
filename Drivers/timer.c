/*
 * timer.c
 *
 * Created: 2026-05-03 오전 10:18:15
 *  Author: loveh
 */ 

#include "timer.h"

void TIMER_init(void)
{
	TCA0.SINGLE.CTRLA = TCA_SINGLE_CLKSEL_DIV64_gc;
}

void TIMER_open(void)
{
	TCA0.SINGLE.CTRLA |= TCA_SINGLE_ENABLE_bm;
}

void TIMER_close(void)
{
	TCA0.SINGLE.CTRLA &= ~TCA_SINGLE_ENABLE_bm;
}
