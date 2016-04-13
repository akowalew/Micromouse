/*
 * MotorsController.c
 *
 *  Created on: 30 mar 2016
 *      Author: akowalew
 */

#include "MotorsController.h"

volatile struct {
	motorControl_t motL, motR;

} cntrlSt;

#ifdef PID_TESTING
	volatile uint16_t test_i ;
	volatile uint16_t maxI ;
	volatile uint32_t btVelSp, btPosSp ;
	volatile int32_t	test_leftValue[PID_TEST_ARRAY_SIZE],
						test_leftSteer[PID_TEST_ARRAY_SIZE],
						test_rightValue[PID_TEST_ARRAY_SIZE],
						test_rightSteer[PID_TEST_ARRAY_SIZE] ;
	char pidTestStr[24] ;

	void pidTestInit() {
		btVelSp = 0 ;
		btPosSp = 0;

		maxI = PID_TEST_ARRAY_SIZE ;
	}

	void pidTestStartTesting(){
		motVelSpSetL(btVelSp);
		motVelSpSetR(btVelSp);
		motPosSpSetR(btPosSp);
		motPosSpSetL(btPosSp);

		usprintf(
				pidTestStr,
				"LPIDVEL P %d I %d D %d\r\n",
				(uint8_t)(cntrlSt.motL.velPid.Kp*10),
				(uint8_t)(cntrlSt.motL.velPid.Ki*10),
				(uint8_t)(cntrlSt.motL.velPid.Kd*10)
				) ;
		PID_TEST_DUMP_FUN(pidTestStr) ;

		usprintf(
				pidTestStr,
				"RPIDVEL P %d I %d D %d\r\n",
				(uint8_t)(cntrlSt.motR.velPid.Kp*10),
				(uint8_t)(cntrlSt.motR.velPid.Ki*10),
				(uint8_t)(cntrlSt.motR.velPid.Kd*10)
				) ;
		PID_TEST_DUMP_FUN(pidTestStr) ;

		usprintf(pidTestStr, "SP %d MAXI %d\r\n", btVelSp, maxI);
		PID_TEST_DUMP_FUN(pidTestStr) ;

		usprintf(pidTestStr, "Pos SP %d\r\n", btPosSp);
		PID_TEST_DUMP_FUN(pidTestStr);

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
		btVelSp = params[0] ;
	}

	void btFunPosSP(uint8_t params[BT_TASKS_PARAM_NUM]){
		uint32_t val = (((uint32_t)params[0]) << 16) | (((uint32_t)params[1]) << 8) | ((uint32_t)params[2]) ;
		btPosSp = val ;
	}

#endif

void motCntrlFunctionL(){
	int32_t velTermL, posTermL;
	int32_t velL = motVelGetL();

#ifdef PID_POS_REGULATOR
	cntrlSt.motL.posCurr += velL ;
	// pid położeniowy
	posTermL = pidIteration(cntrlSt.motL.posSp, cntrlSt.motL.posCurr, &cntrlSt.motL.posPid);
	if(posTermL > cntrlSt.motL.velSp)
		posTermL = cntrlSt.motL.velSp;
#else
	posTermL = cntrlSt.motL.velSp;
#endif

	velTermL = pidIteration(posTermL, velL, &cntrlSt.motL.velPid);

	if(velTermL > 0) {
		motStateSetL(CLOCKWISE) ;
		motDutyCycleSetL((uint32_t)(velTermL));
	} else if(velTermL < 0)  {
		motStateSetL(COUNTER_CLOCKWISE) ;
		motDutyCycleSetL((uint32_t)(-velTermL));
	}

#ifdef PID_TESTING
	test_leftSteer[test_i] = posTermL;
	test_leftValue[test_i] = cntrlSt.motL.posCurr;
#endif
}

void motCntrlFunctionR(){
	int32_t velTermR, posTermR;
	int32_t velR = motVelGetR();

#ifdef PID_POS_REGULATOR
	cntrlSt.motR.posCurr += velR ;
	posTermR = pidIteration(cntrlSt.motR.posSp, cntrlSt.motR.posCurr, &cntrlSt.motR.posPid);
	if(posTermR > cntrlSt.motR.velSp)
		posTermR = cntrlSt.motR.velSp;
#else
	posTermL = cntrlSt.motL.velSp;
#endif

	velTermR = pidIteration(posTermR, velR, &cntrlSt.motR.velPid);

	if(velTermR > 0) {
		motStateSetR(CLOCKWISE) ;
		motDutyCycleSetR((uint32_t)(velTermR)) ;
	} else{
		motStateSetR(COUNTER_CLOCKWISE) ;
		motDutyCycleSetR((uint32_t)(-velTermR)) ;
	}

#ifdef PID_TESTING
	test_rightSteer[test_i] = posTermR;
	test_rightValue[test_i] = cntrlSt.motR.posCurr;
	test_i++;
#endif
}

void motCntrlTimeoutInt() {
	TimerIntClear(MOT_CNTRL_TIMER_BASE, TIMER_TIMA_TIMEOUT) ;

	motCntrlFunctionL();
	motCntrlFunctionR();

}

void motCntrlReset() {

	pidCntrlReset(&cntrlSt.motL.posPid);
	pidCntrlReset(&cntrlSt.motL.velPid);
	cntrlSt.motL.posCurr = 0;
	cntrlSt.motL.posSp = 0;
	cntrlSt.motL.velSp = 0;

	pidCntrlReset(&cntrlSt.motR.posPid);
	pidCntrlReset(&cntrlSt.motR.velPid);
	cntrlSt.motR.posCurr = 0;
	cntrlSt.motR.posSp = 0;
	cntrlSt.motR.velSp = 0;
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
	motCntrlReset();
	pidCntrlConstantsSet(&cntrlSt.motL.posPid, POS_PID_LEFT_KP, POS_PID_LEFT_KI, POS_PID_LEFT_KD);
	pidCntrlConstantsSet(&cntrlSt.motL.velPid, PID_LEFT_KP, PID_LEFT_KI, PID_LEFT_KD);
	pidCntrlConstantsSet(&cntrlSt.motR.posPid, POS_PID_RIGHT_KP, POS_PID_RIGHT_KI, POS_PID_RIGHT_KD);
	pidCntrlConstantsSet(&cntrlSt.motR.velPid, PID_RIGHT_KP, PID_RIGHT_KI, PID_RIGHT_KD);


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

	motCntrlReset() ;
}


void motVelSpSetL(int32_t velocitySp) {
	cntrlSt.motL.velSp = velocitySp ;
}
void motVelSpSetR(int32_t velocitySp) {
	cntrlSt.motR.velSp = velocitySp ;
}
void motPosSpSetL(uint32_t positionLeftSetPoint){
	cntrlSt.motL.posSp = positionLeftSetPoint;
}
void motPosSpSetR(uint32_t positionRightSetPoint){
	cntrlSt.motL.posSp = positionRightSetPoint;
}

void motPidVelSetupL(float kp, float ki, float kd){
	pidCntrlConstantsSet(&cntrlSt.motL.velPid, kp, ki, kd);
}
void motPidVelSetupR(float kp, float ki, float kd){
	pidCntrlConstantsSet(&cntrlSt.motR.velPid, kp, ki, kd);
}
void motPidPosSetupL(float kp, float ki, float kd){
	pidCntrlConstantsSet(&cntrlSt.motL.posPid, kp, ki, kd);
}
void motPidPosSetupR(float kp, float ki, float kd){
	pidCntrlConstantsSet(&cntrlSt.motR.posPid, kp, ki, kd);
}
