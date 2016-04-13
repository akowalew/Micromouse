/*
 * MotorsDrivers.c
 *
 *  Created on: 4 kwi 2016
 *      Author: akowalew
 */
#include "MotorsDrivers.h"

void motDriversInit() {
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

void motDriversEnable() {
	PWMGenEnable(MOT_0_BASE, MOT_0_GEN) ;
	PWMGenEnable(MOT_1_BASE, MOT_1_GEN) ;

	// !STBY -> 1
	motStbySetL() ;
	motStbySetR() ;

	PWMOutputState(MOT_0_BASE, PWM_OUT_6_BIT | PWM_OUT_1_BIT, true) ;
}

void motDriversDisable() {
	PWMGenDisable(MOT_0_BASE, MOT_0_GEN) ;
	PWMGenDisable(MOT_1_BASE, MOT_1_GEN) ;

	// !STBY -> 0
	motStbyClrL() ;
	motStbyClrR() ;

	PWMOutputState(MOT_0_BASE, PWM_OUT_6_BIT | PWM_OUT_1_BIT, false) ;
}


void motStateSetL(MOTORS_SETUP motorsSetup) {
	switch(motorsSetup) {
	case SOFT_STOP :
		// IN2 = 0
		motIn2ClrL() ;
		// IN1 = 0
		motIn1ClrL() ;
		break ;
	case COUNTER_CLOCKWISE :
		// IN2 = 1
		motIn2SetL() ;
		// IN1 = 0
		motIn1ClrL() ;
		break ;
	case CLOCKWISE :
		// IN2 = 0
		motIn2ClrL() ;
		// IN1 = 1
		motIn1SetL() ;
		break ;
	case HARD_STOP :
		// PWM = 0%
		motDutyCycleSetL(1) ;
		break ;
	}
}

void motStateSetR(MOTORS_SETUP motorsSetup) {
	switch(motorsSetup) {
	case SOFT_STOP :
		// IN2 = 0
		motIn2ClrR();
		// IN1 = 0
		motIn1ClrR();
		break ;
	case COUNTER_CLOCKWISE :
		// IN2 = 1
		motIn2SetR() ;
		// IN1 = 0
		motIn1ClrR() ;
		break ;
	case CLOCKWISE :
		// IN2 = 0
		motIn2ClrR() ;
		// IN1 = 1
		motIn1SetR() ;
		break ;
	case HARD_STOP :
		// PWM = 0%
		motDutyCycleSetR(1) ;
		break ;
	}
}

 void motDutyCycleSetL(uint32_t u32pwmVal) {
	if(u32pwmVal > MOT_PWM_0_8_VAL)
		u32pwmVal = MOT_PWM_0_8_VAL ;
	else if(u32pwmVal == 0)
		u32pwmVal = 1;
	PWMPulseWidthSet(MOT_0_BASE, PWM_OUT_6, (u32pwmVal)) ;
}

 void motDutyCycleSetR(uint32_t u32pwmVal) {
	if(u32pwmVal > MOT_PWM_0_8_VAL)
		u32pwmVal = MOT_PWM_0_8_VAL ;
	else if(u32pwmVal == 0)
		u32pwmVal = 1;
	PWMPulseWidthSet(MOT_1_BASE, PWM_OUT_1, (u32pwmVal));
}
