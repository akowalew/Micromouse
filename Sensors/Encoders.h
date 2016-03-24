/*
 * Encoders.h
 *
 *  Created on: 15 mar 2016
 *      Author: akowalew
 */

#ifndef SENSORS_ENCODERS_H_
#define SENSORS_ENCODERS_H_

void encInit() ;
void encEnable() ;
void encDisable() ;

#define ENC_VELOCITY_PERIOD	8000000
#define ENC_MAX_COUNT	3999

uint32_t encRGetPos() ;
uint32_t encLGetPos() ;
uint32_t encRGetVel() ;
uint32_t encLGetVel() ;

#endif /* SENSORS_ENCODERS_H_ */
