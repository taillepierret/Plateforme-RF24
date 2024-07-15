#include "../Inc/tools.h"




uint32_t TOOLS_CRC32(uint8_t *data, uint32_t length)
{
    uint32_t crc = 0xFFFFFFFF;
    uint32_t i;
    uint32_t j;
    for (i = 0; i < length; i++)
    {
        crc ^= data[i];
        for (j = 0; j < 8; j++)
        {
            if (crc & 1)
            {
                crc = (crc >> 1) ^ 0xEDB88320;
            }
            else
            {
                crc = crc >> 1;
            }
        }
    }
    return ~crc;
}