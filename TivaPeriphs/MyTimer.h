/*
 * MyTimer.h
 *
 *  Created on: 30 sty 2016
 *      Author: dicker
 */

#ifndef MYTIMER_H_
#define MYTIMER_H_

#define MYTIMER_SYSCTL_PERIPH	SYSCTL_PERIPH_TIMER5
#define MYTIMER_TIMER_BASE		TIMER5_BASE

#define MYTIMER_PROC_MAX_COUNT	4

#define NULL 0

void myTimerInt() ;
void myTimerInit() ;
inline uint32_t myTimerGetTime() ;
void myTimerWait(uint32_t howLongMillis) ;

typedef int8_t regId ;


regId myTimerRegProc(
		uint32_t cycleMillis,
		void (*functionPointer)(void),
		uint8_t isPeriodical) ;
void myTimerUnregProc(regId registeredProcessId) ;
void myTimerEnableProc(regId registeredProcessId) ;
void myTimerDisableProc(regId registeredProcessId) ;

/*
 * Sample of usage :
 *
 *	// init the Timer to count on 1ms
  	myTimerInit() ;

  	// IDs for processes
	regId redId, blueId ;

	// register and get ID for 2 functions

	 // periodical function with 1000ms cycle
	redId = myTimerRegProc(1000, redLed, 1) ;
	 // non-periodical function. It will be started one time after 500ms after enabling
	blueId = myTimerRegProc(500, blueLed, 0) ;

	if(redId >= 0)
		myTimerEnableProc(redId) ; // start periodical 1Hz
	if(blueId >= 0)
		myTimerEnableProc(blueId) ; // run once after 500ms

 */

#endif /* MYTIMER_H_ */
