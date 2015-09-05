#include "decoder.h"
#include "ev1527.h"
#include "usart.h"

void DecoderProcess(void)
{
	receive_rf_decoder();
	if (return_again_receive_rf_decoder_finished() == 1)
	{
#ifdef DEBUG
		uart_printf("decoder_val is %02x %02x %02x .\r\n", (unsigned int)old2_RF_RECE_REG[0], (unsigned int)old2_RF_RECE_REG[1], (unsigned int)old2_RF_RECE_REG[2]); //²âÊÔ°´¼ü¼üÖµ
#endif

	}
	clear_again_receive_rf_decoder_finished();
}