/*
 * PidController.c
 *
 *  Created on: 14 kwi 2016
 *      Author: dicker
 */

#include "PidController.h"

int32_t pidIteration(int32_t setPoint, int32_t processValue, volatile PidStruct * pidSt) {
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

void pidCntrlReset(volatile PidStruct *pidStruct) {
	pidStruct->lastProcessValue = 0 ;
	pidStruct->sumError = 0.0 ;
}
void pidCntrlConstantsSet(volatile PidStruct *pidStruct, float kp, float ki, float kd) {
	pidStruct->Kp = kp;
	pidStruct->Ki = ki;
	pidStruct->Kd = kd;
}
void pidCntrlConstantsGet(const volatile PidStruct *pidStruct, float *p_kp, float *p_ki, float *p_kd){
	*p_kp = pidStruct->Kp;
	*p_ki = pidStruct->Ki;
	*p_kd = pidStruct->Kd;
}

