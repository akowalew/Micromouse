/*
 * UsbUart.c
 *
 *  Created on: 3 lut 2016
 *      Author: dicker
 */
#include "UsbUart.h"

void usbUartInit() {
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA) ;
	SysCtlDelay(3) ;
    SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);
    SysCtlDelay(3);

	// Pin configures
	GPIOPinConfigure(GPIO_PA0_U0RX) ;
	GPIOPinConfigure(GPIO_PA1_U0TX) ;

	// Uart configuration
	UARTClockSourceSet(
			UART0_BASE ,
			UART_CLOCK_PIOSC) ;

	GPIOPinTypeUART(
			GPIO_PORTA_BASE,
			GPIO_PIN_0 | GPIO_PIN_1) ;

	UARTConfigSetExpClk(
			UART0_BASE,
			16000000,
			115200,
			UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE | UART_CONFIG_PAR_NONE ) ;
	UARTEnable(UART0_BASE) ;

#ifdef USBUART_STDIO
	UARTStdioConfig(0, 115200, 16000000);
#endif
}

inline void usbUartSendStr(const char *str) {
	while(*str)
		UARTCharPut(USBUART_BASE, *(str++)) ;
}
