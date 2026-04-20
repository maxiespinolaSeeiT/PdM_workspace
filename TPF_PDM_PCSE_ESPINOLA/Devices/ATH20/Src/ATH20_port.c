/*
 * ATH20_port.c
 *
 *  Created on: Apr 15, 2026
 *      Author: Espínola Maximiliano Ariel
 */

#ifndef ATH20_SRC_ATH20_PORT_C_
#define ATH20_SRC_ATH20_PORT_C_

#include "ATH20_port.h"
#include "BSP_I2C.h"

extern I2C_HandleTypeDef hi2c1; // Ajustar según tu configuración

#define AHT20_ADDRESS (0x38 << 1) // Dirección I2C

//Funcion para escribir en el puerto I2C, devuelve el estado del sensor, recibe el dato y el tamaño a escribir
ath_status_t ATH_I2C_Write(uint8_t *data, uint16_t size) {

	if (bsp_i2c_write(data,size,AHT20_ADDRESS)) {
        return ATH_OK;
    }
    return ATH_ERROR;
}

ath_status_t ATH_I2C_Read(uint8_t *data, uint16_t size) {
    if (bsp_i2c_read(data, size,AHT20_ADDRESS)) {
        return ATH_OK;
    }
    return ATH_ERROR;
}

void ATH_Delay(uint32_t ms) {
	bsp_i2c_delay(ms);
}

#endif /* ATH20_SRC_ATH20_PORT_C_ */
