/*
 * ATH20_MEF_driver.c
 *
 *  Created on: Apr 15, 2026
 *      Author: Maximiliano Ariel Espínola
 */
#include <stdint.h>
#include "ATH20_MEF_driver.h"
#include "API_uart.h"
#include "ATH20_port.h"
#include "BSP_I2C.h"

//Comandos del sensor
#define CMD_INIT       0xBE // Inicializar
#define CMD_MEASURE    0xAC // Medir
#define CMD_SOFTRESET  0xBA // Reset
#define CMD_CALIB_OK 0x08

#define SIZE_RX_BUFFER 7 //Tamaño del buffer de recepción de datos
#define CMD_SENSOR 3 //Tamaño del buffer para enviar comandos al sensor

static uint8_t rx_buffer[SIZE_RX_BUFFER]; //Buffer para recibir los datos
static uint8_t status; //Controla el estado del sensor leyendo el primer byte

typedef enum {
    ATH_STATE_IDLE,

    ATH_STATE_INIT_SEND,
    ATH_STATE_INIT_WAIT,
    ATH_STATE_INIT_CHECK,

    ATH_STATE_TRIGGER,
    ATH_STATE_WAIT,
    ATH_STATE_READ,
    ATH_STATE_PROCESS,
    ATH_STATE_DONE,
    ATH_STATE_ERROR
} ATH_State_t;

static uint8_t cmdInit1[CMD_SENSOR] = {CMD_INIT, 0x08, 0x00}; //Comando INIT y bytes de configuracion interna segun datasheet
static uint8_t cmdMeasure1[CMD_SENSOR] = {CMD_MEASURE, 0x33, 0x00}; //Comando que dispara la medición
static uint32_t raw_hum1; //humedad sin procesar
static uint32_t raw_temp1; //temperatura sin procesar
static ATH_State_t state = ATH_STATE_IDLE; //Estado de la MEF del sensor
static uint32_t tick_start = 0;//Control de retardos no bloqueantes
static uint32_t tick = 0;//Control de retardos no bloqueantes

static bool_t initialized = false;

static float temp_internal;
static float hum_internal;

void ATH_Update(void)
{
    switch(state)
    {
        case ATH_STATE_IDLE:
            break;

        case ATH_STATE_INIT_SEND:
        	if(ATH_I2C_Write(cmdInit1, 3)==ATH_OK){
        		tick = bsp_i2c_getTick();
        		state = ATH_STATE_INIT_WAIT;
        	}
            break;

        case ATH_STATE_INIT_WAIT:
            if (bsp_i2c_getTick() - tick >= 10) { // tiempo recomendado en el datasheet
                state = ATH_STATE_INIT_CHECK;
            }
            if (bsp_i2c_getTick() - tick > 200) {   // timeout
				state = ATH_STATE_ERROR;
			}
            break;

        case ATH_STATE_INIT_CHECK:
        {
            status = ATH_ReadStatus();

            if (status & CMD_CALIB_OK) { // calibrado OK
                initialized = true;
                state = ATH_STATE_IDLE;
            } else {

                state = ATH_STATE_ERROR;
            }
        }
        break;

        case ATH_STATE_TRIGGER:
			if (!initialized) break;
			if (ATH_I2C_Write(cmdMeasure1, 3) == ATH_OK)
			{
				tick_start = bsp_i2c_getTick();
				state = ATH_STATE_WAIT;
			}
			else
			{
				state = ATH_STATE_ERROR;
			}
		break;




        case ATH_STATE_WAIT:
            if ((bsp_i2c_getTick() - tick_start) >= 80)
            {
                state = ATH_STATE_READ;
            }
            if ((bsp_i2c_getTick() - tick_start) > 500) //Timeout
			{
				state = ATH_STATE_ERROR;
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
	if (!initialized) return;
    if (state == ATH_STATE_IDLE || state == ATH_STATE_DONE)
    {
        state = ATH_STATE_TRIGGER;
    }
}

bool_t ATH_IsReady(void) {
    return (state == ATH_STATE_DONE);
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

void ATH_Init(void) {
	initialized = false;
	state=ATH_STATE_INIT_SEND;
}

bool_t ATH_Is_Init(void) {

	if(state==ATH_STATE_ERROR){
		initialized = false;
		return false;
	}else{
		return true;
	}
}

uint8_t ATH_ReadStatus(void)
{
    uint8_t status = 0;

    if (ATH_I2C_Read(&status, 1) != ATH_OK) {
        return 0xFF; // error
    }

    return status;
}

void ATH_Forced_Error(void){
	initialized = false;
	state=ATH_STATE_ERROR;
}

bool_t ATH_initialized(void) {
    return initialized;
}
