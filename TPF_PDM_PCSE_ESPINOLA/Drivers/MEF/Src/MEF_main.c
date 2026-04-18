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
char buffer_lcd[16];
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

	switch (currentState) {
    case INIT:
				if (stateInit) {
				LCD_SetCursor(0, 0);
				LCD_WriteString("ESTACION");
				LCD_SetCursor(1, 0);
				LCD_WriteString("METEOROLOGICA");
				uartSendString((uint8_t*)"ESTACION METEOROLOGICA\r\n");

				stateInit = false;
				}

                currentState = IDLE;
                stateInit = true;
                break;

        case IDLE:
        	if (stateInit) {
        		uartSendString((uint8_t*)"Escriba MENU para ver los comandos disponible disponible\r\n");
        		uartSendString((uint8_t*)"Ejecute un comando: \r\n");
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
        	        ATH_Start();
        	        BMP_Start();
        	        stateInit = false;
        	    }
        		if (ATH_IsReady() && BMP280_IsReady()) {
        		        currentState = PROCESS_DATA;
        		        stateInit = true;
        		 }
        	    //currentState = PROCESS_DATA;
        	    stateInit = true;

            break;

        case PROCESS_DATA:

        	if (cmd == CMD_TEMP)
			{
				if (ATH_GetData(&t1, &h1)==true && BMP280_GetData(&t2, &p2)==true)
				{
					tempAmbient=(t1+t2)/2.0f; //Calculo el promedio de las temperaturas de los dos sensores
					currentState = SHOW_T_P;
				}
			}
			if (cmd == CMD_PRES)
			{
				if(BMP280_GetData(&t2, &p2)==true)
				{
					sprintf(buffer_show, "Presión ATM: %.2f hPa\r\n", p2/100.0f);
					sprintf(buffer_lcd, "%.2f hPa", p2/100.0f);
					currentState = SHOW_T_P;
				}
			}
			if (cmd == CMD_HUM)
			{
				if(ATH_GetData(&t1, &h1)==true)
				{
				sprintf(buffer_show, "Humedad Relativa: %.2f %c\r\n", h1, 0x25);
				sprintf(buffer_lcd, "%.2f%c", h1,0x25);
				currentState = SHOW_T_P;
				}
			}

            break;

        case SHOW_T_P:
                if (cmd == CMD_TEMP)
				{
                	uartSendString((uint8_t*)"Estoy en SHOW_T_P \r\n");
                	sprintf(buffer_show, "Temperatura Ambiente: %.2f ºC \r\n", tempAmbient);
                	uartSendString((uint8_t*)buffer_show);

                	LCD_Clear();
                	sprintf(buffer_lcd, "%.2f%cC%c", tempAmbient, 0xDF,'\0');
                	LCD_SetCursor(0, 0);
                	LCD_WriteString("Temp. Amb.");
                	LCD_SetCursor(1, 0);
                	LCD_WriteString(buffer_lcd);
				}

                if (cmd == CMD_PRES)
				{
					uartSendString((uint8_t*)buffer_show);

					LCD_Clear();
					LCD_SetCursor(0, 0);
					LCD_WriteString("Pres. Atm.");
					LCD_SetCursor(1, 0);
					LCD_WriteString(buffer_lcd);
				}
                if (cmd == CMD_HUM)
				{
					uartSendString((uint8_t*)buffer_show);
					LCD_Clear();
					LCD_SetCursor(0, 0);
					LCD_WriteString("Humedad Rel.");
					LCD_SetCursor(1, 0);
					LCD_WriteString(buffer_lcd);
				}
				currentState = IDLE;
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
            currentState = INIT;
            break;

        default:
            currentState = ERROR_INIT;
            break;
    }
}
