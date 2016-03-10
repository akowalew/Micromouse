/*
 * IRSensors.c
 *
 *  Created on: 8 mar 2016
 *      Author: akowalew
 */

#ifndef SENSORS_IRSENSORS_C_
#define SENSORS_IRSENSORS_C_

#include <stdint.h>
#include <stdbool.h>

#include "inc/hw_types.h"
#include "inc/hw_memmap.h"
#include "inc/tm4c123gh6pm.h"
#include "inc/hw_gpio.h"
#include "inc/hw_timer.h"
#include "inc/hw_uart.h"
#include "inc/hw_adc.h"

#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"
#include "driverlib/interrupt.h"
#include "driverlib/timer.h"
#include "driverlib/pin_map.h"
#include "driverlib/uart.h"
#include "driverlib/pwm.h"
#include "driverlib/adc.h"

#include "IRSensors.h"

#include "../uartstdio.h"

struct {
	volatile uint32_t values[IRSEN_ADC_CH_NUM] ;
	volatile uint8_t i ;

} irSenStruct;

const uint32_t chsTable[IRSEN_ADC_CH_NUM] = {
		IRSEN_ADC_CH_Q1,
		IRSEN_ADC_CH_Q2,
		IRSEN_ADC_CH_Q3,
		IRSEN_ADC_CH_Q4
};

void irSenTimInt() ;
void irSenAdcInt() ;

void irSenInit() {
	// init PWM channels
	SysCtlPeripheralEnable(SYSCTL_PERIPH_PWM0) ;
	SysCtlPeripheralEnable(SYSCTL_PERIPH_PWM1) ;
	SysCtlDelay(3) ;

	// IR1, IR4
	PWMGenConfigure(PWM1_BASE, PWM_GEN_2, PWM_GEN_MODE_DOWN | PWM_GEN_MODE_NO_SYNC);
	PWMGenPeriodSet(PWM1_BASE, PWM_GEN_2, IRSEN_PWM_PERIOD) ;
	PWMPulseWidthSet(PWM1_BASE, PWM_OUT_5, 1) ;
	PWMPulseWidthSet(PWM1_BASE, PWM_OUT_4, 1) ;

	// IR2, IR3
	PWMGenConfigure(PWM0_BASE, PWM_GEN_0, PWM_GEN_MODE_DOWN | PWM_GEN_MODE_NO_SYNC);
	PWMGenPeriodSet(PWM0_BASE, PWM_GEN_0, IRSEN_PWM_PERIOD) ;
	PWMPulseWidthSet(PWM0_BASE, PWM_OUT_0, 1) ;
	PWMPulseWidthSet(PWM0_BASE, PWM_OUT_1, 1) ;

	// GPIO in PWM mode
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF) ;
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB) ;
	SysCtlDelay(3) ;

	// PF0 IS LOCKED PIN
	HWREG(GPIO_PORTF_BASE + GPIO_O_LOCK) = GPIO_LOCK_KEY;
	HWREG(GPIO_PORTF_BASE + GPIO_O_CR)  |= 0x01;

	GPIOPinConfigure(GPIO_PF1_M1PWM5) ;
	GPIOPinConfigure(GPIO_PF0_M1PWM4) ;
	GPIOPinConfigure(GPIO_PB6_M0PWM0) ;
	GPIOPinConfigure(GPIO_PB7_M0PWM1) ;

	GPIOPinTypePWM(GPIO_PORTF_BASE, GPIO_PIN_1 | GPIO_PIN_0) ;
	GPIOPinTypePWM(GPIO_PORTB_BASE, GPIO_PIN_6 | GPIO_PIN_7) ;

	// configure ADC pins
	SysCtlPeripheralEnable(IRSEN_ADC_PERIPH) ;
	SysCtlDelay(3) ;

	ADCSequenceConfigure(IRSEN_ADC_BASE, IRSEN_ADC_SEQ_NUM,
			ADC_TRIGGER_PROCESSOR, IRSEN_ADC_PRIORITY) ;
	ADCSequenceStepConfigure(IRSEN_ADC_BASE, IRSEN_ADC_SEQ_NUM,
			0, ADC_CTL_END | ADC_CTL_IE | IRSEN_ADC_CH_Q1) ;
	ADCHardwareOversampleConfigure(IRSEN_ADC_BASE, IRSEN_ADC_OVERSAMP) ;
	ADCIntRegister(IRSEN_ADC_BASE, IRSEN_ADC_SEQ_NUM, irSenAdcInt) ;

	// Configure Timer for ADC trigger
	SysCtlPeripheralEnable(IRSEN_TIMER_PERIPH) ;
	SysCtlDelay(3) ;

	TimerConfigure(IRSEN_TIMER_BASE, TIMER_CFG_PERIODIC) ;
	TimerIntRegister(IRSEN_TIMER_BASE, TIMER_BOTH, irSenTimInt) ;
	TimerLoadSet(IRSEN_TIMER_BASE, TIMER_A, IRSEN_TIMER_LD_VAL) ;
	TimerMatchSet(IRSEN_TIMER_BASE, TIMER_A, IRSEN_TIMER_LD_VAL) ;

	irSenStruct.i = 0 ;
}

void irSenEnable() {
	// enable PWM channels

	/*
	// IR1, IR4
	PWMGenEnable(PWM1_BASE, PWM_GEN_2) ;
	PWMOutputState(PWM1_BASE, PWM_OUT_5_BIT | PWM_OUT_4_BIT, true);

	// IR2, IR3
	PWMGenEnable(PWM0_BASE, PWM_GEN_0) ;
	PWMOutputState(PWM0_BASE, PWM_OUT_0_BIT | PWM_OUT_1_BIT, true);
*/

	ADCSequenceEnable(IRSEN_ADC_BASE, IRSEN_ADC_SEQ_NUM) ;
	ADCIntEnable(IRSEN_ADC_BASE, IRSEN_ADC_SEQ_NUM) ;
	ADCIntClear(IRSEN_ADC_BASE, IRSEN_ADC_SEQ_NUM) ;

	TimerEnable(IRSEN_TIMER_BASE, TIMER_BOTH) ;
	TimerIntEnable(IRSEN_TIMER_BASE, TIMER_TIMA_TIMEOUT) ;
	TimerIntClear(IRSEN_TIMER_BASE, TIMER_TIMA_TIMEOUT) ;
}

void irSenTimInt() {
	TimerIntClear(IRSEN_TIMER_BASE, TIMER_TIMA_TIMEOUT) ;

	// Code to enable IR diode
	//
	//

	//ADCProcessorTrigger(IRSEN_ADC_BASE, IRSEN_ADC_SEQ_NUM) ;
}

void irSenAdcInt() {
	ADCIntClear(IRSEN_ADC_BASE, IRSEN_ADC_SEQ_NUM) ;

	/*ADCSequenceDataGet(
			IRSEN_ADC_BASE,
			IRSEN_ADC_SEQ_NUM,
			&irSenStruct.values[irSenStruct.i++]) ;

	ADCSequenceStepConfigure(
			IRSEN_ADC_BASE,
			IRSEN_ADC_SEQ_NUM,
			IRSEN_ADC_STEP_NUM,
			ADC_CTL_END | ADC_CTL_IE | irSenStruct.values[irSenStruct.i] ) ;

*/
	// Code to disable IR diode
	//
	//



}

#endif /* SENSORS_IRSENSORS_C_ */
