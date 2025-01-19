/*
 * lcd.c
 *
 *  Created on: Apr 26, 2024
 *      Author: me
 */
#include "stm32f4xx_hal.h"
#include "lcd.h"
#include "stm32f4xx_hal_cortex.h"
#include "globalfunctions.h"


void LCDInit(void)
{
	// Minden alapállapotba
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_RESET);	// GLCD_RW = 0
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5, GPIO_PIN_SET);		// GLCD_E = 1
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_2, GPIO_PIN_RESET);	// GLCD_RESET = 0
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, GPIO_PIN_RESET);	// #GLCDEN = 0


	TIM4->CCR1 = 70;										//PWM 70%


	// Init GLCD functions
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5, GPIO_PIN_RESET);	// GLCD_E = 0
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_2, GPIO_PIN_SET);	// GLCD_RESET = 1

	LCDWrite(3,0,0x3F); 	//GLCD bekapcsolása
	LCDWrite(3,0,0x40);
	LCDWrite(3,0,0xB8);
	LCDWrite(3,0,0xC0);	//GLCD Start Line
	LCDClear();			//Kijelzõ törlése
}


void LCDEN(int newState)
{
	if(newState)
	{
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, GPIO_PIN_RESET);
	}
	else
	{
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, GPIO_PIN_SET);
	}
}


/** Beírja a g_data értéket a kiválasztott kijelzõvezérlõbe (cs_s->CS1, CS2) utasítás/adat paraméternek megfelelõen.
* Általánosan felhasználható 8bit (adat/utasítás) beírására a kijelzõ vezérlõjébe.
* 	cs_s, 1 = CS1, 2 = CS2, 3 = CS1&CS2
* 	d_i, 0 = instruction, 1 = data
*/
void LCDWrite(char cs_s,char d_i,char g_data)
{
	uint16_t data = 0x0000;
	switch(cs_s)
	{
	case 1:
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_7, GPIO_PIN_RESET);
		break;
	case 2:
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_8, GPIO_PIN_RESET);
		break;
	case 3:
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_7, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_8, GPIO_PIN_RESET);
		break;
	}
	switch(d_i)
	{
	case 0:
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4, GPIO_PIN_RESET);
		break;
	case 1:
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4, GPIO_PIN_SET);
		break;
	}

	data = GPIOC->IDR;
	data &= 0xFF00;
	data |= g_data;
	GPIOC->ODR = data;

	Sys_DelayUs(1);
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5, GPIO_PIN_SET);
	Sys_DelayUs(1);
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_7, GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_8, GPIO_PIN_SET);
}


/** Kijelző törlése. */
void LCDClear(void)
{
	char x,y;
	for(x=0;x<8;x++)
	{
		LCDWrite(3,0,0x40);
		LCDWrite(3,0,(0xB8|x));
		for(y=0;y<64;y++)
		{
			LCDWrite(3,1,0x00);
		}
	}

}

/** A kijelzõ adott sor-oszlop metszetének beállítása az m_data értéknek megfelelõen.
 * 		m_data: adott metszet rajzolata hexába kódolva
 * 		cX: sor (0-7)
 * 		cY: oszlop (0-127) */
void LCDWriteBlock(char m_data,char cX,char cY)
{
	char chip=1;
	if(cY>=64)
	{
		chip=2;
		cY-=64;
	}
	LCDWrite(chip,0,(0x40|cY));
	LCDWrite(chip,0,(0xB8|cX));
	LCDWrite(chip,1,m_data);
}



