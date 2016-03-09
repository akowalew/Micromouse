/*
 * UsbUart.h
 *
 *  Created on: 3 lut 2016
 *      Author: dicker
 */

#ifndef USBUART_H_
#define USBUART_H_

#define USBUART_BASE	UART0_BASE
#define USBUART_STDIO

void usbUartInit() ;
void usbUartSendStr(const char *str) ;
#define usbUartSendChar(chCharacter)	(UARTCharPut(USBUART_BASE, (chCharacter)))
#define usbUartGetChar()				(UARTCharGet(USBUART_BASE))


#endif /* USBUART_H_ */
