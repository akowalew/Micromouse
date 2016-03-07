/*
 * boardButtons.h
 *
 *  Created on: 26 lut 2016
 *      Author: dicker
 */

#ifndef TIVAPERIPHS_BOARDBUTTONS_H_
#define TIVAPERIPHS_BOARDBUTTONS_H_

#define BTN_PERIPH	SYSCTL_PERIPH_GPIOF
#define BTN_PORT	GPIO_PORTF_BASE
#define BTN_LEFT	GPIO_PIN_4
#define BTN_RIGHT	GPIO_PIN_0

#define BTN_LEFT_INT_PIN	GPIO_INT_PIN_4
#define BTN_RIGHT_INT_PIN	GPIO_INT_PIN_0

void boardButtonsInit() ;
bool isLeftBtnPressed() ;
bool isRightBtnPressed() ;

#endif /* TIVAPERIPHS_BOARDBUTTONS_H_ */
