/*
 * Encoders.c
 *
 *  Created on: 15 mar 2016
 *      Author: akowalew
 */

#include <stdint.h>
#include <stdbool.h>

#include "inc/hw_types.h"
#include "inc/hw_memmap.h"
#include "inc/tm4c123gh6pm.h"
#include "inc/hw_gpio.h"
#include "inc/hw_qei.h"

#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"
#include "driverlib/interrupt.h"
#include "driverlib/pin_map.h"
#include "driverlib/qei.h"

#include "Encoders.h"
#include "../uartstdio.h"

/*
void qei0Int() {
	QEIIntClear(QEI0_BASE, QEI_INTTIMER) ;

	//UARTprintf("Q0 %d %d %d\n", QEIPositionGet(QEI0_BASE), QEIVelocityGet(QEI0_BASE), QEIDirectionGet(QEI0_BASE)) ;
}

void qei1Int() {
	QEIIntClear(QEI1_BASE, QEI_INTTIMER) ;

	//UARTprintf("Q1 %d %d %d\n", QEIPositionGet(QEI1_BASE), QEIVelocityGet(QEI1_BASE),  QEIDirectionGet(QEI1_BASE)) ;
}
*/

uint32_t encRGetPos() 	{ return QEIPositionGet(QEI0_BASE);}
uint32_t encLGetPos() 	{return QEIPositionGet(QEI1_BASE);}
uint32_t encRGetVel() 	{ return QEIVelocityGet(QEI0_BASE);}
uint32_t encLGetVel()	{ return QEIVelocityGet(QEI1_BASE);}

void encInit() {
	SysCtlPeripheralEnable(SYSCTL_PERIPH_QEI0) ;
	SysCtlPeripheralEnable(SYSCTL_PERIPH_QEI1) ;
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOC) ;
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD) ;
	SysCtlDelay(3) ;

	// PD7, that is PhB0, is locked Pin
	HWREG(GPIO_PORTD_BASE + GPIO_O_LOCK) = GPIO_LOCK_KEY;
	HWREG(GPIO_PORTD_BASE + GPIO_O_CR)   |= 0x81;

	GPIOPinConfigure(GPIO_PD6_PHA0) ;
	GPIOPinConfigure(GPIO_PD7_PHB0) ;
	GPIOPinConfigure(GPIO_PC5_PHA1) ;
	GPIOPinConfigure(GPIO_PC6_PHB1) ;

	GPIOPinTypeQEI(GPIO_PORTD_BASE, GPIO_PIN_6 | GPIO_PIN_7) ;
	GPIOPinTypeQEI(GPIO_PORTC_BASE, GPIO_PIN_5 | GPIO_PIN_6) ;

	QEIConfigure(QEI0_BASE, QEI_CONFIG_CAPTURE_A_B | QEI_CONFIG_NO_RESET |
			QEI_CONFIG_QUADRATURE | QEI_CONFIG_SWAP, ENC_MAX_COUNT) ;
	QEIConfigure(QEI1_BASE, QEI_CONFIG_CAPTURE_A_B | QEI_CONFIG_NO_RESET |
			QEI_CONFIG_QUADRATURE | QEI_CONFIG_SWAP, ENC_MAX_COUNT) ;

	QEIVelocityConfigure(QEI0_BASE, QEI_VELDIV_1, ENC_VELOCITY_PERIOD) ;
	QEIVelocityConfigure(QEI1_BASE, QEI_VELDIV_1, ENC_VELOCITY_PERIOD) ;

	/*
	QEIIntRegister(QEI0_BASE, qei0Int) ;
	QEIIntRegister(QEI1_BASE, qei1Int) ;*/
}

void encEnable() {
	QEIEnable(QEI0_BASE) ;
	QEIEnable(QEI1_BASE) ;

	QEIVelocityEnable(QEI0_BASE) ;
	QEIVelocityEnable(QEI1_BASE) ;

	/*
	QEIIntEnable(QEI0_BASE, QEI_INTTIMER) ;
	QEIIntClear(QEI0_BASE, QEI_INTTIMER) ;

	QEIIntEnable(QEI1_BASE, QEI_INTTIMER) ;
	QEIIntClear(QEI1_BASE, QEI_INTTIMER) ; */
}

void encDisable() {
	QEIDisable(QEI0_BASE) ;
	QEIDisable(QEI1_BASE) ;

	QEIVelocityDisable(QEI0_BASE) ;
	QEIVelocityDisable(QEI1_BASE) ;
}
