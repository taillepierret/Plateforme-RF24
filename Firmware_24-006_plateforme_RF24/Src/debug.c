#include "../Inc/debug.h"
#include "../Inc/main.h"
#include "../Inc/hal.h"
#include <string.h>
#include <stdio.h>





void DEBUG_Init(void)
{
  HAL_InitDebugUart();
}


void DEBUG_PrintString(char* log_to_print_CP, bool show_time_B)
{
  if (show_time_B)
  {
    char time_CA[20];
    HAL_RTC_values_str HAL_RTC_values_STR;

    HAL_GetTime(&HAL_RTC_values_STR);
    sprintf(time_CA, "%02d:%02d:%02d", HAL_RTC_values_STR.hours_U8, HAL_RTC_values_STR.minutes_U8, HAL_RTC_values_STR.seconds_U8);
    HAL_DebugPrint((uint8_t *)time_CA, strlen(time_CA));
  }
  HAL_DebugPrint((uint8_t *)log_to_print_CP, strlen(log_to_print_CP));
}





