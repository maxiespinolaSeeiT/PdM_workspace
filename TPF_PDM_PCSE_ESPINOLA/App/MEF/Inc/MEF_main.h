/*
 * MEF_main.h
 *
 *  Created on: Apr 12, 2026
 *      Author: Usuario
 */

#include "LCD_driver.h"
#include "LCD_port.h"

#ifndef MEF_INC_MEF_MAIN_H_
#define MEF_INC_MEF_MAIN_H_

typedef enum{
INIT,
IDLE,
READ_SENSOR,
PROCESS_DATA,
SHOW_T_P_H,
ERROR_MAIN,
REBOOT,
WAIT_USER,
} MEF_main_state_t;

void MEF_main_init(); // Inicia el dispositivo y sus perisféricos
void MEF_main_update(); // Actualiza el estado de la MEF

#endif /* MEF_INC_MEF_MAIN_H_ */
