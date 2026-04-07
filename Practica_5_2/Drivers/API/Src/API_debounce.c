/*
 * API_debounce.c
 *
 *  Created on: Mar 27, 2026
 *      Author: Maximiliano Espinola
 */
#include "main.h"
#include "API_debounce.h"
#include "API_delay.h"

static debounceState_t debounceFSMstate=BUTTON_UP; //Esta es la variable de estado.
static delay_t delay_debounce; //delay del antirebote
static bool_t key_pressed=false;

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
	key_pressed = false;
	return temp_value;
}

void boardLedOn(){
	HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_SET);
}

void boardLedOff(){
	HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_RESET);
}



// Carga el estado inicial
void debounceFSM_init(){
	delayInit(&delay_debounce,debounceMS);
	debounceFSMstate=BUTTON_UP;
}

void debounceFSM_update(){

	//if (delayIsRunning(&delay_debounce)) return; //Si el delay está corriendo NO sigue
	if (!delayRead(&delay_debounce)) return;

	switch (debounceFSMstate) {

			case BUTTON_UP:
				//buttonReleased();
				if(HAL_GPIO_ReadPin(B1_GPIO_Port, B1_Pin) == GPIO_PIN_RESET){ //Si esta presionado
					// Cambiar a otro estado
					debounceFSMstate = BUTTON_FALLING;
				}
			break;
			case BUTTON_FALLING:
				//buttonReleased();
				if (HAL_GPIO_ReadPin(B1_GPIO_Port, B1_Pin) == GPIO_PIN_RESET){ //Si está presionado y ya paso el retardo
					debounceFSMstate=BUTTON_DOWN;
					key_pressed = true;}
				else{
					 debounceFSMstate = BUTTON_UP;
					}
			break;

			case	BUTTON_DOWN:
				//buttonPressed();
				if(HAL_GPIO_ReadPin(B1_GPIO_Port, B1_Pin) == GPIO_PIN_SET){ //Si esta presionado
					// Cambiar a otro estado
					debounceFSMstate = BUTTON_RISING;
				}
			break;

			case	BUTTON_RISING:
				//buttonPressed();
				if (HAL_GPIO_ReadPin(B1_GPIO_Port, B1_Pin) == GPIO_PIN_SET){
					debounceFSMstate=BUTTON_UP;
				}else {
		            debounceFSMstate = BUTTON_DOWN;
		        }
			break;

			default:
				debounceFSMstate=BUTTON_UP;
			break;
			}
}
