/**
*****************************************************************************
** Kommunikációs mérés - globalfunctions.c
** Egyéb, globális függvények
*****************************************************************************
*/

#include "stm32f4xx.h"
#include "stm32f4xx_hal.h"
#include "globalfunctions.h"
#include "uart.h"
#include "custom_motion_sensors.h"
#include "accelerometer.h"


/* ----------------- Publikus globális változók ----------------- */





/* ----------------- Belso globális változók ----------------- */

// A timer us-onként csökkenti, ha nagyobb, mint 0.
volatile int sys_delay = 0;
volatile unsigned long repeated_log_delay = 0;


/* ----------------- Publikus függvények ----------------- */

/** Globális funkciók inicializálása. */


/** us számú mikroszekundum várakozás (blokkolva). */
void Sys_DelayUs(int us)
{
	sys_delay = us;
	while(sys_delay);
}

/** ms számú milliszekundum várakozás (blokkolva). */
void Sys_DelayMs(int ms)
{
	sys_delay = ms*1000;
	while(sys_delay);
}

/* ----------------- Megszakításkezelő és callback függvények ----------------- */

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *handle)
{

	UNUSED(handle);

	if (sys_delay>0)
	{
		sys_delay--;
	}

	if (repeated_log_delay>0)
	{
		repeated_log_delay--;
	}



}

