/*
 * LCD_driver.h
 *
 *  Created on: Apr 9, 2026
 *      Author: Maximiliano Ariel Espínola
 */

#ifndef API_INC_LCD_DRIVER_H_
#define API_INC_LCD_DRIVER_H_

#include <stdint.h>
#include "LCD_port.h"

//--------Public functions-----------------//
void LCD_Init(void); //Inicia la pantalla
void LCD_SendCommand(uint8_t cmd);//Envia un comando al LCD
void LCD_SendData(uint8_t data);//Envia datos de 8 bits a la pantalla LCD
void LCD_Clear(void); //Borra la pantalla LCD
void LCD_SetCursor(uint8_t row, uint8_t col); //Posiciona el cursor (FILA,COLUMNA)
void LCD_WriteString(char *str); //Imprime un string en la pantalla LCD


#endif /* API_INC_LCD_DRIVER_H_ */
