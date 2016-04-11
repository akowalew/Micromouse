/*
 * Encoders.h
 *
 *  Created on: 15 mar 2016
 *      Author: akowalew
 */

#ifndef ROBOTFILES_MOTORSDIR_ENCODERS_H_
#define ROBOTFILES_MOTORSDIR_ENCODERS_H_

#include "../includeHeader.h"
#include "inc/hw_gpio.h"

#include "driverlib/gpio.h"
#include "driverlib/interrupt.h"
#include "driverlib/pin_map.h"
#include "driverlib/qei.h"

void encInit() ;
void encEnable() ;
void encDisable() ;

// 10 ms period between measures
#define ENC_VELOCITY_PERIOD	800000

#define ENC_MAX_COUNT	9999

uint32_t encRGetPos() ;
uint32_t encLGetPos() ;
int32_t encRGetVel() ;
int32_t encLGetVel() ;

#endif /* ROBOTFILES_MOTORSDIR_ENCODERS_H_ */
