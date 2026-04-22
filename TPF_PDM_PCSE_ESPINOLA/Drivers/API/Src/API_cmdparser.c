/*
 * API_cmdparser.c
 *
 *  Created on: Apr 7, 2026
 *      Author: Maximiliano Ariel Espinola
 */


#include <MEF/Inc/MEF_main.h>
#include "API_cmdparser.h"
#include "API_uart.h"
#include "BSP_uart.h"
#include "API_I2C.h"

typedef enum {
	CMD_IDLE,
	CMD_RECEIVING,
	CMD_PROCESS,
	CMD_EXEC,
	CMD_ERROR
} parserMEF_t;

//---------Funciones Privadas-----------------//
static void cmdPrintMenu(void);
static void cmdProcessLine(void); //Procesa el buffer recibido por la UART
static void strToUpper(char *str);//Pasa todo el buffer a MAYUSCULAS para hacerlo caseINsensitive
static uint16_t cmdIndex = 0;
static uint8_t cmdBuffer[CMD_MAX_LINE];
static uint8_t caracterRecibido; //variable para ir guardando los caracteres que recibo
static uint8_t tokenCount = 0;
static char *token;
static char *tokens[CMD_MAX_TOKENS];
static cmd_t currentCmd = CMD_NONE; //Variable para pasar el comando recibido por la UART
static parserMEF_t state;

void cmdParserInit(){
	state = CMD_IDLE; //Inicio la entrada de la MEF en CMD_IDLE
}

void cmdPoll(void)
{

    switch (state)
    {
    case CMD_IDLE:

    	cmdIndex = 0;
    	state = CMD_RECEIVING;
        break;

    case CMD_RECEIVING:

    	if (uartReceiveByte(&caracterRecibido)) {
    		//me fijo si hay algo en el buffer
    		uartSendStringSize(&caracterRecibido, 1); // eco para ver lo que escribo por la consola
    		if (caracterRecibido == '\r' || caracterRecibido == '\n') //detecto final de linea o retorno de carro para procesar el buffer
    		{
    			if (cmdIndex > 0) { // evita líneas vacías, si pongo ENTER y el index esta en 0 no procesa
    				state = CMD_PROCESS;
    		    }
    		}
    		else
    		{
				if (cmdIndex >= CMD_MAX_LINE - 1) //Control de overflow
				{
					state = CMD_ERROR;
				}
				else
				{
					cmdBuffer[cmdIndex++] = caracterRecibido;
				}
			}
    	}
        break;

    case CMD_PROCESS:
    	cmdProcessLine();
    	state=CMD_EXEC;
        break;

    case CMD_EXEC:
    	cmdIndex = 0;
    	state = CMD_RECEIVING;
        break;

    case CMD_ERROR:
    	uartSendString((uint8_t*)"\r\nERROR: ");
    	uartSendString((uint8_t*)"\r\n");
    	cmdIndex = 0;
    	state = CMD_IDLE;
    	break;

    default:
    	state=CMD_IDLE;
    	break;

    }
}

void cmdPrintHelp(){
	uartSendString((uint8_t*)"\r\n");
	uartSendString((uint8_t*)"=====================================\r\n");
	uartSendString((uint8_t*)"               AYDUA\r\n");
	uartSendString((uint8_t*)"=====================================\r\n");
	uartSendString((uint8_t*)"\r\n");
	uartSendString((uint8_t*)"-------------------------------------\r\n");
	uartSendString((uint8_t*)"- Escriba el nombre de los comandos indicados en el menú y luego presione ENTER\r\n");
	uartSendString((uint8_t*)"- No importa si están en mayúscula o minúscula\r\n");
	uartSendString((uint8_t*)"- Si escribió mal un caracter, presione ENTER y vuelva a ecribir el comando\r\n");
	uartSendString((uint8_t*)"\r\n");
}

static void cmdPrintMenu(void){
	uartSendString((uint8_t*)"\r\n");
	uartSendString((uint8_t*)"==================================================\r\n");
	uartSendString((uint8_t*)"            ESTACIÓN METEOROLÓGICA\r\n");
	uartSendString((uint8_t*)"=================================================\r\n");
	uartSendString((uint8_t*)"\r\n Comandos disponibles:\r\n");
	uartSendString((uint8_t*)"--------------------------------------------------\r\n");
	uartSendString((uint8_t*)" TEMP?  -> Ver temperatura actual [°C]\r\n");
	uartSendString((uint8_t*)" PRES?  -> Ver presión [hPa]\r\n");
	uartSendString((uint8_t*)" HUM?   -> Ver humedad relativa [%]\r\n");
	uartSendString((uint8_t*)" HELP?  -> Mostrar ayuda\r\n");
	uartSendString((uint8_t*)" MENU   -> Mostrar este menú\r\n");
	uartSendString((uint8_t*)" REBOOT -> Reinicia el sistema\r\n");
	uartSendString((uint8_t*)" BAUD?  -> Muestra la configuración de la UART\r\n");
	uartSendString((uint8_t*)" I2C?   -> Muestra la configuración del puerto I2C\r\n");
	uartSendString((uint8_t*)"--------------------------------------------------\r\n");
	uartSendString((uint8_t*)"\r\n");
	uartSendString((uint8_t*)"Escriba un comando y presione ENTER: \r\n");
}

static void cmdProcessLine(void)
{
    cmdBuffer[cmdIndex] = '\0'; // pongo '\0' para cerrar el string

    if (cmdBuffer[0] == '#' ||
       (cmdBuffer[0] == '/' && cmdBuffer[1] == '/')) {
        cmdIndex = 0;
        uartSendString((uint8_t*)"Comentario detectado\r\n");
        return;
    }
    strToUpper((char*)cmdBuffer);

    tokenCount = 0;
	token = strtok((char*)cmdBuffer, " "); //Armo los tokens para analizar el comando recibido

	while (token != NULL && tokenCount < CMD_MAX_TOKENS)
	{
		tokens[tokenCount++] = token;
		token = strtok(NULL, " ");
	}

	if (tokenCount == 0) return;

	//Comando para mostrar la temperatura
	if (strcmp(tokens[0], "TEMP?") == 0)
	{
		currentCmd = CMD_TEMP;
		return;
	}
	//Comando para mostrar la presion atm
	if (strcmp(tokens[0], "PRES?") == 0)
	{
		currentCmd = CMD_PRES;
		return;
	}
	// Comando para mostrar la humedad relativa
	if (strcmp(tokens[0], "HUM?") == 0)
	{
		currentCmd = CMD_HUM;
		return;
	}
	//Comando de ayuda
	if (strcmp(tokens[0], "HELP?") == 0)
	{
		currentCmd = CMD_HELP;
		cmdPrintHelp();
		return;
	}
	//Comando para imprimir el menú
	if (strcmp(tokens[0], "MENU") == 0)
	{
		currentCmd = CMD_MENU;
		cmdPrintMenu();
		return;
	}
	//Comando para reiniciar el sistema
	if (strcmp(tokens[0], "REBOOT") == 0)
		{
		currentCmd = CMD_REBOOT;
		return;
		}
	//Comando para reiniciar despues de error de HW
	if (strcmp(tokens[0], "RESTART") == 0)
	{
		currentCmd = CMD_RESTART;
		return;
	}
	//Comando para imprimir información de la configuracion de la UART
	if (strcmp(tokens[0], "BAUD?") == 0)
	{
		bsp_uart_print_cfg();
		currentCmd = CMD_BAUD;
		return;
	}
	//Comando para imprimir información de la configuracion del I2C
	if (strcmp(tokens[0], "I2C?") == 0)
	{
		api_i2c_print_cfg();

		currentCmd = CMD_I2C_CFG;
		return;
	}


	// comando desconocido
	uartSendString((uint8_t*)"ERROR: Comando desconocido\r\n");
	uartSendString((uint8_t*)"Escriba MENU para ver los comandos disponible disponible\r\n");
	cmdIndex = 0;
}

//Funcion para hacer case-INsensitve la recepción del buffer
static void strToUpper(char *str)
{
    while (*str)
    {
        *str = toupper((unsigned char)*str);
        str++;
    }
}

//GETER para obtener el comando recibido por la UART
cmd_t cmdParser_GetCommand(void)
{
    cmd_t cmd = currentCmd;
    currentCmd = CMD_NONE;
    return cmd;
}
