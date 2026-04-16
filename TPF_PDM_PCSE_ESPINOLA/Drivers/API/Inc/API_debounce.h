/*
 * API_debounce.h
 *
 *  Created on: Mar 27, 2026
 *      Author: Espinola Maxmiiliano
 */

#include "stm32f4xx_hal.h"
#include <stdint.h> 	// Para compilar diferentes tipos de datos (Ejemplo: uint32_t)
#include <stdbool.h>


#ifndef API_INC_API_DEBOUNCE_H_
#define API_INC_API_DEBOUNCE_H_

typedef bool bool_t;

//Pulsador de la placa
#define B1_Pin GPIO_PIN_13
#define B1_GPIO_Port GPIOC

#define LD2_GPIO_Port GPIOA
#define LD2_Pin GPIO_PIN_5


void debounceFSM_init();		//Carga el estado inicial
void debounceFSM_update();	// Lee las entradad y resuelve la logica de transición de estados del pulsador
void buttonPressed();
void buttonReleased();
bool_t readKey(); //Devuelve true si fue presionado el boton

typedef enum{
BUTTON_UP,
BUTTON_FALLING,
BUTTON_DOWN,
BUTTON_RISING,
} debounceState_t;




#endif /* API_INC_API_DEBOUNCE_H_ */
