#include "../Inc/applications.h"
#include "../Inc/radio.h"
#include "../Inc/Motor_driver.h"
#include "../Inc/hal.h"
#include "../Inc/NRF24L01.h"


bool treatment_function_B_PF(RADIO_trame_UN RADIO_trame_UN);

NRF_HAL_function_str NRF_HAL_function_STR = 
{
    .setCe_PF = HAL_setCE,
    .readSpiValue_EN_PF = HAL_readSpiValue_EN,
    .delay_ms_PF = HAL_delay_ms,
    .writeSpiValue_EN_PF = HAL_writeSpiValue_EN
};



void APP_Init(void)
{
  // Initialize the bride manager
  RADIO_Init(treatment_function_B_PF, NRF_HAL_function_STR);
}
void APP_process(void)
{
  // Process the bride manager
  RADIO_process();
}

bool treatment_function_B_PF(RADIO_trame_UN RADIO_trame_UN)
{
    switch (RADIO_trame_UN.trame_str.cerced_data_UN.cerced_data_str.trame_type_EN_6)
    {
    case RADIO_COMMAND_DIRECTION_EN:
        // Set the direction of the bride
        break;
    case RADIO_COMMAND_SPEED_EN:
        // Set the speed of the bride
        break;
    case RADIO_COMMAND_BRAKING_EN: 
        // Set the brake of the bride
        break;
    case RADIO_COMMAND_LIGHT_EN:   
    if(RADIO_trame_UN.trame_str.cerced_data_UN.cerced_data_str.payload_U8A[0] == 0xFF)
    {
        MD_setLight(true);
    }
    else
    {
        MD_setLight(false);
    }
        break;
    default:
        return false;
    }
    return true;
}