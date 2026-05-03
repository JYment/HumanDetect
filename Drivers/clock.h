/*
 * clock.h
 *
 * Created: 2026-05-02 오후 3:09:40
 *  Author: jymen
 */ 
#ifndef CLOCK_H_
#define CLOCK_H_

#include "def.h"

// 클록 소스 선택
typedef enum {
	CLOCK_SOURCE_OSC20M,      // 20MHz 내부 RC
	CLOCK_SOURCE_OSCULP32K,   // 32.768kHz ULP
	CLOCK_SOURCE_EXTCLK       // 외부 클록 입력
} ClockSource_t;

// 초기화 함수 (기본 20MHz)
void CLOCK_Init(void);

// 클록 설정 함수 (소스 + 분주기)
void CLOCK_Set(void);

// 현재 클록 주파수 반환
uint32_t CLOCK_GetFrequency(void);

#endif /* CLOCK_H_ */
