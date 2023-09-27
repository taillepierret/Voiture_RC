/*
 * nrf24l01.h
 *
 *  Created on: Sep 19, 2023
 *      Author: teddy
 */

#ifndef INC_NRF24L01_H_
#define INC_NRF24L01_H_

#include "hal.h"

typedef enum{
	CONFIG_REG = 0x00,
	EN_AA_REG = 0x01,
	EN_RXADDR_REG = 0x02,
	SETUP_AW_REG = 0x03,
	SETUP_RETR_REG = 0x04,
	RF_CH_REG = 0x05,
	RF_SETUP_REG = 0x06,
	STATUS_REG = 0x07,
	OBSERVE_TX_REG = 0x08,
	CD_REG = 0x09,
	RX_ADDR_P0_REG = 0x0A,
	RX_ADDR_P1_REG = 0x0B,
	RX_ADDR_P2_REG = 0x0B,
	RX_ADDR_P3_REG = 0x0D,
	RX_ADDR_P4_REG = 0x0E,
	RX_ADDR_P5_REG = 0x0F,
	TX_ADDR_REG = 0x10,
	RX_PW_P0_REG = 0x11,
	RX_PW_P1_REG = 0x12,
	RX_PW_P2_REG = 0x13,
	RX_PW_P3_REG = 0x14,
	RX_PW_P4_REG = 0x15,
	RX_PW_P5_REG = 0x16,
	FIFO_STATUS_REG = 0x17
}NRF_register_REG;

typedef enum{
	INIT_OK_EN,
	NRF_NOT_INIT_EN,
	NRF_SET_MODE_OK_EN,
	NRF_SET_MODE_ERROR_EN,
	NRF_SPI_ERROR_EN,
	NRF_SPI_OK_EN,
	NRF_POWER_MODULE_ALREADY_SET_EN,
	NRF_POWER_MODULE_SET_ERROR_EN,
	NRF_POWER_MODULE_SET_EN,
	NRF_TX_MODE_SET_EN,
	NRF_RX_MODE_SET_EN,
	NRF_TX_DONE_EN,
	NRF_RX_DONE_EN,
	NRF_SIZE_BUFFER_TX_TOO_LARGE_EN,
	NRF_OK_EN
}NRF_ret_val_en;

typedef struct{
	void (*setCe_PF)(bool state_B);
	void (*setIrq_PF)(bool state_B);
	HAL_ret_val_en (*writeSpiValue_EN_PF)(uint8_t* write_value_U8, uint16_t size_value_U16);
	HAL_ret_val_en (*readSpiValue_EN_PF)(uint8_t reg_U8, uint8_t* read_value_U8A,uint16_t size_read_value_U16);
}NRF_HAL_function_str;





void NRF24_Init (NRF_HAL_function_str NRF_HAL_function_STR);

NRF_ret_val_en NRF24_TxMode (uint8_t *Address, uint8_t channel);
NRF_ret_val_en NRF24_Transmit (uint8_t *data, uint8_t size_data_U8);

NRF_ret_val_en NRF24_RxMode (uint8_t *Address, uint8_t channel);
NRF_ret_val_en isDataAvailable (uint8_t pipenum_U8, bool* flag_availability_B);
void NRF24_Receive (uint8_t *data);
HAL_ret_val_en NRF24_ReadAll (uint8_t *data);

/* Instruction Mnemonics */
#define R_REGISTER    0x00
#define W_REGISTER    0x20
#define REGISTER_MASK 0x1F
#define ACTIVATE      0x50
#define R_RX_PL_WID   0x60
#define R_RX_PAYLOAD  0x61
#define W_TX_PAYLOAD  0xA0
#define W_ACK_PAYLOAD 0xA8
#define FLUSH_TX      0xE1
#define FLUSH_RX      0xE2
#define REUSE_TX_PL   0xE3
#define NOP           0xFF

#endif /* INC_NRF24L01_H_ */
