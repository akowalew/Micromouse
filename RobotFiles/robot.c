/*
 * robot.c
 *
 *  Created on: 17 mar 2016
 *      Author: akowalew
 */

#include "robot.h"

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
	SysCtlClockSet(SYSCTL_SYSDIV_2_5 | SYSCTL_USE_PLL | SYSCTL_OSC_MAIN | SYSCTL_XTAL_16MHZ ) ;

	utilsInit() ;
	btn1IntRegister(btn1Int) ;
	btn2IntRegister(btn2Int) ;

	usbUartInit() ;

	motorsInit() ;

	btInit() ;
	btAddMessage(0x07, btFunLPid) ;
	btAddMessage(0x08, btFunRPid) ;
	btAddMessage(0x09, btFunStart) ;
	btAddMessage(0x10, btFunMotor) ;
#ifdef PID_TESTING
	btAddMessage(0x05, btFunPidTestConfigure) ;
	btAddMessage(0x04, btFunPosSP) ;
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
			break;
	}

	robotStop() ;
	pidTestSendData() ;
}


