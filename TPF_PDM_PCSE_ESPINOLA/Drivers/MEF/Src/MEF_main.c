/*
 * MEF_main.c
 *
 * Máquina de Estados Principal
 *
 *  Created on: Apr 12, 2026
 *      Author: Maxmiliano Ariel Espinola
 */
#include "MEF_main.h"
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include "API_debounce.h"
#include "API_uart.h"
#include "API_cmdparser.h"
#include "ATH20_MEF_driver.h"
#include "BMP280_MEF_driver.h"

bool_t flag;
float temp, hum;
//static float bmp_t, bmp_p;

static MEF_main_state_t currentState = INIT;
static bool stateInit = true;
static bool_t keyP=false;

static bool ath_done = false;
static bool bmp_done = false;

cmd_t cmd = CMD_NONE;



float t1, h1; //Temperatura y Humedad del ATH20
float t2, p2; //Temperatura y Presion ATM del BMP280
float tempAmbient=0.00f; //Variable para calcular el promedio entre las dos temperaturas del sensor

char buffer_show[32];
void MEF_main_init() {
    currentState = INIT;
    uartInit();
    debounceFSM_init();
    LCD_Init();
    ATH_Init();
    BMP_Init();

}

void MEF_main_update() {
	debounceFSM_update();
	cmdPoll();
	ATH_Update();
	BMP280_Update();
	//keyP=readKey();
	keyP=flag;

	//BMP280_Task(&bmp_t, &bmp_p);

	switch (currentState) {
    case INIT:
                LCD_SetCursor(0, 0);
                LCD_WriteString("STATE: INIT");
                uartSendString((uint8_t*)"STATE: INIT\r\n");
                currentState = IDLE;
                stateInit = true;
                break;

        case IDLE:
        	if (stateInit) {
        	        LCD_SetCursor(0, 0);
        	        LCD_WriteString("STATE: IDLE");
        	        uartSendString((uint8_t*)"STATE: IDLE\r\n");
        	        stateInit = false;
        	    }

        	cmd= cmdParser_GetCommand();
        	if (cmd != CMD_NONE && cmd != CMD_MENU && cmd!=CMD_HELP)
        	{
        	    currentState = READ_SENSOR;
        	    stateInit = true;
        	}

        	break;

        case READ_SENSOR:
        	if (stateInit) {
        	        LCD_SetCursor(0, 0);
        	        LCD_WriteString("READ_SENSOR");
        	        uartSendString((uint8_t*)"STATE: READ_SENSOR\r\n");
        	        //ATH_Read(&temp, &hum);
        	        ATH_Start();
        	        BMP_Start();
        	        stateInit = false;
        	    }

        	    currentState = PROCESS_DATA;
        	    stateInit = true;

            break;

        case PROCESS_DATA:

        	if (stateInit) {
        	        LCD_SetCursor(0, 0);
        	        LCD_WriteString("PROCESS_DATA");
        	        uartSendString((uint8_t*)"STATE: PROCESS_DATA\r\n");
        	        stateInit = false;
        	    }

        	if (ATH_IsReady())
        	{
        	    ath_done = true;
        	}

        	if (BMP280_IsReady())
        	{
        	    bmp_done = true;
        	}

        	if (ath_done && bmp_done)
        	{
        	    currentState = SHOW_T_P;
        	    stateInit = true;
        	    ath_done = false;
        	    bmp_done = false;
        	}

            break;

        case SHOW_T_P:

                LCD_SetCursor(0, 0);
                LCD_WriteString("SHOW_T_P");

                if (cmd == CMD_TEMP)
				{
                	ATH_GetData(&t1, &h1);
                	BMP280_GetData(&t2, &p2);
                	tempAmbient=(t1+t2)/2.0f;
                	sprintf(buffer_show, "Temperatura Ambiente: %.2f C \r\n", tempAmbient);
                	uartSendString((uint8_t*)buffer_show);
				}

                if (cmd == CMD_PRES)
				{
					BMP280_GetData(&t2, &p2);

					sprintf(buffer_show, "Presión ATM: %.2f hPa\r\n", p2/100.0f);
					uartSendString((uint8_t*)buffer_show);
				}




                /*

                if (ATH_GetData(&t1, &h1) && BMP280_GetData(&t2, &p2))
                {
                    sprintf(buffer, "ATH: %.2fC %.2f%%\r\n", t1, h1);
                    uartSendString((uint8_t*)buffer);

                    sprintf(buffer2, "BMP: %.2fC %.2f hPa\r\n", t2, p2/100.0f);
                    uartSendString((uint8_t*)buffer2);
                }

				*/

                LCD_SetCursor(1, 0);
                LCD_WriteString("T:--.-C P:----");




            currentState = IDLE;
            flag=false; //Cambio para que solo muestre una vez hasta el próximo comando TEMP? en Pharser
            break;

        case ERROR_INIT:
            LCD_SetCursor(0, 0);
            LCD_WriteString("ERROR_INIT");
            uartSendString((uint8_t*)"STATE: ERROR INIT\r\n");
            break;

        case ERROR_SENS:
            LCD_SetCursor(0, 0);
            LCD_WriteString("ERROR_SENS");
            uartSendString((uint8_t*)"STATE: ERROR_SENS\r\n");
            break;

        default:
            currentState = ERROR_INIT;
            break;
    }
}
