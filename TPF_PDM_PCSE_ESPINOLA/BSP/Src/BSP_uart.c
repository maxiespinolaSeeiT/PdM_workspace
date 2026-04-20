/*
 * BSP_uart.c
 *
 *  Created on: Apr 19, 2026
 *      Author: Usuario
 */
#include "BSP_uart.h"
#include "API_uart.h"
static UART_HandleTypeDef huart2;
bool_t bsp_uart_init(uint32_t baudrate){
	huart2.Instance = USART2;
	  huart2.Init.BaudRate = baudrate;
	  huart2.Init.WordLength = UART_WORDLENGTH_8B;
	  huart2.Init.StopBits = UART_STOPBITS_1;
	  huart2.Init.Parity = UART_PARITY_NONE;
	  huart2.Init.Mode = UART_MODE_TX_RX;
	  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
	  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
	  if (HAL_UART_Init(&huart2) != HAL_OK)
	  {
		return false;
		HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_SET); //LD2 de la Nucleo-F446RE indica ERROR
	  }

	  return true;
}

bool_t bsp_uart_write(uint8_t *data, uint16_t size){
	if ((HAL_UART_Transmit(&huart2, data, size, UARTTIMEOUT) == HAL_OK)){
		return true;
	}else{
		HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_SET);
		return false;
	}
}
bool_t bsp_uart_read(uint8_t *data, uint16_t size, uint32_t timeout){
	if((HAL_UART_Receive(&huart2, data, size, timeout))!=HAL_OK){
		HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_SET); //se enciende el LD2 de la Nucleo-F446RE indicando error
	}
}
bool_t bsp_uart_read_byte(uint8_t *data){
	if (HAL_UART_Receive(&huart2, data, 1, 0) == HAL_OK) {
		return true;   // llegó dato nuevo
	}
	return false;      // no llegó nada
}

bool_t bsp_uart_set_baudrate(uint32_t baudrate){
	if (baudrate < 9600 || baudrate > 921600)
		return false;

	// Desinicializar UART
	if (HAL_UART_DeInit(&huart2) != HAL_OK)
		return false;

	// Cambiar baudrate
	huart2.Init.BaudRate = baudrate;

	// Reinicializar UART
	if (HAL_UART_Init(&huart2) != HAL_OK)
		return false;

	return true;
}
uint32_t bsp_uart_get_baudrate(void){
	return huart2.Init.BaudRate;
}

void bsp_uart_flush(void){
	__HAL_UART_CLEAR_OREFLAG(&huart2);
	__HAL_UART_CLEAR_NEFLAG(&huart2);
	__HAL_UART_CLEAR_FEFLAG(&huart2);
	__HAL_UART_CLEAR_PEFLAG(&huart2);

	uint8_t clear;

	// vaciar FIFO RX (si hay datos colgados)
	while (HAL_UART_Receive(&huart2, &clear, 1, 0) == HAL_OK);
}
bool_t bsp_uart_tx_done(void){ //esperarEnvio()
	while (__HAL_UART_GET_FLAG(&huart2, UART_FLAG_TC) == RESET);
	return true;
}

void bsp_uart_print_cfg() //Imprime los valores de configuración de la UART huart2
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
