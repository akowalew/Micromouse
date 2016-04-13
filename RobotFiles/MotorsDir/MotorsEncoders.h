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

// 10 ms period between measures
#define ENC_VELOCITY_PERIOD	800000

#define ENC_MAX_COUNT	9999

uint32_t motPosGetR() ;
uint32_t motPosGetL() ;
int32_t motVelGetR() ;
int32_t motVelGetL() ;

#endif /* ROBOTFILES_MOTORSDIR_MOTORSENCODERS_ */
