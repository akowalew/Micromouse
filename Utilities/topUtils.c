/*
 * TopLeds.c
 *
 *  Created on: 10 mar 2016
 *      Author: akowalew
 */

#include "topUtils.h"

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

// PA6, PA7, PD2 LEDS

void ledsTurnOn(uint8_t whichLed) {
	switch(whichLed) {
		case LEDS_WHITE:
			GPIOPinWrite(GPIO_PORTD_BASE, LEDS_WHITE_PIN, LEDS_WHITE_PIN) ;
			break ;
		case LEDS_RED :
			GPIOPinWrite(GPIO_PORTA_BASE, LEDS_RED_PIN, LEDS_RED_PIN) ;
			break ;
		case LEDS_YELLOW :
			GPIOPinWrite(GPIO_PORTA_BASE, LEDS_YELLOW_PIN, LEDS_YELLOW_PIN) ;
			break ;
	}
}

void ledsTurnOff(uint8_t whichLed) {
	switch(whichLed) {
		case LEDS_WHITE:
			GPIOPinWrite(GPIO_PORTD_BASE, LEDS_WHITE_PIN, 0) ;
			break ;
		case LEDS_RED :
			GPIOPinWrite(GPIO_PORTA_BASE, LEDS_RED_PIN, 0) ;
			break ;
		case LEDS_YELLOW :
			GPIOPinWrite(GPIO_PORTA_BASE, LEDS_YELLOW_PIN, 0) ;
			break ;
	}
}

void utilsInit() {

	// LEDS
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA) ;
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD) ;
	SysCtlDelay(3) ;

	GPIOPinTypeGPIOOutput(GPIO_PORTA_BASE, GPIO_PIN_6 | GPIO_PIN_7) ;
	GPIOPinTypeGPIOOutput(GPIO_PORTD_BASE, GPIO_PIN_2 | GPIO_PIN_0) ;

	// BUZZER
	SysCtlPeripheralEnable(BUZ_PERIPH) ;
	SysCtlDelay(3) ;

	GPIOPinTypeGPIOOutput(BUZ_BASE, BUZ_PIN) ;

	// BUTTONS
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOC) ;
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF) ;
	SysCtlDelay(3) ;

	GPIOPinTypeGPIOInput(GPIO_PORTC_BASE, GPIO_PIN_7) ;
	GPIOPinTypeGPIOInput(GPIO_PORTF_BASE, GPIO_PIN_4) ;
	GPIOPadConfigSet(GPIO_PORTC_BASE, GPIO_PIN_7, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPD) ;
	GPIOPadConfigSet(GPIO_PORTF_BASE, GPIO_PIN_4, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPD) ;
}

inline void buzTurnOn() {
	GPIOPinWrite(BUZ_BASE, BUZ_PIN, BUZ_PIN) ;
}

inline void buzTurnOff() {
	GPIOPinWrite(BUZ_BASE, BUZ_PIN, 0) ;
}

void buzSequence(uint32_t duration, uint32_t repeatCycle) {
	// TODO : maybe task scheduler?
}

int32_t btn1isPushed() {
	return GPIOPinRead(GPIO_PORTC_BASE, GPIO_PIN_7) ;
}
int32_t btn2isPushed() {
	return GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_4) ;
}
