/*
 * ATH20_MEF_driver.c
 *
 *  Created on: Apr 15, 2026
 *      Author: Usuario
 */
#include "ATH20_port.h"
#include "ATH20_MEF_driver.h"
#include "stdint.h"

//Comandos del sensor
#define CMD_INIT       0xBE // Inicializar
#define CMD_MEASURE    0xAC // Medir
#define CMD_SOFTRESET  0xBA // Reset

static uint8_t rx_buffer[7]; //Buffer para recibir los datos

typedef enum {
    ATH_STATE_IDLE,
    ATH_STATE_TRIGGER,
    ATH_STATE_WAIT,
    ATH_STATE_READ,
    ATH_STATE_PROCESS,
    ATH_STATE_DONE,
    ATH_STATE_ERROR
} ATH_State_t;

uint8_t cmdInit1[3] = {CMD_INIT, 0x08, 0x00}; //Comando INIT y bytes de configuracion interna segun datasheet
uint8_t cmdMeasure1[3] = {CMD_MEASURE, 0x33, 0x00}; //Comando que dispara la medición
uint32_t raw_hum1; //humedad sin procesar
uint32_t raw_temp1; //temperatura sin procesar


static ATH_State_t state = ATH_STATE_IDLE;
static uint32_t tick_start = 0;

static float temp_internal;
static float hum_internal;

void ATH_Update(void)
{
    switch(state)
    {
        case ATH_STATE_IDLE:
            break;

        case ATH_STATE_TRIGGER:
            if (ATH_I2C_Write(cmdMeasure1, 3) == ATH_OK)
            {
                tick_start = HAL_GetTick(); // guardo tiempo
                state = ATH_STATE_WAIT;
            }
            else
            {
                state = ATH_STATE_ERROR;
            }
            break;

        case ATH_STATE_WAIT:
            if ((HAL_GetTick() - tick_start) >= 80)
            {
                state = ATH_STATE_READ;
            }
            break;

        case ATH_STATE_READ:
            if (ATH_I2C_Read(rx_buffer, 7) == ATH_OK)
            {
                state = ATH_STATE_PROCESS;
            }
            else
            {
                state = ATH_STATE_ERROR;
            }
            break;

        case ATH_STATE_PROCESS:

            if (rx_buffer[0] & 0x80)
            {
                state = ATH_STATE_ERROR;
                break;
            }

            raw_hum1 = ((uint32_t)rx_buffer[1] << 12) |
                      ((uint32_t)rx_buffer[2] << 4)  |
                      (rx_buffer[3] >> 4);

            raw_temp1 = ((uint32_t)(rx_buffer[3] & 0x0F) << 16) |
                       ((uint32_t)rx_buffer[4] << 8) |
                       rx_buffer[5];

            hum_internal = ((float)raw_hum1 / (1 << 20)) * 100.0f;
            temp_internal = ((float)raw_temp1 / (1 << 20)) * 200.0f - 50.0f;

            state = ATH_STATE_DONE;
            break;

        case ATH_STATE_DONE:
            break;

        case ATH_STATE_ERROR:
            break;
    }
}

void ATH_Start(void)
{
    if (state == ATH_STATE_IDLE)
    {
        state = ATH_STATE_TRIGGER;
    }
}

bool_t ATH_IsReady(void)
{
    if(state == ATH_STATE_DONE){
    	return true;
    }else{
    	return false;
    }
}

bool_t ATH_GetData(float *temp, float *hum)
{
    if (state == ATH_STATE_DONE)
    {
        *temp = temp_internal;
        *hum = hum_internal;
        state = ATH_STATE_IDLE;
        return true;
    }

    return false;
}

bool_t ATH_Init2(void) {

	ATH_Delay(40);//Asegura que espera 40ms una vez encendido

    if (ATH_I2C_Write(cmdInit1, 3) != ATH_OK)
        return false;

    ATH_Delay(10);
    return true;
}


