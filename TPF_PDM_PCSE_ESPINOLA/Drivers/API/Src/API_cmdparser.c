/*
 * API_cmdparser.c
 *
 *  Created on: Apr 7, 2026
 *      Author: Maximiliano Ariel Espinola
 */


#include "API_cmdparser.h"
#include "API_uart.h"
#include "MEF_main.h"

typedef enum {
	CMD_IDLE,
	CMD_RECEIVING,
	CMD_PROCESS,
	CMD_EXEC,
	CMD_ERROR
} parserMEF_t;

extern bool_t flag;
static cmd_t currentCmd = CMD_NONE; //Variable para enviar el estado actual recibido por la UART

static void cmdProcessLine(void);
static void strToUpper(char *str);

static uint8_t cmdBuffer[CMD_MAX_LINE];
static uint16_t cmdIndex = 0;
static uint8_t caracterRecibido; //variable para ir guardando los caracteres que recibo
static char *tokens[CMD_MAX_TOKENS];
static uint8_t tokenCount = 0;
static char *token;



static parserMEF_t state;


void cmdParserInit(){
	//uartInit(); //inicio la UART
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
    		uartSendStringSize(&caracterRecibido, 1); // eco para ver lo que escribo
    		if (caracterRecibido == '\r' || caracterRecibido == '\n') //me fijo si no es un final de linea
    		{
    			if (cmdIndex > 0) { // evita líneas vacías, si pongo ENTER y el index esta en 0 no procesa
    				state = CMD_PROCESS;
    		    }
    		}
    		else
    		{
				if (cmdIndex >= CMD_MAX_LINE - 1) //me fijo si no hay overflow
				{
					state = CMD_ERROR;
				}
				else
				{
					cmdBuffer[cmdIndex++] = caracterRecibido; //voy armando el buffer con cada caracter
				}
			}
    	}
        break;

    case CMD_PROCESS:
    	cmdProcessLine(); //proceso la linea
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
	//uartSendString((uint8_t*)"ESTOY EN IMPRIMIR HELP\r\n");
	uartSendString((uint8_t*)"\r\nMENU AYUDA\r\n");
	uartSendString((uint8_t*)"- Escriba el nombre de los comandos de la lista\r\n");
	uartSendString((uint8_t*)"- No importa si están en maypuscula o minúscula\r\n");
	uartSendString((uint8_t*)"- Si escribió mal un caracter, presione ENTER y vuelva a ecribir el comando\r\n");
	uartSendString((uint8_t*)"\r\n");
}

void cmdPrintMenu(){

	uartSendString((uint8_t*)"\r\n");
	uartSendString((uint8_t*)"=====================================\r\n");
	uartSendString((uint8_t*)"      ESTACIÓN METEOROLÓGICA\r\n");
	uartSendString((uint8_t*)"=====================================\r\n");

	uartSendString((uint8_t*)"\r\n Comandos disponibles:\r\n");
	uartSendString((uint8_t*)"-------------------------------------\r\n");

	uartSendString((uint8_t*)" TEMP?  -> Ver temperatura actual [°C]\r\n");
	uartSendString((uint8_t*)" PRES?  -> Ver presión [hPa]\r\n");
	uartSendString((uint8_t*)" HUM?   -> Ver humedad relativa [%]\r\n");
	uartSendString((uint8_t*)" HELP?  -> Mostrar ayuda\r\n");
	uartSendString((uint8_t*)" MENU   -> Mostrar este menú\r\n");

	uartSendString((uint8_t*)"-------------------------------------\r\n");
	uartSendString((uint8_t*)"\r\n");
}






static void cmdProcessLine(void)
{
    cmdBuffer[cmdIndex] = '\0'; // pongo '\0' para cerrar el string
    //saltea espacios iniciales

    // ignora los comentarios
    if (cmdBuffer[0] == '#' ||
       (cmdBuffer[0] == '/' && cmdBuffer[1] == '/')) {
        cmdIndex = 0;
        uartSendString((uint8_t*)"Comentario detectado\r\n");
        return;
    }

    //paso todo a mayuscula
    strToUpper((char*)cmdBuffer);

    //Hago los tokens con el buffer recibido

    tokenCount = 0;
	token = strtok((char*)cmdBuffer, " "); //corta el string usando el espacio " "

	while (token != NULL && tokenCount < CMD_MAX_TOKENS) //verifico si hay palabras y que no se pase del limite
	{
		tokens[tokenCount++] = token; //formio el array con los tokens
		token = strtok(NULL, " "); //voy al siguiente token
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


	// comando desconocido
	uartSendString((uint8_t*)"ERROR: Comando desconocido\r\n");
	uartSendString((uint8_t*)"Escriba MENU para ver los comandos disponible disponible\r\n");


	cmdIndex = 0; // reset buffer
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
