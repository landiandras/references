/*
 * uart.c
 *
 *  Created on: May 20, 2024
 *      Author: me
 */
#include "uart.h"
#include "stm32f4xx.h"
#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_cortex.h"
uint8_t lastReceivedUartCommand = 0;
char rxBuffer;
#define TXBUFFERSIZE 255
char txBuffer[TXBUFFERSIZE];


uint8_t counter = 0;
uint8_t start = 0;
uint8_t received = 0;
uint8_t size = 0;
uint8_t message[255]={0};

extern UART_HandleTypeDef huart2;
extern int32_t rotx;
extern int32_t roty;

HAL_StatusTypeDef uartinit(){

	return HAL_UART_Receive_IT(&huart2, (uint8_t*)&rxBuffer, 1);
}

HAL_StatusTypeDef UART_SendString(char *str)
{

	while ((huart2.gState != HAL_UART_STATE_BUSY_RX) && (huart2.gState != HAL_UART_STATE_READY))
	{
	}

	uint16_t length = 0;
    while(*(str+length) && length<TXBUFFERSIZE)
    {
    	*(txBuffer+length) = *(str+length);
    	length++;
    }
	return HAL_UART_Transmit_IT(&huart2, (uint8_t*)txBuffer, length);
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *handle)
{



	if (rxBuffer == 0xAA && start == 0){
		start=1;
		counter = 0;
		  for(int i = 0; i<255; i++){
			  message[i] = 0;
		  }
	}
	else if(start == 1 && counter == 0 && rxBuffer!=0xAA){
		size = rxBuffer;
		counter++;
	}
	else if(start == 1 && counter != 0)
	{
		message[counter-1] = rxBuffer;
		counter++;
		if(counter > size){
			start=0;
			received = 1;
			counter = 0;
		}
	}

	HAL_UART_Receive_IT(handle, (uint8_t*)&rxBuffer, 1);
}
