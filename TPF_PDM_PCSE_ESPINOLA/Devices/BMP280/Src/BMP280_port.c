/*
 * BMP280_port.c
 *
 *  Created on: Apr 11, 2026
 *      Author: Usuario
 */

#include "../../../Devices/BMP280/Inc/BMP280_port.h"

#include "../../../BSP/Inc/BSP_I2C.h"

extern I2C_HandleTypeDef hi2c1;

#define BMP280_ADDRESS (0x77 << 1) //  Direccion del BMP280

bmp280_status_t BMP280_WriteReg(uint8_t reg, uint8_t *data, uint16_t size)
{
    uint8_t buffer[16];

    buffer[0] = reg;
    for (uint16_t i = 0; i < size; i++) {
        buffer[i + 1] = data[i];
    }

    if (HAL_I2C_Master_Transmit(&hi2c1, BMP280_ADDRESS, buffer, size + 1, HAL_MAX_DELAY) == HAL_OK)
    {
    	return BMP280_OK;
    }
        return BMP280_ERROR;
}

bmp280_status_t BMP280_ReadReg(uint8_t reg, uint8_t *data, uint16_t size)
{
    if (HAL_I2C_Mem_Read(&hi2c1, BMP280_ADDRESS, reg, I2C_MEMADD_SIZE_8BIT,data, size,HAL_MAX_DELAY) == HAL_OK)
    {
        return BMP280_OK;
    }
    return BMP280_ERROR;
}

void BMP280_Delay(uint32_t ms)
{
    HAL_Delay(ms);
}
