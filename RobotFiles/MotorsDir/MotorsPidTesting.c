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
	uint32_t btVelSp, btPosSp ;
	int32_t	data1L[PID_TEST_ARRAY_SIZE],
						data2L[PID_TEST_ARRAY_SIZE],
						data1R[PID_TEST_ARRAY_SIZE],
						data2R[PID_TEST_ARRAY_SIZE] ;

	bool isPidTesting;
} testSt;

void pidTestInit() {
	testSt.btVelSp = 0 ;
	testSt.btPosSp = 0;

	testSt.maxI = PID_TEST_ARRAY_SIZE ;
}

void pidTestStartTesting(){

	motVelSpSetL(testSt.btVelSp);
	motVelSpSetR(testSt.btVelSp);
	motPosSpSetR(testSt.btPosSp);
	motPosSpSetL(testSt.btPosSp);
	motCntrlRegulationStart();

	testSt.isPidTesting = true;
	testSt.test_i = 0 ;

	/*usprintf(
			pidTestStr,
			"LPIDVEL P %d I %d D %d\r\n",
			(uint8_t)(motL.velPid.Kp*10),
			(uint8_t)(cntrlSt.motL.velPid.Ki*10),
			(uint8_t)(cntrlSt.motL.velPid.Kd*10)
			) ;
	PID_TEST_DUMP_FUN(pidTestStr) ;

	usprintf(
			pidTestStr,
			"RPIDVEL P %d I %d D %d\r\n",
			(uint8_t)(cntrlSt.motR.velPid.Kp*10),
			(uint8_t)(cntrlSt.motR.velPid.Ki*10),
			(uint8_t)(cntrlSt.motR.velPid.Kd*10)
			) ;
	PID_TEST_DUMP_FUN(pidTestStr) ;

	usprintf(pidTestStr, "SP %d MAXI %d\r\n", testSt.btVelSp, testSt.maxI);
	PID_TEST_DUMP_FUN(pidTestStr) ;

	usprintf(pidTestStr, "Pos SP %d\r\n", testSt.btPosSp);
	PID_TEST_DUMP_FUN(pidTestStr); */
}

void pidTestStopTesting(){
	testSt.isPidTesting = false;
	pidTestSendData() ;
}

bool pidTestIsStillTesting(){
	return (testSt.isPidTesting) ;
}

void pidTestSendData(){
	char pidTestStr[24] ;

	uint16_t i ;
	for(i = 0 ; i < testSt.maxI ; i++) {
		usprintf(pidTestStr, "%d %d %d %d\n", testSt.data1L[i], testSt.data2L[i], testSt.data1R[i], testSt.data2R[i]) ;
		PID_TEST_DUMP_FUN(pidTestStr) ;
	}
}

void pidTestSetPointsSet(uint8_t params[BT_TASKS_PARAM_NUM]){
	uint32_t val = (((uint32_t)params[0]) << 8) | (((uint32_t)params[1]));
	testSt.btPosSp = val ;
	testSt.btVelSp = params[2] ;
}

void pidTestAddDataL(int32_t data1, int32_t data2){
	testSt.data1L[testSt.test_i] = data1;
	testSt.data2L[testSt.test_i] = data2;
}
void pidTestAddDataR(int32_t data1, int32_t data2){
	testSt.data1R[testSt.test_i] = data1;
	testSt.data2R[testSt.test_i] = data2;

	testSt.test_i++;
}

#endif
