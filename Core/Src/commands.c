/*
 * commands.c
 *
 *  Created on: 24/07/2022
 *      Author: Said Guouihaj
 */
#include <stdlib.h>
#include <string.h>
#include "stm32f4xx_hal.h"


#define _INCLUDE_VARIABLES
#include "commands.h"
/*
 * @brief	Terminal initialization Function
 * @param	None
 * @retval	None
 */
void initializeTerminal(){
	handleStdout("\rSerial communication is established\r\n");
}

/*
 * @brief	Commands initialization Function
 * @param	None
 * @retval	None
 */
void initializeCommands(){
	commands[0].command = "device";
	commands[0].callback = &getDevInfos;
	commands[1].command = "turn on led green";
	commands[1].callback = &toggleLed;
	commands[2].command = "turn off led green";
	commands[2].callback = &toggleLed;
	commands[3].command = "turn on led red";
	commands[3].callback = &toggleLed;
	commands[4].command = "turn off led red";
	commands[4].callback = &toggleLed;
	commands[5].command = "reboot";
	commands[5].callback = &resetDevice;
}

/*
 * @brief	Receiving and interacting with serial data received
 * @param	None
 * @retval	None
 */
void receiveSerialCommand(){
	int i = 0;
	while (1){
		while(HAL_UART_Receive(&huart1, received+i, 1, 10));
		if (received[i]=='\r'){
			for (uint8_t j=i;j<100;j++)
				received[j] = '\0';
			break;
		}
		else if (received[i]==BACKSPACE){
			if (i>0){
				HAL_UART_Transmit(&huart1, (uint8_t *)received+i, 1, 100);
				received[i] = '\0';
				received[i-1] = '\0';
				i--;
			}
			else {
				i = 0;
			}

		}
		else if (received[i]>31 && received[i]<127){
			HAL_UART_Transmit(&huart1, (uint8_t *)received+i, 1, 100);
			i++;
		}
	}
}
/*
 * @brief	Handle serial command and send the response
 * @param	None
 * @retval	None
 */
void handleSerialCommand(){
	sendCommandResponse((char *)received);
}

void sendCommandResponse(char * command){
	void * params;
	if (!strcmp(command, commands[0].command))
		(*commands[0].callback)(NULL);
	else if (!strcmp(command, commands[1].command)){
		led.color = GREEN;
		led.state = ON;
		params = &led;
		(*commands[1].callback)(params);
	}
	else if (!strcmp(command, commands[2].command)){
		led.color = GREEN;
		led.state = OFF;
		params = &led;
		(*commands[2].callback)(params);
	}
	else if (!strcmp(command, commands[3].command)){
		led.color = RED;
		led.state = ON;
		params = &led;
		(*commands[3].callback)(params);
	}
	else if (!strcmp(command, commands[4].command)){
		led.color = RED;
		led.state = OFF;
		params = &led;
		(*commands[4].callback)(params);
	}
	else if (!strcmp(command, commands[5].command)){
		(*commands[5].callback)(NULL);
	}
	else
		handleStdout("\r\nOK\r\n");
}

/*
 * @brief	Send text in serial
 * @param	out_text : the text to be sent
 * @retval	None
 */
void handleStdout(char * outText){
	size_t outSize = strlen(outText);
	HAL_UART_Transmit(&huart1, (uint8_t *)outText, (uint8_t)outSize, 100);
	HAL_UART_Transmit(&huart1, (uint8_t *)"$ ", 2, 100);
}

/* Here you define your command functions*/

void getDevInfos(void * ptr){
	handleStdout("\r\nstm32\r\n");
}

void toggleLed(void * ledConf){
	LEDTypeDef ledtemp = *(LEDTypeDef *)ledConf;
	if (ledtemp.color == GREEN){
		HAL_GPIO_WritePin(GPIOG,  GPIO_PIN_13, ledtemp.state);
		if (ledtemp.state == ON)
			handleStdout("\r\nLED Green is on\r\n");
		else
			handleStdout("\r\nLED Green is off\r\n");
	}
	else if (ledtemp.color == RED){
		HAL_GPIO_WritePin(GPIOG,  GPIO_PIN_14, ledtemp.state);
		if (ledtemp.state == ON)
			handleStdout("\r\nLED Red is on\r\n");
		else
			handleStdout("\r\nLED Red is off\r\n");
	}
}

void resetDevice(void * ptr){
	handleStdout("\r\nRebooting...\r\n");
	NVIC_SystemReset();
}

/* End define command functions */
