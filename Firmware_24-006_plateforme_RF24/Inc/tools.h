#ifndef TOOLS_H
#define TOOLS_H
#include <stdint.h>
#include <stdbool.h>

#define UINT32_MAX_VALUE_U32 ((uint32_t)0xFFFFFFFF)

uint32_t TOOLS_CRC32(uint8_t *data, uint32_t length);
bool TOOLS_IsTimeoutEnded_B(uint32_t start_time_U32, uint32_t timeout_U32, uint32_t current_time_U32);


#endif // TOOLS_H
