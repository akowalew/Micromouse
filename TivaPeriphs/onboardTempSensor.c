/*
 * onboardTempSensor.c
 *
 *  Created on: 28 lut 2016
 *      Author: dicker
 */

#include <stdint.h>
#include <stdbool.h>

#include "inc/hw_types.h"
#include "inc/hw_memmap.h"
#include "inc/tm4c123gh6pm.h"
#include "inc/hw_gpio.h"
#include "inc/hw_timer.h"
#include "inc/hw_adc.h"

#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"
#include "driverlib/timer.h"
#include "driverlib/pin_map.h"
#include "driverlib/adc.h"

#include "onboardTempSensor.h"

struct {
	uint32_t ADCValues[TEMP_SENS_SEQ_FIFO_SZ] ;
	uint32_t tempCelsius ;
} tempSensor;

void tempSensInit() {
	SysCtlPeripheralEnable(TEMP_SENS_ADC_PERIPH) ;
	SysCtlDelay(3) ;

	ADCSequenceConfigure(
			TEMP_SENS_ADC_BASE,
			TEMP_SENS_SEQ_NUM,
			ADC_TRIGGER_PROCESSOR,	// triggered by mcu
			3 ) ; 					// 3 - The lowest priority

	ADCSequenceStepConfigure(
			TEMP_SENS_ADC_BASE,
			TEMP_SENS_SEQ_NUM,
			TEMP_SENS_STEP_NUM,
			ADC_CTL_TS | ADC_CTL_IE | ADC_CTL_END) ;
	// Temperature sensor | interrupt enable | only one step

	ADCSequenceEnable(TEMP_SENS_ADC_BASE, TEMP_SENS_SEQ_NUM) ;
	ADCIntClear(TEMP_SENS_ADC_BASE, TEMP_SENS_SEQ_NUM) ;
}

uint32_t tempSensRead() {
	ADCProcessorTrigger(TEMP_SENS_ADC_BASE, TEMP_SENS_SEQ_NUM) ;

	while(!ADCIntStatus(TEMP_SENS_ADC_BASE, TEMP_SENS_SEQ_NUM, false)){

	}

	ADCIntClear(TEMP_SENS_ADC_BASE, TEMP_SENS_SEQ_NUM) ;
	ADCSequenceDataGet(TEMP_SENS_ADC_BASE, TEMP_SENS_SEQ_NUM, tempSensor.ADCValues) ;
	// TEMP = 147.5 - ((75 * (VREFP - VREFN) × ADCCODE) / 4096)

	return (uint32_t)(147.5 - ((75.0 * 3.3 * (float)(tempSensor.ADCValues[0])) / 4096.0)) ;
}
