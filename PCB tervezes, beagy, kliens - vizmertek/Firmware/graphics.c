/*
 * graphics.c
 *
 *  Created on: May 6, 2024
 *      Author: me
 */
#include "stm32f4xx.h"
#include "stm32f4xx_hal.h"
#include "graphics.h"
#include "math.h"
#include "lcd.h"

extern uint64_t screen[128];

void createcircle(int16_t x, int16_t y){
	if(x >121) x = 121;
	if(x < 6) x = 6;
	if(y>57) y = 57;
	if(y<6) y = 6;
	for(int i = 0; i<128; i++){
		screen[i] = 0;
	}
	uint64_t temp = 0x000000000000001F;
	temp = temp<<(y-2);
	screen[x+6] = temp;
	temp = 0x00000000000001C7;
	temp = temp <<(y-4);
	screen[x+5] = temp;
	temp = 0x0000000000000603;
	temp = temp <<(y-5);
	screen[x+4] = temp;
	temp = 0x0000000000000401;
	temp = temp <<(y-5);
	screen[x+3] = temp;
	temp = 0x0000000000001803;
	temp = temp <<(y-6);
	screen[x+2] = temp;
	temp = 0x0000000000001001;
	temp = temp <<(y-6);
	screen[x+1] = temp;
	temp = 0x0000000000001001;
	temp = temp <<(y-6);
	screen[x] = temp;
	temp = 0x0000000000001001;
	temp = temp <<(y-6);
	screen[x-1] = temp;
	temp = 0x0000000000001803;
	temp = temp <<(y-6);
	screen[x-2] = temp;
	temp = 0x0000000000000401;
	temp = temp <<(y-5);
	screen[x-3] = temp;
	temp = 0x0000000000000603;
	temp = temp <<(y-5);
	screen[x-4] = temp;
	temp = 0x00000000000001C7;
	temp = temp <<(y-4);
	screen[x-5] = temp;
	temp = 0x000000000000001F;
	temp = temp<<(y-2);
	screen[x-6] = temp;
}

void createcentrallines(){
	screen[63]=0xFFFFFFFFFFFFFFFF;
	uint64_t temp = 0x0000000100000000;
	for(int i =0; i<128; i++){
		screen[i]|=temp;
	}
}


void drawrectangle(char xpos, char ypos, char width, char height){
	char blocks = 0;
	char data = 0xFF;


	data = data << xpos%8;
	LCDWriteBlock(data, xpos/8, ypos);


	blocks = (height - ((xpos%8) + ((xpos+height)%8))) / 8;



	for(int i=1; i<=blocks; i++){
		LCDWriteBlock(0xFF, xpos/8+i, ypos);

	}


}

void draw(){
	for(int i = 0; i<128; i++){
		for (int j = 0; j<8; j++){
			LCDWriteBlock((char)(screen[i]>>(j*8)),j,i);
		}
	}
}

