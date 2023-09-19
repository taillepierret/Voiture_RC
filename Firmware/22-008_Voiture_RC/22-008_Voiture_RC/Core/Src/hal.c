/*
 * hal.c
 *
 *  Created on: Sep 18, 2023
 *      Author: teddy
 */

#include "hal.h"
#include "stm32l0xx_hal.h"
#include "main.h"


uint32_t HAL_millis_U32(void)
{
	return HAL_GetTick();
}


HAL_ret_val_en HAL_setGpio(led_en led_EN, bool state_B)
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





