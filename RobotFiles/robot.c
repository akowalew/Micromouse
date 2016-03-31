/*
 * robot.c
 *
 *  Created on: 17 mar 2016
 *      Author: akowalew
 */

#include "robot.h"

#include <stdint.h>
#include <stdbool.h>

#include "../Utilities/topUtils.h"
#include "../Utilities/Bluetooth.h"
#include "driverlib/sysctl.h"
#include "../Sensors/BatterySensor.h"
#include "../Sensors/Encoders.h"
#include "../Sensors/IRSensors.h"
#include "../Effectors/Motors.h"
#include "../TivaPeriphs/UsbUart.h"
#include "../Effectors/MotorsController.h"
#include "../uartstdio.h"


#include "inc/hw_types.h"
#include "inc/hw_memmap.h"
#include "inc/tm4c123gh6pm.h"
#include "driverlib/uart.h"
#include "inc/hw_uart.h"

void btFunL(uint8_t params[BT_TASKS_PARAM_NUM]);
void btFunR(uint8_t params[BT_TASKS_PARAM_NUM]);
void btFunE(uint8_t params[BT_TASKS_PARAM_NUM]);
void btFunI(uint8_t params[BT_TASKS_PARAM_NUM]);
void btFunB(uint8_t params[BT_TASKS_PARAM_NUM]);

void motorStop() ;

void irStop() ;

struct {
	volatile bool isRunning ;
} robotStruct ;


void btn1Int() {
	robotStruct.isRunning = false ;
	robotStop() ;
}

void btn2Int() {
	robotStruct.isRunning = true ;
}

void robotInit() {

	utilsInit() ;
	btn1IntRegister(btn1Int) ;
	btn2IntRegister(btn2Int) ;

	usbUartInit() ;

	motorsInit() ;
	motCntrlInit() ;

	btInit() ;
	btAddMessage('L', btFunL) ;
	btAddMessage('R', btFunR) ;
	btAddMessage('B', btFunB) ;
	btAddMessage('I', btFunI) ;
	btAddMessage('E', btFunE) ;
	batSensInit() ;
	batSensEnable() ;

	irSenInit() ;
	encInit() ;


	UARTStdioConfig(1, 115200, 16000000) ;
}

void robotStart() {
	irSenEnable() ;
	motorsEnable() ;
	motCntrlEnable() ;
	encEnable() ;
}

void robotStop() {
	irSenDisable() ;
	motorsDisable() ;
	motCntrlDisable() ;
	encDisable() ;
}

void robotProcedure() {

	ledsTurnOn1() ;
	while(!robotStruct.isRunning) ;
	ledsTurnOff1() ;

	robotStart() ;

	motVelSetPointLeft(20) ;
	motVelSetPointRight(20) ;

	while(robotStruct.isRunning) {
		//SysCtlDelay(8000000) ;
		//UARTprintf("%d %d %d %d\n", irSenGetVal(0), irSenGetVal(1), irSenGetVal(2), irSenGetVal(3) );
	}
}


