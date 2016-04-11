/*
 * IRSensors.h
 *
 *  Created on: 8 mar 2016
 *      Author: akowalew
 */

#ifndef ROBOTFILES_SENSORS_IRSENSORS_H_
#define ROBOTFILES_SENSORS_IRSENSORS_H_

#include "../includeHeader.h"

#include "inc/hw_gpio.h"
#include "driverlib/pin_map.h"

#include "driverlib/adc.h"
#include "driverlib/pwm.h"
#include "driverlib/gpio.h"
#include "driverlib/interrupt.h"
#include "driverlib/timer.h"

/*
 * 	IR phototransistors ADC pins

	Q1_IN	PE0	AIN3
	Q2_IN	PB5	AIN11
	Q3_IN	PE4	AIN9
	Q4_IN	PE3	AIN0

	IR diodes, PWM pins

	IR1 (aka IR3_OUT)	PB7	M0PWM1
	IR2 (aka IR4_OUT)	PF0	M1PWM4
	IR3 (aka IR1_OUT)	PF1	M1PWM5
	IR4 (aka IR2_OUT)	PB6	M0WPM0

	----	Remember that PF0 is locked pin.
 */

#define IRSEN_PWM_PERIOD	400
#define IRSEN_DEFAULT_PWM	300
// Frequency of PWM can be calculated : 80MHz / IRSEN_PWM_PERIOD(400) = 0.05MHz = 50kHz

#define IRSEN_ADC_PERIPH	SYSCTL_PERIPH_ADC1
#define	 IRSEN_ADC_BASE		ADC1_BASE
#define IRSEN_ADC_PRIORITY	0
#define IRSEN_ADC_OVERSAMP	8
#define IRSEN_ADC_SEQ_NUM	3
#define IRSEN_ADC_CH_Q1		ADC_CTL_CH3
#define IRSEN_ADC_CH_Q2		ADC_CTL_CH11
#define IRSEN_ADC_CH_Q3		ADC_CTL_CH9
#define IRSEN_ADC_CH_Q4		ADC_CTL_CH0
#define IRSEN_ADC_SENSORS_NUM	4
#define IRSEN_ADC_STEP_NUM	0

// TIMER should be 32-bit.
#define IRSEN_TIMER_PERIPH	SYSCTL_PERIPH_TIMER0
#define IRSEN_TIMER_BASE	TIMER0_BASE
#define IRSEN_TIMER_LD_VAL	400000

// 80MHz -> 12,5ns
// 400000 * 12,5ns = 5ms

#define IRSEN_TIMER_DELAY_PERIPH	SYSCTL_PERIPH_TIMER1
#define IRSEN_TIMER_DELAY_BASE		TIMER1_BASE
#define IRSEN_TIMER_DELAY_LD_VAL	1600

// 1600 * 12,5ns = 20000 ns = 20 us

void irSenInit() ;
void irSenEnable() ;
void irSenDisable() ;
uint32_t irSenGetVal(uint8_t whichSensor) ;
void irSenGetAllVals(uint32_t tab[IRSEN_ADC_SENSORS_NUM]) ;

#endif /* ROBOTFILES_SENSORS_IRSENSORS_H_ */
