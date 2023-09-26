/*
 * nrf24l01.h
 *
 *  Created on: Sep 19, 2023
 *      Author: teddy
 */

#ifndef INC_NRF24L01_H_
#define INC_NRF24L01_H_

#include "hal.h"
/*
typedef enum{
	CONFIG = 0x00,
	EN_AA = 0x01,
	EN_RXADDR = 0x02,
	SETUP_AW = 0x03,
	SETUP_RETR = 0x04,
	RF_CH = 0x05,
	RF_SETUP = 0x06,
	STATUS = 0x07,
	OBSERVE_TX = 0x08,
	CD = 0x09,
	RX_ADDR_P0 = 0x0A,
	RX_ADDR_P1 = 0x0B,
	RX_ADDR_P2 = 0x0B,
	RX_ADDR_P3 = 0x0D,
	RX_ADDR_P4 = 0x0E,
	RX_ADDR_P5 = 0x0F,
	TX_ADDR = 0x10,
	RX_PW_P0 = 0x11,
	RX_PW_P1 = 0x12,
	RX_PW_P2 = 0x13,
	RX_PW_P3 = 0x14,
	RX_PW_P4 = 0x15,
	RX_PW_P5 = 0x16,
	FIFO_STATUS = 0x17
}NRF_register_en;

typedef enum{
	INIT_OK_EN,
	NRF_NOT_INIT_EN,
	NRF_SET_MODE_OK_EN,
	NRF_SET_MODE_ERROR_EN,
	NRF_SPI_ERROR_EN,
	NRF_POWER_MODULE_ALREADY_SET_EN,
	NRF_POWER_MODULE_SET_ERROR_EN,
	NRF_POWER_MODULE_SET_EN
}NRF_ret_val_en;

typedef struct{
	void (*setCe_PF)(bool state_B);
	void (*setIrq_PF)(bool state_B);
	HAL_ret_val_en (*writeSpiValue_EN_PF)(uint8_t address_register_U8, uint8_t* value_U8, uint16_t size_value_U16);
	HAL_ret_val_en (*readSpiValue_EN_PF)(uint8_t addresaddr_register_U8, uint8_t* ret_value_U8, uint16_t size_ret_value_U16);
}NRF_HAL_function_str;*/





void NRF24_Init (void);

void NRF24_TxMode (uint8_t *Address, uint8_t channel);
uint8_t NRF24_Transmit (uint8_t *data);

void NRF24_RxMode (uint8_t *Address, uint8_t channel);
uint8_t isDataAvailable (int pipenum);
void NRF24_Receive (uint8_t *data);

void NRF24_ReadAll (uint8_t *data);

/* Memory Map */
#define CONFIG      0x00
#define EN_AA       0x01
#define EN_RXADDR   0x02
#define SETUP_AW    0x03
#define SETUP_RETR  0x04
#define RF_CH       0x05
#define RF_SETUP    0x06
#define STATUS      0x07
#define OBSERVE_TX  0x08
#define CD          0x09
#define RX_ADDR_P0  0x0A
#define RX_ADDR_P1  0x0B
#define RX_ADDR_P2  0x0C
#define RX_ADDR_P3  0x0D
#define RX_ADDR_P4  0x0E
#define RX_ADDR_P5  0x0F
#define TX_ADDR     0x10
#define RX_PW_P0    0x11
#define RX_PW_P1    0x12
#define RX_PW_P2    0x13
#define RX_PW_P3    0x14
#define RX_PW_P4    0x15
#define RX_PW_P5    0x16
#define FIFO_STATUS 0x17
#define DYNPD	    0x1C
#define FEATURE	    0x1D

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
