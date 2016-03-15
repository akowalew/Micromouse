
#include <stdint.h>
#include <stdbool.h>

#include "inc/hw_types.h"
#include "inc/hw_memmap.h"
#include "inc/tm4c123gh6pm.h"
#include "inc/hw_gpio.h"
#include "inc/hw_timer.h"
#include "inc/hw_uart.h"
#include "inc/hw_adc.h"

#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"
#include "driverlib/interrupt.h"
#include "driverlib/timer.h"
#include "driverlib/pin_map.h"
#include "driverlib/uart.h"
#include "driverlib/pwm.h"
#include "driverlib/adc.h"

#include "utils/uartstdio.h"
#include "TivaPeriphs/TivaPeriphs.h"
#include "Utilities/Bluetooth.h"

#include "Sensors/BatterySensor.h"
#include "Sensors/IRSensors.h"

#include "Utilities/topUtils.h"
#include "Sensors/Encoders.h"
#include "Effectors/Motors.h"


int main(void)
                                                                                                                                                                                                                                            {
	SysCtlClockSet(SYSCTL_SYSDIV_2_5 | SYSCTL_USE_PLL | SYSCTL_OSC_MAIN | SYSCTL_XTAL_16MHZ ) ;

	/* usbUartInit() ;
	myTimerInit() ;
	btInit() ;

	batSensInit() ;
	batSensEnable() ;

	irSenInit() ;
	irSenEnable() ;

	utilsInit() ;

	encInit() ;
	encEnable() ; */

	motorsInit() ;
	motorsEnable() ;

	motorsSetupM0(CLOCKWISE) ;
	motorsSetupM1(CLOCKWISE) ;

	motorsM0PwmSet(400) ;
	motorsM1PwmSet(400) ;


	while(1) {
		/*
		if(btn1isPushed())
			ledsTurnOn(LEDS_RED) ;
		else
			ledsTurnOff(LEDS_RED) ;

		if(btn2isPushed())
			ledsTurnOn(LEDS_YELLOW) ;
		else
			ledsTurnOff(LEDS_YELLOW) ; */
	}
}
