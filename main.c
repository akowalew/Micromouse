
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
#include "Bluetooth.h"

#include "Sensors/BatterySensor.h"
#include "Sensors/IRSensors.h"

int main(void)
                                                                                                                                                                                                                                            {
	SysCtlClockSet(SYSCTL_SYSDIV_2_5 | SYSCTL_USE_PLL | SYSCTL_OSC_MAIN | SYSCTL_XTAL_16MHZ ) ;


	boardButtonsInit() ;
	usbUartInit() ;
	myTimerInit() ;
	btInit() ;

	batSensInit() ;
	batSensEnable() ;

	irSenInit() ;
	irSenEnable() ;

	// PA6, PA7, PD2 LEDS
		// PD0 BUZZ
		// BT PB0 PB1
		// PC7 PF4 SWITCHE

	/*
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA) ;
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD) ;
	SysCtlDelay(3) ;

	GPIOPinTypeGPIOOutput(GPIO_PORTA_BASE, GPIO_PIN_6 | GPIO_PIN_7) ;
	GPIOPinTypeGPIOOutput(GPIO_PORTD_BASE, GPIO_PIN_2 | GPIO_PIN_0) ;

	GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_6 | GPIO_PIN_7 , GPIO_PIN_6 | GPIO_PIN_7 ) ;
	GPIOPinWrite(GPIO_PORTD_BASE, GPIO_PIN_2 | GPIO_PIN_0, GPIO_PIN_2 | GPIO_PIN_0) ;
*/

	uint8_t tab[17] ;
	uint8_t i ;
	uint8_t j ;

	while(1) {
		if(btIsDataAvail()) {
			i = btGetData(tab) ;
			for(j = 0 ; j < i ; j++)
				usbUartSendChar(tab[j]);
		}
	}
}
