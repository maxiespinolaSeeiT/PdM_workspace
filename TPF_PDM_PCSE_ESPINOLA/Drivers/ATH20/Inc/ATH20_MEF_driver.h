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

void ATH_Update(void);
void ATH_Start(void);
bool_t ATH_IsReady(void);
bool_t ATH_GetData(float *temp, float *hum);
uint8_t ATH_ReadStatus(void);

void ATH_Init(void);
#endif /* ATH20_INC_ATH20_MEF_DRIVER_H_ */
