
/*
 * robotBluetoothMsgs.c
 *
 *  Created on: 24 mar 2016
 *      Author: akowalew
 */


#include "robot.h"


void btFunMotor(uint8_t params[BT_TASKS_PARAM_NUM]) {
	// params[0] : 0 - LEFT MOTOR, 1 - RIGHT MOTOR
	// params[1] : 1 - LESS THAN 0 VELOCITY, 0 - GREATER THAN 0 VELOCITY
	// params[2] : VELOCITY VALUE

	int32_t val = (params[1] == 1) ? -params[2] : params[2] ;
	val *= 40 ;
	val /= 255 ;

	if(params[0] == 0) { // LEFT MOTOR
		motVelSpSetL(val) ;
	} else {	// RIGHT MOTOR
 		motVelSpSetR(val) ;
	}
}


void btFunStart(uint8_t params[BT_TASKS_PARAM_NUM]) {
	robotStart() ;
}

void btFunLPid(uint8_t params[BT_TASKS_PARAM_NUM]) {
	float kp = ((float)(params[0]))/10, ki = ((float)(params[1]))/10, kd = ((float)(params[2]))/10 ;
	motPidVelSetupL(kp, ki, kd);
}

void btFunRPid(uint8_t params[BT_TASKS_PARAM_NUM]) {
	float kp = ((float)(params[0]))/10, ki = ((float)(params[1]))/10, kd = ((float)(params[2]))/10 ;
	motPidVelSetupR(kp, ki, kd);
}
