/*
 * Motors.c
 *
 *  Created on: 15 mar 2016
 *      Author: akowalew
 */
#include "Motors.h"

#include <stdint.h>
#include <stdbool.h>

#include "inc/hw_types.h"
#include "inc/hw_memmap.h"
#include "inc/tm4c123gh6pm.h"
#include "inc/hw_gpio.h"
#include "inc/hw_pwm.h"

#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"
#include "driverlib/interrupt.h"
#include "driverlib/pin_map.h"
#include "driverlib/pwm.h"

#include "Encoders.h"
#include "MotorsController.h"
#include "MotorsDrivers.h"

void motorsInit() {
	motorsDriversInit();
	motorsSetupML(SOFT_STOP) ;
	motorsSetupMR(SOFT_STOP) ;
	encInit() ;
	motCntrlInit() ;
}

void motorsEnable() {

	// initial gpio status
	motorsDriversEnable() ;
	encEnable() ;
	motCntrlEnable();
}

void motorsDisable() {
	motorsDriversDisable() ;
	motCntrlDisable() ;
	encDisable() ;
}
