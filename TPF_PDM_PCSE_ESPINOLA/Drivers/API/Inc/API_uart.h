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
#include "API_common_types.h"

//--------Public functions-----------------//
void uartSendString(uint8_t * pstring); //Envia un string de datos por la UART
void uartSendStringSize(uint8_t * pstring, uint16_t size);//Envia una cantidad definida por size por la UART
void uartReceiveStringSize(uint8_t * pstring, uint16_t size); //Recibe en el puntero que indiquemos como argumento la cantidad de bytes que carguemos en size
void uartImprimirConfig(); //Envia por la UART la configuracion actual
void uartFlush(); //Limpia el buffer de la UART
uint32_t consultaBaudrate(); //Es un getter para saber el baudrate actual de la UART
bool_t uartInit(); //Inicia la comunicacion por la UART
bool_t uartSetBaudrate(uint32_t); //Es un setter para cambiar el baudrate de la UART
bool_t esperarEnvio(); //Espera que se envíe todo lo que tenga en el buffer
bool_t uartReceiveByte(uint8_t *pstring); //funcion para ver si recibí o no algo
#endif /* API_INC_API_UART_H_ */
