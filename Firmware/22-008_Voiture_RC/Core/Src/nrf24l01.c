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
static bool NRF_isInit = false;


// write a single byte to the particular register
static NRF_ret_val_en NRF_WriteReg (NRF_register_REG register_REG, uint8_t Data_U8)
{
	const uint16_t size_buf_U16 = 2;
	uint8_t buf_U8[size_buf_U16];
	HAL_ret_val_en HAL_ret_val_EN;

	buf_U8[0] = register_REG|1<<5;
	buf_U8[1] = Data_U8;

	HAL_ret_val_EN = HAL_writeSpiValue_EN(buf_U8, size_buf_U16);

	if (HAL_ret_val_EN != SPI_WRITE_OK_EN)
	{
		return NRF_SPI_ERROR_EN;
	}
	else
	{
		return NRF_SPI_OK_EN;
	}

}


static HAL_ret_val_en NRF_WriteRegMulti (NRF_register_REG register_REG, uint8_t* Data_U8A, uint16_t size_U16)
{
	uint8_t buf_U8A[1+size_U16];
	buf_U8A[0] = register_REG|1<<5;
	HAL_ret_val_en HAL_ret_val_EN;

	for (uint16_t counter_U16=0 ; counter_U16<size_U16 ; counter_U16++){
		buf_U8A[1+counter_U16] = Data_U8A[counter_U16];
	}

	HAL_ret_val_EN = HAL_writeSpiValue_EN(buf_U8A, 1+size_U16);

	if (HAL_ret_val_EN != SPI_WRITE_OK_EN)
	{
		return NRF_SPI_ERROR_EN;
	}
	else
	{
		return NRF_SPI_OK_EN;
	}

	/*
	// Pull the CS Pin LOW to select the device
	CS_Select();

	HAL_SPI_Transmit(NRF24_SPI, buf, 1, 100);
	HAL_SPI_Transmit(NRF24_SPI, data, size, 1000);

	// Pull the CS HIGH to release the device
	CS_UnSelect();*/
}


static NRF_ret_val_en nrf24_ReadReg (NRF_register_REG register_REG, uint8_t* read_value_U8P)
{
	HAL_ret_val_en HAL_ret_val_EN;
	read_value_U8P = 0;
	HAL_ret_val_EN = HAL_readSpiValue_EN((uint8_t)register_REG,read_value_U8P,1);

	if (HAL_ret_val_EN != SPI_READ_OK_EN)
	{
		return NRF_SPI_ERROR_EN;
	}
	else
	{
		return NRF_SPI_OK_EN;
	}
	// Pull the CS Pin LOW to select the device
	/*CS_Select();

	HAL_SPI_Transmit(NRF24_SPI, &Reg, 1, 100);
	HAL_SPI_Receive(NRF24_SPI, &data, 1, 100);

	// Pull the CS HIGH to release the device
	CS_UnSelect();*/
}


/* Read multiple bytes from the register */
static HAL_ret_val_en nrf24_ReadReg_Multi (NRF_register_REG register_REG, uint8_t* read_value_U8A,uint16_t size_read_value_U16)
{
	HAL_ret_val_en HAL_ret_val_EN;
	HAL_ret_val_EN = HAL_readSpiValue_EN((uint8_t)register_REG,read_value_U8A,size_read_value_U16);

	if (HAL_ret_val_EN != SPI_READ_OK_EN)
	{
		return NRF_SPI_ERROR_EN;
	}
	else
	{
		return NRF_SPI_OK_EN;
	}
	// Pull the CS Pin LOW to select the device
	/*CS_Select();

	HAL_SPI_Transmit(NRF24_SPI, &Reg, 1, 100);
	HAL_SPI_Receive(NRF24_SPI, data, size, 1000);

	// Pull the CS HIGH to release the device
	CS_UnSelect();*/
}


// send the command to the NRF
static HAL_ret_val_en nrfsendCmd (uint8_t cmd)
{
	HAL_ret_val_en HAL_ret_val_EN;
	HAL_ret_val_EN = HAL_writeSpiValue_EN(&cmd, 1);

	if (HAL_ret_val_EN != SPI_READ_OK_EN)
	{
		return NRF_SPI_ERROR_EN;
	}
	else
	{
		return NRF_SPI_OK_EN;
	}
}

static void nrf24_reset(uint8_t REG)
{
	if (REG == STATUS)
	{
		NRF_WriteReg(STATUS, 0x00);
	}

	else if (REG == FIFO_STATUS)
	{
		NRF_WriteReg(FIFO_STATUS, 0x11);
	}

	else {
	NRF_WriteReg(CONFIG, 0x08);
	NRF_WriteReg(EN_AA, 0x3F);
	NRF_WriteReg(EN_RXADDR, 0x03);
	NRF_WriteReg(SETUP_AW, 0x03);
	NRF_WriteReg(SETUP_RETR, 0x03);
	NRF_WriteReg(RF_CH, 0x02);
	NRF_WriteReg(RF_SETUP, 0x0E);
	NRF_WriteReg(STATUS, 0x00);
	NRF_WriteReg(OBSERVE_TX, 0x00);
	NRF_WriteReg(CD, 0x00);
	uint8_t rx_addr_p0_def[5] = {0xE7, 0xE7, 0xE7, 0xE7, 0xE7};
	NRF_WriteRegMulti(RX_ADDR_P0, rx_addr_p0_def, 5);
	uint8_t rx_addr_p1_def[5] = {0xC2, 0xC2, 0xC2, 0xC2, 0xC2};
	NRF_WriteRegMulti(RX_ADDR_P1, rx_addr_p1_def, 5);
	NRF_WriteReg(RX_ADDR_P2, 0xC3);
	NRF_WriteReg(RX_ADDR_P3, 0xC4);
	NRF_WriteReg(RX_ADDR_P4, 0xC5);
	NRF_WriteReg(RX_ADDR_P5, 0xC6);
	uint8_t tx_addr_def[5] = {0xE7, 0xE7, 0xE7, 0xE7, 0xE7};
	NRF_WriteRegMulti(TX_ADDR, tx_addr_def, 5);
	NRF_WriteReg(RX_PW_P0, 0);
	NRF_WriteReg(RX_PW_P1, 0);
	NRF_WriteReg(RX_PW_P2, 0);
	NRF_WriteReg(RX_PW_P3, 0);
	NRF_WriteReg(RX_PW_P4, 0);
	NRF_WriteReg(RX_PW_P5, 0);
	NRF_WriteReg(FIFO_STATUS, 0x11);
	NRF_WriteReg(DYNPD, 0);
	NRF_WriteReg(FEATURE, 0);
	}
}




void NRF24_Init (NRF_HAL_function_str NRF_HAL_function_STR)
{
	NRF_HAL_function_local_STR.readSpiValue_EN_PF = NRF_HAL_function_STR.readSpiValue_EN_PF;
	NRF_HAL_function_local_STR.setCe_PF = NRF_HAL_function_STR.setCe_PF;
	NRF_HAL_function_local_STR.setIrq_PF = NRF_HAL_function_STR.setIrq_PF;
	NRF_HAL_function_local_STR.writeSpiValue_EN_PF = NRF_HAL_function_STR.writeSpiValue_EN_PF;

	// disable the chip before configuring the device
	NRF_HAL_function_local_STR.setCe_PF(false);


	// reset everything
	nrf24_reset (0);

	NRF_WriteReg(CONFIG_REG, 0);  // will be configured later

	NRF_WriteReg(EN_AA_REG, 0);  // No Auto ACK

	NRF_WriteReg (EN_RXADDR_REG, 0);  // Not Enabling any data pipe right now

	NRF_WriteReg (SETUP_AW_REG, 0x03);  // 5 Bytes for the TX/RX address

	NRF_WriteReg (SETUP_RETR_REG, 0);   // No retransmission

	NRF_WriteReg (RF_CH_REG, 0);  // will be setup during Tx or RX

	NRF_WriteReg (RF_SETUP_REG, 0x0E);   // Power= 0db, data rate = 2Mbps

	// Enable the chip after configuring the device
	NRF_HAL_function_local_STR.setCe_PF(true);

	NRF_isInit = true;

}


// set up the Tx mode

NRF_ret_val_en NRF24_TxMode (uint8_t *Address, uint8_t channel)
{
	if (NRF_isInit == false)
	{
		return NRF_NOT_INIT_EN;
	}
	// disable the chip before configuring the device
	NRF_HAL_function_local_STR.setCe_PF(false);

	NRF_WriteReg (RF_CH, channel);  // select the channel

	NRF_WriteRegMulti(TX_ADDR, Address, 5);  // Write the TX address


	// power up the device
	uint8_t config = nrf24_ReadReg(CONFIG);
	config = config & (0xF2);    // write 0 in the PRIM_RX, and 1 in the PWR_UP, and all other bits are masked
	NRF_WriteReg (CONFIG, config);

	// Enable the chip after configuring the device
	NRF_HAL_function_local_STR.setCe_PF(true);

	return NRF_TX_MODE_SET_EN;
}


// transmit the data

NRF_ret_val_en NRF24_Transmit (uint8_t *data, uint8_t size_data_U8)
{
	uint8_t cmdtosend = 0;
	uint8_t data_to_send_U8A[size_data_U8+1];
	HAL_ret_val_en HAL_ret_val_EN;

	if (size_data_U8>cSIZE_BUFFER_TX_MAC_U8)
	{
		return NRF_SIZE_BUFFER_TX_TOO_LARGE;
	}

	// payload command
	data_to_send[0] = W_TX_PAYLOAD;
	for (uint8_t cnt_U8=0 ; cnt_U8<size_data_U8 ; cnt_U8++)
	{
		data_to_send_U8A[cnt_U8+1] = data[cnt_U8];
	}

	HAL_ret_val_EN = HAL_writeSpiValue_EN(data_to_send_U8A, (uint16_t)size_data_U8, 1000);

	if (HAL_ret_val_EN != SPI_READ_OK_EN)
	{
		return NRF_SPI_ERROR_EN;
	}

	HAL_Delay(1);

	uint8_t fifostatus = 0;
	HAL_ret_val_EN = nrf24_ReadReg(FIFO_STATUS, &fifostatus);

	if (HAL_ret_val_EN != SPI_READ_OK_EN)
	{
		return NRF_SPI_ERROR_EN;
	}

	// check the fourth bit of FIFO_STATUS to know if the TX fifo is empty
	if ((fifostatus&(1<<4)) && (!(fifostatus&(1<<3))))
	{
		cmdtosend = FLUSH_TX;
		HAL_ret_val_EN = nrfsendCmd(cmdtosend);

		if (HAL_ret_val_EN != SPI_READ_OK_EN)
		{
			return NRF_SPI_ERROR_EN;
		}

		// reset FIFO_STATUS
		nrf24_reset (FIFO_STATUS);

		return 1;
	}

	return 0;
}


NRF_ret_val_en NRF24_RxMode (uint8_t *Address, uint8_t channel)
{
	if (NRF_isInit == false)
	{
		return NRF_NOT_INIT_EN;
	}
	// disable the chip before configuring the device
	NRF_HAL_function_local_STR.setCe_PF(false);

	nrf24_reset (STATUS);

	NRF_WriteReg (RF_CH, channel);  // select the channel

	// select data pipe 2
	uint8_t en_rxaddr = nrf24_ReadReg(EN_RXADDR);
	en_rxaddr = en_rxaddr | (1<<2);
	NRF_WriteReg (EN_RXADDR, en_rxaddr);

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
	NRF_WriteRegMulti(RX_ADDR_P1, Address, 5);  // Write the Pipe1 address
	NRF_WriteReg(RX_ADDR_P2, 0xEE);  // Write the Pipe2 LSB address

	NRF_WriteReg (RX_PW_P2, 32);   // 32 bit payload size for pipe 2


	// power up the device in Rx mode
	uint8_t config = nrf24_ReadReg(CONFIG);
	config = config | (1<<1) | (1<<0);
	NRF_WriteReg (CONFIG, config);

	// Enable the chip after configuring the device
	NRF_HAL_function_local_STR.setCe_PF(true);

	return NRF_RX_MODE_SET_EN;
}


uint8_t isDataAvailable (int pipenum)
{
	uint8_t status = nrf24_ReadReg(STATUS);

	if ((status&(1<<6))&&(status&(pipenum<<1)))
	{

		NRF_WriteReg(STATUS, (1<<6));

		return 1;
	}

	return 0;
}


void NRF24_Receive (uint8_t *data)
{
	/*uint8_t cmdtosend = 0;

	// select the device
	CS_Select();

	// payload command
	cmdtosend = R_RX_PAYLOAD;
	HAL_SPI_Transmit(NRF24_SPI, &cmdtosend, 1, 100);

	// Receive the payload
	HAL_SPI_Receive(NRF24_SPI, data, 32, 1000);

	// Unselect the device
	CS_UnSelect();

	HAL_Delay(1);

	cmdtosend = FLUSH_RX;
	nrfsendCmd(cmdtosend);*/
}



// Read all the Register data
void NRF24_ReadAll (uint8_t *data)
{
	for (int i=0; i<10; i++)
	{
		*(data+i) = nrf24_ReadReg(i);
	}

	nrf24_ReadReg_Multi(RX_ADDR_P0, (data+10), 5);

	nrf24_ReadReg_Multi(RX_ADDR_P1, (data+15), 5);

	*(data+20) = nrf24_ReadReg(RX_ADDR_P2);
	*(data+21) = nrf24_ReadReg(RX_ADDR_P3);
	*(data+22) = nrf24_ReadReg(RX_ADDR_P4);
	*(data+23) = nrf24_ReadReg(RX_ADDR_P5);

	nrf24_ReadReg_Multi(RX_ADDR_P0, (data+24), 5);

	for (int i=29; i<38; i++)
	{
		*(data+i) = nrf24_ReadReg(i-12);
	}

}
