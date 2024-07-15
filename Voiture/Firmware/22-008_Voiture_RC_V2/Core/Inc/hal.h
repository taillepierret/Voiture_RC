/*
 * hal.h
 *
 *  Created on: Sep 18, 2023
 *      Author: teddy
 */

#ifndef INC_HAL_H_
#define INC_HAL_H_

#include "stdint.h"
#include "stdbool.h"


typedef enum {
	LED_VERTE_EN,
	LED_ROUGE_EN
}led_en;

typedef enum
{
	HAL_LED_SET_OK,
	HAL_LED_SET_ERROR,
	HAL_SPI_WRITE_ERROR_EN,
	HAL_SPI_WRITE_OK_EN,
	HAL_SPI_READ_ERROR_EN,
	HAL_SPI_READ_OK_EN
}HAL_ret_val_en;

typedef struct
{
	uint8_t seconds_U8;
	uint8_t minutes_U8;
	uint8_t hours_U8;
}HAL_RTC_values_str;


void HAL_InitDebugUart(void);
HAL_ret_val_en HAL_setLed(led_en led_EN, bool state_B);
uint32_t HAL_millis_U32(void);
HAL_ret_val_en HAL_writeSpiValue_EN(uint8_t* value_U8A, uint16_t size_value_U16);
HAL_ret_val_en HAL_readSpiValue_EN(uint8_t reg_U8, uint8_t* read_value_U8A,uint16_t size_read_value_U16);
void HAL_setCE(bool state_B);
void HAL_DebugPrint(uint8_t* data_U8P, uint16_t size_data_U16);
void HAL_GetTime(HAL_RTC_values_str* HAL_RTC_values_STR);
void HAL_delay_ms (uint32_t delay);
//void HAL_setIRQ(bool state_B);


#endif /* INC_HAL_H_ */
