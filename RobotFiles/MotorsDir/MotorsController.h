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
#include "MotorsPidTesting.h"

#define PID_LEFT_KP 1.375
#define PID_LEFT_KI 0.95
#define PID_LEFT_KD 1.0

#define PID_RIGHT_KP 1.375
#define PID_RIGHT_KI 1.0
#define PID_RIGHT_KD 1.75

#define POS_PID_LEFT_KP	0.18
#define POS_PID_LEFT_KI	0.0
#define POS_PID_LEFT_KD	2.85

#define POS_PID_RIGHT_KP	0.19
#define POS_PID_RIGHT_KI	0.0
#define POS_PID_RIGHT_KD	2.85

#define POS_PID_DELTA_TICKS	4

typedef struct {
	PidStruct velPid;
	PidStruct posPid;

	int32_t velSp;
	int32_t posSp;
	int32_t posCurr;

	bool isRegulationDone;
} motorControl_t;

#define PID_POS_REGULATOR

void motCntrlInit();
void motCntrlEnable();
void motCntrlDisable();
void motCntrlReset();
void motCntrlClear();

void motPidVelSetupL(float kp, float ki, float kd);
void motPidVelSetupR(float kp, float ki, float kd);

void motPidPosSetupL(float kp, float ki, float kd);
void motPidPosSetupR(float kp, float ki, float kd);

void motCntrlRegulationStart();
void motCntrlRegulationStop();
bool motCntrlRegulationIsRunning();

void motVelSpSetL(int32_t velocityLeftSetPoint) ;
void motVelSpSetR(int32_t velocityRightSetPoint);
void motPosSpSetL(int32_t positionLeftSetPoint);
void motPosSpSetR(int32_t positionRightSetPoint);

void motSetPointsGet(int32_t *posLeftSp, int32_t *velLeftSp, int32_t *posRightSp, int32_t *velRightSp) ;

#endif /* MOTORS_MOTORSCONTROLLER_H_ */
