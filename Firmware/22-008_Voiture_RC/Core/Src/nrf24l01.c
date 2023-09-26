/*
 * nrf24l01.c
 *
 *  Created on: Sep 19, 2023
 *      Author: teddy
 */

#include "nrf24l01.h"

NRF_HAL_function_str NRF_HAL_function_local_STR;
static bool NRF_isInit = false;

void NRFInitModule_EN(NRF_HAL_function_str NRF_HAL_function_STR)
{
	NRF_HAL_function_local_STR.setCe_PF = NRF_HAL_function_STR.setCe_PF;
	NRF_HAL_function_local_STR.setIrq_PF = NRF_HAL_function_STR.setIrq_PF;
	NRF_HAL_function_local_STR.writeSpiValue_EN_PF = NRF_HAL_function_STR.writeSpiValue_EN_PF;
	NRF_HAL_function_local_STR.readSpiValue_EN_PF = NRF_HAL_function_STR.readSpiValue_EN_PF;


	NRF_isInit = true;
}

NRF_ret_val_en NRF_setRxMode()
{
	uint8_t addr_register_U8 = CONFIG;
	uint8_t value_U8[2] = {0};
	uint16_t size_value_U8 = 2;
	HAL_ret_val_en HAL_ret_val_EN;
	NRF_HAL_function_local_STR.setCe_PF(true);
	HAL_ret_val_EN = NRF_HAL_function_local_STR.readSpiValue_EN_PF(addr_register_U8,value_U8,size_value_U8);
	if (HAL_ret_val_EN != SPI_READ_OK_EN)
	{
		return NRF_SPI_ERROR_EN;
	}

	value_U8[1] |= 0b00000001;

	HAL_ret_val_EN = NRF_HAL_function_local_STR.writeSpiValue_EN_PF(addr_register_U8,&value_U8[1],1);
	if (HAL_ret_val_EN != SPI_WRITE_OK_EN)
	{
		return NRF_SPI_ERROR_EN;
	}

	HAL_ret_val_EN = NRF_HAL_function_local_STR.readSpiValue_EN_PF(addr_register_U8,value_U8,size_value_U8);
	//NRF_HAL_function_local_STR.setCe_PF(false);
	if (HAL_ret_val_EN != SPI_READ_OK_EN)
	{
		return NRF_SPI_ERROR_EN;
	}

	if ((value_U8[1] & 0b000000001) != 0b00000001)
	{
		return NRF_SET_MODE_ERROR_EN;
	}
	else
	{
		return NRF_SET_MODE_OK_EN;
	}
}

NRF_ret_val_en NRF_setTxMode()
{
	uint8_t addr_register_U8 = CONFIG;
	uint8_t value_U8[2] = {0};
	uint16_t size_value_U8 = 2;
	HAL_ret_val_en HAL_ret_val_EN;
	NRF_HAL_function_local_STR.setCe_PF(true);
	HAL_ret_val_EN = NRF_HAL_function_local_STR.readSpiValue_EN_PF(addr_register_U8,value_U8,size_value_U8);
	if (HAL_ret_val_EN != SPI_READ_OK_EN)
	{
		return NRF_SPI_ERROR_EN;
	}

	value_U8[1] &= 0b11111110;

	HAL_ret_val_EN = NRF_HAL_function_local_STR.writeSpiValue_EN_PF(addr_register_U8,&value_U8[1],1);
	if (HAL_ret_val_EN != SPI_WRITE_OK_EN)
	{
		return NRF_SPI_ERROR_EN;
	}

	HAL_ret_val_EN = NRF_HAL_function_local_STR.readSpiValue_EN_PF(addr_register_U8,value_U8,size_value_U8);
	NRF_HAL_function_local_STR.setCe_PF(false);
	if (HAL_ret_val_EN != SPI_READ_OK_EN)
	{
		return NRF_SPI_ERROR_EN;
	}

	if ((value_U8[1] & 0b000000001) != 0b00000000)
	{
		return NRF_SET_MODE_ERROR_EN;
	}
	else
	{
		return NRF_SET_MODE_OK_EN;
	}
}

NRF_ret_val_en NRF_powerOn(bool state_B)
{
	uint8_t addr_register_U8 = CONFIG;
	uint8_t value_U8 = 0x00;
	uint16_t size_value_U8 = 1;
	HAL_ret_val_en HAL_ret_val_EN;

	HAL_ret_val_EN = NRF_HAL_function_local_STR.readSpiValue_EN_PF(addr_register_U8,&value_U8,size_value_U8);
	if (HAL_ret_val_EN != SPI_READ_OK_EN)
	{
		return NRF_SPI_ERROR_EN;
	}
	else if (((value_U8>>1)&0b00000001) == state_B)
	{
		return NRF_POWER_MODULE_ALREADY_SET_EN;
	}

	if (state_B)
	{
		value_U8 |= state_B<<1;
	}
	else
	{
		value_U8 &= state_B<<1;
	}

	HAL_ret_val_EN = NRF_HAL_function_local_STR.writeSpiValue_EN_PF(addr_register_U8,&value_U8,size_value_U8);
	if (HAL_ret_val_EN != SPI_WRITE_OK_EN)
	{
		return NRF_SPI_ERROR_EN;
	}

	HAL_ret_val_EN = NRF_HAL_function_local_STR.readSpiValue_EN_PF(addr_register_U8,&value_U8,size_value_U8);
	if (HAL_ret_val_EN != SPI_READ_OK_EN)
	{
		return NRF_SPI_ERROR_EN;
	}

	if (((value_U8>>1) & 0b000000001) != state_B)
	{
		return NRF_POWER_MODULE_SET_ERROR_EN;
	}
	else
	{
		return NRF_POWER_MODULE_SET_EN;
	}
}
