/*
 * MotorsPidTesting.h
 *
 *  Created on: 14 kwi 2016
 *      Author: dicker
 */

#ifndef ROBOTFILES_MOTORSDIR_MOTORSPIDTESTING_H_
#define ROBOTFILES_MOTORSDIR_MOTORSPIDTESTING_H_

#include "MotorsController.h"


#define PID_TESTING

#ifdef PID_TESTING
#include "../Communication/Bluetooth.h"
	#define PID_TEST_ARRAY_SIZE	 200
	#define PID_TEST_DUMP_FUN(x)	(UARTprintf((x)))
	void pidTestInit() ;
	void pidTestStartTesting();
	void pidTestStopTesting();
	bool pidTestIsStillTesting() ;
	void pidTestSendData() ;

	void pidTestAddDataL(int32_t data1, int32_t data2);
	void pidTestAddDataR(int32_t data1, int32_t data2);
#endif

#endif /* ROBOTFILES_MOTORSDIR_MOTORSPIDTESTING_H_ */
