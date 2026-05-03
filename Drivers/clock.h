/*
 * clock.h
 *
 * Created: 2026-05-02 오후 3:09:40
 *  Author: jymen
 */ 
#ifndef CLOCK_H_
#define CLOCK_H_

#include "def.h"


typedef enum {
    CLOCK_SOURCE_INTERNAL = 0,  // 내부 20MHz RC 오실레이터
    CLOCK_SOURCE_EXTERNAL = 1   // 외부 크리스털/클럭 입력
} clock_source_t;

/**
 * @brief 클럭 초기화
 */
void clock_init(clock_source_t source);

/**
 * @brief 클럭 주파수 설정 (분주기)
 */
void clock_set_prescaler(uint8_t div);

/**
 * @brief 현재 클럭 소스 확인
 */
clock_source_t clock_get_source(void);


#endif /* CLOCK_H_ */
