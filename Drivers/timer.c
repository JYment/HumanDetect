/*
 * timer.c
 *
 * Created: 2026-05-03 오전 10:18:15
 *  Author: loveh
 */ 

#include "timer.h"

void TIMER_init(timer_instance_t *inst)
{
	TCA0.SINGLE.CTRLA = TCA_SINGLE_CLKSEL_DIV64_gc;
	inst->is_open = false;
}

void TIMER_open(timer_instance_t *inst)
{
	TCA0.SINGLE.CTRLA |= TCA_SINGLE_ENABLE_bm;
	inst->is_open = true;
}

void TIMER_close(timer_instance_t *inst)
{
	TCA0.SINGLE.CTRLA &= ~TCA_SINGLE_ENABLE_bm;
	inst->is_open = false;
}
