/*
 * i2c.c
 *
 * Created: 2026-05-02 오후 2:40:25
 *  Author: jymen
 */ 
#include "i2c.h"

void I2C_Init(void) 
{
	// 기본 초기화: Enable TWI
	TWI0.MCTRLA = TWI_ENABLE_bm;
}

void I2C_Open(uint32_t f_cpu, uint32_t scl_freq) 
{
	// 클록 설정: Baud 계산
	// Baud = (F_CPU / (2 * F_SCL)) - 5
	uint32_t baud = (f_cpu / (2 * scl_freq)) - 5;
	TWI0.MBAUD = (uint8_t)baud;

	// Master Enable
	TWI0.MCTRLA = TWI_ENABLE_bm;
	TWI0.MSTATUS = TWI_BUSSTATE_IDLE_gc;
}

bool I2C_Write(uint8_t slave_addr, const uint8_t *data, uint8_t length)
{
	// Start + Slave Address (Write)
	TWI0.MADDR = (slave_addr << 1) | 0;

	for (uint8_t i = 0; i < length; i++) 
	{
		TWI0.MDATA = data[i];
		while (!(TWI0.MSTATUS & TWI_WIF_bm)); // Write Interrupt Flag
		if (TWI0.MSTATUS & TWI_RXACK_bm) 
		{
			return false; // NACK
		}
	}

	// Stop
	TWI0.MCTRLB = TWI_MCMD_STOP_gc;
	return true;
}

bool I2C_Read(uint8_t slave_addr, uint8_t *data, uint8_t length) 
{
	// Start + Slave Address (Read)
	TWI0.MADDR = (slave_addr << 1) | 1;

	for (uint8_t i = 0; i < length; i++) {
		while (!(TWI0.MSTATUS & TWI_RIF_bm)); // Read Interrupt Flag
		data[i] = TWI0.MDATA;

		if (i < (length - 1)) 
		{
			TWI0.MCTRLB = TWI_MCMD_RECVTRANS_gc; // ACK
		} 
		else 
		{
			TWI0.MCTRLB = TWI_ACKACT_bm | TWI_MCMD_STOP_gc; // NACK + STOP
		}
	}
	return true;
}
