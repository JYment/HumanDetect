/*
 * timer.h
 *
 * Created: 2026-05-03 오전 10:18:24
 *  Author: loveh
 */ 


#ifndef TIMER_H_
#define TIMER_H_

#include "def.h"

typedef struct {
	bool is_open;
} timer_instance_t;


bool TIMER_Open(uint32_t f_cpu, uint16_t prescaler, uint32_t target_freq);
bool TIMER_OpenMillis(uint32_t f_cpu, uint16_t prescaler);
void TIMER_Close(void);
uint32_t millis(void);

#endif /* TIMER_H_ */