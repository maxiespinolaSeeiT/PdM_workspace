/*
 * API_uart.h
 *
 *  Created on: Apr 2, 2026
 *      Author: Espinola Maximiliano Ariel
 */

#ifndef API_INC_API_UART_H_
#define API_INC_API_UART_H_

#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include "API_delay.h"

#define LD2_Pin GPIO_PIN_5
#define LD2_GPIO_Port GPIOA

bool_t uartInit();
void uartSendString(uint8_t * pstring);
void uartSendStringSize(uint8_t * pstring, uint16_t size);
void uartReceiveStringSize(uint8_t * pstring, uint16_t size);
void uartImprimirConfig();
bool_t uartReceiveByte(uint8_t *pstring); //funcion para ver si recibí o no algo

#endif /* API_INC_API_UART_H_ */
