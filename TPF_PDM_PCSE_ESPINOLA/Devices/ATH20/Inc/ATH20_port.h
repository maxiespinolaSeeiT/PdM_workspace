/*
 * ATH20_port.h
 *
 *  Created on: Apr 15, 2026
 *      Author: Maximiliano Ariel Espínola
 */

#ifndef ATH20_INC_ATH20_PORT_H_
#define ATH20_INC_ATH20_PORT_H_

#include <stdint.h>

//------Estados del sensor-------//
typedef enum {
    ATH_OK = 0,
    ATH_ERROR
} ath_status_t;

//--------Public functions-----------------//
ath_status_t ATH_I2C_Write(uint8_t *data, uint16_t size); //Manda datos al sensor
ath_status_t ATH_I2C_Read(uint8_t *data, uint16_t size); //Recibe datos del sensor
void ATH_Delay(uint32_t ms); //delay para configurar el sensor

#endif /* ATH20_INC_ATH20_PORT_H_ */
