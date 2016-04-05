/*
 * robotBluetoothMsgs.h
 *
 *  Created on: 4 kwi 2016
 *      Author: akowalew
 */

#ifndef ROBOTFILES_ROBOTBLUETOOTHMSGS_H_
#define ROBOTFILES_ROBOTBLUETOOTHMSGS_H_

#include <stdint.h>

void btFunMotor(uint8_t params[BT_TASKS_PARAM_NUM]);
void btFunStart(uint8_t params[BT_TASKS_PARAM_NUM]);
void btFunLPid(uint8_t params[BT_TASKS_PARAM_NUM]);
void btFunRPid(uint8_t params[BT_TASKS_PARAM_NUM]);
void btFunBothPid(uint8_t params[BT_TASKS_PARAM_NUM]);

#endif /* ROBOTFILES_ROBOTBLUETOOTHMSGS_H_ */
