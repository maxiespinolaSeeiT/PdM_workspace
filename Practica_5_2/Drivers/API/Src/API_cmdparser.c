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

static char *tokens[CMD_MAX_TOKENS];
static uint8_t tokenCount = 0;

static cmd_status_t cmdStatus;
static parserMEF_t state;


void cmdParserInit(){


	  if(!uartInit()){ //inicio la UART
		  return 0;
	  }
	  state = CMD_IDLE; //Inicio la entrada de la MEF en CMD_IDLE
}

void cmdPoll(void)
{


    uartReceiveStringSize(&cmdBuffer, 1); // polling de 1 byte

    switch (state)
    {
    case CMD_IDLE:
    	if (cmdBuffer[0]!=0) { //me fijo si hay algo en el buffer
    		uartSendStringSize(&cmdBuffer, 1); //envio lo que tengo en el buffer
    		cmdBuffer[0] = 0; //reseteo el buffer
    	}
    	state=CMD_RECEIVING;
        break;

    case CMD_RECEIVING:
    	state=CMD_PROCESS;
        break;

    case CMD_PROCESS:
    	state=CMD_EXEC;
        break;

    case CMD_EXEC:
    	state = CMD_IDLE;
        break;

    case CMD_ERROR:
    	break;

    default:
    	state=CMD_IDLE;
    	break;

    }
}

void cmdPrintHelp(){
	uartSendString((uint8_t*)"ESTOY EN IMPRIMIR HELP\r\n");
}


