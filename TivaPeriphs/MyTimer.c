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

// STRUKTURY

// STRUKTURA PROCESU
typedef struct {
	uint32_t cycleMillis ;
	uint32_t lastRun ;

	uint8_t isEnabled : 1 ;
	uint8_t isPeriodical : 1 ;

	void (*ptr)() ;
} CycleProcess ;

// STRUKTURA
struct {
	volatile uint32_t millis ;
	regId enabledProcessCount ;
	CycleProcess cycleProcesses[MYTIMER_PROC_MAX_COUNT] ;

	uint8_t isInitiated : 1 ;
} myTimerStruct ;


regId myTimerRegProc(
		uint32_t cycleMillis,
		void (*functionPointer)(void),
		uint8_t isPeriodical )
{
	regId i ;
	CycleProcess *elem = myTimerStruct.cycleProcesses ;
	for(i = 0 ; i < MYTIMER_PROC_MAX_COUNT ; i++, elem++) {
		if(elem->ptr == NULL) {
			elem->isEnabled = 0 ;
			elem->cycleMillis = cycleMillis ;
			elem->ptr = functionPointer ;
			elem->isPeriodical = isPeriodical ;

			return i ;
		}
	}
	return -1 ; // no free place
}

void myTimerUnregProc(regId id) {
	if(myTimerStruct.cycleProcesses[id].ptr != NULL) {
		myTimerStruct.cycleProcesses[id].isEnabled = 0 ;
		myTimerStruct.cycleProcesses[id].ptr = NULL ;
	}
}

void myTimerEnableProc(regId id) {
	if(myTimerStruct.cycleProcesses[id].ptr != NULL &&
			!myTimerStruct.cycleProcesses[id].isEnabled) {

		myTimerStruct.cycleProcesses[id].lastRun = myTimerGetTime() ; // save the actual time

		myTimerStruct.cycleProcesses[id].isEnabled = 1 ;
		myTimerStruct.enabledProcessCount++ ;
	}
}

void myTimerDisableProc(regId id) {
	if(myTimerStruct.cycleProcesses[id].ptr != NULL &&
			myTimerStruct.cycleProcesses[id].isEnabled) {

		myTimerStruct.cycleProcesses[id].isEnabled = 0 ;
		myTimerStruct.enabledProcessCount-- ;
	}
}

void myTimerInt() {
	uint32_t status = TimerIntStatus(MYTIMER_TIMER_BASE, true) ;
	TimerIntClear(MYTIMER_TIMER_BASE, status) ;
	myTimerStruct.millis++ ;

	if(myTimerStruct.enabledProcessCount > 0) {
		regId i ;
		CycleProcess *elem = myTimerStruct.cycleProcesses ;
		for(i = 0 ; i < MYTIMER_PROC_MAX_COUNT ; i++, elem++) {
			if(		elem->isEnabled &&	// only this ?!
					myTimerGetTime() - elem->lastRun >= elem->cycleMillis) {

				// run process
				elem->lastRun = myTimerGetTime() ;
				elem->ptr() ;
				if(elem->isPeriodical == false) {
					// stop proc
					elem->isEnabled = 0 ;
					myTimerStruct.enabledProcessCount-- ;
				}
			}
		}
	}
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
	myTimerStruct.enabledProcessCount = 0 ;

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
