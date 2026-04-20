/*
 * LCD_port.h
 *
 *  Created on: Apr 9, 2026
 *      Author: Maximiliano Ariel Espínola
 */

#ifndef API_INC_LCD_PORT_H_
#define API_INC_LCD_PORT_H_

#include <stdint.h>
//#include <stdbool.h>
#include "stm32f4xx_hal.h"

#define ADDR_I2C_LCD_PCF8574T 0x27 //Configurar dirección I2C del módulo

//--------Public functions-----------------//
void LCD_Port_Init(void);// Inicializa el puerto de comunicación del (I2C)
void LCD_Port_Write(uint8_t data);// Envía un byte al expansor (PCF8574)
void LCD_Port_Delay(uint32_t ms);// Delay en ms
#endif /* API_INC_LCD_PORT_H_ */
