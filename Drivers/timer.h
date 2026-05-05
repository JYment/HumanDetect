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

void TIMER_init(void);
void TIMER_open(void);
void TIMER_close(void);


#endif /* TIMER_H_ */