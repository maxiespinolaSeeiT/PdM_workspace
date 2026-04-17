/*
 * ATH20_MEF_driver.h
 *
 *  Created on: Apr 15, 2026
 *      Author: Usuario
 */

#ifndef ATH20_INC_ATH20_MEF_DRIVER_H_
#define ATH20_INC_ATH20_MEF_DRIVER_H_

#include "stdint.h"

typedef bool bool_t;
void ATH_Init(void); //Inicializa el sensor
void ATH_Start(void); //Dispara la lectura del driver
void ATH_Update(void); //Actualiza la MEF del sensor
bool_t ATH_IsReady(void); //Avisa si termió de procesar
bool_t ATH_GetData(float *temp, float *hum); //Obtiene los datos del sensor
uint8_t ATH_ReadStatus(void); //Lee el primer byte para controlar el estado del sensor

#endif /* ATH20_INC_ATH20_MEF_DRIVER_H_ */
