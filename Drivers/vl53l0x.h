/*
 * vl53l0x.h
 *
 * Created: 2026-05-06 오후 9:36:27
 *  Author: loveh
 */ 


#ifndef VL53L0X_H_
#define VL53L0X_H_


#include "def.h"


#define VL53L0X_ADDR                    0x29   /* 기본 7-bit 주소 */

/* 식별 */
#define REG_IDENTIFICATION_MODEL_ID     0xC0   /* 고정값 0xEE */
#define REG_IDENTIFICATION_REVISION_ID  0xC2

/* 전원 / 시퀀서 */
#define REG_VHV_CONFIG_PAD_SCL_SDA     0x88
#define REG_MSRC_CONFIG_CONTROL        0x60
#define REG_FINAL_RANGE_CONFIG_MIN_CNT 0x44
#define REG_GLOBAL_CONFIG_SPAD_EN_REF  0xB0   /* 0xB0~0xB3 (4 bytes) */
#define REG_DYNAMIC_SPAD_REF_EN_START  0x4F
#define REG_POWER_MANAGEMENT_GO1       0x80
#define REG_SYSTEM_SEQUENCE_CONFIG     0x01
#define REG_SYSRANGE_START             0x00
#define REG_RESULT_INTERRUPT_STATUS    0x13
#define REG_RESULT_RANGE_STATUS        0x14   /* +2 = range high byte */
#define REG_SYSTEM_INTERRUPT_CLEAR     0x0B
#define REG_GPIO_HV_MUX_ACTIVE_HIGH    0x84
#define REG_SYSTEM_INTERRUPT_CONFIG    0x0A
#define REG_OSC_CALIBRATE_VAL          0xF8

enum regAddr
{
	SYSRANGE_START                              = 0x00,

	SYSTEM_THRESH_HIGH                          = 0x0C,
	SYSTEM_THRESH_LOW                           = 0x0E,

	SYSTEM_SEQUENCE_CONFIG                      = 0x01,
	SYSTEM_RANGE_CONFIG                         = 0x09,
	SYSTEM_INTERMEASUREMENT_PERIOD              = 0x04,

	SYSTEM_INTERRUPT_CONFIG_GPIO                = 0x0A,

	GPIO_HV_MUX_ACTIVE_HIGH                     = 0x84,

	SYSTEM_INTERRUPT_CLEAR                      = 0x0B,

	RESULT_INTERRUPT_STATUS                     = 0x13,
	RESULT_RANGE_STATUS                         = 0x14,

	RESULT_CORE_AMBIENT_WINDOW_EVENTS_RTN       = 0xBC,
	RESULT_CORE_RANGING_TOTAL_EVENTS_RTN        = 0xC0,
	RESULT_CORE_AMBIENT_WINDOW_EVENTS_REF       = 0xD0,
	RESULT_CORE_RANGING_TOTAL_EVENTS_REF        = 0xD4,
	RESULT_PEAK_SIGNAL_RATE_REF                 = 0xB6,

	ALGO_PART_TO_PART_RANGE_OFFSET_MM           = 0x28,

	I2C_SLAVE_DEVICE_ADDRESS                    = 0x8A,

	MSRC_CONFIG_CONTROL                         = 0x60,

	PRE_RANGE_CONFIG_MIN_SNR                    = 0x27,
	PRE_RANGE_CONFIG_VALID_PHASE_LOW            = 0x56,
	PRE_RANGE_CONFIG_VALID_PHASE_HIGH           = 0x57,
	PRE_RANGE_MIN_COUNT_RATE_RTN_LIMIT          = 0x64,

	FINAL_RANGE_CONFIG_MIN_SNR                  = 0x67,
	FINAL_RANGE_CONFIG_VALID_PHASE_LOW          = 0x47,
	FINAL_RANGE_CONFIG_VALID_PHASE_HIGH         = 0x48,
	FINAL_RANGE_CONFIG_MIN_COUNT_RATE_RTN_LIMIT = 0x44,

	PRE_RANGE_CONFIG_SIGMA_THRESH_HI            = 0x61,
	PRE_RANGE_CONFIG_SIGMA_THRESH_LO            = 0x62,

	PRE_RANGE_CONFIG_VCSEL_PERIOD               = 0x50,
	PRE_RANGE_CONFIG_TIMEOUT_MACROP_HI          = 0x51,
	PRE_RANGE_CONFIG_TIMEOUT_MACROP_LO          = 0x52,

	SYSTEM_HISTOGRAM_BIN                        = 0x81,
	HISTOGRAM_CONFIG_INITIAL_PHASE_SELECT       = 0x33,
	HISTOGRAM_CONFIG_READOUT_CTRL               = 0x55,

	FINAL_RANGE_CONFIG_VCSEL_PERIOD             = 0x70,
	FINAL_RANGE_CONFIG_TIMEOUT_MACROP_HI        = 0x71,
	FINAL_RANGE_CONFIG_TIMEOUT_MACROP_LO        = 0x72,
	CROSSTALK_COMPENSATION_PEAK_RATE_MCPS       = 0x20,

	MSRC_CONFIG_TIMEOUT_MACROP                  = 0x46,

	SOFT_RESET_GO2_SOFT_RESET_N                 = 0xBF,
	IDENTIFICATION_MODEL_ID                     = 0xC0,
	IDENTIFICATION_REVISION_ID                  = 0xC2,

	OSC_CALIBRATE_VAL                           = 0xF8,

	GLOBAL_CONFIG_VCSEL_WIDTH                   = 0x32,
	GLOBAL_CONFIG_SPAD_ENABLES_REF_0            = 0xB0,
	GLOBAL_CONFIG_SPAD_ENABLES_REF_1            = 0xB1,
	GLOBAL_CONFIG_SPAD_ENABLES_REF_2            = 0xB2,
	GLOBAL_CONFIG_SPAD_ENABLES_REF_3            = 0xB3,
	GLOBAL_CONFIG_SPAD_ENABLES_REF_4            = 0xB4,
	GLOBAL_CONFIG_SPAD_ENABLES_REF_5            = 0xB5,

	GLOBAL_CONFIG_REF_EN_START_SELECT           = 0xB6,
	DYNAMIC_SPAD_NUM_REQUESTED_REF_SPAD         = 0x4E,
	DYNAMIC_SPAD_REF_EN_START_OFFSET            = 0x4F,
	POWER_MANAGEMENT_GO1_POWER_FORCE            = 0x80,

	VHV_CONFIG_PAD_SCL_SDA__EXTSUP_HV           = 0x89,

	ALGO_PHASECAL_LIM                           = 0x30,
	ALGO_PHASECAL_CONFIG_TIMEOUT                = 0x30,
};


bool VL53_Init(void);
bool VL53_WriteByte(uint8_t reg, uint8_t value);
bool VL53_WriteWord(uint8_t reg, uint16_t value);
uint8_t VL53_ReadByte(uint16_t reg);
void VL53_ReadBytes(uint8_t reg, uint8_t *data, uint8_t len);
void VL53_ReadBytes16(uint16_t reg, uint8_t *data, uint8_t len);

uint16_t VL53_ReadDistance(void);

#endif /* VL53L0X_H_ */