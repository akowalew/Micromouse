/*
 * MyTimer.c
 *
 *  Created on: 30 sty 2016
 *      Author: dicker
 */

#include <stdint.h>
#include <stdbool.h>

#include "inc/hw_memmap.h"
#include "inc/tm4c123gh6pm.h"
#include "inc/hw_timer.h"

#include "driverlib/sysctl.h"
#include "driverlib/interrupt.h"
#include "driverlib/timer.h"

#include "MyTimer.h"

// STRUKTURA
struct {
	volatile uint32_t millis ;

	uint8_t isInitiated : 1 ;
} myTimerStruct ;


void myTimerInt() {
	uint32_t status = TimerIntStatus(MYTIMER_TIMER_BASE, true) ;
	TimerIntClear(MYTIMER_TIMER_BASE, status) ;
	myTimerStruct.millis++ ;
}

void myTimerInit() {
	if(myTimerStruct.isInitiated == 1)
		return ;

	uint32_t period ;
	period = SysCtlClockGet() / 1000 ; // 1kHz

	SysCtlPeripheralEnable(MYTIMER_SYSCTL_PERIPH) ;
	SysCtlDelay(3) ;

	// periodic mode, count down
	TimerConfigure(MYTIMER_TIMER_BASE, TIMER_CFG_PERIODIC) ;
	TimerLoadSet(MYTIMER_TIMER_BASE, TIMER_A, period - 1) ;
	TimerIntRegister(MYTIMER_TIMER_BASE, TIMER_A, myTimerInt) ;
	TimerIntEnable(MYTIMER_TIMER_BASE, TIMER_TIMA_TIMEOUT) ;

	myTimerStruct.millis = 0 ;

	TimerEnable(MYTIMER_TIMER_BASE, TIMER_A) ;

	myTimerStruct.isInitiated  = 1 ;
}

inline uint32_t myTimerGetTime() {
	return myTimerStruct.millis ;
}

void myTimerWait(uint32_t howLongMillis) {
	uint32_t startWaiting = myTimerGetTime() + howLongMillis ;
	while(myTimerGetTime() < startWaiting) ;
}
