/*
 * cycleBuffer.c
 *
 *  Created on: 13 lis 2015
 *      Author: akowalew
 */
#include <stdbool.h>
#include <stdint.h>


#include "cycleBuffer.h"

bool cycleBufferPush(CycleBuffer *cb, uint8_t data) {
	if(!(cb->full)) {
		cb->buff[cb->i] = data ;
		cb->i = (cb->i+1) & cb->MASK ;
		cb->empty = false ;
		if( cb->i == cb->j )
			cb->full = true ;

		cb->lastAdd = data ;
		return true ;
	}
	return false ;
}

bool cycleBufferPop(CycleBuffer *cb, volatile uint8_t *dest) {
	if(!(cb->empty)) {
		*dest = cb->buff[cb->j] ;
		cb->j = (cb->j+1) & cb->MASK ;

		cb->full = false ;
		if(cb->i == cb->j)
			cb->empty = true ;
		return true ;
	}

	return false ;
}

void cycleBufferInit(CycleBuffer *cb, uint8_t *buffer, uint8_t SZz) {
	cb->i = 0;
	cb->j = 0;
	cb->full = false ;
	cb->empty = true;
	cb->buff = buffer ;
	cb->SZ = SZz ;
	cb->MASK = SZz-1 ;
}



