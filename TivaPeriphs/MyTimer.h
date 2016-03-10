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

#endif /* MYTIMER_H_ */
