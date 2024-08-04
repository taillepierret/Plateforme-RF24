#ifndef typedef_h
#define typedef_h

#include <stdint.h>
#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

typedef union
{
    float float32;
    uint32_t uint32;
}float32_uint32_UN;


typedef enum
{
	HAL_LED_SET_OK,
	HAL_LED_SET_ERROR,
	HAL_SPI_WRITE_ERROR_EN,
	HAL_SPI_WRITE_OK_EN,
	HAL_SPI_READ_ERROR_EN,
	HAL_SPI_READ_OK_EN
}HAL_ret_val_en;


typedef struct
{
	uint8_t seconds_U8;
	uint8_t minutes_U8;
	uint8_t hours_U8;
}HAL_RTC_values_str;




#endif // typedef_h
