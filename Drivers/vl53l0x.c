/*
 * vl53l0x.c
 *
 * Created: 2026-05-06 오후 9:36:36
 *  Author: loveh
 */ 

#include "vl53l0x.h"
#include "i2c.h"


uint8_t stop_variable;



#if 1
bool VL53_Init(void)
{
	if (VL53_ReadByte(0x00C0) != 0xEE) return false;

	VL53_WriteByte(0x88, 0x00);
	VL53_WriteByte(0x80, 0x01);
	VL53_WriteByte(0xFF, 0x01);
	VL53_WriteByte(0x00, 0x00);
	stop_variable = VL53_ReadByte(0x0091);
	VL53_WriteByte(0x00, 0x01);
	VL53_WriteByte(0xFF, 0x00);
	VL53_WriteByte(0x80, 0x00);
	
	return true;
}

#else
bool VL53_Init(void)
{
	// 1. ID 확인
	if (VL53_ReadBytes16(0x00C0) != 0xEE) return false;

	// 2. 센서 언락
	VL53_WriteByte(0x88, 0x00);
	VL53_WriteByte(0x80, 0x01);
	VL53_WriteByte(0xFF, 0x01);
	VL53_WriteByte(0x00, 0x00);
	stop_variable = VL53_ReadByte(0x0091);
	VL53_WriteByte(0x00, 0x01);
	VL53_WriteByte(0xFF, 0x00);
	VL53_WriteByte(0x80, 0x00);

	// 3. interrupt 설정 (중요!)
	VL53_WriteByte(0x0A, 0x04); // new sample ready
	VL53_WriteByte(0x0B, 0x01); // clear

	// ---------------------------
	// 4. SPAD 정보 읽기
	// ---------------------------
	uint8_t spad_count;
	bool spad_is_aperture;

	VL53_WriteByte(0x80, 0x01);
	VL53_WriteByte(0xFF, 0x01);
	VL53_WriteByte(0x00, 0x00);
	VL53_WriteByte(0xFF, 0x06);
	VL53_WriteByte(0x83, VL53_ReadByte(0x0083) | 0x04);
	VL53_WriteByte(0xFF, 0x07);
	VL53_WriteByte(0x81, 0x01);
	VL53_WriteByte(0x80, 0x01);
	VL53_WriteByte(0x94, 0x6B);
	VL53_WriteByte(0x83, 0x00);

	uint16_t timeout = 5000;
	while (VL53_ReadByte(0x0083) == 0x00)
	{
		if (--timeout == 0) return false;
		_delay_us(100);
	}

	VL53_WriteByte(0x83, 0x01);

	uint8_t tmp = VL53_ReadByte(0x0092);
	spad_count = tmp & 0x7F;
	spad_is_aperture = (tmp >> 7) & 0x01;

	VL53_WriteByte(0x81, 0x00);
	VL53_WriteByte(0xFF, 0x06);
	VL53_WriteByte(0x83, VL53_ReadByte(0x0083) & ~0x04);
	VL53_WriteByte(0xFF, 0x01);
	VL53_WriteByte(0x00, 0x01);
	VL53_WriteByte(0xFF, 0x00);
	VL53_WriteByte(0x80, 0x00);

	// ---------------------------
	// 5. SPAD enable (핵심)
	// ---------------------------
	uint8_t ref_spad_map[6];

	for (uint8_t i = 0; i < 6; i++)
	ref_spad_map[i] = VL53_ReadByte(0x00B0 + i);

	uint8_t first_spad = spad_is_aperture ? 12 : 0;
	uint8_t enabled = 0;

	for (uint8_t i = 0; i < 48; i++)
	{
		if (i < first_spad || enabled >= spad_count)
		{
			ref_spad_map[i/8] &= ~(1 << (i % 8));
		}
		else
		{
			ref_spad_map[i/8] |= (1 << (i % 8));
			enabled++;
		}
	}

	for (uint8_t i = 0; i < 6; i++)
	VL53_WriteByte(0xB0 + i, ref_spad_map[i]);

	// ---------------------------
	// 6. tuning (최소 버전)
	// ---------------------------
	VL53_WriteByte(0xFF, 0x01);
	VL53_WriteByte(0x00, 0x00);
	VL53_WriteByte(0xFF, 0x00);
	VL53_WriteByte(0x09, 0x00);
	VL53_WriteByte(0x10, 0x00);
	VL53_WriteByte(0x11, 0x00);

	// ---------------------------
	// 🔥 7. Calibration (핵심)
	// ---------------------------

	// VHV calibration only
	VL53_WriteByte(0x01, 0x01);
	VL53_WriteByte(0x00, 0x01);
	while (VL53_ReadByte(0x0000) & 0x01);

	timeout = 10000;
	while ((VL53_ReadByte(0x0013) & 0x07) == 0)
	{
		if (--timeout == 0) return false;
		_delay_us(100);
	}
	VL53_WriteByte(0x0B, 0x01);

	// Phase calibration only
	VL53_WriteByte(0x01, 0x02);
	VL53_WriteByte(0x00, 0x01);
	while (VL53_ReadByte(0x0000) & 0x01);

	timeout = 1000;
	while ((VL53_ReadByte(0x0013) & 0x07) == 0)
	{
		if (--timeout == 0) return false;
		_delay_us(100);
	}
	VL53_WriteByte(0x0B, 0x01);

	// ---------------------------
	// 8. 최종 sequence 설정
	// ---------------------------
	VL53_WriteByte(0x01, 0xE8);

	return true;
}
#endif

uint16_t VL53_ReadDistance(void)
{
	VL53_WriteByte(0x80, 0x01);
	VL53_WriteByte(0xFF, 0x01);
	VL53_WriteByte(0x00, 0x00);
	VL53_WriteByte(0x91, stop_variable);
	VL53_WriteByte(0x00, 0x01);
	VL53_WriteByte(0xFF, 0x00);
	VL53_WriteByte(0x80, 0x00);

	// SYSRANGE_START 쓰기
	VL53_WriteByte(0x00, 0x01);
	while (VL53_ReadByte(0x0000) & 0x01);

	uint16_t timeout = 5000;
	uint8_t data;
	
	while ((data & 0x07) == 0)
	{
		VL53_ReadBytes16(0x0013, &data, 1);
		if (--timeout == 0) return 0xFFFF;
		_delay_us(10);
	}
	
	uint8_t rangeArr[2];
	uint16_t range;
	VL53_ReadBytes(0x1E, rangeArr, 2);
	range = (rangeArr[0] << 8) | rangeArr[1];
	
	VL53_WriteByte(0x0B, 0x01);
	VL53_WriteByte(0x00, 0x00);

	return range;
}


bool VL53_WriteByte(uint8_t reg, uint8_t value)
{
	uint8_t buf[2] = {reg, value};
	return I2C_WriteBytes(VL53L0X_ADDR, buf, 2);
}

bool VL53_WriteWord(uint8_t reg, uint16_t value)
{
	uint8_t buf[3] = {reg, (value >> 8) & 0xFF, value & 0xFF};
	return I2C_WriteBytes(VL53L0X_ADDR, buf, 3);
}

uint8_t VL53_ReadByte(uint16_t reg)
{
	uint8_t val = 0;
	I2C_ReadRegAddr8(VL53L0X_ADDR, reg & 0xFF, &val, 1);
	return val;
}

void VL53_ReadBytes(uint8_t reg, uint8_t *data, uint8_t len)
{
	uint8_t buf[64];
	
	if (!I2C_ReadRegAddr8(VL53L0X_ADDR, reg, buf, len))
	{
		buf[0] = 0xFF;
		buf[1] = 0xFF;
	}
	
	for(uint8_t i=0; i<len; i++)
	{
		data[i] = buf[i];
	}
}

void VL53_ReadBytes16(uint16_t reg, uint8_t *data, uint8_t len)
{
	uint8_t buf[64];
	
	if (!I2C_ReadRegAddr16(VL53L0X_ADDR, reg, buf, len))
	{
		buf[0] = 0xFF;
		buf[1] = 0xFF;
	}
	
	for(uint8_t i=0; i<len; i++)
	{
		data[i] = buf[i];
	}
}