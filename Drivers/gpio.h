/*
 * gpio.h
 *
 * Created: 2026-05-02 오후 1:56:05
 *  Author: jymen
 */ 

#ifndef GPIO_H_
#define GPIO_H_

#include "def.h"

// 핀 방향 정의
typedef enum {
	GPIO_DIR_INPUT,
	GPIO_DIR_OUTPUT
} gpio_dir_t;

// 입력 옵션 정의
typedef struct {
	bool pullup;       // 내부 Pull-up 사용 여부
	bool ext_int;      // 외부 인터럽트 사용 여부
	uint8_t isc_mode;  // 인터럽트 감지 모드 (PORT_ISC_RISING_gc 등)
} gpio_input_opt_t;

// 초기화 함수
void GPIO_Init(void);

// 핀 Open 함수 (입출력 설정)
void GPIO_Open(volatile PORT_t *port, uint8_t pin, gpio_dir_t dir, gpio_input_opt_t *opt);

// 출력 제어 함수
void GPIO_Write(volatile PORT_t *port, uint8_t pin, bool value);

// 입력 읽기 함수
bool GPIO_Read(volatile PORT_t *port, uint8_t pin);

#endif /* GPIO_H_ */

