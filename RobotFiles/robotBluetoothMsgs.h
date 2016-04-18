/*
 * robotBluetoothMsgs.h
 *
 *  Created on: 4 kwi 2016
 *      Author: akowalew
 */

#ifndef ROBOTFILES_ROBOTBLUETOOTHMSGS_H_
#define ROBOTFILES_ROBOTBLUETOOTHMSGS_H_

#include "includeHeader.h"
#include "Communication/Bluetooth.h"
#include "robot.h"
#include "MotorsDir/MotorsController.h"

void btFunMotor(uint8_t params[BT_TASKS_PARAM_NUM]);
void btFunStart(uint8_t params[BT_TASKS_PARAM_NUM]);
void btFunLPid(uint8_t params[BT_TASKS_PARAM_NUM]);
void btFunRPid(uint8_t params[BT_TASKS_PARAM_NUM]);
void btFunBothPid(uint8_t params[BT_TASKS_PARAM_NUM]);
void btFunPosSpLeftSet(uint8_t params[BT_TASKS_PARAM_NUM]);
void btFunPosSpRightSet(uint8_t params[BT_TASKS_PARAM_NUM]);
void btFunVelSpBothSet(uint8_t params[BT_TASKS_PARAM_NUM]);
void btFunDumpTest(uint8_t params[BT_TASKS_PARAM_NUM]);
#endif /* ROBOTFILES_ROBOTBLUETOOTHMSGS_H_ */
