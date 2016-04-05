/*
 * Motors.h
 *
 *  Created on: 15 mar 2016
 *      Author: akowalew
 */

#ifndef EFFECTORS_MOTORS_H_
#define EFFECTORS_MOTORS_H_

#include <stdint.h>

typedef enum {
	SOFT_STOP = 0,
	COUNTER_CLOCKWISE = 1,
	CLOCKWISE = 2,
	HARD_STOP = 3
} MOTORS_SETUP;

void motorsInit() ;
void motorsEnable() ;
void motorsDisable() ;

void motVelSetPointLeft(int32_t velocityLeftSetPoint) ;
void motVelSetPointRight(int32_t velocityRightSetPoint) ;
void motVelSetPoint(int32_t velocityLeftSetPoint, int32_t velocityRightSetPoint) ;

#endif /* EFFECTORS_MOTORS_H_ */
