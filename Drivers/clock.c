/*
 * clock.c
 *
 * Created: 2026-05-02 오후 3:09:50
 *  Author: jymen
 */ 
#include "clock.h"


static uint32_t current_freq = 20000000UL; // 기본 20MHz


void CLOCK_Init(void) {
	// 기본 클록: 20MHz RC 오실레이터, 분주기 없음
  CCP = CCP_IOREG_gc; // 보호 해제
  CLKCTRL.MCLKCTRLA = CLKCTRL_CLKSEL_OSC20M_gc;

  CCP = CCP_IOREG_gc; // 보호 해제
  CLKCTRL.MCLKCTRLB = 0; // 분주기 비활성화

	current_freq = 20000000UL;
}

void CLOCK_Set(void) 
{
//	_PROTECTED_WRITE(CLKCTRL_MCLKCTRLB, 0);		// 20MHz
 	_PROTECTED_WRITE(CLKCTRL_MCLKCTRLB, (CLKCTRL_PEN_bm | CLKCTRL_PDIV_2X_gc));		// 10MHz
// 	_PROTECTED_WRITE(CLKCTRL_MCLKCTRLB, (CLKCTRL_PEN_bm | CLKCTRL_PDIV_4X_gc));		// 5MHz
// 	_PROTECTED_WRITE(CLKCTRL_MCLKCTRLB, (CLKCTRL_PEN_bm | CLKCTRL_PDIV_6X_gc));		// 3.333..MHz
// 	_PROTECTED_WRITE(CLKCTRL_MCLKCTRLB, (CLKCTRL_PEN_bm | CLKCTRL_PDIV_48X_gc));		// 0.416..MHz
}

uint32_t CLOCK_GetFrequency(void) {
	return current_freq;
}
