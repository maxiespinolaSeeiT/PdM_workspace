/*
 * I2C_port.c
 *
 *  Created on: Apr 15, 2026
 *      Author: Usuario
 */
#include <BSP_I2C.h>

I2C_HandleTypeDef hi2c1;



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
    Error_Handler();
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
