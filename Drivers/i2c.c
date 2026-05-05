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
	
	PORTB.DIRSET |= PIN0_bm | PIN1_bm;  // Output 설정
	PORTB.OUTSET |= PIN0_bm | PIN1_bm;  // High (오픈드레인 기본 상태)

	// Master Enable
	TWI0.MCTRLA = TWI_ENABLE_bm | TWI_SMEN_bm;
	TWI0.MSTATUS = TWI_BUSSTATE_IDLE_gc;
}

bool I2C_Write(uint8_t slave_addr, const uint8_t *data, uint8_t length)
{
	while ((TWI0.MSTATUS & TWI_BUSSTATE_gm) == TWI_BUSSTATE_BUSY_gc);

	TWI0.MADDR = (slave_addr << 1) | 0;
	while (!(TWI0.MSTATUS & (TWI_WIF_bm | TWI_RIF_bm)));
	if (TWI0.MSTATUS & TWI_RXACK_bm)
	{
		TWI0.MCTRLB = TWI_MCMD_STOP_gc;
		return false;
	}

	for (uint8_t i = 0; i < length; i++)
	{
		TWI0.MDATA = data[i];
		while (!(TWI0.MSTATUS & TWI_WIF_bm));
		if (TWI0.MSTATUS & TWI_RXACK_bm)
		{
			TWI0.MCTRLB = TWI_MCMD_STOP_gc;
			return false;
		}
	}

	TWI0.MCTRLB = TWI_MCMD_STOP_gc;
	return true;
}

bool I2C_Read(uint8_t slave_addr, uint8_t *data, uint8_t length)
{
	while ((TWI0.MSTATUS & TWI_BUSSTATE_gm) == TWI_BUSSTATE_BUSY_gc);

	TWI0.MADDR = (slave_addr << 1) | 1;
	while (!(TWI0.MSTATUS & (TWI_WIF_bm | TWI_RIF_bm)));
	if (TWI0.MSTATUS & TWI_RXACK_bm)
	{
		TWI0.MCTRLB = TWI_MCMD_STOP_gc;
		return false;
	}

	for (uint8_t i = 0; i < length; i++)
	{
		while (!(TWI0.MSTATUS & TWI_RIF_bm));
		data[i] = TWI0.MDATA;

		if (i < length - 1)
		TWI0.MCTRLB = TWI_MCMD_RECVTRANS_gc; // ACK + 계속
		else
		TWI0.MCTRLB = TWI_ACKACT_NACK_gc | TWI_MCMD_STOP_gc; // NACK + STOP
	}

	return true;
}

bool I2C_ReadReg(uint8_t addr, uint8_t reg, uint8_t *data, uint8_t len)
{
	// START + SLA+W
	TWI0.MADDR = (addr << 1) | 0;
	while (!(TWI0.MSTATUS & TWI_WIF_bm));

	if (TWI0.MSTATUS & TWI_RXACK_bm)
	{
		TWI0.MCTRLB = TWI_MCMD_STOP_gc;
		return false;
	}

	// register address
	TWI0.MDATA = reg;
	while (!(TWI0.MSTATUS & TWI_WIF_bm));

	// ⭐ REPEATED START (중요)
	TWI0.MADDR = (addr << 1) | 1;
	while (!(TWI0.MSTATUS & TWI_RIF_bm));

	for (uint8_t i = 0; i < len; i++)
	{
		data[i] = TWI0.MDATA;

		if (i < len - 1)
		TWI0.MCTRLB = TWI_MCMD_RECVTRANS_gc; // ACK
		else
		TWI0.MCTRLB = TWI_ACKACT_NACK_gc | TWI_MCMD_STOP_gc; // NACK + STOP
	}
	
	_delay_ms(5);

	return true;
}