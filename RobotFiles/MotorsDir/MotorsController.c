/*
 * MotorsController.c
 *
 *  Created on: 30 mar 2016
 *      Author: akowalew
 */

#include "MotorsController.h"

volatile struct {
	motorControl_t motL, motR;
	bool isRegulationRunning;
} cntrlSt;

#define abs(x) ((x) >= 0 ? (x) : (-x))

void motCntrlFunctionL(){
	motEncodersIntClearL();

	int32_t velTermL, posTermL;
	int32_t velL = motVelGetL();

#ifdef PID_POS_REGULATOR
	cntrlSt.motL.posCurr += velL ;
	// pid położeniowy
	posTermL = pidIteration(cntrlSt.motL.posSp, cntrlSt.motL.posCurr, &cntrlSt.motL.posPid);


	if(abs(posTermL) > abs(cntrlSt.motL.velSp))
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

	if(velL == 0 &&
			((cntrlSt.motL.posCurr <= cntrlSt.motL.posSp + POS_PID_DELTA_TICKS) &&
			(cntrlSt.motL.posCurr >= cntrlSt.motL.posSp - POS_PID_DELTA_TICKS)) )
		cntrlSt.motL.isRegulationDone = true;
	else
		cntrlSt.motL.isRegulationDone = false;

	if(cntrlSt.motL.isRegulationDone && cntrlSt.motR.isRegulationDone)
		motCntrlRegulationStop();

#ifdef PID_TESTING
	pidTestAddDataL(posTermL, cntrlSt.motL.posCurr);
#endif
}

void motCntrlFunctionR(){
	motEncodersIntClearR();

	int32_t velTermR, posTermR;
	int32_t velR = motVelGetR();

#ifdef PID_POS_REGULATOR
	cntrlSt.motR.posCurr += velR ;
	posTermR = pidIteration(cntrlSt.motR.posSp, cntrlSt.motR.posCurr, &cntrlSt.motR.posPid);
	if(abs(posTermR) > abs(cntrlSt.motR.velSp))
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

	if(velR == 0 &&
			((cntrlSt.motR.posCurr <= cntrlSt.motR.posSp + POS_PID_DELTA_TICKS) &&
			(cntrlSt.motR.posCurr >= cntrlSt.motR.posSp - POS_PID_DELTA_TICKS)) )
		cntrlSt.motR.isRegulationDone = true;
	else
		cntrlSt.motR.isRegulationDone = false;

	if(cntrlSt.motL.isRegulationDone && cntrlSt.motR.isRegulationDone)
		motCntrlRegulationStop();

#ifdef PID_TESTING
	pidTestAddDataR(posTermR, cntrlSt.motR.posCurr);
#endif
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

void motCntrlClear(){
	pidCntrlReset(&cntrlSt.motL.posPid);
	pidCntrlReset(&cntrlSt.motL.velPid);
	pidCntrlReset(&cntrlSt.motR.posPid);
	pidCntrlReset(&cntrlSt.motR.velPid);
}


void motCntrlInit() {
// enable FPU to make calculations better
	FPUEnable() ;

	motEncodersInterruptsConfigure(motCntrlFunctionL, motCntrlFunctionR);

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
	//motEncodersInterruptsEnable();
}

void motCntrlDisable() {
	//motEncodersInterruptsDisable();
	//motCntrlReset() ;
}

bool motCntrlRegulationIsRunning(){
	return cntrlSt.isRegulationRunning;
}

void motCntrlRegulationStart(){
	cntrlSt.motL.posCurr = 0;
	cntrlSt.motR.posCurr = 0;
	cntrlSt.isRegulationRunning = true;
	motEncodersInterruptsEnable();
}
void motCntrlRegulationStop(){
	cntrlSt.isRegulationRunning = false;

	// zatrzymaj silniki
	motDutyCycleSetR(1) ;
	motDutyCycleSetL(1) ;

	motEncodersInterruptsDisable();

	motCntrlClear();

#ifdef PID_TESTING
	if(pidTestIsStillTesting())
		pidTestStopTesting();
#endif
}

void motVelSpSetL(int32_t velocitySp) {
	cntrlSt.motL.velSp = velocitySp ;
}
void motVelSpSetR(int32_t velocitySp) {
	cntrlSt.motR.velSp = velocitySp ;
}
void motPosSpSetL(int32_t positionLeftSetPoint){
	cntrlSt.motL.posSp = positionLeftSetPoint;
}
void motPosSpSetR(int32_t positionRightSetPoint){
	cntrlSt.motR.posSp = positionRightSetPoint;
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

void motSetPointsGet(int32_t *posLeftSp, int32_t *velLeftSp, int32_t *posRightSp, int32_t *velRightSp) {
	*posLeftSp = cntrlSt.motL.posSp;
	*posRightSp = cntrlSt.motR.posSp;
	*velLeftSp = cntrlSt.motL.velSp;
	*velRightSp = cntrlSt.motR.velSp;
}
