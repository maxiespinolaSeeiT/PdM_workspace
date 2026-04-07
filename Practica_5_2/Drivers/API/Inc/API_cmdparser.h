/*
 * API_cmdparser.h
 *
 *  Created on: Apr 7, 2026
 *      Author: Maximiliano Ariel Espinola
 */

#ifndef API_INC_API_CMDPARSER_H_
#define API_INC_API_CMDPARSER_H_

#define CMD_MAX_LINE 64 //Incluye ´/0'
#define CMD_MAX_TOKENS 3 // COMANDO+ máximo 2 argumentos

typedef enum {
	CMD_OK = 0,
	CMD_ERR_OVERFLOW,
	CMD_ERR_SYNTAX,
	CMD_ERR_UNKNOWN,
	CMD_ERR_ARG
} cmd_status_t;

typedef enum {
	CMD_IDLE,
	CMD_RECEIVING,
	CMD_PROCESS,
	CMD_EXEC,
	CMD_ERROR
} parserMEF_t;
/*----------Funciones Públicas------------------*/

void cmdParserInit(void);
void cmdPoll(void);
void cmdPrintHelp(void);

static void cmdProcessLine(void);

#endif /* API_INC_API_CMDPARSER_H_ */
