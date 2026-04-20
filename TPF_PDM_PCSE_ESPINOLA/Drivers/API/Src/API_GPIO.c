/*
 * API_GPIO.c
 *
 *  Created on: Apr 20, 2026
 *      Author: Usuario
 */

#include <stdint.h>
#include "API_GPIO.h"
#include "BSP_GPIO.h"

void led_error_on(){
	HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_SET);
}

void GPIO_init(){
	BSP_GPIO_Init();
}
