/*
 * hal.c
 *
 *  Created on: Sep 18, 2023
 *      Author: teddy
 */

#include "../Inc/hal.h"
#include "../Inc/main.h"

extern SPI_HandleTypeDef hspi1;
extern UART_HandleTypeDef huart2;
extern RTC_HandleTypeDef hrtc;
static bool HAL_spiIsInit = false;

uint32_t HAL_millis_U32(void)
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
			HAL_GPIO_WritePin(LED_V_EN_GPIO_Port, LED_V_EN_Pin, state_B);
			return HAL_LED_SET_OK;
		case LED_ROUGE_EN:
			HAL_GPIO_WritePin(LED_R_EN_GPIO_Port, LED_R_EN_Pin, state_B);
			return HAL_LED_SET_OK;
		default:
			return HAL_LED_SET_ERROR;
	}
}

void HAL_setCE(bool state_B)
{
	HAL_GPIO_WritePin(CE_EN_GPIO_Port, CE_EN_Pin, state_B);
}

/*inline void HAL_setIRQ(bool state_B)
{
	HAL_GPIO_WritePin(GPIOB, IRQ_Pin, state_B);
}*/

HAL_ret_val_en HAL_writeSpiValue_EN(uint8_t* value_U8A, uint16_t size_value_U16)
{
	HAL_StatusTypeDef ret_val;
	HAL_GPIO_WritePin(CSN_EN_GPIO_Port, CSN_EN_Pin, GPIO_PIN_RESET);
	ret_val = HAL_SPI_Transmit(&hspi1, value_U8A, size_value_U16, 100);
	HAL_GPIO_WritePin(CSN_EN_GPIO_Port, CSN_EN_Pin, GPIO_PIN_SET);
	if (ret_val != HAL_OK)
	{
		return HAL_SPI_WRITE_ERROR_EN;
	}
	else
	{
		return HAL_SPI_WRITE_OK_EN;
	}
}

HAL_ret_val_en HAL_readSpiValue_EN(uint8_t reg_U8, uint8_t* read_value_U8A,uint16_t size_read_value_U16)
{
	HAL_StatusTypeDef ret_val;
	HAL_GPIO_WritePin(CSN_EN_GPIO_Port, CSN_EN_Pin, GPIO_PIN_RESET);
	ret_val = HAL_SPI_Transmit(&hspi1, &reg_U8, 1, 100);
	if (ret_val != HAL_OK)
	{
		HAL_GPIO_WritePin(CSN_EN_GPIO_Port, CSN_EN_Pin, GPIO_PIN_SET);
		return HAL_SPI_READ_ERROR_EN;
	}
	HAL_SPI_Receive(&hspi1, read_value_U8A, size_read_value_U16, 100);
	HAL_GPIO_WritePin(CSN_EN_GPIO_Port, CSN_EN_Pin, GPIO_PIN_SET);
	if (ret_val != HAL_OK)
	{
		return HAL_SPI_READ_ERROR_EN;
	}
	else
	{
		return HAL_SPI_READ_OK_EN;
	}


}



void HAL_InitDebugUart(void)
{
  huart2.Instance = USART2;
  huart2.Init.BaudRate = 115200;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  huart2.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart2.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
    Error_Handler();
  }
}

void HAL_DebugPrint(uint8_t* data_U8P, uint16_t size_data_U16)
{
	HAL_UART_Transmit(&huart2, data_U8P, size_data_U16, 1000);
}

void HAL_GetTime(HAL_RTC_values_str* HAL_RTC_values_STR)
{ //Gerer la valeur de retour de HAL_RTC_getTime
	RTC_TimeTypeDef sTime_EN;
	HAL_RTC_GetTime(&hrtc, &sTime_EN, RTC_FORMAT_BIN);
	HAL_RTC_values_STR->hours_U8 = sTime_EN.Hours;
	HAL_RTC_values_STR->minutes_U8 = sTime_EN.Minutes;
	HAL_RTC_values_STR->seconds_U8 = sTime_EN.Seconds;
}


void HAL_delay_ms (uint32_t delay)
{
	HAL_Delay(delay);
}