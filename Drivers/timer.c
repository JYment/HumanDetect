/*
 * timer.c
 *
 * Created: 2026-05-03 오전 10:18:15
 *  Author: loveh
 */ 

#include "timer.h"
volatile uint32_t millis_counter = 0;

ISR(TCA0_OVF_vect)
{
	millis_counter++;
	TCA0.SINGLE.INTFLAGS = TCA_SINGLE_OVF_bm;
}

static void set_prescaler(uint16_t prescaler)
{
	switch (prescaler)
	{
		case 1:    TCA0.SINGLE.CTRLA = TCA_SINGLE_CLKSEL_DIV1_gc;    break;
		case 2:    TCA0.SINGLE.CTRLA = TCA_SINGLE_CLKSEL_DIV2_gc;    break;
		case 4:    TCA0.SINGLE.CTRLA = TCA_SINGLE_CLKSEL_DIV4_gc;    break;
		case 8:    TCA0.SINGLE.CTRLA = TCA_SINGLE_CLKSEL_DIV8_gc;    break;
		case 16:   TCA0.SINGLE.CTRLA = TCA_SINGLE_CLKSEL_DIV16_gc;   break;
		case 64:   TCA0.SINGLE.CTRLA = TCA_SINGLE_CLKSEL_DIV64_gc;   break;
		case 256:  TCA0.SINGLE.CTRLA = TCA_SINGLE_CLKSEL_DIV256_gc;  break;
		case 1024: TCA0.SINGLE.CTRLA = TCA_SINGLE_CLKSEL_DIV1024_gc; break;
		default:   TCA0.SINGLE.CTRLA = TCA_SINGLE_CLKSEL_DIV64_gc;   break;
	}
}

// ✅ bool 반환으로 PER 범위 초과 감지
bool TIMER_Open(uint32_t f_cpu, uint16_t prescaler, uint32_t target_freq)
{
	// ✅ uint32_t 캐스팅으로 오버플로우 방지
	uint32_t per = (f_cpu / ((uint32_t)prescaler * target_freq)) - 1;

	if (per > 0xFFFF) return false; // ❌ 범위 초과

	set_prescaler(prescaler);
	TCA0.SINGLE.PER     = (uint16_t)per;
	TCA0.SINGLE.INTCTRL = TCA_SINGLE_OVF_bm;
	TCA0.SINGLE.CTRLA  |= TCA_SINGLE_ENABLE_bm;
	return true;
}

bool TIMER_OpenMillis(uint32_t f_cpu, uint16_t prescaler)
{
	// ✅ uint32_t 캐스팅으로 오버플로우 방지
	uint32_t per = (f_cpu / ((uint32_t)prescaler * 1000UL)) - 1;

	if (per > 0xFFFF) return false; // ❌ 범위 초과

	set_prescaler(prescaler);
	TCA0.SINGLE.PER     = (uint16_t)per;
	TCA0.SINGLE.INTCTRL = TCA_SINGLE_OVF_bm;
	TCA0.SINGLE.CTRLA  |= TCA_SINGLE_ENABLE_bm;
	return true;
}

void TIMER_Close(void)
{
	TCA0.SINGLE.CTRLA  &= ~TCA_SINGLE_ENABLE_bm;
	TCA0.SINGLE.INTCTRL = 0;
}

uint32_t millis(void)
{
	uint32_t m;
	cli();
	m = millis_counter;
	sei();
	return m;
}