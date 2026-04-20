/*
 * API_cmdparser.h
 *
 *  Created on: Apr 7, 2026
 *      Author: Maximiliano Ariel Espinola
 */

#include <string.h>
#include <ctype.h>
#include <stdlib.h>



#ifndef API_INC_API_CMDPARSER_H_
#define API_INC_API_CMDPARSER_H_

#define CMD_MAX_LINE 64 //Incluye ´/0'
#define CMD_MAX_TOKENS 3 // COMANDO+ máximo 2 argumentos

//Lista de comandos
typedef enum {
    CMD_NONE = 0,
    CMD_TEMP,
    CMD_PRES,
    CMD_HUM,
    CMD_HELP,
    CMD_MENU,
	CMD_REBOOT,
	CMD_RESTART,
	CMD_BAUD
} cmd_t;

//--------Public functions-----------------//
void cmdParserInit(void); //Inicial el cmdParser
void cmdPoll(void); //Actualiza la MEF del cmdParser
void cmdPrintHelp(void); //Imprime por UART la ayuda para manejar el sistema
cmd_t cmdParser_GetCommand(void); //Getter del último comando recibido por UART

#endif /* API_INC_API_CMDPARSER_H_ */
