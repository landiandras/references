/*
 * lcd.h
 *
 *  Created on: Apr 26, 2024
 *      Author: danny
 */

#ifndef INC_LCD_H_
#define INC_LCD_H_



#endif /* INC_LCD_H_ */

#define LCD_ON				0x3F
#define LCD_OFF				0x3E
#define LCD_SET_ADD			0x40
#define LCD_DISP_START		0xC0
#define LCD_SET_PAGE		0xB8

void LCDInit(void);

void LCDEN(int newState);

void LCDWrite(char cs_s,char d_i,char g_data);

void LCDClear(void);

void LCDWriteBlock(char m_data,char cX,char cY);
