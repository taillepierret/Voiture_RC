#include "../Inc/debug.h"
#include "../Inc/main.h"




void DEBUG_Init(void)
{
  MX_USART2_UART_Init();
}


void DEBUG_PrintString(char* log_to_print_CP, bool show_time_B)
{
  if (show_time_B)
  {
    char time[20];
    RTC_TimeTypeDef sTime;

    HAL_RTC_GetTime(&hrtc, &sTime, RTC_FORMAT_BIN);
    sprintf(time, "%02d:%02d:%02d", sTime.Hours, sTime.Minutes, sTime.Seconds);
    HAL_UART_Transmit(&huart2, (uint8_t *)time, strlen(time), 1000);
  }
  HAL_UART_Transmit(&huart2, (uint8_t *)log_to_print_CP, strlen(log_to_print_CP), 1000);
}





