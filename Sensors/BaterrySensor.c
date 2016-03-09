/*
 * BaterrySensor.c
 *
 *  Created on: 7 mar 2016
 *      Author: akowalew
 */

#include "BatterySensor.h"

#include "driverlib/sysctl.h"
#include "inc/hw_types.h"
#include "inc/hw_memmap.h"
#include "inc/tm4c123gh6pm.h"
#include "driverlib/pin_map.h"
#include "driverlib/interrupt.h"

#include "driverlib/adc.h"
#include "inc/hw_adc.h"

#include "driverlib/timer.h"
#include "inc/hw_timer.h"

#include "utils/uartstdio.h"
#include "../TivaPeriphs/UsbUart.h"

struct {
	volatile float cell1st, cell2nd, cell1stdiff ;
	bool batState ;
} batSensStruct;

void batSensAdcInt() ;

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
			ADC_TRIGGER_TIMER,
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

	TimerConfigure(
			BATSENS_TIMER_BASE,

			TIMER_CFG_SPLIT_PAIR |

#if BATSENS_TIMER_LETTER == TIMER_A
			TIMER_CFG_A_PERIODIC
#elif BATSENS_TIMER_LETTER == TIMER_B
			TIMER_CFG_B_PERIODIC
#else
#error "Bad Timer Letter. Should be TIMER_A or TIMER_B"
#endif

			) ;

	TimerControlTrigger(
			BATSENS_TIMER_BASE,
			BATSENS_TIMER_LETTER,
			true) ;

	TimerADCEventSet(
			BATSENS_TIMER_BASE,
#if BATSENS_TIMER_LETTER == TIMER_A
			TIMER_ADC_CAPMATCH_A
#elif BATSENS_TIMER_LETTER == TIMER_B
			TIMER_ADC_CAPMATCH_B
#else
#error "Bad Timer Letter. Should be TIMER_A or TIMER_B"
#endif
			) ;

	TimerLoadSet(BATSENS_TIMER_BASE, BATSENS_TIMER_LETTER, BATSENS_TIMER_MATCH_VAL) ;
	TimerMatchSet(
			BATSENS_TIMER_BASE,
			BATSENS_TIMER_LETTER,
			BATSENS_TIMER_MATCH_VAL) ;

#ifdef DEBUG
	UARTprintf("BatSens initiated.\n") ;
#endif
}

void batSensEnable() {
	ADCSequenceEnable(BATSENS_ADC_BASE, BATSENS_ADC_SEQ_NUM) ;
	ADCIntEnable(BATSENS_ADC_BASE, BATSENS_ADC_SEQ_NUM) ;
	ADCIntClear(BATSENS_ADC_BASE, BATSENS_ADC_SEQ_NUM) ;

	TimerEnable(BATSENS_TIMER_BASE, BATSENS_TIMER_LETTER) ;

#ifdef DEBUG
	UARTprintf("BatSens enabled.\n") ;
#endif
}

void batSensDisable() {
	ADCSequenceDisable(BATSENS_ADC_BASE, BATSENS_ADC_SEQ_NUM) ;
	ADCIntDisable(BATSENS_ADC_BASE, BATSENS_ADC_SEQ_NUM) ;

	TimerDisable(BATSENS_TIMER_BASE, BATSENS_TIMER_LETTER) ;

#ifdef DEBUG
	UARTprintf("BatSens disabled.\n") ;
#endif
}

void batSensAdcInt() {
	ADCIntClear(BATSENS_ADC_BASE, BATSENS_ADC_SEQ_NUM) ;

	uint32_t adcVal[2] ;
	ADCSequenceDataGet(BATSENS_ADC_BASE, BATSENS_ADC_SEQ_NUM, adcVal) ;

	batSensStruct.cell1st = ( ((float)(adcVal[0])) * 3.239 ) / 4096.0  ;
	batSensStruct.cell2nd = ( ((float)(adcVal[1])) * 3.239 ) / 4096.0  ;

	// cell1st pokazuje teraz wartość z zakresu 0-3.3v
	// cell2nd pokazuje teraz wartość z zakresu 0-3.3v
	// a tak naprawdę wyświetla coś niżej, z racji istnienia dzielnika napięciowego, który ogranicza napięcie

	batSensStruct.cell1st /= DIVIDER_8_4 ;
	batSensStruct.cell2nd /= DIVIDER_4_2 ;
	batSensStruct.cell1stdiff = batSensStruct.cell1st - batSensStruct.cell2nd ;

	if(batSensStruct.cell1st < 7.2)
			batSensStruct.batState = false ;
		else
			batSensStruct.batState = true ;

#ifdef DEBUG
	UARTprintf("Voltage : 1st %d , 2nd %d, Both %d\n",  (int)((batSensStruct.cell1stdiff)*1000.0), (int)(batSensStruct.cell2nd * 1000.0), (int)(batSensStruct.cell1st * 1000.0)) ;
#endif
}

