/*
 * API_uart.c
 *
 * Inicializa, envia y recibe datos de la USART2.
 *
 *  Created on: Apr 2, 2026
 *      Author: Espinola Maximiliano Ariel
 *
 *  Cuando hay un error se enciende el LD2 de la Nucleo-F446RE
 */


#include "stm32f4xx_hal.h"
#include <API_uart.h>

#define USART_TX_Pin GPIO_PIN_2
#define USART_RX_Pin GPIO_PIN_3
#define USART_TX_GPIO_Port GPIOA
#define USART_RX_GPIO_Port GPIOA
#define TIMEOUT 100

static UART_HandleTypeDef huart2;



bool_t uartInit(){ //Inicia la uart huart2

  huart2.Instance = USART2;
  huart2.Init.BaudRate = 115200;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
	return false;
	HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_SET); //se enciende el LD2 de la Nucleo-F446RE indicando error
  }
  uartSendString((uint8_t*)"Inicialización de la UART OK \n\r");
  uartImprimirConfig();
  return true; //En caso de que inicialice de forma exitosa

}

void uartSendString(uint8_t * pstring){
	if(pstring==NULL){ //Verifica que el puntero no sea NULL
			return;
		}

	uint16_t size;
    size = strlen((char*)pstring); //se hace el cast poruqe srlen espera char*, no uint8_t*

    if((HAL_UART_Transmit(&huart2, pstring, size, TIMEOUT)!=HAL_OK)){

    	HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_SET);
	}
}





void uartSendStringSize(uint8_t * pstring, uint16_t size){

	if(pstring==NULL){ //Verifica que el puntero no sea NULL
		return;
	}
	if(size < 1 ||  size > 256){ //Tomo como válido de 1 a 255
		return;
	}

	if((HAL_UART_Transmit(&huart2, pstring, size, TIMEOUT)!=HAL_OK)){

		HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_SET); //se enciende el LD2 de la Nucleo-F446RE indicando error
	}




}
void uartReceiveStringSize(uint8_t * pstring, uint16_t size){
	if(pstring==NULL){ //Verifica que el puntero no sea NULL
			return;
		}
	if(size < 1 || size > 256){ //Tomo como válido de 1 a 255
			return;
		}
	if((HAL_UART_Receive(&huart2, pstring, size, TIMEOUT))!=HAL_OK){
		HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_SET); //se enciende el LD2 de la Nucleo-F446RE indicando error
	}

	uartSendString((uint8_t*)"Recibido: ");
	uartSendString(pstring);

}




void uartImprimirConfig() //Imprime los valores de configuración de la UART huart2
{
    char buffer[100];

    uartSendString((uint8_t*)"\r\nUART CONFIG:\r\n");

    sprintf(buffer, "Baudrate: %lu\r\n", huart2.Init.BaudRate);
    uartSendString((uint8_t*)buffer);

    sprintf(buffer, "Paridad: %lu\r\n", huart2.Init.Parity);
    uartSendString((uint8_t*)buffer);

    sprintf(buffer, "Modo: %lu\r\n", huart2.Init.Mode);
    uartSendString((uint8_t*)buffer);

    sprintf(buffer, "Bit de Parada: %lu\r\n", huart2.Init.StopBits);
    uartSendString((uint8_t*)buffer);

    sprintf(buffer, "Control de Flujo: %lu\r\n", huart2.Init.HwFlowCtl);
    uartSendString((uint8_t*)buffer);

}
