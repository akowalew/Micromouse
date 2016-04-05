/*
 * MotorsController.h
 *
 *  Created on: 30 mar 2016
 *      Author: akowalew
 */

#ifndef MOTORS_MOTORSCONTROLLER_H_
#define MOTORS_MOTORSCONTROLLER_H_

#include <stdint.h>
#include <stdbool.h>

#define PID_INIT_LEFT_KP 1.0
#define PID_INIT_LEFT_KI 3.0
#define PID_INIT_LEFT_KD 1.0

#define PID_INIT_RIGHT_KP 1.0
#define PID_INIT_RIGHT_KI 3.0
#define PID_INIT_RIGHT_KD 1.0

typedef struct {
	volatile float	Kp ;
	volatile float	Ki ;
	volatile float	Kd ;
} PidConstants_t ;

typedef struct {

	volatile int32_t lastProcessValue; // used to derivative of process value
	volatile float sumError; // sumation of errors, used for integrate calculations

	PidConstants_t pidConstants;
} PidStruct ;

#define MOT_CNTRL_TIMER_PERIPH	SYSCTL_PERIPH_TIMER4
#define MOT_CNTRL_TIMER_BASE	TIMER4_BASE
#define MOT_CNTRL_TIMER_DELAY 	800000	// 10ms cycle

#define PID_TESTING

#ifdef PID_TESTING
#include "../Utilities/Bluetooth.h"
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
