/*
 * Motors.c
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
#include "inc/hw_pwm.h"

#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"
#include "driverlib/interrupt.h"
#include "driverlib/pin_map.h"
#include "driverlib/pwm.h"

#include "Motors.h"

void motorsInit() {
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB) ;
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOC) ;
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD) ;
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE) ;
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF) ;
	SysCtlPeripheralEnable(SYSCTL_PERIPH_PWM0) ;
	SysCtlPeripheralEnable(SYSCTL_PERIPH_PWM1) ;
	SysCtlDelay(3) ;

	// GPIO configuration

	// PWM pins

	// MOT_0_PWM
	GPIOPinConfigure(GPIO_PF2_M1PWM6) ;
	GPIOPinTypePWM(GPIO_PORTF_BASE, GPIO_PIN_2) ;

	// MOT_1_PWM
	GPIOPinConfigure(GPIO_PD1_M1PWM1) ;
	GPIOPinTypePWM(GPIO_PORTD_BASE, GPIO_PIN_1) ;

	// other control pins
	GPIOPinTypeGPIOOutput(GPIO_PORTB_BASE, GPIO_PIN_3) ;
	GPIOPinTypeGPIOOutput(GPIO_PORTC_BASE, GPIO_PIN_4) ;
	GPIOPinTypeGPIOOutput(GPIO_PORTD_BASE, GPIO_PIN_3) ;
	GPIOPinTypeGPIOOutput(GPIO_PORTE_BASE, GPIO_PIN_2 | GPIO_PIN_1) ;
	GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_3) ;

	// initial gpio status

	motorsSetupM0(SOFT_STOP) ;
	motorsSetupM1(SOFT_STOP) ;

	// PWM Configuration

	PWMGenConfigure(MOT_0_BASE, MOT_0_GEN, PWM_GEN_MODE_DOWN | PWM_GEN_MODE_NO_SYNC) ;
	PWMGenConfigure(MOT_1_BASE, MOT_1_GEN, PWM_GEN_MODE_DOWN | PWM_GEN_MODE_NO_SYNC) ;
	PWMGenPeriodSet(MOT_0_BASE, MOT_0_GEN, MOT_PWM_PERIOD) ;
	PWMGenPeriodSet(MOT_1_BASE, MOT_1_GEN, MOT_PWM_PERIOD) ;

	motorsM0PwmSet(1) ;
	motorsM1PwmSet(1) ;
}

void motorsEnable() {
	PWMGenEnable(MOT_0_BASE, MOT_0_GEN) ;
	PWMGenEnable(MOT_1_BASE, MOT_1_GEN) ;

	// !STBY -> 1
	motorsM0StbySet() ;
	motorsM1StbySet() ;

	PWMOutputState(MOT_0_BASE, PWM_OUT_6_BIT | PWM_OUT_1_BIT, true) ;
}

void motorsDisable() {
	PWMGenDisable(MOT_0_BASE, MOT_0_GEN) ;
	PWMGenDisable(MOT_1_BASE, MOT_1_GEN) ;

	// !STBY -> 0
	motorsM0StbyClr() ;
	motorsM1StbyClr() ;

	PWMOutputState(MOT_0_BASE, PWM_OUT_6_BIT | PWM_OUT_1_BIT, false) ;

}

void motorsSetupM0(MOTORS_SETUP motorsSetup) {
	switch(motorsSetup) {
	case SOFT_STOP :
		// IN2 = 0
		motorsM0In2Clr() ;
		// IN1 = 0
		motorsM0In1Clr() ;
		break ;
	case COUNTER_CLOCKWISE :
		// IN2 = 1
		motorsM0In2Set() ;
		// IN1 = 0
		motorsM0In1Clr() ;
		break ;
	case CLOCKWISE :
		// IN2 = 0
		motorsM0In2Clr() ;
		// IN1 = 1
		motorsM0In1Set() ;
		break ;
	case HARD_STOP :
		// PWM = 0%
		// TODO : is it right ?
		PWMOutputState(MOT_0_BASE, PWM_OUT_6_BIT, false) ;
		PWMOutputState(MOT_1_BASE, PWM_OUT_1_BIT, false) ;
		break ;
	}
}

void motorsSetupM1(MOTORS_SETUP motorsSetup) {
	switch(motorsSetup) {
	case SOFT_STOP :
		// IN2 = 0
		motorsM1In2Clr();
		// IN1 = 0
		motorsM1In1Clr();
		break ;
	case COUNTER_CLOCKWISE :
		// IN2 = 1
		motorsM1In2Set() ;
		// IN1 = 0
		motorsM1In1Clr() ;
		break ;
	case CLOCKWISE :
		// IN2 = 0
		motorsM1In2Clr() ;
		// IN1 = 1
		motorsM1In1Set() ;
		break ;
	case HARD_STOP :
		// PWM = 0%
		// TODO : is it right ?
		PWMOutputState(MOT_0_BASE, PWM_OUT_6_BIT, false) ;
		PWMOutputState(MOT_1_BASE, PWM_OUT_1_BIT, false) ;
		break ;
	}
}
