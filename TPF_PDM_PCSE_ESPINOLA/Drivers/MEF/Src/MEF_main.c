/*
 * MEF_main.c
 *
 * Máquina de Estados Principal
 *
 *  Created on: Apr 12, 2026
 *      Author: Maxmiliano Ariel Espinola
 */


#include <BMP280_driver.old>
#include "MEF_main.h"
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include "API_debounce.h"
#include "API_uart.h"
//#include "ATH20_driver.h"
#include "API_cmdparser.h"
#include "ATH20_MEF_driver.h"
#include "BMP280_MEF_driver.h"


bool_t flag;
float temp, hum;
static float bmp_t, bmp_p;

// Estado actual
static MEF_main_state_t currentState = INIT;
static bool stateInit = true;
static bool_t keyP=false;

void MEF_main_init() {
    currentState = INIT;
    uartInit();
    debounceFSM_init();
    LCD_Init();
    //BMP280_Init();
    ATH_Init2();
    /*
    if (BMP280_Init())
    {
        uartSendString((uint8_t*)"BMP INIT OK\r\n");
    }
    else
    {
        uartSendString((uint8_t*)"BMP INIT ERROR\r\n");
    }
    */
}

void MEF_main_update() {
	debounceFSM_update();
	cmdPoll();
	ATH_Update();
	//keyP=readKey();
	keyP=flag;



	BMP280_Task(&bmp_t, &bmp_p);


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

				if (keyP) {
					uartSendString((uint8_t*)"BOTON DETECTADO\r\n");
				}

        	    if (keyP) {
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

        	if (ATH_IsReady() && BMP280_IsReady()) //Espera que este la medición de los dos sensores
        	    {
        	        currentState = SHOW_T_P;
        	        stateInit = true;
        	    }

        	    //currentState = SHOW_T_P;
        	   // stateInit = true;

            break;

        case SHOW_T_P:

                LCD_SetCursor(0, 0);
                LCD_WriteString("SHOW_T_P");

                /*Mostrar ATH20
                char buffer[32];
                if (ATH_Read(&temp, &hum)) {

                        // Mostrar por UART
                        sprintf(buffer, "ATH20 -> Temp: %.2f C | HumRel: %.2f %%\r\n", temp, hum);
                        uartSendString((uint8_t*)buffer);

                        // Mostrar en LCD (16x2 típico)
                        LCD_SetCursor(1, 0);
                        sprintf(buffer, "T:%2.1fC H:%2.1f%%", temp, hum);
                        LCD_WriteString(buffer);

                    } else {

                        uartSendString((uint8_t*)"Error lectura AHT20\r\n");

                        LCD_SetCursor(1, 0);
                        LCD_WriteString("Error sensor   ");
                    }

                //fin mostrar ath20

                if (ATH_IsReady())
                    {
                        float t, h;
                        char buffer[32];

                        if (ATH_GetData(&t, &h))
                        {
                        	sprintf(buffer, "ATH20 -> Temp: %.2f C | HumRel: %.2f %%\r\n", t, h);
                        	uartSendString((uint8_t*)buffer);
                        }
                    }

                //mostrar BMP280
                char buffer2[32];
                sprintf(buffer2, "BMP280 -> Temp: %.2f C | PressAtm: %.2f hPa\r\n",
                        bmp_t,
                        bmp_p/100.0f);

                uartSendString((uint8_t*)buffer2);

                //fin mostrar BMP280*/

                float t1, h1;
                float t2, p2;
                char buffer[32];
                char buffer2[32];

                if (ATH_GetData(&t1, &h1) && BMP280_GetData(&t2, &p2))
                {
                    sprintf(buffer, "ATH: %.2fC %.2f%%\r\n", t1, h1);
                    uartSendString((uint8_t*)buffer);

                    sprintf(buffer2, "BMP: %.2fC %.2f hPa\r\n", t2, p2/100.0f);
                    uartSendString((uint8_t*)buffer2);
                }



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

