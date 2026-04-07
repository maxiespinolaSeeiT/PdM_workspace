/*
 * API_cmdparser.c
 *
 *  Created on: Apr 7, 2026
 *      Author: Maximiliano Ariel Espinola
 */


#include "API_cmdparser.h"
#include "API_uart.h"

static uint8_t cmdBuffer[CMD_MAX_LINE];
static uint16_t cmdIndex = 0;
static uint8_t caracterRecibido; //variable para ir guardando los caracteres que recibo

static cmd_status_t cmdStatus;
static parserMEF_t state;


void cmdParserInit(){
	uartInit(); //inicio la UART
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

    	if (uartReceiveByte(&caracterRecibido)) { //me fijo si hay algo en el buffer
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
    	uartSendString((uint8_t*)"\r\n ERROR: OVERFLOW\r\n");
    	cmdIndex = 0;
    	state = CMD_IDLE;
    	break;

    default:
    	state=CMD_IDLE;
    	break;

    }
}

void cmdPrintHelp(){
	uartSendString((uint8_t*)"ESTOY EN IMPRIMIR HELP\r\n");
}

void cmdProcessLine(void)
{
    cmdBuffer[cmdIndex] = '\0'; // cerrar string

    // ignorar comentarios
    if (cmdBuffer[0] == '#' ||
       (cmdBuffer[0] == '/' && cmdBuffer[1] == '/')) {
        cmdIndex = 0;
        return;
    }

    uartSendString((uint8_t*)"LINEA RECIBIDA: ");
    uartSendString(cmdBuffer); //muestro la linea recibida
    uartSendString((uint8_t*)"\r\n");

    cmdIndex = 0; // reset buffer
}

