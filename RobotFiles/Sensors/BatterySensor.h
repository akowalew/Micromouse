/*
 * BatterySensor.h
 *
 *  Created on: 7 mar 2016
 *      Author: akowalew
 */

#ifndef ROBOTFILES_SENSORS_BATTERYSENSOR_H_
#define ROBOTFILES_SENSORS_BATTERYSENSOR_H_

/*
 * Battery Sensor
 *
 */

#include "../includeHeader.h"

#include "driverlib/adc.h"
#include "driverlib/pin_map.h"
#include "driverlib/interrupt.h"
#include "driverlib/timer.h"

#include "../Communication/topUtils.h"

// Voltage Divider Vout = Vin * R2 / (R2 + R1)

#define DIVIDER_8_4_R1	3.9 // KOhm
#define DIVIDER_8_4_R2	2.181	// KOhm

#define DIVIDER_8_4	(DIVIDER_8_4_R2 / (DIVIDER_8_4_R1 + DIVIDER_8_4_R2))

#define DIVIDER_4_2_R1	1.996	// KOhm
#define DIVIDER_4_2_R2	4.72	// KOhm

#define DIVIDER_4_2	(DIVIDER_4_2_R2 / (DIVIDER_4_2_R1 + DIVIDER_4_2_R2))

/*
 * Balancer pins :
 * 	1st Cell	-	PB4	AIN10
 * 	2nd Cell	-	PE5	AIN8
 */

// ADC configuration
#define BATSENS_ADC_BASE		ADC0_BASE
#define BATSENS_ADC_PERIPH		SYSCTL_PERIPH_ADC0
#define BATSENS_ADC_SEQ_NUM		2
#define BATSENS_ADC_PRIORITY	1
#define BATSENS_ADC_1ST_CH		ADC_CTL_CH10
#define BATSENS_ADC_2ND_CH		ADC_CTL_CH8
#define BATSENS_ADC_OVERSAMP	16

// Timer trigger configuration
#define BATSENS_TIMER_PERIPH	SYSCTL_PERIPH_TIMER3
#define BATSENS_TIMER_BASE		TIMER3_BASE
#define BATSENS_TIMER_PERIOD	80000000UL

// 80MHz -> 12.5 ns per cycle
// 80000000 * 12.5 ns = 1sec

void batSensInit() ;
void batSensEnable() ;
void batSensDisable() ;
bool batSensCheckState() ;
void batSensGetMeasures(float destination[3]) ;

#endif /* ROBOTFILES_SENSORS_BATTERYSENSOR_H_ */
