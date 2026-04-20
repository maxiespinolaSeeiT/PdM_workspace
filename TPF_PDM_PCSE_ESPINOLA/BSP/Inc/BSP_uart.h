/*
 * BSP_uart.h
 *
 * Inicializa, envia y recibe datos de la USART2.
 *
 *  Created on: Apr 20, 2026
 *      Author: Maximiliano Ariel Espínola
 *
 *  Cuando hay un error se enciende el LD2 de la Nucleo-F446RE
 */

#ifndef INC_BSP_UART_H_
#define INC_BSP_UART_H_

#include "API_common_types.h"

#define UARTTIMEOUT 10
#define BAUDRATE_UART 115200

//--------Public functions-----------------//
bool_t bsp_uart_init(uint32_t baudrate);
bool_t bsp_uart_write(uint8_t *data, uint16_t size);
bool_t bsp_uart_read(uint8_t *data, uint16_t size, uint32_t timeout);
bool_t bsp_uart_read_byte(uint8_t *data);
bool_t bsp_uart_set_baudrate(uint32_t baudrate);
uint32_t bsp_uart_get_baudrate(void);
void bsp_uart_flush(void);
bool_t bsp_uart_tx_done(void);
void bsp_uart_print_cfg();


#endif /* INC_BSP_UART_H_ */
