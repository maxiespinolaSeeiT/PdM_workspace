/*
 * BMP280_driver.c
 *
 *  Created on: Apr 11, 2026
 *      Author: Usuario
 */
#include "stdint.h"
#include "BMP280_driver.h"
#include "BMP280_port.h"

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
static int32_t t_fine;

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

bool_t BMP280_Init(void)
{
    uint8_t ctrl   = 0x27; // temp x1, press x1, normal mode
    uint8_t config = 0xA0; // standby + filtro

    uint8_t id;



    // Configurar control (modo normal)
    if (BMP280_Write(REG_CTRL, &ctrl, 1) != BMP280_OK)
        return false;
    // Configurar filtro y standby
    if (BMP280_Write(REG_CONFIG, &config, 1) != BMP280_OK)
        return false;
    // Leer coeficientes de calibración
    BMP280_ReadCalibration();

    return true;
}

bool_t BMP280_ReadData(float *temperature, float *pressure)
{
    uint8_t data[6];
    char buffer[80];

    // Leo el dato
    if (BMP280_ReadReg(REG_DATA, data, 6) != BMP280_OK)
        return false;

    // Armo el ADC con los 20bits que me envia el sensor
    int32_t adc_P = (data[0] << 12) | (data[1] << 4) | (data[2] >> 4);
    int32_t adc_T = (data[3] << 12) | (data[4] << 4) | (data[5] >> 4);

    // Calculo la temperatura con la calibración
    float var1 = (((float)adc_T) / 16384.0f - ((float)calib.dig_T1) / 1024.0f) * ((float)calib.dig_T2);
    float var2 = ((((float)adc_T) / 131072.0f - ((float)calib.dig_T1) / 8192.0f) *
                  (((float)adc_T) / 131072.0f - ((float)calib.dig_T1) / 8192.0f)) *
                 ((float)calib.dig_T3);

    float t_fine_f = var1 + var2;
    *temperature = t_fine_f / 5120.0f;

    //Calculo la presión con la calibración
    float var1_p = (t_fine_f / 2.0f) - 64000.0f;
    float var2_p = var1_p * var1_p * ((float)calib.dig_P6) / 32768.0f;
    var2_p = var2_p + var1_p * ((float)calib.dig_P5) * 2.0f;
    var2_p = (var2_p / 4.0f) + (((float)calib.dig_P4) * 65536.0f);

    var1_p = (((float)calib.dig_P3) * var1_p * var1_p / 524288.0f +
              ((float)calib.dig_P2) * var1_p) / 524288.0f;

    var1_p = (1.0f + var1_p / 32768.0f) * ((float)calib.dig_P1);

    if (var1_p == 0.0f)
        return false;

    float p = 1048576.0f - (float)adc_P;
    p = (p - (var2_p / 4096.0f)) * 6250.0f / var1_p;

    var1_p = ((float)calib.dig_P9) * p * p / 2147483648.0f;
    var2_p = p * ((float)calib.dig_P8) / 32768.0f;

    p = p + (var1_p + var2_p + ((float)calib.dig_P7)) / 16.0f;

    *pressure = p;

    return true;
}
