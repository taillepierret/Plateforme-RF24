#ifndef DEBUG_H
#define DEBUG_H

#include <stdbool.h>
#include <stdint.h>
#include "typedef.h"

typedef struct
{
    void(*InitDebugUart)(void);
    void(*HAL_GetTime)(HAL_RTC_values_str* HAL_RTC_values_STR);
    void(*HAL_DebugPrint)(uint8_t* data_U8P, uint16_t size_data_U16);
}DEBUG_HAL_functions_str;


void DEBUG_Init(DEBUG_HAL_functions_str* DEBUG_HAL_functions_STR);
void DEBUG_PrintString(char* log_to_print_CP, bool show_time_B);










#endif // DEBUG_H