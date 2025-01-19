/**
*****************************************************************************
** Kommunikációs mérés - globalfunctions.h
** Globális funkciók
*****************************************************************************
*/
#pragma once
#ifndef _GLOBALFUNCTIONS_H__
#define _GLOBALFUNCTIONS_H__

extern volatile unsigned long repeated_log_delay;

void Sys_DelayUs(int us);
void Sys_DelayMs(int ms);
#endif
