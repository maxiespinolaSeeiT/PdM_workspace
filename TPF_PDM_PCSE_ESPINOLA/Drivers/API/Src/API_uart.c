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
#include <BSP_uart.h>

#define USART_TX_Pin GPIO_PIN_2
#define USART_RX_Pin GPIO_PIN_3
#define USART_TX_GPIO_Port GPIOA
#define USART_RX_GPIO_Port GPIOA
#define TIMEOUT 10



bool_t uartInit(){

	if(bsp_uart_init(BAUDRATE_UART)){
		return true;
	}else{
		return false;
	}
}

void uartSendString(uint8_t * pstring){
	if(pstring==NULL){
		return;
	}
	uint16_t size;
    size = strlen((char*)pstring); //se hace el cast poruqe srlen espera char*, no uint8_t*
    bsp_uart_write(pstring,size);
}

void uartSendStringSize(uint8_t * pstring, uint16_t size){

	if(pstring==NULL){ //Verifica que el puntero no sea NULL
		return;
	}
	if(size < 1 ||  size > 256){ //Tomo como válido de 1 a 255
		return;
	}

	bsp_uart_write(pstring,size);
}
void uartReceiveStringSize(uint8_t * pstring, uint16_t size){
	if(pstring==NULL){ //Verifica que el puntero no sea NULL
			return;
		}
	if(size < 1 || size > 256){ //Tomo como válido de 1 a 255
			return;
		}
	bsp_uart_read(pstring, size,TIMEOUT);

}

bool_t uartReceiveByte(uint8_t *pstring)
{
    if (pstring == NULL) {
        return false;
    }

    if(bsp_uart_read_byte(pstring)){
    	return true;
    }else{
    	return false;
    }
}

