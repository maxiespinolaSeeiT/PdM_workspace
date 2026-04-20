/*
 * BSP_Uart.h
 *
 *  Created on: Apr 19, 2026
 *      Author: Usuario
 */

#include "API_common_types.h"
#ifndef INC_BSP_UART_H_
#define INC_BSP_UART_H_

#define UARTTIMEOUT 10

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
