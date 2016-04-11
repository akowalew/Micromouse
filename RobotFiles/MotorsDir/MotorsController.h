/*
 * MotorsController.h
 *
 *  Created on: 30 mar 2016
 *      Author: akowalew
 */

#ifndef MOTORS_MOTORSCONTROLLER_H_
#define MOTORS_MOTORSCONTROLLER_H_

#include "../includeHeader.h"
#include "Motors.h"
#include "driverlib/timer.h"
#include "driverlib/fpu.h"

#define PID_INIT_LEFT_KP 5.0
#define PID_INIT_LEFT_KI 2.5
#define PID_INIT_LEFT_KD 3.0

#define PID_INIT_RIGHT_KP 5.0
#define PID_INIT_RIGHT_KI 2.5
#define PID_INIT_RIGHT_KD 3.0

#define POS_PID_INIT_LEFT_KP	0.15
#define POS_PID_INIT_LEFT_KI	0.0
#define POS_PID_INIT_LEFT_KD	0.0

#define POS_PID_INIT_RIGHT_KP	0.15
#define POS_PID_INIT_RIGHT_KI	0.0
#define POS_PID_INIT_RIGHT_KD	0.0

typedef struct {
	float	Kp ;
	float	Ki ;
	float	Kd ;
} PidConstants_t ;

typedef struct {

	int32_t lastProcessValue; // used to derivative of process value
	float sumError; // sumation of errors, used for integrate calculations

	PidConstants_t pidConstants;
} PidStruct ;

#define MOT_CNTRL_TIMER_PERIPH	SYSCTL_PERIPH_TIMER4
#define MOT_CNTRL_TIMER_BASE	TIMER4_BASE
#define MOT_CNTRL_TIMER_DELAY 	800000	// 10ms cycle

#define PID_TESTING

#ifdef PID_TESTING
#include "../Communication/Bluetooth.h"
	#define PID_TEST_ARRAY_SIZE	 300
	#define PID_TEST_DUMP_FUN(x)	(UARTprintf((x)))
	void pidTestInit() ;
	void pidTestStartTesting() ;
	bool pidTestIsStillTesting() ;
	void pidTestSendData() ;
	void btFunPidTestConfigure(uint8_t params[BT_TASKS_PARAM_NUM]);
#endif

void motCntrlInit() ;
void motCntrlEnable() ;
void motCntrlDisable() ;

void motCntrlClearControl() ;
void motCntrlSetLeftPid(PidConstants_t leftPid) ;
void motCntrlSetRightPid(PidConstants_t rightPid) ;

#endif /* MOTORS_MOTORSCONTROLLER_H_ */
