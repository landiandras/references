#include "accelerometer.h"

#include "stm32f4xx.h"
#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_cortex.h"
#include "globalfunctions.h"

#include "stm32f4xx_nucleo_bus.h"
#include "lsm6dsl.h"

#include <stdio.h>

#include "custom_motion_sensors.h"
extern LSM6DSL_Object_t MotionSensor;
CUSTOM_MOTION_SENSOR_Axes_t readaccelero(){

	CUSTOM_MOTION_SENSOR_Axes_t custom_acc_axes = {0};
	CUSTOM_MOTION_SENSOR_GetAxes(CUSTOM_LSM6DSL_0, MOTION_ACCELERO, &custom_acc_axes);
	return custom_acc_axes;

}

