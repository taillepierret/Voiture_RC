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
ÉMETTEUR (Transmetteur)                RÉCEPTEUR
--------------------------             --------------------------
                   |                                |
                   |                                |
             [Alimentation]                 [Alimentation]
                   |                                |
                   |                                |
              [Microcontrôleur]           [Microcontrôleur]
                   |                                |
                   |                                |
                [Module NRF24L01]          [Module NRF24L01]
                   |                                |
                   |                                |
                   |               [Configurer les registres SPI]
                   |                                |
                   |               [----------------------------------]
                   |               | Registre (adresse)    | Valeur |
                   |               [----------------------------------]
                   |               | CONFIG (0x00)          | 0x0E   |   (Active le module en mode TX)
                   |               | EN_CRC (0x01)          | 0x01   |   (Active la vérification CRC)
                   |               | CRCO (0x00)            | 0x00   |   (CRC 1 octet)
                   |               | SETUP_AW (0x03)        | 0x03   |   (Configuration de l'adresse sur 5 octets)
                   |               | TX_ADDR (0x10)         | [Adresse du pipe] |   (Adresse du pipe à configurer)
                   |               | RX_ADDR_P0 (0x0A)      | [Adresse du pipe] |   (Adresse du pipe à configurer)
                   |               | EN_AA (0x01)           | 0x00   |   (Désactive l'Auto Ack)
                   |               | EN_RXADDR (0x02)       | 0x01   |   (Active le pipe 0 pour la réception)
                   |               | RF_CH (0x05)           | [Fréquence] |   (Configurer la fréquence)
                   |               | RF_SETUP (0x06)        | 0x00   |   (Configuration de la puissance de transmission)
                   |               | RX_PW_P0 (0x11)        | [Taille des données] |   (Configurer la taille des données pour le pipe)
                   |               [----------------------------------]
                   |                                |
                   |               [Mettre en veille / Réception]
                   |                                |
                   |               [Attendre / Lire les données reçues]
                   |                                |
                   |                                |
                   |               [Configurer les registres SPI]
                   |                                |
                   |               [----------------------------------]
                   |               | Registre (adresse)    | Valeur |
                   |               [----------------------------------]
                   |               | CONFIG (0x00)          | 0x0F   |   (Active le module en mode RX)
                   |               [----------------------------------]
                   |                                |
                   |               [Lire les données]
                   |                                |e
*/

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
}NRF_HAL_function_str;

void NRFInitModule_EN(NRF_HAL_function_str NRF_HAL_function_STR);
NRF_ret_val_en NRF_setRxMode();

#endif /* INC_NRF24L01_H_ */
