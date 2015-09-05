#ifndef __EV1527_H_
#define __EV1527_H_

#include "STC15F2K60S2.H"

sbit P3_RF_RXD = P3 ^ 2; //–≈∫≈ ‰»ÎΩ≈

extern void exint0_init(void);
extern void RF_ini_receive(void);
extern void RF_decode_main(void);
extern void receive_rf_decoder(void);

extern unsigned char old2_RF_RECE_REG[3];

extern unsigned char return_again_receive_rf_decoder_finished(void);
extern void clear_again_receive_rf_decoder_finished(void);

#endif