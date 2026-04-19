/*
 * LCD_driver.h
 *
 *  Created on: Apr 9, 2026
 *      Author: Usuario
 */

#ifndef API_INC_LCD_DRIVER_H_
#define API_INC_LCD_DRIVER_H_

#include <stdint.h>
#include "stm32f4xx_hal.h"

#include "../../../Devices/LCD/Inc/LCD_port.h"

// API pública
void LCD_Init(void);
void LCD_SendCommand(uint8_t cmd);
void LCD_SendData(uint8_t data);
void LCD_Clear(void);
void LCD_SetCursor(uint8_t row, uint8_t col);
void LCD_WriteString(char *str);


#endif /* API_INC_LCD_DRIVER_H_ */
