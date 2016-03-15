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

#define ENC_VELOCITY_PERIOD	80000000
#define ENC_MAX_COUNT	3999

#define enc0GetPos() 	(QEIPositionGet(QEI0_BASE))
#define enc1GetPos() 	(QEIPositionGet(QEI1_BASE))
#define enc0GetVel() 	(QEIVelocityGet(QEI0_BASE))
#define enc1GetVel()	(QEIVelocityGet(QEI1_BASE))

#endif /* SENSORS_ENCODERS_H_ */
