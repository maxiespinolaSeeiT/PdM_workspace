/*
 * I2C_port.h
 *
 *  Created on: Apr 15, 2026
 *      Author: Usuario
 */

#ifndef API_INC_API_I2C_H_
#define API_INC_API_I2C_H_

#include "stm32f4xx_hal.h"
#include "API_common_types.h"


bool_t I2C_init(void);
void I2C_Force_Restart(void);

#endif /* API_INC_API_I2C_H_ */
