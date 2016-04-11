/*
 * MotorsDrivers.h
 *
 *  Created on: 4 kwi 2016
 *      Author: akowalew
 */

#ifndef MOTORS_DRIVERS_H
#define MOTORS_DRIVERS_H

#include "Motors.h"

#include "inc/hw_gpio.h"
#include "inc/hw_pwm.h"

#include "driverlib/gpio.h"
#include "driverlib/interrupt.h"
#include "driverlib/pin_map.h"
#include "driverlib/pwm.h"

typedef enum {
	SOFT_STOP = 0,
	COUNTER_CLOCKWISE = 1,
	CLOCKWISE = 2,
	HARD_STOP = 3
} MOTORS_SETUP;


#define motorsMLIn1Set()	(GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_3, GPIO_PIN_3))
#define motorsMLIn2Set()	(GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_3, GPIO_PIN_3))
#define motorsMLIn1Clr()	(GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_3, 0) )
#define motorsMLIn2Clr()	(GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_3, 0))

#define motorsMRIn1Set()	(GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_1, GPIO_PIN_1))
#define motorsMRIn2Set()	(GPIOPinWrite(GPIO_PORTD_BASE, GPIO_PIN_3, GPIO_PIN_3))
#define motorsMRIn1Clr()	(GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_1, 0))
#define motorsMRIn2Clr()	(GPIOPinWrite(GPIO_PORTD_BASE, GPIO_PIN_3, 0))

#define motorsMLStbySet()	(GPIOPinWrite(GPIO_PORTC_BASE, GPIO_PIN_4, GPIO_PIN_4) )
#define motorsMLStbyClr() 	(GPIOPinWrite(GPIO_PORTC_BASE, GPIO_PIN_4, 0))

#define motorsMRStbySet()	(GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_2, GPIO_PIN_2))
#define motorsMRStbyClr()	(GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_2, 0))

// MOT_0_PWM	// PF2 	- RightPWM, M1PWM6
// MOT_0_STBY  	// PC4	- Right!Stby
// MOT_0_IN_2	// PF3	- RightIn2
// MOT_0_IN_1	// PB3	- RightIn1

// MOT_1_PWM	MOTORS_SETUP// PD1 - LeftPWM, M1PWM1
// MOT_1_STBY	// PE2 - Left!Stby
// MOT_1_IN_2	// PD3 - LeftIn2
// MOT_1_IN_1	// PE1 - LeftIn1

#define MOT_0_BASE	PWM1_BASE
#define MOT_0_GEN	PWM_GEN_3

#define MOT_1_BASE	PWM1_BASE
#define MOT_1_GEN	PWM_GEN_0

#define MOT_PWM_PERIOD	1000
#define MOT_PWM_0_8_VAL	((MOT_PWM_PERIOD * 8) / 10)

void motorsDriversInit() ;
void motorsDriversEnable() ;
void motorsDriversDisable() ;

void motorsSetupML(MOTORS_SETUP motorsSetup) ;
void motorsSetupMR(MOTORS_SETUP motorsSetup) ;

void motorsMLPwmSet(uint32_t u32pwmVal) ;
void motorsMRPwmSet(uint32_t u32pwmVal) ;


#endif
