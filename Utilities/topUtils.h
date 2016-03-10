/*
 * TopLeds.h
 *
 *  Created on: 10 mar 2016
 *      Author: akowalew
 */

#ifndef UTILITIES_TOPUTILS_H_
#define UTILITIES_TOPUTILS_H_

#include <stdint.h>

void ledsTurnOn(uint8_t whichLed) ;
void ledsTurnOff(uint8_t whichLed) ;
void utilsInit() ;

void buzTone(uint32_t pwm, uint32_t duration) ;
void buzSequence(uint32_t duration, uint32_t repeatCycle) ;
void buzTurnOn() ;
void buzTurnOff() ;

int32_t btn1isPushed() ;
int32_t btn2isPushed() ;

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


#endif /* UTILITIES_TOPUTILS_H_ */
