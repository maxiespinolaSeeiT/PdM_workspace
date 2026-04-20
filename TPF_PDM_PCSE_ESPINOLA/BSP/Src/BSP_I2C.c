/*
 * I2C_port.c
 *
 *  Created on: Apr 15, 2026
 *      Author: Espínola Maximiliano Ariel
 */
#include <BSP_I2C.h>
#include "stm32f4xx_hal.h"

I2C_HandleTypeDef hi2c1;

static void Error_Handler_I2C(void);

bool_t I2C_init(void)
{
  hi2c1.Instance = I2C1;
  hi2c1.Init.ClockSpeed = 100000;
  hi2c1.Init.DutyCycle = I2C_DUTYCYCLE_2;
  hi2c1.Init.OwnAddress1 = 0;
  hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c1.Init.OwnAddress2 = 0;
  hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c1) != HAL_OK)
  {

    Error_Handler_I2C();
    return false;
  }else{
	  return true;
  }

}

void I2C_Force_Restart(void){
	// Reset del periférico I2C para liberar el bus si quedó bloqueado
	__HAL_RCC_I2C1_FORCE_RESET();
	HAL_Delay(10);
	__HAL_RCC_I2C1_RELEASE_RESET();
	HAL_Delay(10);
}

bool_t bsp_i2c_write(uint8_t *data, uint16_t size, uint16_t address){
	if (HAL_I2C_Master_Transmit(&hi2c1, address, data, size, HAL_MAX_DELAY) == HAL_OK) {
		return true;
	}
	return false;
}

bool_t bsp_i2c_read(uint8_t *data, uint16_t size, uint16_t address){
	if (HAL_I2C_Master_Receive(&hi2c1, address, data, size, HAL_MAX_DELAY) == HAL_OK) {
		return true;
	}
	return false;
}

bool_t bsp_i2c_mem_read(uint8_t reg, uint8_t *data, uint16_t size, uint16_t address){
	if ((HAL_I2C_Mem_Read(&hi2c1, address, reg, I2C_MEMADD_SIZE_8BIT,data, size,HAL_MAX_DELAY) == HAL_OK)){
		return true;
		}
	return false;
}

void bsp_i2c_delay(uint32_t ms){
	HAL_Delay(ms);
}

uint32_t  bsp_i2c_getTick(void){
	return HAL_GetTick();
}

static void Error_Handler_I2C(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}
