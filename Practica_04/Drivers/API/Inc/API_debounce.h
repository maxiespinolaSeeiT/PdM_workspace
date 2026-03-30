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


void debounceFSM_init();		// debe cargar el estado inicial
void debounceFSM_update();	// debe leer las entradas, resolver la lógica de
// transición de estados y actualizar las salidas
void buttonPressed();			// debe encender el LED
void buttonReleased();		// debe apagar el LED


typedef enum{
BUTTON_UP,
BUTTON_FALLING,
BUTTON_DOWN,
BUTTON_RISING,
} debounceState_t;



void buttonPressed();
void buttonReleased();
bool_t readKey();

#endif /* API_INC_API_DEBOUNCE_H_ */
