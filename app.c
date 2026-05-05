/*
 * app.c
 *
 * Created: 2026-05-02 오후 2:00:22
 *  Author: jymen
 */ 
#include "app.h"


app_state_t status;
uint8_t stop_variable;


ISR(PORTA_PORT_vect)
{
	if (VPORTA.INTFLAGS & (1 << 2))
	{
		VPORTA.INTFLAGS = (1 << 2); // 플래그 클리어
	}
}

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


bool VL53L0X_ReadReg(uint8_t slave_addr, uint8_t reg, uint8_t *data, uint8_t len) 
{
	uint8_t addr_buf = reg & 0xFF;
		
//	return I2C_RepeatedStart(slave_addr, &addr_buf, 1, data, len);
}

bool VL53L0X_WriteReg(uint8_t slave_addr, uint8_t reg, uint8_t value) 
{
	uint8_t buf[2] = { reg & 0xFF, value };
	
	return I2C_Write(slave_addr, buf, 2);
}

bool VL53_WriteReg8(uint8_t reg, uint8_t value)
{
	uint8_t buf[2] = {reg, value};
	return I2C_Write(VL53L0X_ADDR, buf, 2);
}

bool VL53_WriteReg16(uint8_t reg, uint16_t value)
{
	uint8_t buf[3] = {reg, (value >> 8) & 0xFF, value & 0xFF};
	return I2C_Write(VL53L0X_ADDR, buf, 3);
}

uint8_t VL53_ReadReg8(uint8_t reg)
{
	uint8_t val = 0;
	I2C_ReadReg(VL53L0X_ADDR, reg, &val, 1);
	return val;
}

uint16_t VL53_ReadReg16(uint8_t reg)
{
	uint8_t buf[2];
	I2C_ReadReg(VL53L0X_ADDR, reg, buf, 2);
	return ((uint16_t)buf[0] << 8) | buf[1];
}


bool VL53L0X_Init(void)
{
	// 1. ID 확인
	if (VL53_ReadReg8(0xC0) != 0xEE) return false;

	// 2. 센서 언락
	VL53_WriteReg8(0x88, 0x00);
	VL53_WriteReg8(0x80, 0x01);
	VL53_WriteReg8(0xFF, 0x01);
	VL53_WriteReg8(0x00, 0x00);
	stop_variable = VL53_ReadReg8(0x91);
	VL53_WriteReg8(0x00, 0x01);
	VL53_WriteReg8(0xFF, 0x00);
	VL53_WriteReg8(0x80, 0x00);

	// 3. interrupt 설정 (중요!)
	VL53_WriteReg8(0x0A, 0x04); // new sample ready
	VL53_WriteReg8(0x0B, 0x01); // clear

	// ---------------------------
	// 4. SPAD 정보 읽기
	// ---------------------------
	uint8_t spad_count;
	bool spad_is_aperture;

	VL53_WriteReg8(0x80, 0x01);
	VL53_WriteReg8(0xFF, 0x01);
	VL53_WriteReg8(0x00, 0x00);
	VL53_WriteReg8(0xFF, 0x06);
	VL53_WriteReg8(0x83, VL53_ReadReg8(0x83) | 0x04);
	VL53_WriteReg8(0xFF, 0x07);
	VL53_WriteReg8(0x81, 0x01);
	VL53_WriteReg8(0x80, 0x01);
	VL53_WriteReg8(0x94, 0x6B);
	VL53_WriteReg8(0x83, 0x00);

	uint16_t timeout = 5000;
	while (VL53_ReadReg8(0x83) == 0x00)
	{
		if (--timeout == 0) return false;
		_delay_us(100);
	}

	VL53_WriteReg8(0x83, 0x01);

	uint8_t tmp = VL53_ReadReg8(0x92);
	spad_count = tmp & 0x7F;
	spad_is_aperture = (tmp >> 7) & 0x01;

	VL53_WriteReg8(0x81, 0x00);
	VL53_WriteReg8(0xFF, 0x06);
	VL53_WriteReg8(0x83, VL53_ReadReg8(0x83) & ~0x04);
	VL53_WriteReg8(0xFF, 0x01);
	VL53_WriteReg8(0x00, 0x01);
	VL53_WriteReg8(0xFF, 0x00);
	VL53_WriteReg8(0x80, 0x00);

	// ---------------------------
	// 5. SPAD enable (핵심)
	// ---------------------------
	uint8_t ref_spad_map[6];

	for (uint8_t i = 0; i < 6; i++)
	ref_spad_map[i] = VL53_ReadReg8(0xB0 + i);

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
	VL53_WriteReg8(0xB0 + i, ref_spad_map[i]);

	// ---------------------------
	// 6. tuning (최소 버전)
	// ---------------------------
	VL53_WriteReg8(0xFF, 0x01);
	VL53_WriteReg8(0x00, 0x00);
	VL53_WriteReg8(0xFF, 0x00);
	VL53_WriteReg8(0x09, 0x00);
	VL53_WriteReg8(0x10, 0x00);
	VL53_WriteReg8(0x11, 0x00);

	// ---------------------------
	// 🔥 7. Calibration (핵심)
	// ---------------------------

	// VHV calibration only
	VL53_WriteReg8(0x01, 0x01);
	VL53_WriteReg8(0x00, 0x01);
	while (VL53_ReadReg8(0x00) & 0x01);

	timeout = 10000;
	while ((VL53_ReadReg8(0x13) & 0x07) == 0)
	{
		if (--timeout == 0) return false;
		_delay_us(100);
	}
	VL53_WriteReg8(0x0B, 0x01);

	// Phase calibration only
	VL53_WriteReg8(0x01, 0x02);
	VL53_WriteReg8(0x00, 0x01);
	while (VL53_ReadReg8(0x00) & 0x01);

	timeout = 1000;
	while ((VL53_ReadReg8(0x13) & 0x07) == 0)
	{
		if (--timeout == 0) return false;
		_delay_us(100);
	}
	VL53_WriteReg8(0x0B, 0x01);

	// ---------------------------
	// 8. 최종 sequence 설정
	// ---------------------------
	VL53_WriteReg8(0x01, 0xE8);

	return true;
}

uint16_t VL53L0X_ReadRange(void)
{
	VL53_WriteReg8(0x80, 0x01);
	VL53_WriteReg8(0xFF, 0x01);
	VL53_WriteReg8(0x00, 0x00);
	VL53_WriteReg8(0x91, stop_variable);
	VL53_WriteReg8(0x00, 0x01);
	VL53_WriteReg8(0xFF, 0x00);
	VL53_WriteReg8(0x80, 0x00);

	// SYSRANGE_START 쓰기
	VL53_WriteReg8(0x00, 0x01);
	while (VL53_ReadReg8(0x00) & 0x01);

	uint16_t timeout = 5000;
	while ((VL53_ReadReg8(0x13) & 0x07) == 0)
	{
		if (--timeout == 0) return 0xFFFF;
		_delay_us(100);
	}

	uint16_t range = VL53_ReadReg16(0x1E);
	VL53_WriteReg8(0x0B, 0x01);
	VL53_WriteReg8(0x00, 0x00);

	return range;
}


void APP_Init(void)
{
	clock_init(CLOCK_SOURCE_INTERNAL);
	clock_set_prescaler(2);

	GPIO_Init();
	I2C_Init();
	UART_Init();
	TIMER_init();

	GPIO_Open(&PORTA, 1, GPIO_DIR_OUTPUT, NULL);
	UART_Open(F_CPU, 9600); // F_CPU=10MHz, Baud=9600
	I2C_Open(F_CPU, 100000ul);
	
	
	_delay_ms(500);
		
	sei();

	printf("Hello VL53LX!\r\n");
	
	 if (!VL53L0X_Init()) 
	 {
		 printf("[ERR] Init failed. Halt.\r\n");
		 while (1);
	 }
	 else
	 {
		 printf("stop_variable: 0x%x\r\n", stop_variable);
	 }
}

void APP_Run(void)
{
	GPIO_Write(&PORTA, 1, true);
	
	uint16_t distance = VL53L0X_ReadRange();
	printf("distance = %d\r\n", distance);
	
	printf("0x00 after start: 0x%X\r\n", VL53_ReadReg8(0x00));
	_delay_ms(200);
	
	switch(status)
	{
		case IDLE:
		// TIMER 동작... 10초 지나면 SLEEP으로 이동
		break;

		case SLEEP:
		break;

		case ACTIVE:
		break;

		case ERROR:
		break;
	}
}