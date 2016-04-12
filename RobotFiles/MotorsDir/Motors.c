/*
 * Motors.c
 *
 *  Created on: 15 mar 2016
 *      Author: akowalew
 */
#include "Motors.h"

void motorsInit() {
	motorsDriversInit();
	motorsSetupML(SOFT_STOP) ;
	motorsSetupMR(SOFT_STOP) ;
	encInit() ;
	motCntrlInit() ;
}

void motorsEnable() {
	// initial gpio status
	motorsDriversEnable() ;
	encEnable() ;
	motCntrlEnable();
}

void motorsDisable() {
	motorsDriversDisable() ;
	motCntrlDisable() ;
	encDisable() ;
}
