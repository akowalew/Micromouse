/*
 * PidController.h
 *
 *  Created on: 14 kwi 2016
 *      Author: dicker
 */

#ifndef ROBOTFILES_MOTORSDIR_PIDCONTROLLER_H_
#define ROBOTFILES_MOTORSDIR_PIDCONTROLLER_H_

#include <stdint.h>

typedef struct {

	float	Kp ;
	float	Ki ;
	float	Kd ;

	int32_t lastProcessValue; // used to derivative of process value
	float sumError; // sumation of errors, used for integrate calculations

} PidStruct ;

int32_t pidIteration(int32_t setPoint, int32_t processValue, volatile PidStruct * pidSt) ;

void pidCntrlReset(volatile PidStruct *pidStruct) ;
void pidCntrlConstantsSet(volatile PidStruct *pidStruct, float kp, float ki, float kd) ;
void pidCntrlConstantsGet(const volatile PidStruct *pidStruct, float *p_kp, float *p_ki, float *p_kd) ;


#endif /* ROBOTFILES_MOTORSDIR_PIDCONTROLLER_H_ */
