/*
 * ATH20_MEF_driver.h
 *
 *  Created on: Apr 15, 2026
 *      Author: Maximiliano Ariel Espínola
 */

#ifndef ATH20_INC_ATH20_MEF_DRIVER_H_
#define ATH20_INC_ATH20_MEF_DRIVER_H_

#include "stdint.h"
#include "API_common_types.h"

//--------Public functions-----------------//
void ATH_Init(void); //Inicializa el sensor
void ATH_Start(void); //Dispara la lectura del driver
void ATH_Update(void); //Actualiza la MEF del sensor
bool_t ATH_IsReady(void); //Avisa si termió de procesar
bool_t ATH_GetData(float *temp, float *hum); //Obtiene los datos del sensor
uint8_t ATH_ReadStatus(void); //Lee el primer byte para controlar el estado del sensor
bool_t ATH_Is_Init(void); //Devuelve true si el sensor está iniciado
void ATH_Forced_Error(void); //Fuerza la MEF del sensor al estado de error
bool_t ATH_initialized(void); //Devuelve true si el sensor está iniciado y listo para medir

#endif /* ATH20_INC_ATH20_MEF_DRIVER_H_ */
