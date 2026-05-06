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

bool I2C_WriteBytes(uint8_t slave_addr, const uint8_t *data, uint8_t length)
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

bool I2C_ReadBytes(uint8_t slave_addr, uint8_t *data, uint8_t length)
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



bool I2C_ReadRegAddr8(uint8_t addr, uint8_t reg, uint8_t *data, uint8_t len)
{
	// START + SLA+W
	TWI0.MADDR = (addr << 1) | 0;
	while (!(TWI0.MSTATUS & TWI_WIF_bm));

	TWI0.MDATA = (uint8_t)(reg & 0xFF);
	while (!(TWI0.MSTATUS & TWI_WIF_bm));
	
	// REPEATED START + SLA+R
	TWI0.MADDR = (addr << 1) | 1;
	
	for (uint8_t i = 0; i < len; i++)
	{
		// RIF 대기
		while (!(TWI0.MSTATUS & TWI_RIF_bm));

		if (i == len - 1)
		{
			TWI0.MCTRLB = TWI_ACKACT_NACK_gc | TWI_MCMD_STOP_gc;
		}
		else
		{
			TWI0.MCTRLB = TWI_MCMD_RECVTRANS_gc;
		}
		
		data[i] = TWI0.MDATA; // ✅ ACK/NACK 설정 후 읽기
	}

	return true;
}


bool I2C_ReadRegAddr16(uint8_t addr, uint16_t reg, uint8_t *data, uint8_t len)
{
	// START + SLA+W
	TWI0.MADDR = (addr << 1) | 0;
	while (!(TWI0.MSTATUS & TWI_WIF_bm));
	if (TWI0.MSTATUS & TWI_RXACK_bm)
	{
		TWI0.MCTRLB = TWI_MCMD_STOP_gc;
		return false;
	}

	// 레지스터 주소 상위 바이트
	TWI0.MDATA = (uint8_t)(reg >> 8);
	while (!(TWI0.MSTATUS & TWI_WIF_bm));
	if (TWI0.MSTATUS & TWI_RXACK_bm)
	{
		TWI0.MCTRLB = TWI_MCMD_STOP_gc;
		return false;
	}

	// 레지스터 주소 하위 바이트
	TWI0.MDATA = (uint8_t)(reg & 0xFF);
	while (!(TWI0.MSTATUS & TWI_WIF_bm));
	if (TWI0.MSTATUS & TWI_RXACK_bm)
	{
		TWI0.MCTRLB = TWI_MCMD_STOP_gc;
		return false;
	}

	// REPEATED START + SLA+R
	TWI0.MADDR = (addr << 1) | 1;

	for (uint8_t i = 0; i < len; i++)
	{
		// RIF 대기
		while (!(TWI0.MSTATUS & TWI_RIF_bm));
		
		if (i == len - 1)
		{
			TWI0.MCTRLB = TWI_ACKACT_NACK_gc | TWI_MCMD_STOP_gc;
		}
		else
		{
			TWI0.MCTRLB = TWI_MCMD_RECVTRANS_gc;
		}
		
		data[i] = TWI0.MDATA; // MDATA 읽기가 ACK/NACK 전송 트리거
	}

	return true;
}