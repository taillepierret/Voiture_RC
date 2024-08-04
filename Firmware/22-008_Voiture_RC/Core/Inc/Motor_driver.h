#ifndef MOTOR_DRIVER_H
#define MOTOR_DRIVER_H

#include <stdint.h>
#include <stdbool.h>


typedef enum
{
    FORWARD_EN = 0,
    BACKWARD_EN = 1
}MD_direction_en;

//faire un enum pour le brake à partir d'un bool qui serait STOP ou roue libre en anglais
typedef enum
{
    STOP_EN = 0,
    FREE_WHEEL_EN = 1
}MD_brake_en;










void MD_Init(void);
void MD_setSpeed(uint8_t speed);
void MD_setDirection(MD_direction_en MD_direction_EN);
void MD_setBrake(bool flag_B);
void MD_SetDirectionAngle(uint8_t angle);
void MD_setLight(bool flag_B);


#endif // MOTOR_DRIVER_H