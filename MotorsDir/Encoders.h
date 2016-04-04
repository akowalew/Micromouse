/*
 * Encoders.h
 *
 *  Created on: 15 mar 2016
 *      Author: akowalew
 */

#ifndef MOTORSDIR_ENCODERS_H_
#define MOTORSDIR_ENCODERS_H_

#include <stdint.h>

void encInit() ;
void encEnable() ;
void encDisable() ;

// 10 ms period between measures
#define ENC_VELOCITY_PERIOD	800000

#define ENC_MAX_COUNT	3999

uint32_t encRGetPos() ;
uint32_t encLGetPos() ;
int32_t encRGetVel() ;
int32_t encLGetVel() ;

#endif /* MOTORSDIR_ENCODERS_H_ */
