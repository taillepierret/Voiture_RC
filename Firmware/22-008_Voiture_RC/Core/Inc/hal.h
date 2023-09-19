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
	LED_SET_OK,
	LED_SET_ERROR
}HAL_ret_val_en;


HAL_ret_val_en HAL_setLed(led_en led_EN, bool state_B);
uint32_t HAL_millis_U32(void);

#endif /* INC_HAL_H_ */
