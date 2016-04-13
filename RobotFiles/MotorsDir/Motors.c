/*
 * Motors.c
 *
 *  Created on: 15 mar 2016
 *      Author: akowalew
 */
#include "Motors.h"

void motorsInit() {
	motDriversInit();
	motStateSetL(SOFT_STOP) ;
	motStateSetR(SOFT_STOP) ;
	motEncodersInit() ;
	motCntrlInit() ;
}

void motorsEnable() {
	// initial gpio status
	motDriversEnable() ;
	motEncodersEnable() ;
	motCntrlEnable();
}

void motorsDisable() {
	motDriversDisable() ;
	motCntrlDisable() ;
	motEncodersDisable() ;
}
