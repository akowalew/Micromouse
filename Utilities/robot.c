/*
 * robot.c
 *
 *  Created on: 17 mar 2016
 *      Author: akowalew
 */

#include <stdint.h>
#include <stdbool.h>

#include "topUtils.h"
#include "Bluetooth.h"
#include "robot.h"

#include "../Sensors/BatterySensor.h"
#include "../Sensors/Encoders.h"
#include "../Sensors/IRSensors.h"

#include "../Effectors/Motors.h"

#include "../TivaPeriphs/MyTimer.h"
#include "../TivaPeriphs/UsbUart.h"

#include "../uartstdio.h"

void robotInit() {
	motorsInit() ;
	usbUartInit() ;
	myTimerInit() ;
	btInit() ;
	batSensInit() ;
	irSenInit() ;
	utilsInit() ;
	encInit() ;
}

void motorStop() ;
void irStop() ;

void robotStart() {


	while(!btn1isPushed()) ;

	myTimerWait(50) ;

	irSenEnable() ;

	motorsEnable() ;
	motorsSetupM0(CLOCKWISE) ;
	motorsSetupM1(CLOCKWISE) ;

	motorsM0PwmSet(400) ;
	motorsM1PwmSet(400) ;

	while(1) {

		if(btn1isPushed())
			motorStop() ;
		if(btn2isPushed())
			irStop() ;
	}
}

void irStop() {
	irSenDisable() ;
}

void motorStop() {
	motorsDisable() ;
}
