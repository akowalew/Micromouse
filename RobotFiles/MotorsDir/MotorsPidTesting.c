/*
 * MotorsPidTesting.c
 *
 *  Created on: 14 kwi 2016
 *      Author: dicker
 */

#include "MotorsPidTesting.h"

#ifdef PID_TESTING

volatile struct {
	uint16_t test_i ;
	uint16_t maxI ;
	int32_t	data1L[PID_TEST_ARRAY_SIZE],
			data2L[PID_TEST_ARRAY_SIZE],
			data1R[PID_TEST_ARRAY_SIZE],
			data2R[PID_TEST_ARRAY_SIZE] ;

	bool isPidTesting;
} testSt;

void pidTestInit() {
	testSt.maxI = PID_TEST_ARRAY_SIZE ;
}

void pidTestStartTesting(){
	motCntrlRegulationStart();

	testSt.isPidTesting = true;
	testSt.test_i = 0 ;
}

void pidTestStopTesting(){
	testSt.isPidTesting = false;
	//pidTestSendData() ;
}

bool pidTestIsStillTesting(){
	return (testSt.isPidTesting) ;
}

void pidTestSendData(){
	char pidTestStr[24] ;

	int32_t lp, rp ;
	int32_t lv, rv ;
	motSetPointsGet(&lp, &lv, &rp, &rv) ;
	usprintf(pidTestStr, "PSpL:%d VSpL:%d\nPSpR:%d VSpR:%d", lp, lv, rp, rv) ;
	PID_TEST_DUMP_FUN(pidTestStr);

	uint16_t i ;
	for(i = 0 ; i < testSt.test_i ; i++) {
		usprintf(pidTestStr, "%d %d %d %d\n", testSt.data1L[i], testSt.data2L[i], testSt.data1R[i], testSt.data2R[i]) ;
		PID_TEST_DUMP_FUN(pidTestStr) ;
	}
}

void pidTestAddDataL(int32_t data1, int32_t data2){
	if(!pidTestIsStillTesting())
		return;
	testSt.data1L[testSt.test_i] = data1;
	testSt.data2L[testSt.test_i] = data2;
}
void pidTestAddDataR(int32_t data1, int32_t data2){
	if(!pidTestIsStillTesting())
		return;

	testSt.data1R[testSt.test_i] = data1;
	testSt.data2R[testSt.test_i] = data2;

	testSt.test_i++;
	if(testSt.test_i == PID_TEST_ARRAY_SIZE)
		pidTestStopTesting();
}

#endif
