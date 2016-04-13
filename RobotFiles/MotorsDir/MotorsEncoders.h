/*
 * Encoders.h
 *
 *  Created on: 15 mar 2016
 *      Author: akowalew
 */

#ifndef ROBOTFILES_MOTORSDIR_MOTORSENCODERS_
#define ROBOTFILES_MOTORSDIR_MOTORSENCODERS_

#include "../includeHeader.h"
#include "inc/hw_gpio.h"

#include "driverlib/gpio.h"
#include "driverlib/interrupt.h"
#include "driverlib/pin_map.h"
#include "driverlib/qei.h"

void motEncodersInit() ;
void motEncodersEnable() ;
void motEncodersDisable() ;
void motEncodersInterruptsConfigure(void (*p_leftFunction)(), void (*p_rightFunction)()) ;
void motEncodersInterruptsEnable();
void motEncodersInterruptsDisable();

#define motEncodersIntClearL()	(QEIIntClear(ENC_LEFT_BASE, QEI_INTTIMER))
#define motEncodersIntClearR()	(QEIIntClear(ENC_RIGHT_BASE, QEI_INTTIMER))

#define ENC_LEFT_BASE	QEI1_BASE
#define ENC_RIGHT_BASE	QEI0_BASE


// 10 ms period between measures
#define ENC_VELOCITY_PERIOD	800000

#define ENC_MAX_COUNT	9999

uint32_t motPosGetR() ;
uint32_t motPosGetL() ;
int32_t motVelGetR() ;
int32_t motVelGetL() ;

#endif /* ROBOTFILES_MOTORSDIR_MOTORSENCODERS_ */
