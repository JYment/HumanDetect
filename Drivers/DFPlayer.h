/*
 * DFPlayer.h
 *
 * Created: 2026-05-14 오후 9:29:23
 *  Author: loveh
 */ 


#ifndef DFPLAYER_H_
#define DFPLAYER_H_



#include "def.h"


void DF_Init(void);
void DF_Play(uint16_t track);
void DF_Stop(void);
void DF_Next(void);
void DF_Prev(void);
void DF_SetVolume(uint8_t volume);

#endif /* DFPLAYER_H_ */