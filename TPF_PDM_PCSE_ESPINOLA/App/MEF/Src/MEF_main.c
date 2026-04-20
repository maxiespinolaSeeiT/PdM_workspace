/*
 * MEF_main.c
 *
 * Máquina de Estados Principal
 *
 *  Created on: Apr 12, 2026
 *      Author: Maxmiliano Ariel Espinola
 */
#include <MEF/Inc/MEF_main.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

#include "API_uart.h"
#include "API_cmdparser.h"
#include "ATH20_MEF_driver.h"
#include "BMP280_MEF_driver.h"

#include "../../../BSP/Inc/BSP_I2C.h"




static MEF_main_state_t currentState = INIT;
static bool stateInit = true;
static cmd_t lastCmd = CMD_NONE;

cmd_t cmd = CMD_NONE;

float t1, h1; //Temperatura y Humedad del ATH20
float t2, p2; //Temperatura y Presion ATM del BMP280
float tempAmbient=0.00f; //Variable para calcular el promedio entre las dos temperaturas del sensor

static bool_t ath_ready = false;
static bool_t bmp_ready = false;


char buffer_show[32];
char buffer_lcd[16];
void MEF_main_init() {

    uartInit();
    cmdParserInit();

    HAL_Delay(50); //Para estabilizar el bus luego de reiniciar
    if(I2C_init()){
    	HAL_Delay(10);//Dar tiempo para la primer consulta de los perisféricos
    	LCD_Init();
		ATH_Init();
		BMP_Init();
		currentState = INIT;
    }else
    {
    	uartSendString((uint8_t*)"No se pudo inicializar el puerto I2C \r\n");
    	currentState = ERROR_INIT;
    }


}

void MEF_main_update() {

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
			    // Esperar a que ambos sensores terminen su init (pueden tardar varios ciclos)
			    if (!ATH_Is_Init() ) {
			        uartSendString((uint8_t*)"Error init ATH20 en arranque\r\n");
			        currentState = ERROR_INIT;
			        stateInit = true;
			        break;
			    }

			    if (!BMP_Is_Init()) {
			        uartSendString((uint8_t*)"Error init BMP280 en arranque\r\n");
			        currentState = ERROR_INIT;
			        stateInit = true;
			        break;
			    }

			    // Solo pasar a IDLE cuando ambos están inicializados
			    if (ATH_initialized() && BMP_initialized()) {
			        currentState = IDLE;
			        stateInit = true;
			    }
			    break;

        case IDLE:
        	if (stateInit) {
        		uartSendString((uint8_t*)"Escriba MENU y presione ENTER para ver los comandos disponibles\r\n");

        		stateInit = false;
        	}

        	cmd= cmdParser_GetCommand();
        	if (cmd != CMD_NONE && cmd != CMD_MENU && cmd != CMD_HELP) {
        	    lastCmd = cmd;  // ← guardarlo antes de cambiar de estado
        	    currentState = READ_SENSOR;
        	    stateInit = true;
        	}

        	if (cmd == CMD_REBOOT)
			{
				currentState = REBOOT;
			}

        	break;

        case READ_SENSOR:
        		if (stateInit) {
        			ath_ready = false;
					bmp_ready = false;
        			if (lastCmd == CMD_TEMP) { ATH_Start(); BMP_Start(); }
					if (lastCmd == CMD_PRES) { BMP_Start(); }
					if (lastCmd == CMD_HUM)  { ATH_Start(); }
        	        stateInit = false;
        	    }
        		if(!ATH_Is_Init()){
        			uartSendString((uint8_t*)"No se pudo inicializar el sensor ATH20 \r\n");
        			uartSendString((uint8_t*)"Verifique la conexion del hardware\r\n");
        			LCD_Clear();
        			LCD_SetCursor(0, 0);
        			LCD_WriteString("ERROR ATH20");

        			currentState=ERROR_INIT;
        			break;
        		}
        		if(!BMP_Is_Init()){
					uartSendString((uint8_t*)"No se pudo inicializar el sensor BMP280 \r\n");
					uartSendString((uint8_t*)"Verifique la conexion del hardware\r\n");
					LCD_Clear();
					LCD_SetCursor(0, 0);
					LCD_WriteString("ERROR BMP280");

					currentState=ERROR_INIT;
					break;
				}

        		if (ATH_IsReady())  ath_ready  = true;
        		if (BMP280_IsReady()) bmp_ready = true;

        		if (lastCmd == CMD_TEMP && ath_ready && bmp_ready) {
					currentState = PROCESS_DATA; stateInit = true;
				}
				if (lastCmd == CMD_PRES && bmp_ready) {
					currentState = PROCESS_DATA; stateInit = true;
				}
				if (lastCmd == CMD_HUM && ath_ready) {
					currentState = PROCESS_DATA; stateInit = true;
				}

            break;

        case PROCESS_DATA:

        	if (lastCmd == CMD_TEMP)
			{
				if (ATH_GetData(&t1, &h1)==true && BMP280_GetData(&t2, &p2)==true)
				{
					tempAmbient=(t1+t2)/2.0f; 	//Reduzco el error de medición tomando el promedio de las temperaturas
												//de los dos sensores
					currentState = SHOW_T_P_H;
				}
			}
			if (lastCmd == CMD_PRES)
			{
				if(BMP280_GetData(&t2, &p2)==true)
				{
					sprintf(buffer_show, "Presión ATM: %.2f hPa\r\n", p2/100.0f);
					sprintf(buffer_lcd, "%.2f hPa", p2/100.0f);
					currentState = SHOW_T_P_H;
				}
			}
			if (lastCmd == CMD_HUM)
			{
				if(ATH_GetData(&t1, &h1)==true)
				{
				sprintf(buffer_show, "Humedad Relativa: %.2f %c\r\n", h1, 0x25);
				sprintf(buffer_lcd, "%.2f%c", h1,0x25);
				currentState = SHOW_T_P_H;
				}
			}

            break;

        case SHOW_T_P_H:
                if (lastCmd == CMD_TEMP)
				{
                	sprintf(buffer_show, "Temperatura Ambiente: %.2f ºC \r\n", tempAmbient);
                	uartSendString((uint8_t*)buffer_show);

                	LCD_Clear();
                	sprintf(buffer_lcd, "%.2f%cC%c", tempAmbient, 0xDF,'\0');
                	LCD_SetCursor(0, 0);
                	LCD_WriteString("Temp. Ambiente");
                	LCD_SetCursor(1, 0);
                	LCD_WriteString(buffer_lcd);
				}

                if (lastCmd == CMD_PRES)
				{
					uartSendString((uint8_t*)buffer_show);

					LCD_Clear();
					LCD_SetCursor(0, 0);
					LCD_WriteString("Pres.Atmosferica");
					LCD_SetCursor(1, 0);
					LCD_WriteString(buffer_lcd);
				}
                if (lastCmd == CMD_HUM)
				{
					uartSendString((uint8_t*)buffer_show);
					LCD_Clear();
					LCD_SetCursor(0, 0);
					LCD_WriteString("Humedad Relativa");
					LCD_SetCursor(1, 0);
					LCD_WriteString(buffer_lcd);
				}
				currentState = IDLE;
				break;
        case REBOOT:
        	uartSendString((uint8_t*)"Reiniciando el sistema, aguarde por favor \r\n");
        	LCD_Clear();
			LCD_SetCursor(0, 0);
			LCD_WriteString("REINICIANDO");
        	for (uint8_t i=0;i<5;i++){ //Interactua con el usuario para que obtenga el feedback de los 5 segundos
				uartSendString((uint8_t*)".");
				LCD_SetCursor(1, i);
				LCD_WriteString(".");
				HAL_Delay(1000);
			}

        	uartSendString((uint8_t*)"\r\n \r\n");
        	stateInit=true;
        	MEF_main_init();
        	currentState = INIT;
        	break;
        case ERROR_INIT:
            ATH_Forced_Error();
            BMP_Forced_Error();

            uartSendString((uint8_t*)"Error de hardware detectado\r\n");
            uartSendString((uint8_t*)"Verifique conexiones I2C (SCL/SDA)\r\n");
            uartSendString((uint8_t*)"Escriba el comando RESTART y presione ENTER para reiniciar...\r\n");

            currentState = WAIT_USER;
            stateInit = true;
            break;

        case WAIT_USER:
            if(stateInit){
                stateInit = false;
            }

            cmd = cmdParser_GetCommand();

            if(cmd == CMD_RESTART || cmd != CMD_NONE){
                uartSendString((uint8_t*)"Reintentando inicializacion...\r\n");

                I2C_Force_Restart();
                HAL_Delay(50);

                stateInit = true;
                MEF_main_init();
                currentState = INIT;
            }
            break;
        default:
            currentState = ERROR_INIT;
            break;
    }
}
