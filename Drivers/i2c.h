/*
 * i2c.h
 *
 * Created: 2026-05-02 오후 2:40:18
 *  Author: jymen
 */ 

#ifndef I2C_H_
#define I2C_H_

#include  "def.h"



void I2C_Init(void);
void I2C_Open(uint32_t f_cpu, uint32_t scl_freq);
bool I2C_WriteBytes(uint8_t slave_addr, const uint8_t *data, uint8_t length);
bool I2C_ReadBytes(uint8_t slave_addr, uint8_t *data, uint8_t length);
bool I2C_ReadRegAddr8(uint8_t addr, uint8_t reg, uint8_t *data, uint8_t len);
bool I2C_ReadRegAddr16(uint8_t addr, uint16_t reg, uint8_t *data, uint8_t len);

#endif /* I2C_H */
 