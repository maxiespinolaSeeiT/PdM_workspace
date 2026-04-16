/*
 * LCD_port.c
 *
 *  Created on: Apr 9, 2026
 *      Author: Maximiliano Ariel Espinola
 */
#include "LCD_port.h"

extern I2C_HandleTypeDef hi2c1; //Struct de I2C

#define LCD_I2C_ADDR (ADDR_I2C_LCD_PCF8574T << 1) // Dirección del módulo LCD

void LCD_Port_Init(void) //Inicia el puerto I2C
{
	MX_I2C1_Init();
}

void LCD_Port_Write(uint8_t data)
{
    HAL_I2C_Master_Transmit(&hi2c1, LCD_I2C_ADDR, &data, 1, HAL_MAX_DELAY);
}

void LCD_Port_Delay(uint32_t ms)
{
    HAL_Delay(ms);
}

/*
void MX_I2C1_Init(void)
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
	  Error_Handler_LCDPort();
  }
}
*/

void Error_Handler_LCDPort(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}


