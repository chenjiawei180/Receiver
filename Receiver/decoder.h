#ifndef __DECODER_H_
#define __DECODER_H_

#include "STC15F2K60S2.H"
#include "tpyedef.h"

extern uint8_t  decoder_num;
extern void DecoderProcess(void);
extern void set_Two_menu_set_success(unsigned char temp);
extern unsigned char return_Two_menu_set_success(void);
#endif