/*
 * MotorsDrivers.c
 *
 *  Created on: 4 kwi 2016
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


#include "MotorsDrivers.h"

void motorsDriversInit() {
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

	// PWM Configuration

	PWMGenConfigure(MOT_0_BASE, MOT_0_GEN, PWM_GEN_MODE_DOWN | PWM_GEN_MODE_NO_SYNC) ;
	PWMGenConfigure(MOT_1_BASE, MOT_1_GEN, PWM_GEN_MODE_DOWN | PWM_GEN_MODE_NO_SYNC) ;
	PWMGenPeriodSet(MOT_0_BASE, MOT_0_GEN, MOT_PWM_PERIOD) ;
	PWMGenPeriodSet(MOT_1_BASE, MOT_1_GEN, MOT_PWM_PERIOD) ;
}

void motorsDriversEnable() {
	PWMGenEnable(MOT_0_BASE, MOT_0_GEN) ;
	PWMGenEnable(MOT_1_BASE, MOT_1_GEN) ;

	// !STBY -> 1
	motorsMLStbySet() ;
	motorsMRStbySet() ;

	PWMOutputState(MOT_0_BASE, PWM_OUT_6_BIT | PWM_OUT_1_BIT, true) ;
}

void motorsDriversDisable() {
	PWMGenDisable(MOT_0_BASE, MOT_0_GEN) ;
	PWMGenDisable(MOT_1_BASE, MOT_1_GEN) ;

	// !STBY -> 0
	motorsMLStbyClr() ;
	motorsMRStbyClr() ;

	PWMOutputState(MOT_0_BASE, PWM_OUT_6_BIT | PWM_OUT_1_BIT, false) ;
}


void motorsSetupML(MOTORS_SETUP motorsSetup) {
	switch(motorsSetup) {
	case SOFT_STOP :
		// IN2 = 0
		motorsMLIn2Clr() ;
		// IN1 = 0
		motorsMLIn1Clr() ;
		break ;
	case COUNTER_CLOCKWISE :
		// IN2 = 1
		motorsMLIn2Set() ;
		// IN1 = 0
		motorsMLIn1Clr() ;
		break ;
	case CLOCKWISE :
		// IN2 = 0
		motorsMLIn2Clr() ;
		// IN1 = 1
		motorsMLIn1Set() ;
		break ;
	case HARD_STOP :
		// PWM = 0%
		motorsMLPwmSet(1) ;
		break ;
	}
}

void motorsSetupMR(MOTORS_SETUP motorsSetup) {
	switch(motorsSetup) {
	case SOFT_STOP :
		// IN2 = 0
		motorsMRIn2Clr();
		// IN1 = 0
		motorsMRIn1Clr();
		break ;
	case COUNTER_CLOCKWISE :
		// IN2 = 1
		motorsMRIn2Set() ;
		// IN1 = 0
		motorsMRIn1Clr() ;
		break ;
	case CLOCKWISE :
		// IN2 = 0
		motorsMRIn2Clr() ;
		// IN1 = 1
		motorsMRIn1Set() ;
		break ;
	case HARD_STOP :
		// PWM = 0%
		motorsMRPwmSet(1) ;
		break ;
	}
}

 void motorsMLPwmSet(uint32_t u32pwmVal) {
	if(u32pwmVal > MOT_PWM_0_8_VAL)
		u32pwmVal = MOT_PWM_0_8_VAL ;
	else if(u32pwmVal == 0)
		u32pwmVal = 1;
	PWMPulseWidthSet(MOT_0_BASE, PWM_OUT_6, (u32pwmVal)) ;
}

 void motorsMRPwmSet(uint32_t u32pwmVal) {
	if(u32pwmVal > MOT_PWM_0_8_VAL)
		u32pwmVal = MOT_PWM_0_8_VAL ;
	else if(u32pwmVal == 0)
		u32pwmVal = 1;
	PWMPulseWidthSet(MOT_1_BASE, PWM_OUT_1, (u32pwmVal));
}
