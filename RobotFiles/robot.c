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
#include "../TivaPeriphs/MyTimer.h"

#include "robotBluetoothMsgs.h"
#include "../MotorsDir/MotorsController.h"

struct {
	volatile bool isRunning ;
} robotStruct ;


void btn1Int() {
	robotStop() ;
}

void btn2Int() {
	robotStart() ;
}

void robotInit() {
	utilsInit() ;
	btn1IntRegister(btn1Int) ;
	btn2IntRegister(btn2Int) ;

	usbUartInit() ;
	myTimerInit() ;

	motorsInit() ;

	btInit() ;
	btAddMessage(0x06, btFunBothPid) ;
	btAddMessage(0x07, btFunLPid) ;
	btAddMessage(0x08, btFunRPid) ;
	btAddMessage(0x09, btFunStart) ;
	btAddMessage(0x10, btFunMotor) ;
#ifdef PID_TESTING
	btAddMessage(0x05, btFunPidTestConfigure) ;
#endif

 	batSensInit() ;
	batSensEnable() ;

	irSenInit() ;
}

void robotStartOthers() {
	irSenEnable() ;
	motorsEnable() ;
}

void robotStart() {
	robotStruct.isRunning = true ;
}

void robotStop() {
	robotStruct.isRunning = false ;
	irSenDisable() ;
	motorsDisable() ;
}

void robotProcedure() {

	ledsTurnOn1() ;
	while(!robotStruct.isRunning) ;
	ledsTurnOff1() ;

	pidTestStartTesting() ;
	robotStartOthers() ;
	while(robotStruct.isRunning) {
		if(!pidTestIsStillTesting())
			break ;
	}
	robotStop() ;
	pidTestSendData() ;
}


