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
#include "PidController.h"
#include "driverlib/timer.h"
#include "driverlib/fpu.h"

#define PID_LEFT_KP 1.5
#define PID_LEFT_KI 0.95
#define PID_LEFT_KD 1.0

#define PID_RIGHT_KP 1.5
#define PID_RIGHT_KI 1.0
#define PID_RIGHT_KD 1.75

#define POS_PID_LEFT_KP	0.2
#define POS_PID_LEFT_KI	0.0
#define POS_PID_LEFT_KD	2.0

#define POS_PID_RIGHT_KP	0.2
#define POS_PID_RIGHT_KI	0.0
#define POS_PID_RIGHT_KD	2.0

typedef struct {
	PidStruct velPid;
	PidStruct posPid;

	int32_t velSp;
	int32_t posSp;
	int32_t posCurr;
} motorControl_t;

#define PID_TESTING
#define PID_POS_REGULATOR

void motCntrlInit();
void motCntrlEnable();
void motCntrlDisable();

void motPidVelSetupL(float kp, float ki, float kd);
void motPidVelSetupR(float kp, float ki, float kd);

void motPidPosSetupL(float kp, float ki, float kd);
void motPidPosSetupR(float kp, float ki, float kd);

void motVelSpSetL(int32_t velocityLeftSetPoint) ;
void motVelSpSetR(int32_t velocityRightSetPoint);
void motPosSpSetL(uint32_t positionLeftSetPoint);
void motPosSpSetR(uint32_t positionRightSetPoint);

#ifdef PID_TESTING
#include "../Communication/Bluetooth.h"
	#define PID_TEST_ARRAY_SIZE	 200
	#define PID_TEST_DUMP_FUN(x)	(UARTprintf((x)))
	void pidTestInit() ;
	void pidTestStartTesting() ;
	bool pidTestIsStillTesting() ;
	void pidTestSendData() ;
	void btFunPidTestConfigure(uint8_t params[BT_TASKS_PARAM_NUM]);
	void btFunPosSP(uint8_t params[BT_TASKS_PARAM_NUM]);
#endif

#endif /* MOTORS_MOTORSCONTROLLER_H_ */
