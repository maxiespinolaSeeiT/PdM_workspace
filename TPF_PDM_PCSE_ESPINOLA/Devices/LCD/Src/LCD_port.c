/*
 * LCD_port.c
 *
 *  Created on: Apr 9, 2026
 *      Author: Maximiliano Ariel Espinola
 */
#include "LCD_port.h"
#include "BSP_I2C.h"
#include "stm32f4xx_hal.h"

#define LCD_I2C_ADDR (ADDR_I2C_LCD_PCF8574T << 1) // Dirección del módulo LCD
#define LCD_i2C_SEND_1BYTE 1

void LCD_Port_Init(void) //Inicia el puerto I2C
{
	I2C_init();
}

void LCD_Port_Write(uint8_t data)
{
    //HAL_I2C_Master_Transmit(&hi2c1, LCD_I2C_ADDR, &data, 1, HAL_MAX_DELAY);
	bsp_i2c_write(&data, LCD_i2C_SEND_1BYTE, LCD_I2C_ADDR);
}

void LCD_Port_Delay(uint32_t ms)
{
	bsp_i2c_delay(ms);
}
