/*
 * API_delay.c
 *
 *API para realizar delay hasta 10.000 ms.
 *
 *  Created on: Mar 19, 2026
 *      Author: Espinola Maximiliano Ariel
 */

#include "API_delay.h"


/* USER CODE BEGIN PD */
#define MAX_DELAY 10000U //Defino el valor máximo para el delay 10.000 ms
/* USER CODE END PD */

/* USER CODE BEGIN PV */
/* USER CODE END PV */

void delayInit( delay_t * delay, tick_t duration ){

	//Compruebo que el puntero que recibe no sea NULL
	if(delay == NULL) {return;} //Agregar a futuro salida por UART

	//Compruebo una duración válida, que no sea 0 o un numero muy grande, límite MAX_DELAY
	if (duration == 0U || duration > MAX_DELAY) {return;}//Agregar a futuro salida por UART


	delay->duration=duration;
	delay->running=false;
}

bool_t delayRead( delay_t * delay ){

	//Compruebo que el puntero que recibe no sea NULL
	if(delay == NULL) {return false;}

	//Si running no está en TRUE lo inicio y tomo la marca de tiempo
	if(!delay->running){
		delay->startTime=HAL_GetTick();
		delay->running=true;
		return false;
	}

	//Si el tiempo transcurrido es mas grande que la duración lo detengo y devuelvo true
	if((HAL_GetTick()-delay->startTime)>=delay->duration){
			delay->running=false;
			return true;
	}
	return false;
	}

void delayWrite( delay_t * delay, tick_t duration ){

	//Compruebo que el puntero que recibe no sea NULL
	if(delay == NULL) {return ;}
	//Compruebo una duración válida, que no sea 0 o un número muy grande, límite MAX_DELAY
	if (duration == 0U || duration > MAX_DELAY) {return;}

	delay->duration=duration;
}

bool_t delayIsRunning( delay_t* delay ){
	//Compruebo que el puntero que recibe no sea NULL
	if(delay == NULL) {return false;}

	return (delay->running);
}
