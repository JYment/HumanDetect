/*
 * DFPlayer.c
 *
 * Created: 2026-05-14 오후 9:29:13
 *  Author: loveh
 */ 
#include "DFPlayer.h"
#include "uart.h"

// DFPlayer 패킷 구조: 0x7E + 0xFF + 0x06 + CMD + 0x00 + PARAM_H + PARAM_L + CHECKSUM_H + CHECKSUM_L + 0xEF

static void DF_SendCommand(uint8_t cmd, uint16_t param)
{
	uint8_t packet[10];
	uint16_t checksum;

	packet[0] = 0x7E;
	packet[1] = 0xFF;
	packet[2] = 0x06;
	packet[3] = cmd;
	packet[4] = 0x01;
	packet[5] = (param >> 8) & 0xFF;
	packet[6] = param & 0xFF;

	checksum = 0 - (packet[1] + packet[2] + packet[3] + packet[4] + packet[5] + packet[6]);
	packet[7] = (checksum >> 8) & 0xFF;
	packet[8] = checksum & 0xFF;
	packet[9] = 0xEF;


	UART_WriteBuffer(packet, 10);
}

void DF_Init(void)
{
	
}

void DF_Play(uint16_t track)
{
	DF_SendCommand(0x03, track);
}

void DF_Stop(void)
{
	DF_SendCommand(0x16, 0);
}

void DF_Next(void)
{
	DF_SendCommand(0x01, 0);
}

void DF_Prev(void)
{
	DF_SendCommand(0x02, 0);
}

void DF_SetVolume(uint8_t volume)
{
	if (volume > 30) volume = 30; // DFPlayer 최대 볼륨 30
	DF_SendCommand(0x06, volume);
}

uint8_t DF_ReadByte(void)
{
	return UART_ReadChar();
}

// 응답 패킷 파싱 예시
uint8_t DF_ReadResponse(uint8_t *buffer)
{
	// 패킷은 10바이트 고정
	for (int i = 0; i < 10; i++) {
		buffer[i] = DF_ReadByte();
	}

	// 간단히 CMD 코드 반환 (buffer[3]이 명령/이벤트 코드)
	return buffer[3];
}