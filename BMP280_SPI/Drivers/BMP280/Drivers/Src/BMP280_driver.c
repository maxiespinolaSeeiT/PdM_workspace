/*
 * BMP280_driver.c
 *
 *  Created on: Apr 11, 2026
 *      Author: Usuario
 */
#include "BMP280_driver.h"
#include "BMP280_port.h"
#include "API_uart.h"

// ===================== REGISTERS =====================
#define BMP280_REG_CTRL_MEAS 0xF4
#define BMP280_REG_CONFIG    0xF5
#define BMP280_REG_TEMP      0xFA
#define BMP280_REG_PRESS     0xF7
#define BMP280_REG_RESET     0xE0

// ===================== LOW LEVEL =====================
static void BMP280_WriteReg(uint8_t reg, uint8_t data)
{
    uint8_t tx[2];

    tx[0] = reg & 0x7F;
    tx[1] = data;

    BMP280_CS_Select();
    BMP280_SPI_Write(tx, 2);
    BMP280_CS_Deselect();
}

static void BMP280_ReadRegs(uint8_t reg, uint8_t *data, uint8_t len)
{
    uint8_t tx[10] = {0};
    uint8_t rx[10] = {0};

    tx[0] = reg | 0x80;

    BMP280_CS_Select();
    BMP280_SPI_Read(tx, rx, len + 1);
    BMP280_CS_Deselect();

    for (int i = 0; i < len; i++)
        data[i] = rx[i + 1];
}

// ===================== INIT =====================
void BMP280_Init(void)
{
    BMP280_WriteReg(BMP280_REG_RESET, 0xB6);
    HAL_Delay(100);

    BMP280_WriteReg(BMP280_REG_CTRL_MEAS, 0x27);
    BMP280_WriteReg(BMP280_REG_CONFIG, 0x00);


    uartSendString((uint8_t*)"BMP280 inicializado OK\r\n");
}

// ===================== RAW =====================
static int32_t BMP280_ReadTempRaw(void)
{
    uint8_t data[3];
    BMP280_ReadRegs(BMP280_REG_TEMP, data, 3);

    return (data[0] << 12) | (data[1] << 4) | (data[2] >> 4);
}

static int32_t BMP280_ReadPressRaw(void)
{
    uint8_t data[3];
    BMP280_ReadRegs(BMP280_REG_PRESS, data, 3);

    return (data[0] << 12) | (data[1] << 4) | (data[2] >> 4);
}

// ⚠️ versión simplificada (sin calibración por ahora)
float BMP280_ReadTemperature(void)
{
    return (float)BMP280_ReadTempRaw() / 100.0f;
}

float BMP280_ReadPressure(void)
{
    return (float)BMP280_ReadPressRaw() / 256.0f;
}

