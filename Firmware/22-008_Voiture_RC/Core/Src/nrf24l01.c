/*
 * nrf24l01.c
 *
 *  Created on: Sep 19, 2023
 *      Author: teddy
 */

#include "nrf24l01.h"
#define cSIZE_BUFFER_TX_MAC_U8 32


//TODO ajouter les retours de fonction

static NRF_HAL_function_str NRF_HAL_function_local_STR;
static bool NRF_isInit_B = false;


// write a single byte to the particular register
static NRF_ret_val_en NRF_WriteReg_EN(NRF_register_REG register_REG, uint8_t Data_U8)
{
	const uint16_t size_buf_U16 = 2;
	uint8_t buf_U8[size_buf_U16];
	HAL_ret_val_en HAL_ret_val_EN;

	buf_U8[0] = register_REG|1<<5;
	buf_U8[1] = Data_U8;

	HAL_ret_val_EN = HAL_writeSpiValue_EN(buf_U8, size_buf_U16);
	if(HAL_ret_val_EN != SPI_WRITE_OK_EN)
	{
		return NRF_SPI_ERROR_EN;
	}
	else
	{
		return NRF_OK_EN;
	}

}


static HAL_ret_val_en NRF_WriteReg_Multi_EN(NRF_register_REG register_REG, uint8_t* Data_U8A, uint16_t size_U16)
{
	uint8_t buf_U8A[1+size_U16];
	buf_U8A[0] = register_REG|1<<5;
	HAL_ret_val_en HAL_ret_val_EN;

	for(uint16_t counter_U16=0 ; counter_U16<size_U16 ; counter_U16++){
		buf_U8A[1+counter_U16] = Data_U8A[counter_U16];
	}

	HAL_ret_val_EN = HAL_writeSpiValue_EN(buf_U8A, 1+size_U16);
	if(HAL_ret_val_EN != SPI_WRITE_OK_EN)
	{
		return NRF_SPI_ERROR_EN;
	}
	else
	{
		return NRF_OK_EN;
	}

	/*
	// Pull the CS Pin LOW to select the device
	CS_Select();

	HAL_SPI_Transmit(NRF24_SPI, buf, 1, 100);
	HAL_SPI_Transmit(NRF24_SPI, data, size, 1000);

	// Pull the CS HIGH to release the device
	CS_UnSelect();*/
}


static NRF_ret_val_en nrf24_ReadReg_EN(NRF_register_REG register_REG, uint8_t* read_value_U8P)
{
	HAL_ret_val_en HAL_ret_val_EN;
	read_value_U8P = 0;
	HAL_ret_val_EN = HAL_readSpiValue_EN((uint8_t)register_REG,read_value_U8P,1);
	if(HAL_ret_val_EN != SPI_READ_OK_EN)
	{
		return NRF_SPI_ERROR_EN;
	}
	else
	{
		return NRF_OK_EN;
	}
	// Pull the CS Pin LOW to select the device
	/*CS_Select();

	HAL_SPI_Transmit(NRF24_SPI, &Reg, 1, 100);
	HAL_SPI_Receive(NRF24_SPI, &data, 1, 100);

	// Pull the CS HIGH to release the device
	CS_UnSelect();*/
}


/* Read multiple bytes from the register */
static HAL_ret_val_en nrf24_ReadReg_Multi_EN(NRF_register_REG register_REG, uint8_t* read_value_U8A,uint16_t size_read_value_U16)
{
	HAL_ret_val_en HAL_ret_val_EN;
	HAL_ret_val_EN = HAL_readSpiValue_EN((uint8_t)register_REG,read_value_U8A,size_read_value_U16);
	if(HAL_ret_val_EN != SPI_READ_OK_EN)
	{
		return NRF_SPI_ERROR_EN;
	}
	else
	{
		return NRF_OK_EN;
	}
}


// send the command to the NRF
static HAL_ret_val_en nrfsendCmd_EN(uint8_t cmd)
{
	HAL_ret_val_en HAL_ret_val_EN;
	HAL_ret_val_EN = HAL_writeSpiValue_EN(&cmd, 1);
	if(HAL_ret_val_EN != SPI_WRITE_OK_EN)
	{
		return NRF_SPI_ERROR_EN;
	}
	else
	{
		return NRF_OK_EN;
	}
}

//TODO faire cette fonction avec ma HAL
static NRF_ret_val_en nrf24_reset_EN(NRF_register_REG REG)
{
	uint8_t rx_addr_p1_def[5] = {0xC2, 0xC2, 0xC2, 0xC2, 0xC2};
	uint8_t tx_addr_def[5] = {0xE7, 0xE7, 0xE7, 0xE7, 0xE7};
	uint8_t rx_addr_p0_def[5] = {0xE7, 0xE7, 0xE7, 0xE7, 0xE7};
	NRF_ret_val_en NRF_ret_val_EN;
	if(REG == STATUS_REG)
	{
		NRF_ret_val_EN = NRF_WriteReg_EN(STATUS_REG, 0x00);
		if(NRF_ret_val_EN != NRF_OK_EN)
		{
			return NRF_ret_val_EN;
		}
	}

	else if(REG == FIFO_STATUS_REG)
	{
		NRF_ret_val_EN = NRF_WriteReg_EN(FIFO_STATUS_REG, 0x11);
		if(NRF_ret_val_EN != NRF_OK_EN)
		{
			return NRF_ret_val_EN;
		}
	}

	else {
		NRF_ret_val_EN = NRF_WriteReg_EN(CONFIG_REG, 0x08);
		if(NRF_ret_val_EN != NRF_OK_EN)
		{
			return NRF_ret_val_EN;
		}

		NRF_ret_val_EN = NRF_WriteReg_EN(EN_AA_REG, 0x3F);
		if(NRF_ret_val_EN != NRF_OK_EN)
		{
			return NRF_ret_val_EN;
		}

		NRF_ret_val_EN = NRF_WriteReg_EN(EN_RXADDR_REG, 0x03);
		if(NRF_ret_val_EN != NRF_OK_EN)
		{
			return NRF_ret_val_EN;
		}

		NRF_ret_val_EN = NRF_WriteReg_EN(SETUP_AW_REG, 0x03);
		if(NRF_ret_val_EN != NRF_OK_EN)
		{
			return NRF_ret_val_EN;
		}

		NRF_ret_val_EN = NRF_WriteReg_EN(SETUP_RETR_REG, 0x03);
		if(NRF_ret_val_EN != NRF_OK_EN)
		{
			return NRF_ret_val_EN;
		}

		NRF_ret_val_EN = NRF_WriteReg_EN(RF_CH_REG, 0x02);
		if(NRF_ret_val_EN != NRF_OK_EN)
		{
			return NRF_ret_val_EN;
		}

		NRF_ret_val_EN = NRF_WriteReg_EN(RF_SETUP_REG, 0x0E);
		if(NRF_ret_val_EN != NRF_OK_EN)
		{
			return NRF_ret_val_EN;
		}

		NRF_ret_val_EN = NRF_WriteReg_EN(STATUS_REG, 0x00);
		if(NRF_ret_val_EN != NRF_OK_EN)
		{
			return NRF_ret_val_EN;
		}

		NRF_ret_val_EN = NRF_WriteReg_EN(OBSERVE_TX_REG, 0x00);
		if(NRF_ret_val_EN != NRF_OK_EN)
		{
			return NRF_ret_val_EN;
		}

		NRF_ret_val_EN = NRF_WriteReg_EN(CD_REG, 0x00);
		if(NRF_ret_val_EN != NRF_OK_EN)
		{
			return NRF_ret_val_EN;
		}

		NRF_ret_val_EN = NRF_WriteReg_Multi_EN(RX_ADDR_P0_REG, rx_addr_p0_def, 5);
		if(NRF_ret_val_EN != NRF_OK_EN)
		{
			return NRF_ret_val_EN;
		}

		NRF_ret_val_EN = NRF_WriteReg_Multi_EN(RX_ADDR_P1_REG, rx_addr_p1_def, 5);
		if(NRF_ret_val_EN != NRF_OK_EN)
		{
			return NRF_ret_val_EN;
		}

		NRF_ret_val_EN = NRF_WriteReg_EN(RX_ADDR_P2_REG, 0xC3);
		if(NRF_ret_val_EN != NRF_OK_EN)
		{
			return NRF_ret_val_EN;
		}

		NRF_ret_val_EN = NRF_WriteReg_EN(RX_ADDR_P3_REG, 0xC4);
		if(NRF_ret_val_EN != NRF_OK_EN)
		{
			return NRF_ret_val_EN;
		}

		NRF_ret_val_EN = NRF_WriteReg_EN(RX_ADDR_P4_REG, 0xC5);
		if(NRF_ret_val_EN != NRF_OK_EN)
		{
			return NRF_ret_val_EN;
		}

		NRF_ret_val_EN = NRF_WriteReg_EN(RX_ADDR_P5_REG, 0xC6);
		if(NRF_ret_val_EN != NRF_OK_EN)
		{
			return NRF_ret_val_EN;
		}

		NRF_ret_val_EN = NRF_WriteReg_Multi_EN(TX_ADDR_REG, tx_addr_def, 5);
		if(NRF_ret_val_EN != NRF_OK_EN)
		{
			return NRF_ret_val_EN;
		}

		NRF_ret_val_EN = NRF_WriteReg_EN(RX_PW_P0_REG, 0);
		if(NRF_ret_val_EN != NRF_OK_EN)
		{
			return NRF_ret_val_EN;
		}

		NRF_ret_val_EN = NRF_WriteReg_EN(RX_PW_P1_REG, 0);
		if(NRF_ret_val_EN != NRF_OK_EN)
		{
			return NRF_ret_val_EN;
		}

		NRF_ret_val_EN = NRF_WriteReg_EN(RX_PW_P2_REG, 0);
		if(NRF_ret_val_EN != NRF_OK_EN)
		{
			return NRF_ret_val_EN;
		}

		NRF_ret_val_EN = NRF_WriteReg_EN(RX_PW_P3_REG, 0);
		if(NRF_ret_val_EN != NRF_OK_EN)
		{
			return NRF_ret_val_EN;
		}

		NRF_ret_val_EN = NRF_WriteReg_EN(RX_PW_P4_REG, 0);
		if(NRF_ret_val_EN != NRF_OK_EN)
		{
			return NRF_ret_val_EN;
		}

		NRF_ret_val_EN = NRF_WriteReg_EN(RX_PW_P5_REG, 0);
		if(NRF_ret_val_EN != NRF_OK_EN)
		{
			return NRF_ret_val_EN;
		}

		NRF_ret_val_EN = NRF_WriteReg_EN(FIFO_STATUS_REG, 0x11);
		if(NRF_ret_val_EN != NRF_OK_EN)
		{
			return NRF_ret_val_EN;
		}

		NRF_ret_val_EN = NRF_WriteReg_EN(DYNPD_REG, 0);
		if(NRF_ret_val_EN != NRF_OK_EN)
		{
			return NRF_ret_val_EN;
		}

		NRF_ret_val_EN = NRF_WriteReg_EN(FEATURE_REG, 0);
		if(NRF_ret_val_EN != NRF_OK_EN)
		{
			return NRF_ret_val_EN;
		}

	}
	return NRF_OK_EN;
}



//TODO faire cette fonction avec la lecture des retour de fonctions
NRF_ret_val_en NRF24_Init_EN(NRF_HAL_function_str NRF_HAL_function_STR)
{
	NRF_HAL_function_local_STR.readSpiValue_EN_PF = NRF_HAL_function_STR.readSpiValue_EN_PF;
	NRF_HAL_function_local_STR.setCe_PF = NRF_HAL_function_STR.setCe_PF;
	NRF_HAL_function_local_STR.setIrq_PF = NRF_HAL_function_STR.setIrq_PF;
	NRF_HAL_function_local_STR.writeSpiValue_EN_PF = NRF_HAL_function_STR.writeSpiValue_EN_PF;

	// disable the chip before configuring the device
	NRF_HAL_function_local_STR.setCe_PF(false);

	NRF_ret_val_en NRF_ret_val_EN;

	// reset everything
	NRF_ret_val_EN = nrf24_reset_EN(0);
	if(NRF_ret_val_EN != NRF_OK_EN)
	{
		return NRF_ret_val_EN;
	}


	NRF_ret_val_EN = NRF_WriteReg_EN(CONFIG_REG, 0);  // will be configured later
	if(NRF_ret_val_EN != NRF_OK_EN)
	{
		return NRF_ret_val_EN;
	}


	NRF_ret_val_EN = NRF_WriteReg_EN(EN_AA_REG, 0);  // No Auto ACK
	if(NRF_ret_val_EN != NRF_OK_EN)
	{
		return NRF_ret_val_EN;
	}


	NRF_ret_val_EN = NRF_WriteReg_EN(EN_RXADDR_REG, 0);  // Not Enabling any data pipe right now
	if(NRF_ret_val_EN != NRF_OK_EN)
	{
		return NRF_ret_val_EN;
	}


	NRF_ret_val_EN = NRF_WriteReg_EN(SETUP_AW_REG, 0x03);  // 5 Bytes for the TX/RX address
	if(NRF_ret_val_EN != NRF_OK_EN)
	{
		return NRF_ret_val_EN;
	}


	NRF_ret_val_EN = NRF_WriteReg_EN(SETUP_RETR_REG, 0);   // No retransmission
	if(NRF_ret_val_EN != NRF_OK_EN)
	{
		return NRF_ret_val_EN;
	}


	NRF_ret_val_EN = NRF_WriteReg_EN(RF_CH_REG, 0);  // will be setup during Tx or RX
	if(NRF_ret_val_EN != NRF_OK_EN)
	{
		return NRF_ret_val_EN;
	}


	NRF_ret_val_EN = NRF_WriteReg_EN(RF_SETUP_REG, 0x0E);   // Power= 0db, data rate = 2Mbps
	if(NRF_ret_val_EN != NRF_OK_EN)
	{
		return NRF_ret_val_EN;
	}


	// Enable the chip after configuring the device
	NRF_HAL_function_local_STR.setCe_PF(true);

	NRF_isInit_B = true;

	return NRF_OK_EN;
}


//TODO faire cette fonction avec la lecture des retour de fonctions
// set up the Tx mode
NRF_ret_val_en NRF24_TxMode_EN(uint8_t *Address, uint8_t channel)
{
	if(NRF_isInit_B == false)
	{
		return NRF_NOT_INIT_EN;
	}
	NRF_ret_val_en NRF_ret_val_EN;
	// disable the chip before configuring the device
	NRF_HAL_function_local_STR.setCe_PF(false);

	NRF_ret_val_EN = NRF_WriteReg_EN(RF_CH_REG, channel);  // select the channel
	if(NRF_ret_val_EN != NRF_OK_EN)
	{
		return NRF_ret_val_EN;
	}

	NRF_ret_val_EN = NRF_WriteReg_Multi_EN(TX_ADDR_REG, Address, 5);  // Write the TX address
	if(NRF_ret_val_EN != NRF_OK_EN)
	{
		return NRF_ret_val_EN;
	}


	// power up the device
	uint8_t config = 0;
	NRF_ret_val_EN = nrf24_ReadReg_EN(CONFIG_REG,&config);
	if(NRF_ret_val_EN != NRF_OK_EN)
	{
		return NRF_ret_val_EN;
	}

	config = config &(0xF2);    // write 0 in the PRIM_RX, and 1 in the PWR_UP, and all other bits are masked
	NRF_ret_val_EN = NRF_WriteReg_EN(CONFIG_REG, config);
	if(NRF_ret_val_EN != NRF_OK_EN)
	{
		return NRF_ret_val_EN;
	}


	// Enable the chip after configuring the device
	NRF_HAL_function_local_STR.setCe_PF(true);

	return NRF_OK_EN;
}


// transmit the data

NRF_ret_val_en NRF24_Transmit_EN(uint8_t *data, uint8_t size_data_U8)
{
	uint8_t cmdtosend = 0;
	uint8_t data_to_send_U8A[size_data_U8+1];
	NRF_ret_val_en NRF_ret_val_EN;
	uint8_t fifostatus = 0;

	if(size_data_U8>cSIZE_BUFFER_TX_MAC_U8)
	{
		return NRF_SIZE_BUFFER_TX_TOO_LARGE_EN;
	}

	// payload command
	data_to_send_U8A[0] = W_TX_PAYLOAD_REG;
	for(uint8_t cnt_U8=0 ; cnt_U8<size_data_U8 ; cnt_U8++)
	{
		data_to_send_U8A[cnt_U8+1] = data[cnt_U8];
	}

	NRF_ret_val_EN = HAL_writeSpiValue_EN(data_to_send_U8A,(uint16_t)size_data_U8);
	if(NRF_ret_val_EN != NRF_OK_EN)
	{
		return NRF_ret_val_EN;
	}

	HAL_delay_ms(1);

	NRF_ret_val_EN = nrf24_ReadReg_EN(FIFO_STATUS_REG, &fifostatus);
	if(NRF_ret_val_EN != NRF_OK_EN)
	{
		return NRF_ret_val_EN;
	}

	// check the fourth bit of FIFO_STATUS to know if the TX fifo is empty
	if((fifostatus&(1<<4)) &&(!(fifostatus&(1<<3))))
	{
		cmdtosend = FLUSH_TX_REG;
		NRF_ret_val_EN = nrfsendCmd_EN(cmdtosend);
		if(NRF_ret_val_EN != NRF_OK_EN)
		{
			return NRF_ret_val_EN;
		}

		// reset FIFO_STATUS
		NRF_ret_val_EN = nrf24_reset_EN(FIFO_STATUS_REG);
		if(NRF_ret_val_EN != NRF_OK_EN)
		{
			return NRF_ret_val_EN;
		}

		return NRF_OK_EN;
	}

	return NRF_OK_EN;
}


NRF_ret_val_en NRF24_RxMode_EN(uint8_t *Address, uint8_t channel)
{
	if(NRF_isInit_B == false)
	{
		return NRF_NOT_INIT_EN;
	}
	// disable the chip before configuring the device
	NRF_HAL_function_local_STR.setCe_PF(false);
	NRF_ret_val_en NRF_ret_val_EN;

	NRF_ret_val_EN = nrf24_reset_EN(STATUS_REG);
	if(NRF_ret_val_EN != NRF_OK_EN)
	{
		return NRF_SPI_ERROR_EN;
	}

	NRF_ret_val_EN = NRF_WriteReg_EN(RF_CH_REG, channel);  // select the channel
	if(NRF_ret_val_EN != NRF_OK_EN)
	{
		return NRF_SPI_ERROR_EN;
	}

	// select data pipe 2
	uint8_t en_rxaddr = 0;
	NRF_ret_val_EN = nrf24_ReadReg_EN(EN_RXADDR_REG,&en_rxaddr);
	if(NRF_ret_val_EN != NRF_OK_EN)
	{
		return NRF_SPI_ERROR_EN;
	}

	en_rxaddr = en_rxaddr |(1<<2);
	NRF_ret_val_EN = NRF_WriteReg_EN(EN_RXADDR_REG, en_rxaddr);
	if(NRF_ret_val_EN != NRF_OK_EN)
	{
		return NRF_ret_val_EN;
	}

	/* We must write the address for Data Pipe 1, if we want to use any pipe from 2 to 5
	 * The Address from DATA Pipe 2 to Data Pipe 5 differs only in the LSB
	 * Their 4 MSB Bytes will still be same as Data Pipe 1
	 *
	 * For Eg->
	 * Pipe 1 ADDR = 0xAABBCCDD11
	 * Pipe 2 ADDR = 0xAABBCCDD22
	 * Pipe 3 ADDR = 0xAABBCCDD33
	 *
	 */
	NRF_ret_val_EN = NRF_WriteReg_Multi_EN(RX_ADDR_P1_REG, Address, 5);  // Write the Pipe1 address
	if(NRF_ret_val_EN != NRF_OK_EN)
	{
		return NRF_ret_val_EN;
	}

	NRF_ret_val_EN = NRF_WriteReg_EN(RX_ADDR_P2_REG, 0xEE);  // Write the Pipe2 LSB address
	if(NRF_ret_val_EN != NRF_OK_EN)
	{
		return NRF_ret_val_EN;
	}

	NRF_ret_val_EN = NRF_WriteReg_EN(RX_PW_P2_REG, 32);   // 32 bit payload size for pipe 2
	if(NRF_ret_val_EN != NRF_OK_EN)
	{
		return NRF_ret_val_EN;
	}


	// power up the device in Rx mode
	uint8_t config = 0;
	NRF_ret_val_EN = nrf24_ReadReg_EN(CONFIG_REG,&config);
	if(NRF_ret_val_EN != NRF_OK_EN)
	{
		return NRF_ret_val_EN;
	}

	config = config |(1<<1) |(1<<0);
	NRF_ret_val_EN = NRF_WriteReg_EN(CONFIG_REG, config);
	if(NRF_ret_val_EN != NRF_OK_EN)
	{
		return NRF_ret_val_EN;
	}

	// Enable the chip after configuring the device
	NRF_HAL_function_local_STR.setCe_PF(true);

	return NRF_OK_EN;
}


NRF_ret_val_en isDataAvailable_EN(uint8_t pipenum_U8, bool* flag_availability_B)
{
	uint8_t status = 0;
	NRF_ret_val_en NRF_ret_val_EN;

	NRF_ret_val_EN = nrf24_ReadReg_EN(STATUS_REG,&status);
	if(NRF_ret_val_EN != NRF_OK_EN)
	{
		return NRF_ret_val_EN;
	}


	if((status&(1<<6))&&(status&(pipenum_U8<<1)))
	{

		NRF_ret_val_EN = NRF_WriteReg_EN(STATUS_REG,(1<<6));
		if(NRF_ret_val_EN != NRF_OK_EN)
		{
			return NRF_ret_val_EN;
		}

		*flag_availability_B = true;
	}

	*flag_availability_B = false;
	return NRF_OK_EN;
}


//TODO faire cette fonction avec la lecture des retour de fonctions
NRF_ret_val_en NRF24_Receive_EN(uint8_t *data)
{
	uint8_t cmdtosend_U8 = 0;
	HAL_ret_val_en HAL_ret_val_EN;
	NRF_ret_val_en NRF_ret_val_EN;

	HAL_ret_val_EN = HAL_readSpiValue_EN(R_RX_PAYLOAD_REG,data,32);
	if(HAL_ret_val_EN != SPI_READ_OK_EN)
	{
		return NRF_SPI_ERROR_EN;
	}

	HAL_delay_ms(1);

	cmdtosend_U8 = FLUSH_RX_REG;
	NRF_ret_val_EN = nrfsendCmd_EN(cmdtosend_U8);
	if(NRF_ret_val_EN != NRF_OK_EN)
	{
		return NRF_ret_val_EN;
	}
	else
	{
		return NRF_OK_EN;
	}

}



// Read all the Register data
NRF_ret_val_en NRF24_ReadAll_EN(uint8_t *data)
{
	NRF_ret_val_en NRF_ret_val_EN;
	for(int i=0; i<10; i++)
	{
		NRF_ret_val_EN = nrf24_ReadReg_EN(i,(uint8_t*)(data+i));
		if(NRF_ret_val_EN != NRF_OK_EN)
		{
			return NRF_ret_val_EN;
		}
	}

	NRF_ret_val_EN = nrf24_ReadReg_Multi_EN(RX_ADDR_P0_REG,(data+10), 5);
	if(NRF_ret_val_EN != NRF_OK_EN)
	{
		return NRF_ret_val_EN;
	}

	NRF_ret_val_EN = nrf24_ReadReg_Multi_EN(RX_ADDR_P1_REG,(data+15), 5);
	if(NRF_ret_val_EN != NRF_OK_EN)
	{
		return NRF_SPI_ERROR_EN;
	}

	NRF_ret_val_EN = nrf24_ReadReg_EN(RX_ADDR_P2_REG,(uint8_t*)(data+20));
	if(NRF_ret_val_EN != NRF_OK_EN)
	{
		return NRF_SPI_ERROR_EN;
	}

	NRF_ret_val_EN = nrf24_ReadReg_EN(RX_ADDR_P3_REG,(uint8_t*)(data+21));
	if(NRF_ret_val_EN != NRF_OK_EN)
	{
		return NRF_SPI_ERROR_EN;
	}

	NRF_ret_val_EN = nrf24_ReadReg_EN(RX_ADDR_P4_REG,(uint8_t*)(data+22));
	if(NRF_ret_val_EN != NRF_OK_EN)
	{
		return NRF_SPI_ERROR_EN;
	}

	NRF_ret_val_EN = nrf24_ReadReg_EN(RX_ADDR_P5_REG,(uint8_t*)(data+23));
	if(NRF_ret_val_EN != NRF_OK_EN)
	{
		return NRF_SPI_ERROR_EN;
	}


	NRF_ret_val_EN = nrf24_ReadReg_Multi_EN(RX_ADDR_P0_REG,(data+24), 5);
	if(NRF_ret_val_EN != NRF_OK_EN)
	{
		return NRF_SPI_ERROR_EN;
	}

	for(int i=29; i<38; i++)
	{
		NRF_ret_val_EN = nrf24_ReadReg_EN(i-12,(uint8_t*)(data+i));
		if(NRF_ret_val_EN != NRF_OK_EN)
		{
			return NRF_SPI_ERROR_EN;
		}

	}
	return NRF_OK_EN;
}
