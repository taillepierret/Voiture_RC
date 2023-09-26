/*
 * hal.c
 *
 *  Created on: Sep 18, 2023
 *      Author: teddy
 */

#include "hal.h"
#include "main.h"

extern SPI_HandleTypeDef hspi1;
static bool HAL_spiIsInit = false;

inline uint32_t HAL_millis_U32(void)
{
	return HAL_GetTick();
}

void HAL_spiInit(void* (SPI_init)(void))
{
	HAL_spiIsInit = true;
	SPI_init();
}


HAL_ret_val_en HAL_setLed(led_en led_EN, bool state_B)
{
	switch (led_EN)
	{
		case LED_VERTE_EN:
			HAL_GPIO_WritePin(led_v_GPIO_Port, led_v_Pin, state_B);
			return LED_SET_OK;
		case LED_ROUGE_EN:
			HAL_GPIO_WritePin(led_r_GPIO_Port, led_r_Pin, state_B);
			return LED_SET_OK;
		default:
			return LED_SET_ERROR;
	}
}

inline void HAL_setCE(bool state_B)
{
	HAL_GPIO_WritePin(GPIOB, CE_Pin, state_B);
}

inline void HAL_setIRQ(bool state_B)
{
	HAL_GPIO_WritePin(GPIOB, IRQ_Pin, state_B);
}

HAL_ret_val_en HAL_writeSpiValue_EN(uint8_t* value_U8A, uint16_t size_value_U16)
{
	HAL_StatusTypeDef ret_val;
	HAL_GPIO_WritePin(CSN_GPIO_Port, CSN_Pin, GPIO_PIN_RESET);
	ret_val = HAL_SPI_Transmit(&hspi1, value_U8A, size_value_U16, 100);
	HAL_GPIO_WritePin(CSN_GPIO_Port, CSN_Pin, GPIO_PIN_SET);
	if (ret_val != HAL_OK)
	{
		return SPI_WRITE_ERROR_EN;
	}
	else
	{
		return SPI_WRITE_OK_EN;
	}
}

HAL_ret_val_en HAL_readSpiValue_EN(uint8_t reg_U8, uint8_t* read_value_U8A,uint16_t size_read_value_U16)
{
	HAL_StatusTypeDef ret_val;
	HAL_GPIO_WritePin(CSN_GPIO_Port, CSN_Pin, GPIO_PIN_RESET);
	ret_val = HAL_SPI_Transmit(&hspi1, &reg_U8, 1, 100);
	if (ret_val != HAL_OK)
	{
		HAL_GPIO_WritePin(CSN_GPIO_Port, CSN_Pin, GPIO_PIN_SET);
		return SPI_READ_ERROR_EN;
	}
	HAL_SPI_Receive(&hspi1, read_value_U8A, size_read_value_U16, 100);
	HAL_GPIO_WritePin(CSN_GPIO_Port, CSN_Pin, GPIO_PIN_SET);
	if (ret_val != HAL_OK)
	{
		return SPI_READ_ERROR_EN;
	}
	else
	{
		return SPI_READ_OK_EN;
	}


}



