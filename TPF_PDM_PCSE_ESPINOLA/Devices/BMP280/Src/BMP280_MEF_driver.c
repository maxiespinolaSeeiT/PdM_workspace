/*
 * BMP280_driver.c
 *
 *  Created on: Apr 11, 2026
 *      Author: Maximiliano Ariel Espínola
 */
#include "../../../Devices/BMP280/Inc/BMP280_MEF_driver.h"

#include "stdint.h"

#include "../../../Devices/BMP280/Inc/BMP280_port.h"

// Registros
#define REG_CALIB  0x88
#define REG_CTRL   0xF4
#define REG_CONFIG 0xF5
#define REG_DATA   0xF7
#define REG_STATUS 0xF3

// Estructura de calibración
typedef struct {
    uint16_t dig_T1;
    int16_t  dig_T2;
    int16_t  dig_T3;

    uint16_t dig_P1;
    int16_t  dig_P2;
    int16_t  dig_P3;
    int16_t  dig_P4;
    int16_t  dig_P5;
    int16_t  dig_P6;
    int16_t  dig_P7;
    int16_t  dig_P8;
    int16_t  dig_P9;
} BMP280_Calib_t;

static BMP280_Calib_t calib;
//static int32_t t_fine;

//Estados de la MEF BMP280
typedef enum {
	BMP_INIT_CONFIG,
	BMP_INIT_CALIB,
	BMP_INIT_DONE,
    BMP_IDLE,
    BMP_START,
    BMP_WAIT,
    BMP_READ,
    BMP_DONE
} bmp_state_t;

static bmp_state_t state = BMP_IDLE;
static uint32_t tick_start;
static uint8_t data[6];

static float temperature, last_temp;
static float pressure, last_press;
static bool_t data_ready = false;
static bool_t initialized = false;


//Toma los datos de calibración del sensor
static void BMP280_ReadCalibration(void)
{
    uint8_t data[24];
    BMP280_ReadReg(REG_CALIB, data, 24);

    calib.dig_T1 = (data[1] << 8) | data[0];
    calib.dig_T2 = (data[3] << 8) | data[2];
    calib.dig_T3 = (data[5] << 8) | data[4];

    calib.dig_P1 = (data[7] << 8) | data[6];
    calib.dig_P2 = (data[9] << 8) | data[8];
    calib.dig_P3 = (data[11] << 8) | data[10];
    calib.dig_P4 = (data[13] << 8) | data[12];
    calib.dig_P5 = (data[15] << 8) | data[14];
    calib.dig_P6 = (data[17] << 8) | data[16];
    calib.dig_P7 = (data[19] << 8) | data[18];
    calib.dig_P8 = (data[21] << 8) | data[20];
    calib.dig_P9 = (data[23] << 8) | data[22];
}

void BMP280_Update()
{
    uint8_t status;

    switch (state)
    {
        case BMP_IDLE:

            break;

        case BMP_INIT_CONFIG: // CONFIG
		{
			uint8_t ctrl = 0x25; //Mofo FORCED
			uint8_t cfg  = 0xA0;

			//BMP280_WriteReg(uint8_t reg, uint8_t *data, uint16_t size);


			if (BMP280_WriteReg(REG_CTRL, &ctrl, 1) != BMP280_OK)
				state = BMP_IDLE;

			if (BMP280_WriteReg(REG_CONFIG, &cfg, 1) != BMP280_OK)
				state = BMP_IDLE;

			state = BMP_INIT_CALIB;
			break;
		}
        case BMP_INIT_CALIB:
        {
        	BMP280_ReadCalibration();
			state = BMP_INIT_DONE;
			break;
        }

        case BMP_INIT_DONE:
        {
        	initialized = true;
        	state = BMP_IDLE;
        }

        case BMP_START:
        {
            uint8_t ctrl = 0x24; // FORCED MODE
            if (BMP280_WriteReg(REG_CTRL, &ctrl, 1) != BMP280_OK)
            {
                state = BMP_IDLE;
            }

            tick_start = HAL_GetTick();
            state = BMP_WAIT;
            break;
        }

        case BMP_WAIT:
            BMP280_ReadReg(REG_STATUS, &status, 1);

            if (!(status & 0x08)) // terminó medición
            {
                state = BMP_READ;
            }
            else if (HAL_GetTick() - tick_start > 100) // timeout
            {
                state = BMP_IDLE;

            }
            break;

        case BMP_READ:
            if (BMP280_ReadReg(REG_DATA, data, 6) == BMP280_OK)
            {
            	// Armo el ADC con los 20bits que me envia el sensor
            	    int32_t adc_P = (data[0] << 12) | (data[1] << 4) | (data[2] >> 4);
            	    int32_t adc_T = (data[3] << 12) | (data[4] << 4) | (data[5] >> 4);

            	    // Calculo la temperatura con la calibración
            	    float var1 = (((float)adc_T) / 16384.0f - ((float)calib.dig_T1) / 1024.0f) * ((float)calib.dig_T2);
            	    float var2 = ((((float)adc_T) / 131072.0f - ((float)calib.dig_T1) / 8192.0f) *
            	                  (((float)adc_T) / 131072.0f - ((float)calib.dig_T1) / 8192.0f)) *
            	                 ((float)calib.dig_T3);

            	    float t_fine_f = var1 + var2;
            	    temperature = t_fine_f / 5120.0f;

            	    //Calculo la presión con la calibración
            	    float var1_p = (t_fine_f / 2.0f) - 64000.0f;
            	    float var2_p = var1_p * var1_p * ((float)calib.dig_P6) / 32768.0f;
            	    var2_p = var2_p + var1_p * ((float)calib.dig_P5) * 2.0f;
            	    var2_p = (var2_p / 4.0f) + (((float)calib.dig_P4) * 65536.0f);

            	    var1_p = (((float)calib.dig_P3) * var1_p * var1_p / 524288.0f +
            	              ((float)calib.dig_P2) * var1_p) / 524288.0f;

            	    var1_p = (1.0f + var1_p / 32768.0f) * ((float)calib.dig_P1);

            	    if (var1_p == 0.0f)
            	    	state = BMP_IDLE;

            	    float p = 1048576.0f - (float)adc_P;
            	    p = (p - (var2_p / 4096.0f)) * 6250.0f / var1_p;

            	    var1_p = ((float)calib.dig_P9) * p * p / 2147483648.0f;
            	    var2_p = p * ((float)calib.dig_P8) / 32768.0f;

            	    p = p + (var1_p + var2_p + ((float)calib.dig_P7)) / 16.0f;

            	    pressure = p;

            	    last_temp = temperature;
            	    last_press = pressure;

                state = BMP_DONE;
            }
            else
            {
                state = BMP_IDLE;
            }
            break;

        case BMP_DONE:
        	data_ready = true;
        	state = BMP_IDLE;

    }


}

bool_t BMP280_IsReady(void)
{
    return data_ready;
}

bool_t BMP280_GetData(float *t, float *p)
{
    if (data_ready)
    {
        *t = last_temp;
        *p = last_press;
        data_ready = false;
        return true;
    }
    return false;
}

//Controla que el estado esté en BMP_IDLE y el sensor inicializado
void BMP_Start(void)
{
    if (state == BMP_IDLE && initialized)
    {
        state = BMP_START;
    }

}
void BMP_Init(){
	state = BMP_INIT_CONFIG;
}

