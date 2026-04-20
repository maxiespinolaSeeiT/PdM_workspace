/*
 * BMP280_driver.h
 *
 *  Created on: Apr 11, 2026
 *      Author: Maximiliano Ariel Espínola
 */

#ifndef BMP280_DRIVERS_INC_BMP280_DRIVER_H_
#define BMP280_DRIVERS_INC_BMP280_DRIVER_H_

#include <stdint.h>
#include <stdint.h>
#include <stdbool.h>
#include "BMP280_port.h"
#include "API_common_types.h"

//--------Public functions-----------------//
void BMP280_Update();
bool_t BMP280_IsReady(void);
bool_t BMP280_GetData(float *t, float *p);
void BMP_Init();
void BMP_Start(void);
bool_t BMP_Is_Init(void);
void BMP_Forced_Error(void);
bool_t BMP_initialized(void);

#endif /* BMP280_DRIVERS_INC_BMP280_DRIVER_H_ */
