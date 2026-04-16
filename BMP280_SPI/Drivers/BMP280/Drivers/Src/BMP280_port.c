/*
 * BMP280_port.c
 *
 *  Created on: Apr 11, 2026
 *      Author: Usuario
 */


#include "BMP280_port.h"



#include "BMP280_port.h"
#include <string.h>
#include <stdio.h>

// ===================== HANDLES =====================
static SPI_HandleTypeDef hspi1;
static UART_HandleTypeDef huart2;

// ===================== GPIO DEFINES =====================
#define CS_PORT GPIOA
#define CS_PIN  GPIO_PIN_4   // ajustá si usás otro

// ===================== INIT =====================
void BMP280_Port_Init(void)
{
    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_SPI1_CLK_ENABLE();
    __HAL_RCC_USART2_CLK_ENABLE();

    GPIO_InitTypeDef GPIO_InitStruct = {0};

    // CS
    GPIO_InitStruct.Pin = CS_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(CS_PORT, &GPIO_InitStruct);

    HAL_GPIO_WritePin(CS_PORT, CS_PIN, GPIO_PIN_SET);

    // SPI1
    hspi1.Instance = SPI1;
    hspi1.Init.Mode = SPI_MODE_MASTER;
    hspi1.Init.Direction = SPI_DIRECTION_2LINES;
    hspi1.Init.DataSize = SPI_DATASIZE_8BIT;
    hspi1.Init.CLKPolarity = SPI_POLARITY_LOW;
    hspi1.Init.CLKPhase = SPI_PHASE_1EDGE;
    hspi1.Init.NSS = SPI_NSS_SOFT;
    hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_8; // más seguro
    hspi1.Init.FirstBit = SPI_FIRSTBIT_MSB;
    HAL_SPI_Init(&hspi1);

    // UART2
    huart2.Instance = USART2;
    huart2.Init.BaudRate = 115200;
    huart2.Init.WordLength = UART_WORDLENGTH_8B;
    huart2.Init.StopBits = UART_STOPBITS_1;
    huart2.Init.Parity = UART_PARITY_NONE;
    huart2.Init.Mode = UART_MODE_TX_RX;
    HAL_UART_Init(&huart2);
}

// ===================== CS =====================
void BMP280_CS_Select(void)
{
    HAL_GPIO_WritePin(CS_PORT, CS_PIN, GPIO_PIN_RESET);
}

void BMP280_CS_Deselect(void)
{
    HAL_GPIO_WritePin(CS_PORT, CS_PIN, GPIO_PIN_SET);
}

// ===================== SPI =====================
void BMP280_SPI_Write(uint8_t *data, uint16_t len)
{
    HAL_SPI_Transmit(&hspi1, data, len, 10);
}

void BMP280_SPI_Read(uint8_t *txData, uint8_t *rxData, uint16_t len)
{
    HAL_SPI_TransmitReceive(&hspi1, txData, rxData, len, 10);
}

// ===================== UART =====================
void BMP280_UART_Send(char *msg)
{
    HAL_UART_Transmit(&huart2, (uint8_t*)msg, strlen(msg), 10);
}
