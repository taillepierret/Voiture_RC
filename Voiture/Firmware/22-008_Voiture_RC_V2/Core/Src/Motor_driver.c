#include "../Inc/Motor_driver.h"
#include "../Inc/main.h"




void MD_Init(void)
{
  // Initialize the bride manager
}

void MD_setSpeed(uint8_t speed)
{
  // Set the speed of the bride
}

void MD_setDirection(MD_direction_en MD_direction_EN)
{
  // Set the direction of the bride
}

void MD_setBrake(bool flag_B)
{
  // Set the brake of the bride
}
//comment on dit angle en anglais ? répond moi copilot stp?
void MD_SetDirectionAngle(uint8_t angle)
{
  // Set the angle of the bride
}

void MD_setLight(bool flag_B)
{
  HAL_GPIO_WritePin(CMD_PLEINS_PHARES_EN_GPIO_Port, CMD_PLEINS_PHARES_EN_Pin, flag_B);
}