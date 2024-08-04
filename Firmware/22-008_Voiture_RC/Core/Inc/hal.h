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
#include "../../Plateforme-RF24/Firmware_24-006_plateforme_RF24/Inc/typedef.h"

#define cSIZE_BUFFER_UART_2_RX_U16 ((uint16_t)50)

typedef enum {
	LED_VERTE_EN,
	LED_ROUGE_EN
}led_en;


typedef struct
{
	  uint8_t Seconds;
	  uint8_t Minutes;
	  uint8_t Hours;
	  /*uint8_t WeekDay;
	  uint8_t Date;
	  uint8_t Month;
	  uint8_t Year;*/
}HAL_time_str;

HAL_ret_val_en HAL_setLed(led_en led_EN, bool state_B);
uint32_t HAL_millis_U32(void);
HAL_ret_val_en HAL_writeSpiValue_EN(uint8_t* value_U8A, uint16_t size_value_U16);
HAL_ret_val_en HAL_readSpiValue_EN(uint8_t reg_U8, uint8_t* read_value_U8A,uint16_t size_read_value_U16);
void HAL_setCE(bool state_B);
void HAL_DebugPrint(uint8_t* data_U8P, uint16_t size_data_U16);
void HAL_GetTime(HAL_RTC_values_str* HAL_RTC_values_STR);
void HAL_delay_ms (uint32_t delay);
void HAL_InitDebugUart(void);
void HAL_GetUart2Buffer(char* buffer_CA);
void HAL_FlushUart2Buffer(void);
void HAL_PrintString(char* string);
uint32_t HAL_GetTimestamp_U32(void);



#endif /* INC_HAL_H_ */
