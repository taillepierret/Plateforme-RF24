#ifndef LOG_H
#define LOG_H

#include "typedef.h"

#define LOG_SHOW_LOG_LEVEL_B true
#define LOG_HIDE_LOG_LEVEL_B false

#define LOG_SHOW_TIME_B true
#define LOG_HIDE_TIME_B false

#define LOG_SIZE_OF_COMMANDS_U8 ((uint8_t)10)
#define LOG_COMMAND_LENGTH_U8 ((uint8_t)30)

typedef enum {
	LOG_LEVEL_VERBOSE_EN,
	LOG_LEVEL_INFO_EN,
	LOG_LEVEL_WARNING_EN,
	LOG_LEVEL_ERROR_EN
}LOG_level_en;

typedef struct
{
	char command_CA[LOG_COMMAND_LENGTH_U8];
	bool (*command_function_B_FP)(void);
	bool (*command_function_with_argument_B_FP)(char* argument_CA);
}LOG_command_str;

typedef struct
{
	void (*InitDebugUart)(void);
	void (*HAL_GetTime)(HAL_RTC_values_str* HAL_RTC_values_STR);
	void (*HAL_DebugPrint)(char* string);
	void (*HAL_getUart2Buffer)(uint8_t* rx_get_buffer_U8A);
	void (*HAL_cleanUart2Buffer)(void);
	uint32_t (*HAL_GetTimeMs_U32)(void);
}LOG_HAL_functions_str;

void LOG_Init(LOG_HAL_functions_str *LOG_HAL_functions_STR, uint16_t size_buffer_uart_2_rx_U16);
void LOG_PrintString(char* log_to_print_CP, bool show_time_B, LOG_level_en log_level_EN, bool show_log_level_B);
void LOG_PrintStringCRLF(char* log_to_print_CP, bool show_time_B, LOG_level_en log_level_EN, bool show_log_level_B);
void LOG_PrintUint8CRLF(uint8_t value_U8, LOG_level_en log_level_EN);
void LOG_PrintUint8(uint8_t value_U8, LOG_level_en log_level_EN);
void LOG_setLogLevel(LOG_level_en log_level_EN);
bool LOG_TreatCommand_B(char* command_CA);
void LOG_process(char* raw_buffer_CA, char *cleaning_buffer_CA);




#endif // LOG_H
