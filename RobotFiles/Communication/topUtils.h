/*
 * TopLeds.h
 *
 *  Created on: 10 mar 2016
 *      Author: akowalew
 */

#ifndef ROBOTFILES_COMMUNICATION_TOPUTILS_H_
#define ROBOTFILES_COMMUNICATION_TOPUTILS_H_

#include <stdint.h>

void utilsInit() ;

void buzSequence(uint32_t duration, uint32_t repeatCycle) ;
void buzTurnOn() ;
void buzTurnOff() ;

// PA6, PA7, PD2 LEDS
void ledsTurnOn1() ;
void ledsTurnOn2() ;
void ledsTurnOn3() ;
void ledsTurnOff1() ;
void ledsTurnOff2() ;
void ledsTurnOff3() ;
void buzTurnOn() ;
void buzTurnOff() ;
void buzPeriodicalOn() ;
void buzPeriodicalOff() ;
void ledsPeriodical2On() ;
void ledsPeriodical2Off() ;

// BUTTONS
// 1 : PC7
// 2 : PF4

#define BTN1_PERIPH		SYSCTL_PERIPH_GPIOC
#define BTN1_BASE		GPIO_PORTC_BASE
#define BTN1_PIN		GPIO_PIN_7
#define BTN1_INT_PIN	GPIO_INT_PIN_7

#define BTN2_PERIPH		SYSCTL_PERIPH_GPIOF
#define BTN2_BASE		GPIO_PORTF_BASE
#define BTN2_PIN		GPIO_PIN_4
#define BTN2_INT_PIN	GPIO_INT_PIN_4

int32_t btn1isPushed() ;
int32_t btn2isPushed() ;

void btn1IntRegister(void (*pFn1)()) ;
void btn2IntRegister(void (*pFn2)()) ;

/////////////
// MACROS
/////////////

// PA6, PA7, PD2 LEDS

#define LEDS_WHITE	1
#define LEDS_WHITE_PIN	GPIO_PIN_2

#define LEDS_YELLOW	2
#define LEDS_YELLOW_PIN	GPIO_PIN_7

#define LEDS_RED	3
#define LEDS_RED_PIN	GPIO_PIN_6

// PD0 BUZZ

#define BUZ_PERIPH	SYSCTL_PERIPH_GPIOD
#define BUZ_BASE	GPIO_PORTD_BASE
#define BUZ_PIN		GPIO_PIN_0

// PC7 PF4 uSWITCHES

#define TOPUTILS_TIMER_PERIPH	SYSCTL_PERIPH_TIMER2
#define TOPUTILS_TIMER_BASE		TIMER2_BASE
#define TOPUTILS_TIMER_PERIOD	10000000

// 40000000 * 12,5ns = 500ms


#endif /* ROBOTFILES_COMMUNICATION_TOPUTILS_H_ */
