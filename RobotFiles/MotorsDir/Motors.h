/*
 * Motors.h
 *
 *  Created on: 15 mar 2016
 *      Author: akowalew
 */

#ifndef EFFECTORS_MOTORS_H_
#define EFFECTORS_MOTORS_H_

#include "../includeHeader.h"

#include "Encoders.h"
#include "MotorsController.h"
#include "MotorsDrivers.h"

void motorsInit() ;
void motorsEnable() ;
void motorsDisable() ;

void motVelSetPointLeft(int32_t velocityLeftSetPoint) ;
void motVelSetPointRight(int32_t velocityRightSetPoint) ;
void motVelSetPoint(int32_t velocityLeftSetPoint, int32_t velocityRightSetPoint) ;

#endif /* EFFECTORS_MOTORS_H_ */
