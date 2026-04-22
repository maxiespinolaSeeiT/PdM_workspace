/*
 * API_I2C.c
 *
 *  Created on: Apr 22, 2026
 *      Author: Usuario
 */

#include "BSP_I2C.h"
#include "API_uart.h"


void api_i2c_print_cfg(void) //Imprime los valores de configuración de la UART huart2
{
    char buffer[100];
    I2C_HandleTypeDef I2C_config;
    I2C_config=bsp_i2c_getConfig();
    uint8_t devices_found, i;

    uartSendString((uint8_t*)"\r\n==================================================\r\n");
	uartSendString((uint8_t*)"            CONFIGURACION PUERTO I2C\r\n");
	uartSendString((uint8_t*)"==================================================\r\n");

	uartSendString((uint8_t*)"--------------------------------------------------\r\n");

	// Instance (identificación simple)
	if (I2C_config.Instance == I2C1)
		sprintf(buffer, "- Instance: I2C1\r\n");
	else if (I2C_config.Instance == I2C2)
		sprintf(buffer, "- Instance: I2C2\r\n");
	else if (I2C_config.Instance == I2C3)
		sprintf(buffer, "- Instance: I2C3\r\n");
	else
		sprintf(buffer, "- Instance: UNKNOWN\r\n");

	uartSendString((uint8_t*)buffer);

	// Clock Speed
	sprintf(buffer, "- Clock Speed: %lu Hz\r\n", I2C_config.Init.ClockSpeed);
	uartSendString((uint8_t*)buffer);

	// Duty Cycle
	if (I2C_config.Init.DutyCycle == I2C_DUTYCYCLE_2)
		sprintf(buffer, "- Duty Cycle: 2\r\n");
	else if (I2C_config.Init.DutyCycle == I2C_DUTYCYCLE_16_9)
		sprintf(buffer, "- Duty Cycle: 16/9\r\n");
	else
		sprintf(buffer, "- Duty Cycle: UNKNOWN\r\n");

	uartSendString((uint8_t*)buffer);

	// Addressing Mode
	if (I2C_config.Init.AddressingMode == I2C_ADDRESSINGMODE_7BIT)
		sprintf(buffer, "- Addressing Mode: 7-bit\r\n");
	else if (I2C_config.Init.AddressingMode == I2C_ADDRESSINGMODE_10BIT)
		sprintf(buffer, "- Addressing Mode: 10-bit\r\n");
	else
		sprintf(buffer, "- Addressing Mode: UNKNOWN\r\n");

	uartSendString((uint8_t*)buffer);

	// Own Address 1
	sprintf(buffer, "- Own Address 1: 0x%02lX\r\n", I2C_config.Init.OwnAddress1);
	uartSendString((uint8_t*)buffer);

	// Dual Address Mode
	sprintf(buffer, "- Dual Address Mode: %s\r\n",
			(I2C_config.Init.DualAddressMode == I2C_DUALADDRESS_ENABLE) ? "ENABLE" : "DISABLE");
	uartSendString((uint8_t*)buffer);

	// Own Address 2
	sprintf(buffer, "- Own Address 2: 0x%02lX\r\n", I2C_config.Init.OwnAddress2);
	uartSendString((uint8_t*)buffer);

	// General Call
	sprintf(buffer, "- General Call Mode: %s\r\n",
			(I2C_config.Init.GeneralCallMode == I2C_GENERALCALL_ENABLE) ? "ENABLE" : "DISABLE");
	uartSendString((uint8_t*)buffer);

	// No Stretch
	sprintf(buffer, "- No Stretch Mode: %s\r\n",
			(I2C_config.Init.NoStretchMode == I2C_NOSTRETCH_ENABLE) ? "ENABLE" : "DISABLE");
	uartSendString((uint8_t*)buffer);

	uartSendString((uint8_t*)"--------------------------------------------------\r\n");

	uartSendString((uint8_t*)"Dirección de los dispositivos I2C conectados\r\n\r\n");

		for (i = 1; i < 128; i++)  // direcciones válidas 7-bit
		{
			// IMPORTANTE: en HAL la dirección va shiftada << 1
			if (bsp_i2c_isDeviceReady(&I2C_config, i))
			{
				sprintf(buffer, ">> Dispositivo: 0x%02X\r\n", i);
				uartSendString((uint8_t*)buffer);
				devices_found++;
			}
		}

	uartSendString((uint8_t*)"--------------------------------------------------\r\n\r\n");
    }

