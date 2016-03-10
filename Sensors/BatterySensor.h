/*
 * BatterySensor.h
 *
 *  Created on: 7 mar 2016
 *      Author: akowalew
 */

#ifndef SENSORS_BATTERYSENSOR_H_
#define SENSORS_BATTERYSENSOR_H_

/*
 * Battery Sensor
 *
 */

#include <stdbool.h>
#include <stdint.h>

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
#define BATSENS_TIMER_PERIPH	SYSCTL_PERIPH_WTIMER0
#define BATSENS_TIMER_BASE		WTIMER0_BASE
#define BATSENS_TIMER_LETTER	TIMER_A
#define BATSENS_TIMER_MATCH_VAL	80000000

// 80MHz -> 12.5 ns per cycle
// 80000000 * 12.5 ns = 1sec

void batSensInit() ;
void batSensEnable() ;
void batSensDisable() ;
bool batSensCheckState() ;

#endif /* SENSORS_BATTERYSENSOR_H_ */
