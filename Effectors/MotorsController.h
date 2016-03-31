/*
 * MotorsController.h
 *
 *  Created on: 30 mar 2016
 *      Author: akowalew
 */

#ifndef EFFECTORS_MOTORSCONTROLLER_H_
#define EFFECTORS_MOTORSCONTROLLER_H_

#include <stdint.h>
#include <stdbool.h>

typedef uint32_t pidOut_t ;	// output value type from PID controller
typedef uint32_t pidIn_t ;	// input value type to PID controller

#define PID_INIT_LEFT_KP 1.0
#define PID_INIT_LEFT_KI 0.0
#define PID_INIT_LEFT_KD 0.0

#define PID_INIT_RIGHT_KP 1.0
#define PID_INIT_RIGHT_KI 0.0
#define PID_INIT_RIGHT_KD 0.0

typedef struct {

	pidIn_t	lastProcessValue; // used to derivative of process value
	float 	sumError; // sumation of errors, used for integrate calculations
	float	Kp ;
	float	Ki ;
	float	Kd ;

	pidIn_t maxError ;
	pidIn_t maxSumError ;

} PidStruct ;

#define MOT_CNTRL_TIMER_PERIPH	SYSCTL_PERIPH_TIMER4
#define MOT_CNTRL_TIMER_BASE	TIMER4_BASE
#define MOT_CNTRL_TIMER_DELAY 	800000

void motVelSetPointLeft(pidIn_t velocityLeftSetPoint) ;
void motVelSetPointRight(pidIn_t velocityRightSetPoint) ;

void motCntrlInit() ;
void motCntrlEnable() ;
void motCntrlDisable() ;

#endif /* EFFECTORS_MOTORSCONTROLLER_H_ */
