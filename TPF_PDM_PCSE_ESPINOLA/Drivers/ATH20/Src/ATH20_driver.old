/*
 * ATH20_drivers.c
 *
 *  Created on: Apr 15, 2026
 *      Author: Maximiliano Ariel Espinola
 */

#include "ATH20_driver.h"
#include "ATH20_port.h"

//Comandos del sensor
#define CMD_INIT       0xBE // Inicializar
#define CMD_MEASURE    0xAC // Medir
#define CMD_SOFTRESET  0xBA // Reset

static uint8_t rx_buffer[7]; //Buffer para recibir los datos
uint8_t cmdInit[3] = {CMD_INIT, 0x08, 0x00}; //Comando INIT y bytes de configuracion interna segun datasheet
uint8_t cmdMeasure[3] = {CMD_MEASURE, 0x33, 0x00}; //Comando que dispara la medición
uint32_t raw_hum; //humedad sin procesar
uint32_t raw_temp; //temperatura sin procesar


bool_t ATH_Init(void) {

	ATH_Delay(40);//Asegura que espera 40ms una vez encendido

    if (ATH_I2C_Write(cmdInit, 3) != ATH_OK)
        return false;

    ATH_Delay(10);
    return true;
}

//Recibe dos punteros donde guardará los datos leidos del sensor
bool_t ATH_Read(float *temperature, float *humidity) {

    // Envía comando para la medición
    if (ATH_I2C_Write(cmdMeasure, 3) != ATH_OK)
        return false;

    // Esperar medición (~80ms según datasheet)
    ATH_Delay(80);

    // Leer 7 bytes
    if (ATH_I2C_Read(rx_buffer, 7) != ATH_OK)
        return false;

    // Verificar busy bit
    if (rx_buffer[0] & 0x80)
        return false;

    /* Procesar humedad (20 bits que es la resolución interna del sensor)

    Byte 1 → humedad (bits 19-12)
    Byte 2 → humedad (bits 11-4)
    Byte 3 → humedad (bits 3-0) + temperatura (bits 19-16)
    Byte 4 → temperatura (bits 15-8)
    Byte 5 → temperatura (bits 7-0)

	*/

    //datos crudos de humedad
    raw_hum = ((uint32_t)rx_buffer[1] << 12) | ((uint32_t)rx_buffer[2] << 4)  | (rx_buffer[3] >> 4);

    // datos crudos de temperatura
    raw_temp = ((uint32_t)(rx_buffer[3] & 0x0F) << 16) | ((uint32_t)rx_buffer[4] << 8) | rx_buffer[5];

    // Convertir según datasheet

    //RH = (raw / 2^20) * 100
    *humidity = ((float)raw_hum / (1 << 20)) * 100.0f;

    //T = (raw / 2^20) * 200 - 50
    *temperature = ((float)raw_temp / (1 << 20)) * 200.0f - 50.0f;

    return true;
}
