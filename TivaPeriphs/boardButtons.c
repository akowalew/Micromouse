/*
 * boardButtons.c
 *
 *  Created on: 26 lut 2016
 *      Author: dicker
 */

#include <stdint.h>
#include <stdbool.h>

#include "inc/hw_types.h"
#include "inc/hw_memmap.h"
#include "inc/tm4c123gh6pm.h"
#include "inc/hw_gpio.h"
#include "inc/hw_timer.h"

#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"
#include "driverlib/timer.h"
#include "driverlib/pin_map.h"

#include "boardButtons.h"

void boardButtonsInit() {
	SysCtlPeripheralEnable(BTN_PERIPH) ;
	SysCtlDelay(3) ;

	// odblokowanie PF0
	HWREG(BTN_PORT + GPIO_O_LOCK) = GPIO_LOCK_KEY;
	  HWREG(BTN_PORT + GPIO_O_CR)   |= 0x01;

	GPIOPinTypeGPIOInput(BTN_PORT, BTN_LEFT | BTN_RIGHT ) ;
	GPIOPadConfigSet(
			BTN_PORT,
			BTN_LEFT | BTN_RIGHT,
			GPIO_STRENGTH_2MA,
			GPIO_PIN_TYPE_STD_WPU) ;
}

bool isLeftBtnPressed() {
	return !(GPIOPinRead(BTN_PORT, BTN_LEFT) & BTN_LEFT) ;
}

bool isRightBtnPressed() {
	return !(GPIOPinRead(BTN_PORT, BTN_RIGHT) & BTN_RIGHT) ;
}
