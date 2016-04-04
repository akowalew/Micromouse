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

void motorStop() ;

void irStop() ;

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
	btAddMessage(0x10, btFunMotor) ;

 	batSensInit() ;
	batSensEnable() ;

	irSenInit() ;

	UARTStdioConfig(1, 115200, 16000000) ;
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
	motVelSetPointLeft(30) ;
	motVelSetPointRight(30) ;
	robotStartOthers() ;
	while(robotStruct.isRunning) {
		if(!pidTestIsStillTesting())
			break ;
	}
	robotStop() ;
	pidTestSendData() ;
}


