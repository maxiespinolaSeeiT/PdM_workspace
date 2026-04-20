/*
 * BSP_GPIO.h
 *
 *  Created on: Apr 20, 2026
 *      Author: Maximiliano Ariel Espinola
 */

#ifndef INC_BSP_GPIO_H_
#define INC_BSP_GPIO_H_

#include "stm32f4xx_hal.h"

#define LD2_Pin GPIO_PIN_5 //LED para indicar Error en la UART
#define LD2_GPIO_Port GPIOA
//--------Public functions-----------------//
void BSP_GPIO_Init(void);

#endif /* INC_BSP_GPIO_H_ */
