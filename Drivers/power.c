/*
 * power.c
 *
 * Created: 2026-05-03 오전 10:17:22
 *  Author: loveh
 */ 

#include "power.h"

void POWER_init(void)
{
	// 기본적으로 Idle 모드로 설정
	SLPCTRL.CTRLA = POWER_MODE_IDLE | SLPCTRL_SEN_bm;
}

void POWER_set_mode(power_mode_t mode)
{
	// 원하는 모드로 설정

	_PROTECTED_WRITE(SLPCTRL.CTRLA, mode | SLPCTRL_SEN_bm);
}

void POWER_enter(void)
{
	// Sleep 명령 실행
	__asm__ __volatile__("sleep");
}

