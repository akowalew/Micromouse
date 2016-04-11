/*
 * IRSensors.c
 *
 *  Created on: 8 mar 2016
 *      Author: akowalew
 */

#ifndef SENSORS_IRSENSORS_C_
#define SENSORS_IRSENSORS_C_

#include "IRSensors.h"

struct {
	volatile uint32_t values[IRSEN_ADC_SENSORS_NUM] ;
	volatile uint8_t i ;

	uint32_t irPwmVal ;
} irSenStruct;

typedef struct  {
	uint32_t sensorAdcCh ;
	uint32_t sensorPwmBase ;
	uint32_t sensorPwmOutNum ;
	uint32_t sensorPwmOutBitNum ;
} SensConf ;

const SensConf sensorsTable[IRSEN_ADC_SENSORS_NUM] = {
		{ IRSEN_ADC_CH_Q3, PWM1_BASE, PWM_OUT_5 , PWM_OUT_5_BIT } ,
		{ IRSEN_ADC_CH_Q2, PWM1_BASE, PWM_OUT_4 , PWM_OUT_4_BIT } ,
		{ IRSEN_ADC_CH_Q1, PWM0_BASE, PWM_OUT_1 , PWM_OUT_1_BIT } ,
		{ IRSEN_ADC_CH_Q4, PWM0_BASE, PWM_OUT_0 , PWM_OUT_0_BIT }
};

void irSenTimInt() ;
void irSenAdcInt() ;

inline uint32_t irSenGetVal(uint8_t whichSensor) {
	return irSenStruct.values[whichSensor] ;
}

void irSenGetAllVals(uint32_t tab[IRSEN_ADC_SENSORS_NUM]) {
	uint8_t i ;
	for(i = 0 ; i < IRSEN_ADC_SENSORS_NUM ; i++)
		tab[i] = irSenStruct.values[i] ;
}

void irSenInit() {

	irSenStruct.i = 0 ;
	irSenStruct.irPwmVal = IRSEN_DEFAULT_PWM ;

	// init PWM channels
	SysCtlPeripheralEnable(SYSCTL_PERIPH_PWM0) ;
	SysCtlPeripheralEnable(SYSCTL_PERIPH_PWM1) ;
	SysCtlDelay(3) ;

	// IR1, IR4
	PWMGenConfigure(PWM1_BASE, PWM_GEN_2, PWM_GEN_MODE_DOWN | PWM_GEN_MODE_NO_SYNC);
	PWMGenPeriodSet(PWM1_BASE, PWM_GEN_2, IRSEN_PWM_PERIOD) ;
	PWMPulseWidthSet(PWM1_BASE, PWM_OUT_5, irSenStruct.irPwmVal) ;
	PWMPulseWidthSet(PWM1_BASE, PWM_OUT_4, irSenStruct.irPwmVal) ;

	// IR2, IR3
	PWMGenConfigure(PWM0_BASE, PWM_GEN_0, PWM_GEN_MODE_DOWN | PWM_GEN_MODE_NO_SYNC);
	PWMGenPeriodSet(PWM0_BASE, PWM_GEN_0, IRSEN_PWM_PERIOD) ;
	PWMPulseWidthSet(PWM0_BASE, PWM_OUT_0, irSenStruct.irPwmVal) ;
	PWMPulseWidthSet(PWM0_BASE, PWM_OUT_1, irSenStruct.irPwmVal) ;

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
			ADC_TRIGGER_TIMER, IRSEN_ADC_PRIORITY) ;
	ADCSequenceStepConfigure(IRSEN_ADC_BASE, IRSEN_ADC_SEQ_NUM,
			0, ADC_CTL_END | ADC_CTL_IE | IRSEN_ADC_CH_Q1) ;
	ADCHardwareOversampleConfigure(IRSEN_ADC_BASE, IRSEN_ADC_OVERSAMP) ;
	ADCIntRegister(IRSEN_ADC_BASE, IRSEN_ADC_SEQ_NUM, irSenAdcInt) ;

	/* Configure Timer
		In interrupt of this timer mcu has to configure an ADC channel
		and to activate PWM channels
		*/
	SysCtlPeripheralEnable(IRSEN_TIMER_PERIPH) ;
	SysCtlDelay(3) ;

	TimerConfigure(IRSEN_TIMER_BASE, TIMER_CFG_PERIODIC) ;
	TimerIntRegister(IRSEN_TIMER_BASE, TIMER_BOTH, irSenTimInt) ;
	TimerLoadSet(IRSEN_TIMER_BASE, TIMER_A, IRSEN_TIMER_LD_VAL) ;
	TimerMatchSet(IRSEN_TIMER_BASE, TIMER_A, IRSEN_TIMER_LD_VAL) ;

	/* Configure another Timer. Delay Timer
	 * This timer is only to delay some time, in order to generate light from IR diode
	 * This is one-shot timer. After delay, ADC is triggered to start conversion, by hardware
	 */

	SysCtlPeripheralEnable(IRSEN_TIMER_DELAY_PERIPH) ;
	SysCtlDelay(3) ;

	TimerConfigure(IRSEN_TIMER_DELAY_BASE, TIMER_CFG_ONE_SHOT) ;
	TimerADCEventSet(IRSEN_TIMER_DELAY_BASE, TIMER_ADC_TIMEOUT_A) ;
	TimerControlTrigger(IRSEN_TIMER_DELAY_BASE, TIMER_A, true) ;
	TimerLoadSet(IRSEN_TIMER_DELAY_BASE, TIMER_A, IRSEN_TIMER_DELAY_LD_VAL) ;

}

void irSenEnable() {
	// enable PWM channels

	// IR1, IR4
	PWMGenEnable(PWM1_BASE, PWM_GEN_2) ;
	//PWMOutputState(PWM1_BASE, PWM_OUT_5_BIT | PWM_OUT_4_BIT, true);

	// IR2, IR3
	PWMGenEnable(PWM0_BASE, PWM_GEN_0) ;
	//PWMOutputState(PWM0_BASE, PWM_OUT_0_BIT | PWM_OUT_1_BIT, true);

	ADCSequenceEnable(IRSEN_ADC_BASE, IRSEN_ADC_SEQ_NUM) ;
	ADCIntEnable(IRSEN_ADC_BASE, IRSEN_ADC_SEQ_NUM) ;
	ADCIntClear(IRSEN_ADC_BASE, IRSEN_ADC_SEQ_NUM) ;

	TimerEnable(IRSEN_TIMER_BASE, TIMER_BOTH) ;
	TimerIntEnable(IRSEN_TIMER_BASE, TIMER_TIMA_TIMEOUT) ;
	TimerIntClear(IRSEN_TIMER_BASE, TIMER_TIMA_TIMEOUT) ;

	// there is no enable function for Delay Timer.
}

void irSenDisable() {
	//disable PWM channels

	// IR1, IR4
	PWMGenDisable(PWM1_BASE, PWM_GEN_2) ;
	PWMOutputState(PWM1_BASE, PWM_OUT_5_BIT | PWM_OUT_4_BIT, false);

	// IR2, IR3
	PWMGenDisable(PWM0_BASE, PWM_GEN_0) ;
	PWMOutputState(PWM0_BASE, PWM_OUT_0_BIT | PWM_OUT_1_BIT, false);

	ADCSequenceDisable(IRSEN_ADC_BASE, IRSEN_ADC_SEQ_NUM) ;
	ADCIntDisable(IRSEN_ADC_BASE, IRSEN_ADC_SEQ_NUM) ;

	TimerDisable(IRSEN_TIMER_BASE, TIMER_BOTH) ;
	TimerIntDisable(IRSEN_TIMER_BASE, TIMER_TIMA_TIMEOUT) ;
}

/**
 * Long period Timer interrupt procedure
 *
 * This code is executed when long period Timer has reached its timeout
 * First, current IR led is powered on
 * Then, we're setting up current channel for sensor
 * At the end, short delay Timer is enabled to count time for start ADC conversion
 */
void irSenTimInt() {
	TimerIntClear(IRSEN_TIMER_BASE, TIMER_TIMA_TIMEOUT) ;

	uint8_t i = irSenStruct.i ;

	// Code to enable IR diode
	PWMOutputState(
			sensorsTable[i].sensorPwmBase,
			sensorsTable[i].sensorPwmOutBitNum,
			true
	) ;

	// configure current Channel
	ADCSequenceStepConfigure(
			IRSEN_ADC_BASE,
			IRSEN_ADC_SEQ_NUM,
			IRSEN_ADC_STEP_NUM,
			ADC_CTL_END | ADC_CTL_IE | sensorsTable[i].sensorAdcCh ) ;

	// start the delay timer
	TimerDisable(IRSEN_TIMER_DELAY_BASE, TIMER_BOTH) ;
	TimerEnable(IRSEN_TIMER_DELAY_BASE, TIMER_BOTH) ;
}
#include "inc/hw_timer.h"
/**
 * ADC interrupt procedure
 *
 * This code is executed when ADC conversion is completed
 * At First, program has to update table with values od ADC
 * Secondly, current IR diode must be switched off
 * At the end, index of current sensor is icremented
 *
 *
 */
void irSenAdcInt() {
	ADCIntClear(IRSEN_ADC_BASE, IRSEN_ADC_SEQ_NUM) ;

	// get the current index copy and increment original
	uint8_t i = irSenStruct.i++ ;

	uint32_t tmpVal = irSenStruct.values[i] ;

	// save data from sensor
	ADCSequenceDataGet(
			IRSEN_ADC_BASE,
			IRSEN_ADC_SEQ_NUM,
			&tmpVal) ;

	// code to Disable IR diode
	PWMOutputState(
			sensorsTable[i].sensorPwmBase,
			sensorsTable[i].sensorPwmOutBitNum,
			false
	) ;

	// if there is another sensors to check, run once again irSenTimInt procedure
	// if not, stop and wait for next trigger by long period Timer
	if(i == 4)
		irSenStruct.i = 0 ;
	else
		irSenTimInt() ;
}

#endif /* SENSORS_IRSENSORS_C_ */
