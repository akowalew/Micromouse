/*
 * BoardLed.c
 *
 *  Created on: 30 sty 2016
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

#include "BoardLed.h"

#define ALL_LEDS	(RED_LED | BLUE_LED | GREEN_LED)

struct {
	uint32_t digitalOut ;
} ledStruct ;

void boardLedInit() {
	SysCtlPeripheralEnable(LED_PERIPH) ;
	SysCtlDelay(3) ;
	boardLedEnable(ALL_LEDS) ;
	ledStruct.digitalOut = 0 ;
}

inline void boardLedEnable(uint32_t whichLeds) {
	GPIOPinTypeGPIOOutput(LED_BASE, whichLeds) ;
}

inline void boardLedDisable(uint32_t whichLeds) {
	GPIOPinTypeGPIOInput(LED_BASE, whichLeds) ;
}

void boardLedSet(uint32_t combination) {
	ledStruct.digitalOut |= combination ;
	GPIOPinWrite(LED_BASE, ALL_LEDS, ledStruct.digitalOut) ;
}

void boardLedClear(uint32_t combination) {
	ledStruct.digitalOut &= ~(combination) ;
	GPIOPinWrite(LED_BASE, ALL_LEDS, ledStruct.digitalOut) ;
}
