/*
 * UsbUart.h
 *
 *  Created on: 3 lut 2016
 *      Author: dicker
 */

#ifndef USBUART_H_
#define USBUART_H_

#include <stdint.h>
#include <stdbool.h>

#include "inc/hw_memmap.h"
#include "inc/tm4c123gh6pm.h"
#include "inc/hw_gpio.h"
#include "inc/hw_uart.h"

#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"
#include "driverlib/pin_map.h"
#include "driverlib/uart.h"

#define USBUART_STDIO

#ifdef USBUART_STDIO
#include "utils/uartstdio.h"
#endif

#define USBUART_BASE	UART0_BASE


void usbUartInit() ;
void usbUartSendStr(const char *str) ;
#define usbUartSendChar(chCharacter)	(UARTCharPut(USBUART_BASE, (chCharacter)))
#define usbUartGetChar()				(UARTCharGet(USBUART_BASE))

#endif /* USBUART_H_ */
