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
#include "../Sensors/Encoders.h"
#include "../Sensors/IRSensors.h"
#include "../Effectors/Motors.h"
#include "../TivaPeriphs/UsbUart.h"
#include "../uartstdio.h"


#include "inc/hw_types.h"
#include "inc/hw_memmap.h"
#include "inc/tm4c123gh6pm.h"
#include "driverlib/uart.h"
#include "inc/hw_uart.h"


void btFunL(uint8_t params[BT_TASKS_PARAM_NUM]) {
	//UARTprintf("L %d %d %d\n", params[0], params[1], params[2] ) ;

	int val = params[0] ;
	if(val >= 128) {
		motorsSetupML(CLOCKWISE) ;
		val -= 128 ;
		if(val == 0)
			motorsSetupML(SOFT_STOP) ;
		val *= 1000 ;
		val /= 128 ;
		motorsMLPwmSet(val) ;
	} else {
		motorsSetupML(COUNTER_CLOCKWISE) ;
		val = 128 - val ;
		val *= 1000 ;
		val /= 128 ;
		motorsMLPwmSet(val) ;
	}

}

void btFunR(uint8_t params[BT_TASKS_PARAM_NUM]) {
	//UARTprintf("R %d %d %d\n", params[0], params[1], params[2] ) ;

	int val = params[0] ;
	if(val >= 128) {
		motorsSetupMR(CLOCKWISE) ;
		val -= 128 ;
		if(val == 0)
			motorsSetupMR(SOFT_STOP) ;
		val *= 1000 ;
		val /= 128 ;
		motorsMRPwmSet(val) ;
	} else {
		motorsSetupMR(COUNTER_CLOCKWISE) ;
		val = 128 - val ;
		val *= 1000 ;
		val /= 128 ;
		motorsMRPwmSet(val) ;
	}
}

/**
 * Function to execute message from Bluetooth called 'B'
 * 'B' means : get battery state.
 *
 * this function sends to Bluetooth bat voltages
 */
void btFunB(uint8_t params[BT_TASKS_PARAM_NUM]) {
	float tab[3] ;
	batSensGetMeasures(tab) ;
	UARTprintf("B:%d,%d,%d\n", (uint32_t)(tab[0] * 100), (uint32_t)(tab[1] * 100), (uint32_t)(tab[2] * 100));
}

/**
 * Function to execute message from Bluetooth called 'I'
 * 'I' means : get all sensors vals
 *
 * this function sends to Bluetooth ADC values of each sensor
 */
void btFunI(uint8_t params[BT_TASKS_PARAM_NUM]) {
	uint32_t tab[IRSEN_ADC_SENSORS_NUM] ;
	irSenGetAllVals(tab) ;
	uint8_t i ;
	UARTprintf("I:") ;
	for( i = 0 ; i < IRSEN_ADC_SENSORS_NUM ; i++)
		UARTprintf("%d,", tab[i]) ;
	UARTprintf("\n") ;
}

/**
 * Function to execute message from Bluetooth called 'E'
 * 'E' means : get encoders state, both Position and Velocity,
 *
 * This function sends to Bluetooth pos and vel of each encoder.
 */
void btFunE(uint8_t params[BT_TASKS_PARAM_NUM]) {
	UARTprintf("E:%d,%d,%d,%d\n", encLGetPos(), encRGetPos(), encLGetVel(), encRGetVel()) ;
}
