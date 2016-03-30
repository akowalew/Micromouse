/*
 * MotorsController.c
 *
 *  Created on: 30 mar 2016
 *      Author: akowalew
 */

#include "MotorsController.h"
#include "inc/tm4c123gh6pm.h"
#include "inc/hw_memmap.h"
#include "inc/hw_timer.h"
#include "driverlib/sysctl.h"
#include "driverlib/timer.h"
#include "driverlib/fpu.h"

struct {

	pidIn_t velocityRefValLeft;
	pidIn_t velocityRefValRight;

	PidStruct pidStructLeft ;
	PidStruct pidStructRight ;

} cntrlSt;

pidOut_t pidIteration(pidIn_t setPoint, pidIn_t processValue, PidStruct * pidSt) {
	float error ;
	float pTerm, dTerm, iTerm ;

	// proportional
	error = setPoint - processValue ;
	pTerm = pidSt->Kp * error ;

	// integral
	pidSt->sumError = error + pidSt->sumError ;
	iTerm = pidSt->Ki * pidSt->sumError ;

	// derivative
	dTerm = pidSt->Kd * (pidSt->lastProcessValue - processValue);
	pidSt->lastProcessValue = processValue;

	// sum of all
	return pTerm + iTerm + dTerm ;
}

void motVelocitySetPointLeft(uint32_t velocityLeftReference) {
	cntrlSt.velocityRefValLeft = velocityLeftReference ;
}

void motVelocitySetPointRight(uint32_t velocityRightReference) {
	cntrlSt.velocityRefValRight = velocityRightReference ;
}

void motCntrlTimeoutInt() {
	/* TODO :
	 * Pobierz wartości zadane
	 * Zadaj je pidowi
	 * Wykonaj je na silnikach
	 */
}

void motCntrlInit() {
	SysCtlPeripheralEnable(MOT_CNTRL_TIMER_PERIPH) ;
	SysCtlDelay(3) ;

	// configure timer to run PID procedure periodically
	TimerConfigure(MOT_CNTRL_TIMER_BASE, TIMER_CFG_PERIODIC) ;
	TimerLoadSet(MOT_CNTRL_TIMER_BASE, TIMER_A, MOT_CNTRL_TIMER_DELAY) ;
	TimerIntRegister(MOT_CNTRL_TIMER_BASE, TIMER_A, motCntrlTimeoutInt) ;

	// enable FPU to make calculations better
	FPUEnable() ;

	// configure initial values for PID
	cntrlSt.velocityRefValLeft = 0 ;
	cntrlSt.velocityRefValRight = 0 ;

	PidStruct tmpl = {0, 0.0, PID_INIT_LEFT_KP, PID_INIT_LEFT_KI, PID_INIT_LEFT_KD, 0, 0} ;
	cntrlSt.pidStructLeft = tmpl ;

	PidStruct tmpr = {0, 0.0, PID_INIT_RIGHT_KP, PID_INIT_RIGHT_KI, PID_INIT_RIGHT_KD, 0, 0} ;
	cntrlSt.pidStructRight = tmpr ;
}

void motCntrlEnable() {
	TimerEnable(MOT_CNTRL_TIMER_BASE, TIMER_BOTH) ;
	TimerIntEnable(MOT_CNTRL_TIMER_BASE, TIMER_TIMA_TIMEOUT) ;
	TimerIntClear(MOT_CNTRL_TIMER_BASE, TIMER_TIMA_TIMEOUT) ;
}

void motCntrlDisable() {
	TimerDisable(MOT_CNTRL_TIMER_BASE, TIMER_BOTH) ;
	TimerIntDisable(MOT_CNTRL_TIMER_BASE,TIMER_TIMA_TIMEOUT) ;
}


