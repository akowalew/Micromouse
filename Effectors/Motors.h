/*
 * Motors.h
 *
 *  Created on: 15 mar 2016
 *      Author: akowalew
 */

#ifndef EFFECTORS_MOTORS_H_
#define EFFECTORS_MOTORS_H_

typedef enum {
	SOFT_STOP,
	COUNTER_CLOCKWISE,
	CLOCKWISE,
	HARD_STOP
} MOTORS_SETUP;

void motorsInit() ;
void motorsEnable() ;
void motorsDisable() ;
void motorsSetupM0(MOTORS_SETUP motorsSetup) ;
void motorsSetupM1(MOTORS_SETUP motorsSetup) ;

#define motorsM0PwmSet(u32pwmVal) (PWMPulseWidthSet(MOT_0_BASE, PWM_OUT_6, (u32pwmVal)))
#define motorsM1PwmSet(u32pwmVal) (PWMPulseWidthSet(MOT_1_BASE, PWM_OUT_1, (u32pwmVal)))

#define motorsM0In1Set()	(GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_3, GPIO_PIN_3))
#define motorsM0In2Set()	(GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_3, GPIO_PIN_3))
#define motorsM0In1Clr()	(GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_3, 0) )
#define motorsM0In2Clr()	(GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_3, 0))

#define motorsM1In1Set()	(GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_1, GPIO_PIN_1))
#define motorsM1In2Set()	(GPIOPinWrite(GPIO_PORTD_BASE, GPIO_PIN_3, GPIO_PIN_3))
#define motorsM1In1Clr()	(GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_1, 0))
#define motorsM1In2Clr()	(GPIOPinWrite(GPIO_PORTD_BASE, GPIO_PIN_3, 0))

#define motorsM0StbySet()	(GPIOPinWrite(GPIO_PORTC_BASE, GPIO_PIN_4, GPIO_PIN_4) )
#define motorsM0StbyClr() 	(GPIOPinWrite(GPIO_PORTC_BASE, GPIO_PIN_4, 0))

#define motorsM1StbySet()	(GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_2, GPIO_PIN_2))
#define motorsM1StbyClr()	(GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_2, 0))

// MOT_0_PWM	// PF2 	- RightPWM, M1PWM6
// MOT_0_STBY  	// PC4	- Right!Stby
// MOT_0_IN_2	// PF3	- RightIn2
// MOT_0_IN_1	// PB3	- RightIn1

// MOT_1_PWM	// PD1 - LeftPWM, M1PWM1
// MOT_1_STBY	// PE2 - Left!Stby
// MOT_1_IN_2	// PD3 - LeftIn2
// MOT_1_IN_1	// PE1 - LeftIn1

#define MOT_0_BASE	PWM1_BASE
#define MOT_0_GEN	PWM_GEN_3

#define MOT_1_BASE	PWM1_BASE
#define MOT_1_GEN	PWM_GEN_0

#define MOT_PWM_PERIOD	1000

#endif /* EFFECTORS_MOTORS_H_ */
