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
#include "../ustdlib.h"

#include "../uartstdio.h"
#include "../TivaPeriphs/UsbUart.h"
#include "../Sensors/Encoders.h"
#include "../Effectors/Motors.h"

struct {

	pidIn_t velSetPointLeft;
	pidIn_t velSetPointRight;

	MOTORS_SETUP leftSetup ;
	MOTORS_SETUP rightSetup ;

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

inline void motVelSetPointLeft(int32_t velocitySp) {
	if(velocitySp < 0)
		cntrlSt.leftSetup = COUNTER_CLOCKWISE ;
	else
		cntrlSt.leftSetup = CLOCKWISE ;

	cntrlSt.velSetPointLeft = velocitySp ;
}

inline void motVelSetPointRight(int32_t velocitySp) {
	if(velocitySp < 0)
		cntrlSt.rightSetup = COUNTER_CLOCKWISE ;
	else
		cntrlSt.rightSetup = CLOCKWISE ;

	cntrlSt.velSetPointRight = velocitySp ;
}

char str[32] ;
void motCntrlTimeoutInt() {
	TimerIntClear(MOT_CNTRL_TIMER_BASE, TIMER_TIMA_TIMEOUT) ;


	/////////////
	// TODO : what if setPoint value equal 0 ?!?!?!
	/////////////


	pidOut_t leftTerm, rightTerm ;

	pidIn_t	leftVel = encLGetVel() ,
			rightVel = encRGetVel() ;

	leftTerm = pidIteration(cntrlSt.velSetPointLeft, leftVel, &cntrlSt.pidStructLeft);
	rightTerm = pidIteration(cntrlSt.velSetPointRight, rightVel, &cntrlSt.pidStructRight);

	if(leftTerm > 0) {
		motorsSetupML(cntrlSt.leftSetup) ;
		motorsMLPwmSet(leftTerm);
	}
	else {
		if(leftTerm == 0)
			motorsSetupML(SOFT_STOP) ;
		else {
			motorsSetupML(cntrlSt.leftSetup == CLOCKWISE ? COUNTER_CLOCKWISE : CLOCKWISE) ;
			motorsMLPwmSet(-leftTerm);
		}
	}

	if(rightTerm > 0) {
		motorsSetupMR(cntrlSt.rightSetup) ;
		motorsMRPwmSet(rightTerm);
	}
	else {
		if(rightTerm == 0)
			motorsSetupMR(SOFT_STOP) ;
		else {
			motorsSetupMR(cntrlSt.rightSetup == CLOCKWISE ? COUNTER_CLOCKWISE : CLOCKWISE) ;
			motorsMRPwmSet(-rightTerm);
		}
	}

	// print values for Left motor
	usprintf(str, "%d %d %d %d\n", leftVel, leftTerm, rightVel, rightTerm) ;
	usbUartSendStr(str) ;
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
	cntrlSt.velSetPointLeft = 0 ;
	cntrlSt.velSetPointRight = 0 ;

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

	// configure initial values for PID
	cntrlSt.velSetPointLeft = 0 ;
	cntrlSt.velSetPointRight = 0 ;

	PidStruct tmpl = {0, 0.0, PID_INIT_LEFT_KP, PID_INIT_LEFT_KI, PID_INIT_LEFT_KD, 0, 0} ;
	cntrlSt.pidStructLeft = tmpl ;

	PidStruct tmpr = {0, 0.0, PID_INIT_RIGHT_KP, PID_INIT_RIGHT_KI, PID_INIT_RIGHT_KD, 0, 0} ;
	cntrlSt.pidStructRight = tmpr ;
}


