/*
 * onboardTempSensor.h
 *
 *  Created on: 28 lut 2016
 *      Author: dicker
 */

#ifndef TIVAPERIPHS_ONBOARDTEMPSENSOR_H_
#define TIVAPERIPHS_ONBOARDTEMPSENSOR_H_

#define TEMP_SENS_ADC_PERIPH	SYSCTL_PERIPH_ADC0
#define TEMP_SENS_ADC_BASE		ADC0_BASE
#define TEMP_SENS_SEQ_NUM		3
#define TEMP_SENS_SEQ_FIFO_SZ	1
#define TEMP_SENS_STEP_NUM		0

void tempSensInit() ;	// Configure ADC to read tempSensor. MCU triggering, by user
uint32_t tempSensRead() ;



#endif /* TIVAPERIPHS_ONBOARDTEMPSENSOR_H_ */
