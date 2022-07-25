/*
 * commands.h
 *
 *  Created on: 24/07/2022
 *      Author: Said Guouihaj
 */

#ifndef INC_COMMANDS_H_
#define INC_COMMANDS_H_

/*
 * @brief Structure definition of the Command type
 */
typedef void (*operation)(void *);
typedef struct Command {
	char * command;				/* This is command name */
	operation callback;			/* This is the pointer to function that executes the command */

}CommandTypeDef;

/* Here you define the parameters structure if the function needs parameters to be passed */

typedef enum {
	GREEN,
	RED
}LEDColor;

typedef enum {
	OFF = 0,
	ON
}LEDState;

typedef struct LED {
	LEDColor color;
	LEDState state;
}LEDTypeDef;

/* End function parameters data structures */

/* Special characters definitions corresponding to the encoding */
#define NEWLINE	'\n'
#define BACKSPACE	'\177'
// You can add more special characters here

#define MAX_CHARS	100			/* Define the maximum number of characters to be received */
#define NUM_COMMANDS	6		/* Define the number of commands you will create */

extern UART_HandleTypeDef huart1;

#ifdef _INCLUDE_VARIABLES
uint8_t lastUsedPointer;
CommandTypeDef commands[NUM_COMMANDS];			/* The array where commands are stored */

/* Declare every function parameters structure variable here */
LEDTypeDef led;
/* End declare function parameters variables */

uint8_t received[MAX_CHARS];		/* This is the received serial data buffer */
#endif

/* function protoypes for program control */
void initializeTerminal();					/* This is executed after reset to initialize the terminal */
void initializeCommands();					/* This is to add your command to the list of commands */
void receiveSerialCommand();				/* This to handle terminal visualization and prepare command for handling */
void handleSerialCommand();					/* This function is responsible of parsing the command and decide which handler to execute */
void sendCommandResponse(char * command);
void handleStdout(char *);					/* This is responsible for sending text in serial */
/* End function prototypes for program control */

/* Here you add function prototypes for your commands */
void getDevInfos(void *);
void toggleLed(void *);
void resetDevice(void *);
/* End commands function prototypes */

#endif /* INC_COMMANDS_H_ */
