/*
 * clock.c
 *
 * Created: 2026-05-02 오후 3:09:50
 *  Author: jymen
 */ 
#include "clock.h"


void clock_init(clock_source_t source)
{
	if(source == CLOCK_SOURCE_INTERNAL)
	{
		// 내부 20MHz 오실레이터 사용
		_PROTECTED_WRITE(CLKCTRL.MCLKCTRLA, CLKCTRL_CLKSEL_OSC20M_gc);
	}
	else if(source == CLOCK_SOURCE_EXTERNAL)
	{
		// 외부 클럭/크리스털 사용
		_PROTECTED_WRITE(CLKCTRL.MCLKCTRLA, CLKCTRL_CLKSEL_EXTCLK_gc);
	}
}

void clock_set_prescaler(uint8_t div)
{
	// 분주기 설정 (예: 2, 4, 8 ...)
	// CLKCTRL.MCLKCTRLB 레지스터 사용
	switch(div)
	{
		case 2:
		_PROTECTED_WRITE(CLKCTRL.MCLKCTRLB, CLKCTRL_PDIV_2X_gc | CLKCTRL_PEN_bm);
		break;
		case 4:
		_PROTECTED_WRITE(CLKCTRL.MCLKCTRLB, CLKCTRL_PDIV_4X_gc | CLKCTRL_PEN_bm);
		break;
		case 8:
		_PROTECTED_WRITE(CLKCTRL.MCLKCTRLB, CLKCTRL_PDIV_8X_gc | CLKCTRL_PEN_bm);
		break;
		case 16:
		_PROTECTED_WRITE(CLKCTRL.MCLKCTRLB, CLKCTRL_PDIV_16X_gc | CLKCTRL_PEN_bm);
		break;
		default:
		// 기본값: 분주기 없음
		_PROTECTED_WRITE(CLKCTRL_MCLKCTRLB, 0);
		break;
	}
}

clock_source_t clock_get_source(void)
{
	uint8_t sel = CLKCTRL.MCLKCTRLA & CLKCTRL_CLKSEL_gm;
	if(sel == CLKCTRL_CLKSEL_OSC20M_gc)
	{
		return CLOCK_SOURCE_INTERNAL;
	}
	else
	{
		return CLOCK_SOURCE_EXTERNAL;
	}
}
