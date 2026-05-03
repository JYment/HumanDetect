/*
 * power.h
 *
 * Created: 2026-05-03 오전 10:17:16
 *  Author: loveh
 */ 


#ifndef POWER_H_
#define POWER_H_


#include "def.h"


/**
 * @brief 저전력 모드 종류
 */
typedef enum {
    POWER_MODE_IDLE    = SLPCTRL_SMODE_IDLE_gc,
    POWER_MODE_STANDBY = SLPCTRL_SMODE_STDBY_gc,
    POWER_MODE_PDOWN   = SLPCTRL_SMODE_PDOWN_gc
} power_mode_t;

/**
 * @brief 저전력 모드 초기화
 */
void POWER_init(void);

/**
 * @brief 저전력 모드 설정
 */
void POWER_set_mode(power_mode_t mode);

/**
 * @brief 저전력 모드 진입
 */
void POWER_enter(void);




#endif /* POWER_H_ */