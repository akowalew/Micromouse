/*
 * MotorsController.c
 *
 *  Created on: 30 mar 2016
 *      Author: akowalew
 */

#include "MotorsController.h"

volatile struct {
	int32_t velSetPointLeft;
	int32_t velSetPointRight;

	PidStruct pidStructLeft;
	PidStruct pidStructRight;

	PidStruct posPidStructLeft;
	PidStruct posPidStructRight;

	int32_t posSetPointLeft;
	int32_t posSetPointRight;

	int32_t currLeftPos;
	int32_t currRightPos ;
} cntrlSt;

int32_t pidIteration(int32_t setPoint, int32_t processValue, volatile PidStruct * pidSt) {
	float error ;
	float pTerm, dTerm, iTerm ;

	// proportional
	error = setPoint - processValue ;
	pTerm = pidSt->pidConstants.Kp * error ;

	// integral
	pidSt->sumError = error + pidSt->sumError ;
	iTerm = pidSt->pidConstants.Ki * pidSt->sumError ;

	// derivative
	dTerm = pidSt->pidConstants.Kd * (pidSt->lastProcessValue - processValue);
	pidSt->lastProcessValue = processValue;

	// sum of all
	return pTerm + iTerm + dTerm ;
}

void motVelSetPointLeft(int32_t velocitySp) {
	cntrlSt.velSetPointLeft = velocitySp ;
}

void motVelSetPointRight(int32_t velocitySp) {
	cntrlSt.velSetPointRight = velocitySp ;
}

void motPosSetPointLeft(uint32_t positionLeftSetPoint){
	cntrlSt.posSetPointLeft = positionLeftSetPoint;
}
void motPosSetPointRight(uint32_t positionRightSetPoint){
	cntrlSt.posSetPointRight = positionRightSetPoint;
}

inline void motVelSetPoint(int32_t velocityLeftSetPoint, int32_t velocityRightSetPoint) {
	cntrlSt.velSetPointLeft = velocityLeftSetPoint ;
	cntrlSt.velSetPointRight = velocityRightSetPoint ;
}

#ifdef PID_TESTING
	volatile uint16_t test_i ;
	volatile uint16_t maxI ;
	volatile uint8_t btSetPoint ;
	volatile int32_t	test_leftValue[PID_TEST_ARRAY_SIZE],
						test_leftSteer[PID_TEST_ARRAY_SIZE],
						test_rightValue[PID_TEST_ARRAY_SIZE],
						test_rightSteer[PID_TEST_ARRAY_SIZE] ;
	char pidTestStr[24] ;

	void pidTestInit() {
		btSetPoint = 0 ;
		maxI = PID_TEST_ARRAY_SIZE ;
	}

	void pidTestStartTesting(){
		motVelSetPointLeft(btSetPoint) ;
		motVelSetPointRight(btSetPoint) ;

		usprintf(
				pidTestStr,
				"LEFTPID P %d I %d D %d\r\n",
				(uint8_t)(cntrlSt.pidStructLeft.pidConstants.Kp*10),
				(uint8_t)(cntrlSt.pidStructLeft.pidConstants.Ki*10),
				(uint8_t)(cntrlSt.pidStructLeft.pidConstants.Kd*10)
				) ;
		PID_TEST_DUMP_FUN(pidTestStr) ;

		usprintf(
				pidTestStr,
				"RIGHTPID P %d I %d D %d\r\n",
				(uint8_t)(cntrlSt.pidStructRight.pidConstants.Kp*10),
				(uint8_t)(cntrlSt.pidStructRight.pidConstants.Ki*10),
				(uint8_t)(cntrlSt.pidStructRight.pidConstants.Kd*10)
				) ;
		PID_TEST_DUMP_FUN(pidTestStr) ;

		usprintf(pidTestStr, "SP %d MAXI %d\r\n", btSetPoint, maxI);
		PID_TEST_DUMP_FUN(pidTestStr) ;

		test_i = 0 ;
	}

	bool pidTestIsStillTesting(){
		return (test_i < maxI) ;
	}

	void pidTestSendData(){
		uint16_t i ;
		for(i = 0 ; i < maxI ; i++) {
			usprintf(pidTestStr, "%d %d %d %d\n", test_leftValue[i], test_leftSteer[i], test_rightValue[i], test_rightSteer[i]) ;
			PID_TEST_DUMP_FUN(pidTestStr) ;
		}
	}

	void btFunPidTestConfigure(uint8_t params[BT_TASKS_PARAM_NUM]){
		btSetPoint = params[0] ;
		maxI = ((uint16_t)(params[1]) << 8) | params[2] ;
	}


#endif

void motCntrlTimeoutInt() {
	TimerIntClear(MOT_CNTRL_TIMER_BASE, TIMER_TIMA_TIMEOUT) ;

	int32_t leftTerm, rightTerm, leftPosTerm, rightPosTerm ;

	int32_t	leftVel = encLGetVel() ,
			rightVel = encRGetVel() ;

	cntrlSt.currLeftPos += leftVel ;
	cntrlSt.currRightPos += rightVel ;

	// pid położeniowy
	leftPosTerm = pidIteration(cntrlSt.posSetPointLeft, cntrlSt.currLeftPos, &cntrlSt.posPidStructLeft);
	rightPosTerm = pidIteration(cntrlSt.posSetPointRight, cntrlSt.currRightPos, &cntrlSt.posPidStructRight);

	if(leftPosTerm > cntrlSt.velSetPointLeft)
		leftPosTerm = cntrlSt.velSetPointLeft;
	if(rightPosTerm > cntrlSt.velSetPointRight)
		rightPosTerm = cntrlSt.velSetPointRight;

	leftTerm = pidIteration(leftPosTerm, leftVel, &cntrlSt.pidStructLeft);
	rightTerm = pidIteration(rightPosTerm, rightVel, &cntrlSt.pidStructRight);

	if(leftTerm > 0) {
		motorsSetupML(CLOCKWISE) ;
		motorsMLPwmSet((uint32_t)(leftTerm));
	} else if(leftTerm < 0)  {
		motorsSetupML(COUNTER_CLOCKWISE) ;
		motorsMLPwmSet((uint32_t)(-leftTerm));
	}

	if(rightTerm > 0) {
		motorsSetupMR(CLOCKWISE) ;
		motorsMRPwmSet((uint32_t)(rightTerm)) ;
	} else{
		motorsSetupMR(COUNTER_CLOCKWISE) ;
		motorsMRPwmSet((uint32_t)(-rightTerm)) ;
	}

#ifdef PID_TESTING
	if(test_i < PID_TEST_ARRAY_SIZE) {
		test_leftSteer[test_i] = leftPosTerm;
		test_leftValue[test_i] = cntrlSt.currLeftPos;
		test_rightSteer[test_i] = rightPosTerm;
		test_rightValue[test_i] = cntrlSt.currRightPos;
		test_i++;
	}
#endif
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
	cntrlSt.velSetPointLeft = 0;
	cntrlSt.velSetPointRight = 0;
	cntrlSt.currLeftPos = 0;
	cntrlSt.currRightPos = 0;
	cntrlSt.posSetPointLeft = 0;
	cntrlSt.posSetPointRight = 0;

	PidStruct tmpl = {0, 0.0, {PID_INIT_LEFT_KP, PID_INIT_LEFT_KI, PID_INIT_LEFT_KD}} ;
	cntrlSt.pidStructLeft = tmpl ;

	PidStruct tmpr = {0, 0.0, {PID_INIT_RIGHT_KP, PID_INIT_RIGHT_KI, PID_INIT_RIGHT_KD}} ;
	cntrlSt.pidStructRight = tmpr ;

	PidStruct tmpposl = {0, 0.0, {POS_PID_INIT_LEFT_KP, POS_PID_INIT_LEFT_KI, POS_PID_INIT_LEFT_KD}};
	cntrlSt.posPidStructLeft = tmpposl ;

	PidStruct tmpposr = {0, 0.0, {POS_PID_INIT_RIGHT_KP, POS_PID_INIT_RIGHT_KI, POS_PID_INIT_RIGHT_KD}};
	cntrlSt.posPidStructLeft = tmpposr ;

#ifdef PID_TESTING
	pidTestInit() ;
#endif
}

void motCntrlEnable() {
	TimerEnable(MOT_CNTRL_TIMER_BASE, TIMER_BOTH) ;
	TimerIntEnable(MOT_CNTRL_TIMER_BASE, TIMER_TIMA_TIMEOUT) ;
	TimerIntClear(MOT_CNTRL_TIMER_BASE, TIMER_TIMA_TIMEOUT) ;
}

void motCntrlDisable() {
	TimerDisable(MOT_CNTRL_TIMER_BASE, TIMER_BOTH) ;
	TimerIntDisable(MOT_CNTRL_TIMER_BASE,TIMER_TIMA_TIMEOUT) ;

	motCntrlClearControl() ;
}

void motCntrlClearControl() {

	// configure initial values for PID
	cntrlSt.velSetPointLeft = 0 ;
	cntrlSt.velSetPointRight = 0 ;

	cntrlSt.posSetPointLeft = 0;
	cntrlSt.posSetPointRight = 0;

	cntrlSt.posPidStructLeft.lastProcessValue = 0;
	cntrlSt.posPidStructLeft.sumError = 0.0;

	cntrlSt.posPidStructRight.lastProcessValue = 0;
	cntrlSt.posPidStructRight.sumError = 0.0;

	cntrlSt.currLeftPos = cntrlSt.currRightPos = 0;

	cntrlSt.pidStructLeft.lastProcessValue = 0;
	cntrlSt.pidStructLeft.sumError = 0.0;
	cntrlSt.pidStructRight.lastProcessValue = 0;
	cntrlSt.pidStructRight.sumError = 0.0;
}

void motCntrlSetLeftPid(PidConstants_t leftPid){
	cntrlSt.pidStructLeft.pidConstants = leftPid;
}
void motCntrlSetRightPid(PidConstants_t rightPid){
	cntrlSt.pidStructRight.pidConstants = rightPid;
}


