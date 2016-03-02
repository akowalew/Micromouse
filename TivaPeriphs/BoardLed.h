/*
 * BoardLed.h
 *
 *  Created on: 30 sty 2016
 *      Author: dicker
 */

#ifndef BOARDLED_H_
#define BOARDLED_H_

#define LED_PERIPH	SYSCTL_PERIPH_GPIOF
#define LED_BASE	GPIO_PORTF_BASE
#define RED_LED		GPIO_PIN_1		//PF1
#define BLUE_LED	GPIO_PIN_2		//PF2
#define GREEN_LED	GPIO_PIN_3		//PF3

void boardLedInit() ;
void boardLedEnable(uint32_t whichLeds) ;
void boardLedDisable(uint32_t whichLeds) ;
void boardLedSet(uint32_t combination) ;
void boardLedClear(uint32_t combination) ;

#endif /* BOARDLED_H_ */
