#ifndef LOG_H
#define LOG_H

#include "typedef.h"

#define LOG_SHOW_LOG_LEVEL_B true
#define LOG_HIDE_LOG_LEVEL_B false

#define LOG_SHOW_TIME_B true
#define LOG_HIDE_TIME_B false

typedef enum {
	LOG_LEVEL_VERBOSE_EN,
	LOG_LEVEL_INFO_EN,
	LOG_LEVEL_WARNING_EN,
	LOG_LEVEL_ERROR_EN
}LOG_level_en;

typedef struct
{
	void (*InitDebugUart)(void);
	void (*HAL_GetTime)(HAL_RTC_values_str* HAL_RTC_values_STR);
	void (*HAL_DebugPrint)(char* string);
}LOG_HAL_functions_str;

void LOG_Init(LOG_HAL_functions_str* LOG_HAL_functions_STR);
void LOG_PrintString(char* log_to_print_CP, bool show_time_B, LOG_level_en log_level_EN, bool show_log_level_B);
void LOG_PrintStringCRLF(char* log_to_print_CP, bool show_time_B, LOG_level_en log_level_EN, bool show_log_level_B);
void LOG_PrintUint8CRLF(uint8_t value_U8, LOG_level_en log_level_EN);
void LOG_PrintUint8(uint8_t value_U8, LOG_level_en log_level_EN);
void LOG_setLogLevel(LOG_level_en log_level_EN);




#endif // LOG_H
