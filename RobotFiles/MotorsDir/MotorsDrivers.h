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

// MOT_0_PWM	// PF2 	- RightPWM, M1PWM6
// MOT_0_STBY  	// PC4	- Right!Stby
// MOT_0_IN_2	// PF3	- RightIn2
// MOT_0_IN_1	// PB3	- RightIn1

// MOT_1_PWM	MOTORS_SETUP// PD1 - LeftPWM, M1PWM1
// MOT_1_STBY	// PE2 - Left!Stby
// MOT_1_IN_2	// PD3 - LeftIn2
// MOT_1_IN_1	// PE1 - LeftIn1

typedef enum {
	SOFT_STOP = 0,
	COUNTER_CLOCKWISE = 1,
	CLOCKWISE = 2,
	HARD_STOP = 3
} MOTORS_SETUP;

#define MOT_PWM_RAMP_ENABLE

void motDriversInit() ;
void motDriversEnable() ;
void motDriversDisable() ;

void motStateSetL(MOTORS_SETUP motorsSetup) ;
void motStateSetR(MOTORS_SETUP motorsSetup) ;

void motDutyCycleSetL(uint32_t u32pwmVal) ;
void motDutyCycleSetR(uint32_t u32pwmVal) ;

#define motIn1SetL()	(GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_3, GPIO_PIN_3))
#define motIn2SetL()	(GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_3, GPIO_PIN_3))
#define motIn1ClrL()	(GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_3, 0) )
#define motIn2ClrL()	(GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_3, 0))

#define motIn1SetR()	(GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_1, GPIO_PIN_1))
#define motIn2SetR()	(GPIOPinWrite(GPIO_PORTD_BASE, GPIO_PIN_3, GPIO_PIN_3))
#define motIn1ClrR()	(GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_1, 0))
#define motIn2ClrR()	(GPIOPinWrite(GPIO_PORTD_BASE, GPIO_PIN_3, 0))

#define motStbySetL()	(GPIOPinWrite(GPIO_PORTC_BASE, GPIO_PIN_4, GPIO_PIN_4) )
#define motStbyClrL() 	(GPIOPinWrite(GPIO_PORTC_BASE, GPIO_PIN_4, 0))

#define motStbySetR()	(GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_2, GPIO_PIN_2))
#define motStbyClrR()	(GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_2, 0))

#define MOT_0_BASE	PWM1_BASE
#define MOT_0_GEN	PWM_GEN_3

#define MOT_1_BASE	PWM1_BASE
#define MOT_1_GEN	PWM_GEN_0

#define MOT_PWM_PERIOD	1000
#define MOT_PWM_RAMP_VAL	((MOT_PWM_PERIOD * 8) / 10)

#endif
