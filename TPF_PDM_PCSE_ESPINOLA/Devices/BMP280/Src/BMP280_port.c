/*
 * BMP280_port.c
 *
 *  Created on: Apr 11, 2026
 *      Author: Usuario
 */

#include "BMP280_port.h"

#include "BSP_I2C.h"



#define BMP280_ADDRESS (0x77 << 1) //  Direccion del BMP280

bmp280_status_t BMP280_WriteReg(uint8_t reg, uint8_t *data, uint16_t size)
{
    uint8_t buffer[16];

    buffer[0] = reg;
    for (uint16_t i = 0; i < size; i++) {
        buffer[i + 1] = data[i];
    }

    if (bsp_i2c_write(buffer, size+1, BMP280_ADDRESS))
    {
    	return BMP280_OK;
    }
        return BMP280_ERROR;
}

bmp280_status_t BMP280_ReadReg(uint8_t reg, uint8_t *data, uint16_t size)
{
	if(bsp_i2c_mem_read(reg,data,size,BMP280_ADDRESS)){
		return BMP280_OK;
	}
	return BMP280_ERROR;
}

void BMP280_Delay(uint32_t ms)
{
    bsp_i2c_delay(ms);
}
