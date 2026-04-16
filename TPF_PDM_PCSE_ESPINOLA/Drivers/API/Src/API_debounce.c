/*
 * API_debounce.c
 *
 *  Created on: Mar 27, 2026
 *      Author: Maximiliano Espinola
 */

#include "API_debounce.h"
#include "API_delay.h"

static debounceState_t debounceFSMstate=BUTTON_UP; //Esta es la variable de estado.
static bool_t key_pressed=false;
tick_t debounce;


#define debounceMS 40U //Cantidad de tiempo en ms para detectar el flanco

void buttonPressed(){
	boardLedOn();
}
void buttonReleased(){
	boardLedOff();
}


/* La función readKey debe leer una variable interna del módulo y devolver true o false
si la tecla fue presionada.
Si devuelve true, debe resetear (poner en false) el estado de la variable.*/
bool_t readKey(){
	bool_t temp_value = key_pressed;
	if (key_pressed) {
	    uartSendString((uint8_t*)"DEBOUNCE OK\r\n");
	}
	key_pressed = false;
	return temp_value;
}

void boardLedOn(){
	HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_RESET);
}

void boardLedOff(){
	HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_SET);
}



// Carga el estado inicial
void debounceFSM_init(){
	debounceFSMstate=BUTTON_UP;
	key_pressed = false;
}

void debounceFSM_update(){

	switch (debounceFSMstate) {

			case BUTTON_UP:
				if(HAL_GPIO_ReadPin(B1_GPIO_Port, B1_Pin) == GPIO_PIN_RESET){ //Si esta presionado
					// Cambiar a otro estado
					debounceFSMstate = BUTTON_FALLING;
					debounce=HAL_GetTick(); //tomo el tiempo actual
				}
			break;
			case BUTTON_FALLING:
				if((HAL_GetTick() - debounce) >= debounceMS){
					if (HAL_GPIO_ReadPin(B1_GPIO_Port, B1_Pin) == GPIO_PIN_RESET){ //Si está presionado y ya paso el retardo
						debounceFSMstate=BUTTON_DOWN;
						HAL_GPIO_TogglePin(LD2_GPIO_Port, LD2_Pin);
						key_pressed = true;}
				else{
					 debounceFSMstate = BUTTON_UP;
					}
				}
			break;

			case	BUTTON_DOWN:
				if(HAL_GPIO_ReadPin(B1_GPIO_Port, B1_Pin) == GPIO_PIN_SET){ //Si esta suelto
					// Cambiar a otro estado
					debounceFSMstate = BUTTON_RISING;
					debounce=HAL_GetTick(); //tomo el tiempo actual
				}
			break;

			case	BUTTON_RISING:
				if((HAL_GetTick() - debounce) >= debounceMS){
					if (HAL_GPIO_ReadPin(B1_GPIO_Port, B1_Pin) == GPIO_PIN_SET){
						debounceFSMstate=BUTTON_UP;
					}else {
						debounceFSMstate = BUTTON_DOWN;
					}
				}
			break;

			default:
				debounceFSM_init();
			break;
			}
}
