
#include <stdint.h>
#include <stdbool.h>

#include "inc/tm4c123gh6pm.h"
#include "inc/hw_memmap.h"
#include "inc/hw_timer.h"
#include "driverlib/sysctl.h"
#include "driverlib/timer.h"
#include "Utilities/robot.h"

#include "uartstdio.h"

volatile bool a ;

void intt() {
	TimerIntClear(TIMER3_BASE, TIMER_TIMA_TIMEOUT) ;
	UARTprintf("c\n") ;
	a = true ;
}

int main(void)
                                                                                                                                                                                                                                            {
	SysCtlClockSet(SYSCTL_SYSDIV_2_5 | SYSCTL_USE_PLL | SYSCTL_OSC_MAIN | SYSCTL_XTAL_16MHZ ) ;

	robotInit() ;
/*
	SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER3) ;
	SysCtlDelay(3) ;

	TimerConfigure(TIMER3_BASE, TIMER_CFG_ONE_SHOT) ;

	TimerIntRegister(TIMER3_BASE,  TIMER_A, intt) ;
	TimerIntEnable(TIMER3_BASE, TIMER_TIMA_TIMEOUT) ;
	TimerIntClear(TIMER3_BASE, TIMER_TIMA_TIMEOUT) ;

	a = false ;

	TimerEnable(TIMER3_BASE, TIMER_BOTH) ;



	TimerLoadSet(TIMER3_BASE, TIMER_A, 80000000) ;
	while(1) {
		if(a == true) {
			TimerDisable(TIMER3_BASE, TIMER_BOTH) ;
			TimerEnable(TIMER3_BASE, TIMER_BOTH) ;
			a = false ;
		}
	}

	return 0 ;
*/

	robotStart() ;
}
