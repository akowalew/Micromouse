/*
 * TopLeds.c
 *
 *  Created on: 10 mar 2016
 *      Author: akowalew
 */

#include "../Communication/topUtils.h"

#include <stdint.h>
#include <stdbool.h>

#include "inc/hw_types.h"
#include "inc/hw_memmap.h"
#include "inc/tm4c123gh6pm.h"
#include "inc/hw_gpio.h"
#include "inc/hw_pwm.h"
#include "inc/hw_timer.h"

#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"
#include "driverlib/interrupt.h"
#include "driverlib/pin_map.h"
#include "driverlib/pwm.h"
#include "driverlib/timer.h"

struct {
	volatile bool led1State ;
	volatile bool led2State ;
	volatile bool led3State ;

	volatile bool buzState ;

	volatile bool led2Periodical ;
	volatile bool buzPeriodical ;

	void (*pFn1)() ;
	void (*pFn2)() ;
} topStruct ;


// PA6, PA7, PD2 LEDS
inline void ledsTurnOn1() {
	GPIOPinWrite(GPIO_PORTD_BASE, LEDS_WHITE_PIN, LEDS_WHITE_PIN) ;
	topStruct.led1State = true ;
}

inline void ledsTurnOn3() {
	GPIOPinWrite(GPIO_PORTA_BASE, LEDS_RED_PIN, LEDS_RED_PIN) ;
	topStruct.led3State = true ;
}

inline void ledsTurnOn2() {
	GPIOPinWrite(GPIO_PORTA_BASE, LEDS_YELLOW_PIN, LEDS_YELLOW_PIN) ;
	topStruct.led2State = true ;
}

inline void ledsTurnOff1() {
	GPIOPinWrite(GPIO_PORTD_BASE, LEDS_WHITE_PIN, 0) ;
	topStruct.led1State = false ;
}

inline void ledsTurnOff3() {
	GPIOPinWrite(GPIO_PORTA_BASE, LEDS_RED_PIN, 0) ;
	topStruct.led3State = false ;
}

inline void ledsTurnOff2() {
	GPIOPinWrite(GPIO_PORTA_BASE, LEDS_YELLOW_PIN, 0) ;
	topStruct.led2State = false ;
}

inline void buzTurnOn() {
	GPIOPinWrite(BUZ_BASE, BUZ_PIN, BUZ_PIN) ;
	topStruct.buzState = true ;
}

inline void buzTurnOff() {
	GPIOPinWrite(BUZ_BASE, BUZ_PIN, 0) ;
	topStruct.buzState = false ;
}

inline void buzPeriodicalOn() {
	topStruct.buzPeriodical = true ;
}

inline void buzPeriodicalOff() {
	topStruct.buzPeriodical = false ;
}

inline void ledsPeriodical2On() {
	topStruct.led2Periodical = true ;
}

inline void ledsPeriodical2Off() {
	topStruct.led2Periodical = false ;
}


void utilsTimInt() {
	TimerIntClear(TOPUTILS_TIMER_BASE, TIMER_TIMA_TIMEOUT) ;

	if(topStruct.buzPeriodical) {
		if(topStruct.buzState)
			buzTurnOff() ;
		else
			buzTurnOn() ;
	}

	if(topStruct.led2Periodical){
		if(topStruct.led2State)
			ledsTurnOff2() ;
		else
			ledsTurnOn2() ;
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
	SysCtlPeripheralEnable(BTN1_PERIPH) ;
	SysCtlPeripheralEnable(BTN2_PERIPH) ;
	SysCtlDelay(3) ;

	GPIOPinTypeGPIOInput(BTN1_BASE, BTN1_PIN) ;
	GPIOPinTypeGPIOInput(BTN2_BASE, BTN2_PIN) ;
	GPIOPadConfigSet(BTN1_BASE, BTN1_PIN, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPD) ;
	GPIOPadConfigSet(BTN2_BASE, BTN2_PIN, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPD) ;

	//Timer
	SysCtlPeripheralEnable(TOPUTILS_TIMER_PERIPH) ;
	SysCtlDelay(3) ;

	TimerConfigure(TOPUTILS_TIMER_BASE, TIMER_CFG_PERIODIC) ;
	TimerLoadSet(TOPUTILS_TIMER_BASE, TIMER_A, TOPUTILS_TIMER_PERIOD) ;
	TimerIntRegister(TOPUTILS_TIMER_BASE, TIMER_A, utilsTimInt) ;
	TimerIntEnable(TOPUTILS_TIMER_BASE, TIMER_TIMA_TIMEOUT) ;
	TimerIntClear(TOPUTILS_TIMER_BASE, TIMER_TIMA_TIMEOUT) ;
	TimerEnable(TOPUTILS_TIMER_BASE, TIMER_BOTH) ;
}

inline int32_t btn1isPushed() {
	return GPIOPinRead(BTN1_BASE, BTN1_PIN) ;
}

inline int32_t btn2isPushed() {
	return GPIOPinRead(BTN2_BASE, BTN2_PIN) ;
}

void btn1Interrupt() {
	GPIOIntClear(BTN1_BASE, BTN1_INT_PIN) ;
	topStruct.pFn1() ;
}

void btn2Interrupt() {
	GPIOIntClear(BTN2_BASE, BTN2_INT_PIN) ;
	topStruct.pFn2() ;
}

void btn1IntRegister(void (*pFn1)()) {
	GPIOIntRegister(BTN1_BASE, btn1Interrupt) ;
	GPIOIntTypeSet(BTN1_BASE, BTN1_PIN, GPIO_RISING_EDGE) ;
	GPIOIntEnable(BTN1_BASE, BTN1_INT_PIN) ;
	GPIOIntClear(BTN1_BASE, BTN1_INT_PIN) ;
	topStruct.pFn1 = pFn1 ;
}

void btn2IntRegister(void (*pFn2)()) {
	GPIOIntRegister(BTN2_BASE, btn2Interrupt) ;
	GPIOIntTypeSet(BTN2_BASE, BTN2_PIN, GPIO_RISING_EDGE) ;
	GPIOIntEnable(BTN2_BASE, BTN2_INT_PIN) ;
	GPIOIntClear(BTN2_BASE, BTN2_INT_PIN) ;
	topStruct.pFn2 = pFn2 ;
}
