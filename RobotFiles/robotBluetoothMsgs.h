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
void btFunE(uint8_t params[BT_TASKS_PARAM_NUM]);
void btFunI(uint8_t params[BT_TASKS_PARAM_NUM]);
void btFunB(uint8_t params[BT_TASKS_PARAM_NUM]);
void btFunS(uint8_t params[BT_TASKS_PARAM_NUM]);
void btFunP(uint8_t params[BT_TASKS_PARAM_NUM]);


#endif /* ROBOTFILES_ROBOTBLUETOOTHMSGS_H_ */
