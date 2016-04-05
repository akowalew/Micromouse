
/*
 * robotBluetoothMsgs.c
 *
 *  Created on: 24 mar 2016
 *      Author: akowalew
 */


#include "robot.h"

#include <stdint.h>
#include <stdbool.h>

#include "../Utilities/topUtils.h"
#include "../Utilities/Bluetooth.h"
#include "driverlib/sysctl.h"
#include "../Sensors/BatterySensor.h"
#include "../Sensors/IRSensors.h"
#include "../MotorsDir/Motors.h"
#include "../TivaPeriphs/UsbUart.h"
#include "../uartstdio.h"

#include "inc/hw_types.h"
#include "inc/hw_memmap.h"
#include "inc/tm4c123gh6pm.h"
#include "driverlib/uart.h"
#include "inc/hw_uart.h"
#include "../MotorsDir/Encoders.h"

#include "robotBluetoothMsgs.h"
#include "../MotorsDir/MotorsController.h"

void btFunMotor(uint8_t params[BT_TASKS_PARAM_NUM]) {
	// params[0] : 0 - LEFT MOTOR, 1 - RIGHT MOTOR
	// params[1] : 1 - LESS THAN 0 VELOCITY, 0 - GREATER THAN 0 VELOCITY
	// params[2] : VELOCITY VALUE

	int32_t val = (params[1] == 1) ? -params[2] : params[2] ;
	val *= 40 ;
	val /= 255 ;

	if(params[0] == 0) { // LEFT MOTOR
		motVelSetPointLeft(val) ;
	} else {	// RIGHT MOTOR
 		motVelSetPointRight(val) ;
	}
}


void btFunStart(uint8_t params[BT_TASKS_PARAM_NUM]) {
	robotStart() ;
}

void btFunLPid(uint8_t params[BT_TASKS_PARAM_NUM]) {
	PidConstants_t tmp = { ((float)(params[0]))/10, ((float)(params[1]))/10, ((float)(params[2]))/10 };
	motCntrlSetLeftPid(tmp);
}

void btFunRPid(uint8_t params[BT_TASKS_PARAM_NUM]) {
	PidConstants_t tmp = {((float)(params[0]))/10, ((float)(params[1]))/10, ((float)(params[2]))/10 };
	motCntrlSetRightPid(tmp);
}

void btFunBothPid(uint8_t params[BT_TASKS_PARAM_NUM]) {
	PidConstants_t tmp = {((float)(params[0])) / 10 , ((float)(params[1]))/10, ((float)(params[2]))/10 };
	motCntrlSetRightPid(tmp);
	motCntrlSetLeftPid(tmp);
}
