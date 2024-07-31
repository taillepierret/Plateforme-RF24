#include "../Inc/log.h"
#include <stdio.h>
#include <string.h>

static LOG_HAL_functions_str local_LOG_HAL_functions_STR;
static LOG_level_en local_LOG_level_EN;
static LOG_command_str local_LOG_command_STRA[LOG_SIZE_OF_COMMANDS_U8];

void LOG_Init(LOG_HAL_functions_str *LOG_HAL_functions_STR)
{
	local_LOG_HAL_functions_STR = *LOG_HAL_functions_STR;
	local_LOG_HAL_functions_STR.InitDebugUart();
}

void LOG_PrintString(char *log_to_print_CP, bool show_time_B, LOG_level_en log_level_EN, bool show_log_level_B)
{
	if (show_time_B)
	{
		char time_CA[20];
		HAL_RTC_values_str HAL_RTC_values_STR;
		local_LOG_HAL_functions_STR.HAL_GetTime(&HAL_RTC_values_STR);
		sprintf(time_CA, "%02d:%02d:%02d", HAL_RTC_values_STR.hours_U8,	HAL_RTC_values_STR.minutes_U8, HAL_RTC_values_STR.seconds_U8);
		local_LOG_HAL_functions_STR.HAL_DebugPrint(time_CA);
		local_LOG_HAL_functions_STR.HAL_DebugPrint("\r\n");
	}
	if (show_log_level_B)
	{
		switch (log_level_EN)
		{
		case LOG_LEVEL_VERBOSE_EN:
			local_LOG_HAL_functions_STR.HAL_DebugPrint("[VERBOSE] ");
			break;
		case LOG_LEVEL_INFO_EN:
			local_LOG_HAL_functions_STR.HAL_DebugPrint("[INFO] ");
			break;
		case LOG_LEVEL_WARNING_EN:
			local_LOG_HAL_functions_STR.HAL_DebugPrint("[WARNING] ");
			break;
		case LOG_LEVEL_ERROR_EN:
			local_LOG_HAL_functions_STR.HAL_DebugPrint("[ERROR] ");
			break;
		}
	}
	local_LOG_HAL_functions_STR.HAL_DebugPrint(log_to_print_CP);
}

void LOG_PrintStringCRLF(char *log_to_print_CP, bool show_time_B, LOG_level_en log_level_EN, bool show_log_level_B)
{
	LOG_PrintString(log_to_print_CP, show_time_B, log_level_EN,	show_log_level_B);
	local_LOG_HAL_functions_STR.HAL_DebugPrint("\r\n");
}

void LOG_PrintUint8(uint8_t value_U8, LOG_level_en log_level_EN)
{
	char value_CA[4];
	sprintf(value_CA, "%d", value_U8);
	LOG_PrintString(value_CA, false, log_level_EN, false);
}

void LOG_PrintUint8CRLF(uint8_t value_U8, LOG_level_en log_level_EN)
{
	LOG_PrintUint8(value_U8, log_level_EN);
	local_LOG_HAL_functions_STR.HAL_DebugPrint("\r\n");
}

void LOG_setLogLevel(LOG_level_en log_level_EN) {
	local_LOG_level_EN = log_level_EN;
}

bool LOG_addCommand(char *command_CA, bool (*command_function_B_FP)(void), bool (*command_function_with_argument_B_FP)(char *argument_CA))
{
	for (uint8_t counter_U8 = 0; counter_U8 < LOG_SIZE_OF_COMMANDS_U8; counter_U8++)
	{
		if (local_LOG_command_STRA[counter_U8].command_function_B_FP == NULL || local_LOG_command_STRA[counter_U8].command_function_with_argument_B_FP == NULL)
		{
			strcpy(local_LOG_command_STRA[counter_U8].command_CA, command_CA);
			local_LOG_command_STRA[counter_U8].command_function_B_FP = command_function_B_FP;
			local_LOG_command_STRA[counter_U8].command_function_with_argument_B_FP = command_function_with_argument_B_FP;
			return true;
		}
	}
	LOG_PrintString("No more space for the command: ", LOG_SHOW_TIME_B, LOG_LEVEL_ERROR_EN, LOG_SHOW_LOG_LEVEL_B);
	LOG_PrintStringCRLF(command_CA, LOG_HIDE_TIME_B, LOG_LEVEL_ERROR_EN, LOG_SHOW_LOG_LEVEL_B);
	return false;
}

bool LOG_TreatCommand(char *command_CA)
{

}
