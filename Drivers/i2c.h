/*
 * i2c.h
 *
 * Created: 2026-05-02 오후 2:40:18
 *  Author: jymen
 */ 

#ifndef I2C_H_
#define I2C_H_

#include  "def.h"


// 초기화 함수 (기본 설정)
void I2C_Init(void);

// Open 함수 (클록 설정)
void I2C_Open(uint32_t f_cpu, uint32_t scl_freq);

// Write 함수 (슬레이브 주소 + 데이터 전송)
bool I2C_Write(uint8_t slave_addr, const uint8_t *data, uint8_t length);

// Read 함수 (슬레이브 주소 + 데이터 읽기)
bool I2C_Read(uint8_t slave_addr, uint8_t *data, uint8_t length);

#endif /* I2C_H_ */
