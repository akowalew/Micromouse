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

	volatile int32_t	lastProcessValue; // used to derivative of process value
	volatile float 		sumError; // sumation of errors, used for integrate calculations
	volatile float		Kp ;
	volatile float		Ki ;
	volatile float		Kd ;
} PidStruct ;

#define MOT_CNTRL_TIMER_PERIPH	SYSCTL_PERIPH_TIMER4
#define MOT_CNTRL_TIMER_BASE	TIMER4_BASE
#define MOT_CNTRL_TIMER_DELAY 	800000	// 10ms cycle

#define PID_TESTING

#ifdef PID_TESTING
	#define PID_TEST_ARRAY_SIZE	 200
	void pidTestStartTesting() ;
	bool pidTestIsStillTesting() ;
	void pidTestSendData() ;
#endif

#endif /* MOTORS_MOTORSCONTROLLER_H_ */
