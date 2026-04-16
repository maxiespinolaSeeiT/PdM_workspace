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

static void cmdProcessLine(void);
static void strToUpper(char *str);

static uint8_t cmdBuffer[CMD_MAX_LINE];
static uint16_t cmdIndex = 0;
static uint8_t caracterRecibido; //variable para ir guardando los caracteres que recibo
static uint32_t baudrateconfig;
static char bufferBaudrate[32]; //bufer para formatear el numero de baudrate


static char *tokens[CMD_MAX_TOKENS];
static uint8_t tokenCount = 0;
static char *token;


static cmd_status_t cmdStatus;
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
					cmdStatus = CMD_ERR_OVERFLOW;
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
    	uartSendString((uint8_t*)cmdStatusToString(cmdStatus));
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
	uartSendString((uint8_t*)"\r\nComandos disponibles (case-INsensitive):\r\n");
	uartSendString((uint8_t*)"HELP -> Muestra esta lista de ayuda\r\n");
	uartSendString((uint8_t*)"LED ON -> Enciende el LD2 de la placa\r\n");
	uartSendString((uint8_t*)"LED OFF -> Apaga el LD2 de la placa\r\n");
	uartSendString((uint8_t*)"LED TOGGLE -> Cambia el estado del LD2 de la placa\r\n");
	uartSendString((uint8_t*)"STATUS -> Muestra el estado del LED\r\n");
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

    //uartSendString((uint8_t*)"LINEA RECIBIDA: ");
    //uartSendString(cmdBuffer); //muestro la linea recibida
    //uartSendString((uint8_t*)"\r\n");



    //Hago los tokens con el buffer recibido

    tokenCount = 0;
	token = strtok((char*)cmdBuffer, " "); //corta el string usando el espacio " "

	while (token != NULL && tokenCount < CMD_MAX_TOKENS) //verifico si hay palabras y que no se pase del limite
	{
		tokens[tokenCount++] = token; //formio el array con los tokens
		token = strtok(NULL, " "); //voy al siguiente token
	}

	if (tokenCount == 0) return;

	// Analizo el token "HELP"
	if (strcmp(tokens[0], "HELP") == 0) //comparo los strings y devuelve 0 si son iguales
	{
		cmdPrintHelp();
		return;
	}

	// Analizo el token "LED"
	if (strcmp(tokens[0], "LED") == 0) //comparo los strings y devuelve 0 si son iguales
	{
		if (tokenCount < 2)
		{
			uartSendString((uint8_t*)"ERROR: ARG\r\n");
			return;
		}

		if (strcmp(tokens[1], "ON") == 0)
		{
			boardLedOn(); //Enciendo el led con la funcion de la biblioteca  API_debounce
			uartSendString((uint8_t*)"LED ON\r\n");
		}
		else if (strcmp(tokens[1], "OFF") == 0)
		{
			boardLedOff();//Apago el led con la funcion de la biblioteca  API_debounce
			uartSendString((uint8_t*)"LED OFF\r\n");
		}
		else if (strcmp(tokens[1], "TOGGLE") == 0)
		{
			HAL_GPIO_TogglePin(LD2_GPIO_Port, LD2_Pin);
			uartSendString((uint8_t*)"LED TOGGLE\r\n");
		}
		else
		{
			uartSendString((uint8_t*)"ERROR: ARG\r\n");
		}
		return;
	}

	// Analizo el token "STATUS"
	if (strcmp(tokens[0], "STATUS") == 0) //comparo los strings y devuelve 0 si son iguales
	{
		GPIO_PinState state = HAL_GPIO_ReadPin(LD2_GPIO_Port, LD2_Pin);

		if (state == GPIO_PIN_SET)
			uartSendString((uint8_t*)"LED is ON\r\n");
		else
			uartSendString((uint8_t*)"LED is OFF\r\n");

		return;
	}


	//Analizo BAUD? para cambiar el baudrate
	if (strcmp(tokens[0], "BAUD?") == 0) //comparo los strings y devuelve 0 si son iguales
	{
		baudrateconfig=consultaBaudrate();
		sprintf(bufferBaudrate, "Baudrate: %lu\r\n", baudrateconfig);
		uartSendString((uint8_t*)bufferBaudrate);
		return;

	}

	// Busco el valor y seteo el baudrate
	if (strncmp(tokens[0], "BAUD=", 5) == 0)
	{
		baudrateconfig = atoi(&tokens[0][5]); // toma lo que viene después de '='

	    if (baudrateconfig < 9600 || baudrateconfig > 921600)
	    {
	        uartSendString((uint8_t*)"ERROR: Baudrate fuera de rango\r\n");
	        return;
	    }
	    // Espero que termine de transmitir lo que tiene en el buffer
	    esperarEnvio();

	    if (uartSetBaudrate(baudrateconfig))
	    {



	    	uartFlush(); // Limpio el buffer
	    	cmdIndex = 0; // limpia parser
	        sprintf(bufferBaudrate, "Nuevo baudrate: %lu\r\n", baudrateconfig);
	        uartSendString((uint8_t*)bufferBaudrate);
	    }
	    else
	    {
	        uartSendString((uint8_t*)"ERROR: No se pudo configurar UART\r\n");
	    }

	    return;
	}

	//Pregunto si manda TEMP?
	if (strcmp(tokens[0], "TEMP?") == 0){
		flag=true;
		return;
	}

	// comando desconocido
	uartSendString((uint8_t*)"ERROR: Comando desconocido\r\n");


	cmdIndex = 0; // reset buffer
}







static void strToUpper(char *str) //Funcion para pasar todo a MAYUSCULA, con esto la hago case-insensitive
{
    while (*str)
    {
        *str = toupper((unsigned char)*str);
        str++;
    }
}

static const char* cmdStatusToString(cmd_status_t status)
{
    switch (status)
    {
        case CMD_OK:            return "OK";
        case CMD_ERR_OVERFLOW:  return "OVERFLOW";
        case CMD_ERR_SYNTAX:    return "SYNTAX";
        case CMD_ERR_UNKNOWN:   return "UNKNOWN";
        case CMD_ERR_ARG:       return "ARG";
        default:                return "INVALID";
    }
}
