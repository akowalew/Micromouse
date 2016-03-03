
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

#include "utils/uartstdio.h"
#include "TivaPeriphs/TivaPeriphs.h"
#include "Bluetooth.h"

#include <stdio.h>

//TODO: TAKE THIS SHIT TO ANOTHER FILE
#define BTN_PERIPH	SYSCTL_PERIPH_GPIOF
#define BTN_BASE	GPIO_PORTF_BASE
#define BTN_LEFT	GPIO_PIN_4
#define BTN_RIGHT	GPIO_PIN_0
#define BTN_LEFT_INT_PIN	GPIO_INT_PIN_4
#define BTN_RIGHT_INT_PIN	GPIO_INT_PIN_0

void adcIntPhototransistors() {
	ADCIntClear(ADC0_BASE, 1) ;

	uint32_t adcVal[4] ;
	ADCSequenceDataGet(ADC0_BASE, 1, adcVal) ;
	UARTprintf("Photo : %d %d %d %d\n", adcVal[0], adcVal[1], adcVal[2], adcVal[3]) ;
}

#define DZIELNIK_8_4	0.347826087
#define DZIELNIK_4_2	0.714285714

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void adcIntBalancer() {
	ADCIntClear(ADC0_BASE, 2) ;

	uint32_t adcVal[3] ;
	ADCSequenceDataGet(ADC0_BASE, 2, adcVal) ;

	float cell1st, cell2nd ;


	cell1st = ( ((float)(adcVal[0])) * 3.29 ) / 4096.0  ;
	cell2nd = ( ((float)(adcVal[1])) * 3.29 ) / 4096.0  ;
	float vcc = ( ((float)(adcVal[2])) * 3.29 ) / 4096.0 ;

	// cell1st pokazuje teraz wartość z zakresu 0-3.3v
	// cell2nd pokazuje teraz wartość z zakresu 0-3.3v
	// a tak naprawdę wyświetla coś niżej, z racji istnienia dzielnika napięciowego, który ogranicza napięcie

	UARTprintf("VCC : %d\n", adcVal[2]) ;

	UARTprintf("ADC : 1st %d , 2nd %d\n", (int)(cell1st * 100.0) , (int)(cell2nd * 100.0)) ;

	cell1st /= DZIELNIK_8_4 ;
	cell2nd /= DZIELNIK_4_2 ;

	/*
	 * 2ND CELL 30K , 12K	-> 3V MAX (4,2V MAX)
	 * 1ST CELL 16K , 30K	-> 2,92V MAX (8,4V MAX)
	 */

	UARTprintf("Balancer : %d %d\n", (int)(cell1st * 100) , (int)(cell2nd * 100)) ;

	// CH10 1ST CELL
	// CH8 2ND CELL
}

void sampleFunction2() {

	//input ADC pins

	/*
	 * Q1_IN	PE0	AIN3
	 * Q2_IN	PB5	AIN11
	 * Q3_IN	PE4	AIN9
	 * Q4_IN	PE3	AIN0
	 *
	 * Balancer 1st Cell	PB4	AIN10
	 * Balancer 2nd Cell	PE5	AIN8
	 *
	 */

	// PHOTOTRANSISTORS AND BALANCER

	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE) ;
	SysCtlDelay(3) ;

	GPIOPinTypeADC(
			GPIO_PORTE_BASE,
			GPIO_PIN_0 | GPIO_PIN_4 | GPIO_PIN_3 | GPIO_PIN_5) ;
	GPIOPinConfigure(GPIO_PCTL_PE4_AIN9) ;
	GPIOPinConfigure(GPIO_PCTL_PE3_AIN0) ;
	GPIOPinConfigure(GPIO_PCTL_PE0_AIN3) ;
	GPIOPinConfigure(GPIO_PCTL_PE5_AIN8) ;

	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB) ;
	SysCtlDelay(3) ;


	GPIOPinTypeADC(
			GPIO_PORTB_BASE,
			GPIO_PIN_5 | GPIO_PIN_4) ;


	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD) ;
	SysCtlDelay(3) ;

	GPIOPinTypeADC(GPIO_PORTD_BASE, GPIO_PIN_2) ;
	GPIOPinConfigure(GPIO_PCTL_PD2_AIN5) ;

	// there's no macro for PB5 and PB4 (AIN11, AIN10) to GPIOPinConfigure


	// configure ADC, photoransistors

	SysCtlPeripheralEnable(SYSCTL_PERIPH_ADC0) ;
	SysCtlDelay(3) ;

	ADCSequenceConfigure(
			ADC0_BASE,
			1,
			ADC_TRIGGER_PROCESSOR,
			0) ;
	ADCSequenceStepConfigure(
			ADC0_BASE,
			1, 0, ADC_CTL_CH3) ;
	ADCSequenceStepConfigure(
			ADC0_BASE,
			1, 1, ADC_CTL_CH11) ;
	ADCSequenceStepConfigure(
			ADC0_BASE,
			1, 2, ADC_CTL_CH9 ) ;
	ADCSequenceStepConfigure(
			ADC0_BASE,
			1, 3, ADC_CTL_IE | ADC_CTL_CH0 | ADC_CTL_END) ;
	ADCSequenceEnable(ADC0_BASE, 1) ;

	ADCIntRegister(
			ADC0_BASE,
			1, adcIntPhototransistors) ;
	ADCIntEnable(ADC0_BASE, 1) ;
	ADCIntClear(ADC0_BASE, 1) ;

	// configure ADC, balancer pins

	ADCSequenceConfigure(
			ADC0_BASE,
			2,
			ADC_TRIGGER_PROCESSOR,
			1) ;

	ADCSequenceStepConfigure(
			ADC0_BASE,
			2,
			0,
			ADC_CTL_CH10) ;
	ADCSequenceStepConfigure(
			ADC0_BASE,
			2,
			1,
			 ADC_CTL_CH8 ) ;
	ADCSequenceStepConfigure(ADC0_BASE, 2, 2, ADC_CTL_IE |ADC_CTL_CH5 |ADC_CTL_END) ;
	ADCSequenceEnable(ADC0_BASE, 2) ;

	ADCIntRegister(ADC0_BASE, 2, adcIntBalancer) ;
	ADCIntEnable(ADC0_BASE, 2) ;
	ADCIntClear(ADC0_BASE, 2) ;


	// configure PWM channels, IR diodes

	/*
	 * IR1	PF1	M1PWM5
	 * IR2	PB6 M0PWM0
	 * IR3	PB7	M0PWM1
	 * IR4	PF0	M1PWM4
	 */

	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF) ;
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB) ;
	SysCtlDelay(3) ;

	// PF0 IS LOCKED PIN
	HWREG(GPIO_PORTF_BASE + GPIO_O_LOCK) = GPIO_LOCK_KEY;
	HWREG(GPIO_PORTF_BASE + GPIO_O_CR)   |= 0x01;

	GPIOPinTypePWM(GPIO_PORTB_BASE, GPIO_PIN_6 | GPIO_PIN_7) ;
	GPIOPinTypePWM(GPIO_PORTF_BASE, GPIO_PIN_0 | GPIO_PIN_1) ;

	GPIOPinConfigure(GPIO_PF1_M1PWM5) ;
	GPIOPinConfigure(GPIO_PB6_M0PWM0) ;
	GPIOPinConfigure(GPIO_PB7_M0PWM1) ;
	GPIOPinConfigure(GPIO_PF0_M1PWM4) ;


	SysCtlPeripheralEnable(SYSCTL_PERIPH_PWM0) ;
	SysCtlDelay(3) ;

	SysCtlPeripheralEnable(SYSCTL_PERIPH_PWM1) ;
	SysCtlDelay(3) ;

	PWMGenConfigure(PWM1_BASE, PWM_GEN_2, PWM_GEN_MODE_DOWN | PWM_GEN_MODE_NO_SYNC) ;
	PWMGenConfigure(PWM0_BASE, PWM_GEN_0, PWM_GEN_MODE_DOWN | PWM_GEN_MODE_NO_SYNC) ;

	PWMGenPeriodSet(PWM0_BASE, PWM_GEN_0, 400) ;
	PWMGenPeriodSet(PWM1_BASE, PWM_GEN_2, 400) ;

	PWMPulseWidthSet(PWM0_BASE, PWM_OUT_0, 1) ;
	PWMPulseWidthSet(PWM0_BASE, PWM_OUT_1, 1) ;
	PWMPulseWidthSet(PWM1_BASE, PWM_OUT_4, 1) ;
	PWMPulseWidthSet(PWM1_BASE, PWM_OUT_5, 1) ;

	PWMGenEnable(PWM0_BASE, PWM_GEN_0) ;
	PWMGenEnable(PWM1_BASE, PWM_GEN_2) ;

	PWMOutputState(PWM0_BASE, (PWM_OUT_0_BIT | PWM_OUT_1_BIT), true) ;
	PWMOutputState(PWM1_BASE, (PWM_OUT_4_BIT | PWM_OUT_5_BIT), true) ;

	while(1) {
		//ADCProcessorTrigger(ADC0_BASE, 1) ;
		//myTimerWait(500) ;
		ADCProcessorTrigger(ADC0_BASE, 2) ;
		myTimerWait(500) ;
	}
}

int main(void)
                                                                                                                                                                                                                                            {
	SysCtlClockSet(SYSCTL_SYSDIV_2_5 | SYSCTL_USE_PLL | SYSCTL_OSC_MAIN | SYSCTL_XTAL_16MHZ ) ;

	usbUartInit() ;
	myTimerInit() ;
	btInit() ;
	//boardLedInit() ;
	//boardButtonsInit() ;
	//tempSensInit() ;

	// PA6, PA7, PD2 LEDS
	// PD0 BUZZ
	// BT PB0 PB1
	// PC7 PF4 SWITCHE
	//

	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA) ;
	SysCtlDelay(3) ;
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD) ;
	SysCtlDelay(3) ;

	GPIOPinTypeGPIOOutput(GPIO_PORTA_BASE, GPIO_PIN_6 | GPIO_PIN_7) ;
	GPIOPadConfigSet(GPIO_PORTA_BASE, GPIO_PIN_6 | GPIO_PIN_7, GPIO_STRENGTH_12MA, GPIO_PIN_TYPE_STD) ;
	GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_6 | GPIO_PIN_7, GPIO_PIN_6 | GPIO_PIN_7) ;

	GPIOPinTypeGPIOOutput(GPIO_PORTD_BASE, GPIO_PIN_2 | GPIO_PIN_0) ;
	GPIOPadConfigSet(GPIO_PORTD_BASE, GPIO_PIN_2 | GPIO_PIN_0, GPIO_STRENGTH_12MA, GPIO_PIN_TYPE_STD) ;
	GPIOPinWrite(GPIO_PORTD_BASE, GPIO_PIN_2 | GPIO_PIN_0, GPIO_PIN_2) ;

	sampleFunction2() ;

	while(1) {



	}
}
