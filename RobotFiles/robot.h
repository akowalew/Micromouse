/*
 * robot.h
 *
 *  Created on: 17 mar 2016
 *      Author: akowalew
 */

#ifndef ROBOTFILES_ROBOT_H_
#define ROBOTFILES_ROBOT_H_

#include "includeHeader.h"

#include "robotBluetoothMsgs.h"

#include "Sensors/BatterySensor.h"
#include "Sensors/IRSensors.h"

#include "MotorsDir/Motors.h"

#include "Communication/Bluetooth.h"
#include "Communication/topUtils.h"

void robotInit() ;
void robotProcedure() ;
void robotStart() ;
void robotStop() ;

#endif /* ROBOTFILES_ROBOT_H_ */
