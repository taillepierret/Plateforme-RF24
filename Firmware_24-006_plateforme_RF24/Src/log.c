#include "../Inc/log.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../Inc/tools.h"

static LOG_HAL_functions_str local_LOG_HAL_functions_STR;
static LOG_level_en local_LOG_level_EN;
static LOG_command_str local_LOG_command_STRA[LOG_SIZE_OF_COMMANDS_U8];
static uint16_t local_size_buffer_uart_2_rx_U16;

typedef enum
{
	LOG_STATE_LOOK_FOR_CHEVRONS_EN,
	LOG_STATE_LOOK_FOR_CRC32_EN,
	LOG_STATE_CHECK_CRC32_EN,
	LOG_STATE_LOOK_FOR_ARGUMENT_EN,
	LOG_STATE_EXECUTE_COMMMAND_WITH_ARGUMENT_EN,
	LOG_STATE_EXECUTE_COMMAND_WITHOUT_ARGUMENT_EN
}LOG_state_decoder_en;


bool LOG_printHelloWorld(void);
bool LOG_addCommand_B(char *command_CA, bool (*command_function_B_FP)(void), bool (*command_function_with_argument_B_FP)(char *argument_CA));

void LOG_Init(LOG_HAL_functions_str *LOG_HAL_functions_STR, uint16_t size_buffer_uart_2_rx_U16)
{
	local_size_buffer_uart_2_rx_U16 = size_buffer_uart_2_rx_U16;
	local_LOG_HAL_functions_STR = *LOG_HAL_functions_STR;
	local_LOG_HAL_functions_STR.InitDebugUart();
	LOG_addCommand_B("<HELLO WORLD>\r\n", &LOG_printHelloWorld, NULL);
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

bool LOG_addCommand_B(char *command_CA, bool (*command_function_B_FP)(void), bool (*command_function_with_argument_B_FP)(char *argument_CA))
{
	if (strlen(command_CA) > LOG_COMMAND_LENGTH_U8) {
		LOG_PrintString("Command too long", LOG_SHOW_TIME_B, LOG_LEVEL_ERROR_EN, LOG_SHOW_LOG_LEVEL_B);
		return false;
	}
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

bool LOG_TreatCommand_B(char *frame_CA)
{
	/* les commandes sont au format suivant:
	 * <commande: argument>XXX\r\n avec XXX le CRC32
	 * <commande: argument>\r\n
	 * <commande>\r\n
	 * <commande>XXX\r\n avec XXX le CRC32
	 */
	LOG_state_decoder_en state_decoder_EN = LOG_STATE_LOOK_FOR_CHEVRONS_EN;
	char* begin_frame_CP = NULL;
	char* end_frame_CP = NULL;
	char* end_command_CA = NULL;
	uint32_t crc32_U32 = 0;
	char* crc32_CA = NULL;
	bool process_ended_B = false;

	while(process_ended_B == false) //TODO mettre un timeout
	{
		switch (state_decoder_EN)
		{
		case LOG_STATE_LOOK_FOR_CHEVRONS_EN:
			begin_frame_CP = strchr(frame_CA, '<');
			if (begin_frame_CP == NULL)
			{
				LOG_PrintString("No begin frame found", LOG_SHOW_TIME_B, LOG_LEVEL_ERROR_EN, LOG_SHOW_LOG_LEVEL_B);
				return false;
			}
			end_frame_CP = strchr(begin_frame_CP, '>');
			if (end_frame_CP == NULL) {
				LOG_PrintString("No end frame found", LOG_SHOW_TIME_B,
						LOG_LEVEL_ERROR_EN, LOG_SHOW_LOG_LEVEL_B);
				return false;
			}
			state_decoder_EN = LOG_STATE_LOOK_FOR_CRC32_EN;
			break;

		case LOG_STATE_LOOK_FOR_CRC32_EN:
			end_frame_CP++;
			if (strncmp(end_frame_CP, "\r\n", 2) == 0)
			{
				state_decoder_EN = LOG_STATE_LOOK_FOR_ARGUMENT_EN;
			}
			else
			{
				crc32_CA = end_frame_CP + 2;
				crc32_U32 = strtol(crc32_CA, NULL, 10);
				state_decoder_EN = LOG_STATE_CHECK_CRC32_EN;
			}
			break;

		case LOG_STATE_CHECK_CRC32_EN:
			if (crc32_U32 == TOOLS_CRC32((uint8_t*) begin_frame_CP,	(uint32_t)(end_frame_CP - begin_frame_CP)))
			{
				state_decoder_EN = LOG_STATE_LOOK_FOR_ARGUMENT_EN;
			}
			else
			{
				LOG_PrintString("CRC32 not valid", LOG_SHOW_TIME_B,	LOG_LEVEL_ERROR_EN, LOG_SHOW_LOG_LEVEL_B);
				return false;
			}
			break;

		case LOG_STATE_LOOK_FOR_ARGUMENT_EN:
			end_command_CA = strchr(begin_frame_CP, ':');
			if (end_command_CA == NULL)
			{
				state_decoder_EN = LOG_STATE_EXECUTE_COMMAND_WITHOUT_ARGUMENT_EN;
			}
			else
			{
				state_decoder_EN = LOG_STATE_EXECUTE_COMMMAND_WITH_ARGUMENT_EN;
			}
			break;

		case LOG_STATE_EXECUTE_COMMAND_WITHOUT_ARGUMENT_EN:
			for (uint8_t counter_U8 = 0; counter_U8 < LOG_SIZE_OF_COMMANDS_U8; counter_U8++) {
				if (strncmp(local_LOG_command_STRA[counter_U8].command_CA, begin_frame_CP, strlen(local_LOG_command_STRA[counter_U8].command_CA)) == 0)
				{
					return local_LOG_command_STRA[counter_U8].command_function_B_FP();
				}
			}
			LOG_PrintString("Command not found", LOG_SHOW_TIME_B, LOG_LEVEL_ERROR_EN,LOG_SHOW_LOG_LEVEL_B);
			return false;

		case LOG_STATE_EXECUTE_COMMMAND_WITH_ARGUMENT_EN:
			char *begin_argument_CA = end_command_CA + 1;
			char *end_argument_CA = strchr(begin_argument_CA, '>');
			if (end_argument_CA == NULL) {
				LOG_PrintString("No end argument found", LOG_SHOW_TIME_B, LOG_LEVEL_ERROR_EN, LOG_SHOW_LOG_LEVEL_B);
				return false;
			}
			for (uint8_t counter_U8 = 0; counter_U8 < LOG_SIZE_OF_COMMANDS_U8;counter_U8++)
			{
				if (strncmp(local_LOG_command_STRA[counter_U8].command_CA,	begin_frame_CP, end_command_CA - begin_frame_CP) == 0)
				{
					return local_LOG_command_STRA[counter_U8].command_function_with_argument_B_FP(begin_argument_CA);
				}
			}
			LOG_PrintString("Command not found", LOG_SHOW_TIME_B, LOG_LEVEL_ERROR_EN,LOG_SHOW_LOG_LEVEL_B);
			return false;
		default:
			LOG_PrintString("Unknown state", LOG_SHOW_TIME_B, LOG_LEVEL_ERROR_EN, LOG_SHOW_LOG_LEVEL_B);
			return false;
		}
	}
}

bool LOG_CleanReceivedBuffer_B(char* raw_buffer_CA, char *cleanin_buffer_CA) {
	char* begin_frame_CP = strchr(raw_buffer_CA, '<');
	char* end_frame_CP = strchr(raw_buffer_CA, '\n');
	if (begin_frame_CP == NULL)
	{
		return false;
	}
	else if (end_frame_CP == NULL)
	{
		return false;
	}
	else if (end_frame_CP > begin_frame_CP)
	{
		strcpy(cleanin_buffer_CA, begin_frame_CP);
	}
	else
	{
		memcpy(cleanin_buffer_CA, begin_frame_CP,raw_buffer_CA + local_size_buffer_uart_2_rx_U16 - begin_frame_CP);
		memcpy(cleanin_buffer_CA + (raw_buffer_CA + local_size_buffer_uart_2_rx_U16 - begin_frame_CP), raw_buffer_CA, end_frame_CP-raw_buffer_CA+1);
	}
	memset(raw_buffer_CA, 0, local_size_buffer_uart_2_rx_U16);
	return true;
}

void LOG_process(char* raw_buffer_CA, char *cleaning_buffer_CA)
{
	bool command_treated_B = false;
	HAL_getUart2Buffer(raw_buffer_CA);
	if (LOG_CleanReceivedBuffer_B(raw_buffer_CA, cleaning_buffer_CA) == true)
	{
		command_treated_B = LOG_TreatCommand_B(cleaning_buffer_CA);
		memset(raw_buffer_CA, 0, local_size_buffer_uart_2_rx_U16);
		memset(cleaning_buffer_CA, 0, local_size_buffer_uart_2_rx_U16);
		HAL_flushUart2Buffer();
	}
}

bool LOG_printHelloWorld(void)
{
	LOG_PrintString("Hello World", LOG_SHOW_TIME_B, LOG_LEVEL_INFO_EN,LOG_SHOW_LOG_LEVEL_B);
	return true;
}


