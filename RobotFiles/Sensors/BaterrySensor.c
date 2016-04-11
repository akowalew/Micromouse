/*
 * BaterrySensor.c
 *
 *  Created on: 7 mar 2016
 *      Author: akowalew
 */

#include "BatterySensor.h"

struct {
	volatile float cell1st, cell2nd, cell1stdiff ;
	volatile bool batState ;
} batSensStruct;

void batSensGetMeasures(float destination[3]) {
	destination[0] = batSensStruct.cell1st ;
	destination[1] = batSensStruct.cell2nd ;
	destination[2] = batSensStruct.cell1stdiff ;
}

void batSensAdcInt() ;
void batSensTimInt() ;

bool batSensCheckState() {
	return batSensStruct.batState ;
}

void batSensInit() {
	// ADC Configure
	SysCtlPeripheralEnable(BATSENS_ADC_PERIPH) ;
	SysCtlDelay(3) ;

	ADCSequenceConfigure(
			BATSENS_ADC_BASE,
			BATSENS_ADC_SEQ_NUM,
			ADC_TRIGGER_PROCESSOR,
			BATSENS_ADC_PRIORITY) ;
	ADCSequenceStepConfigure(
			BATSENS_ADC_BASE,
			BATSENS_ADC_SEQ_NUM,
			0,
			BATSENS_ADC_1ST_CH) ;
	ADCSequenceStepConfigure(
			BATSENS_ADC_BASE,
			BATSENS_ADC_SEQ_NUM,
			1,
			BATSENS_ADC_2ND_CH | ADC_CTL_IE | ADC_CTL_END) ;
	ADCIntRegister(
			BATSENS_ADC_BASE,
			BATSENS_ADC_SEQ_NUM,
			batSensAdcInt) ;

	ADCHardwareOversampleConfigure(BATSENS_ADC_BASE, BATSENS_ADC_OVERSAMP) ;

	// Timer trigger configure
	SysCtlPeripheralEnable(BATSENS_TIMER_PERIPH) ;
	SysCtlDelay(3) ;

	TimerConfigure( BATSENS_TIMER_BASE, TIMER_CFG_PERIODIC) ;
	TimerControlTrigger( BATSENS_TIMER_BASE, TIMER_BOTH, true) ;
	TimerADCEventSet(BATSENS_TIMER_BASE, TIMER_ADC_TIMEOUT_A ) ;
	TimerLoadSet(BATSENS_TIMER_BASE, TIMER_A, BATSENS_TIMER_PERIOD) ;
	TimerIntRegister(BATSENS_TIMER_BASE, TIMER_A, batSensTimInt) ;
}

void batSensEnable() {
	ADCSequenceEnable(BATSENS_ADC_BASE, BATSENS_ADC_SEQ_NUM) ;
	ADCIntEnable(BATSENS_ADC_BASE, BATSENS_ADC_SEQ_NUM) ;
	ADCIntClear(BATSENS_ADC_BASE, BATSENS_ADC_SEQ_NUM) ;

	TimerEnable(BATSENS_TIMER_BASE, TIMER_BOTH) ;
	TimerIntEnable(BATSENS_TIMER_BASE, TIMER_TIMA_TIMEOUT) ;
}

void batSensDisable() {
	ADCSequenceDisable(BATSENS_ADC_BASE, BATSENS_ADC_SEQ_NUM) ;
	ADCIntDisable(BATSENS_ADC_BASE, BATSENS_ADC_SEQ_NUM) ;

	TimerDisable(BATSENS_TIMER_BASE, TIMER_BOTH) ;
	TimerIntDisable(BATSENS_TIMER_BASE, TIMER_TIMA_TIMEOUT) ;
}

void batSensTimInt() {
	TimerIntClear(BATSENS_TIMER_BASE, TIMER_TIMA_TIMEOUT) ;
	ADCProcessorTrigger(BATSENS_ADC_BASE, BATSENS_ADC_SEQ_NUM) ;
}

void batSensAdcInt() {
	ADCIntClear(BATSENS_ADC_BASE, BATSENS_ADC_SEQ_NUM) ;

	uint32_t adcVal[4] ;
	ADCSequenceDataGet(BATSENS_ADC_BASE, BATSENS_ADC_SEQ_NUM, adcVal) ;

	batSensStruct.cell1st = ( ((float)(adcVal[0])) * 3.239 ) / 4096.0  ;
	batSensStruct.cell2nd = ( ((float)(adcVal[1])) * 3.239 ) / 4096.0  ;

	// cell1st pokazuje teraz wartość z zakresu 0-3.3v
	// cell2nd pokazuje teraz wartość z zakresu 0-3.3v
	// a tak naprawdę wyświetla coś niżej, z racji istnienia dzielnika napięciowego, który ogranicza napięcie

	batSensStruct.cell1st /= DIVIDER_8_4 ;
	batSensStruct.cell2nd /= DIVIDER_4_2 ;
	batSensStruct.cell1stdiff = batSensStruct.cell1st - batSensStruct.cell2nd ;

	if(batSensStruct.cell1st < 6.5) {
			batSensStruct.batState = false ;
			buzPeriodicalOn() ;
			ledsPeriodical2On() ;
	}
	else {
			batSensStruct.batState = true ;
			buzPeriodicalOff() ;
			ledsPeriodical2Off() ;
	}
}



