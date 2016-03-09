/*
 * Bluetooth.cpp
 *
 *  Created on: 3 lut 2016
 *      Author: dicker
 */

#include <stdint.h>
#include <stdbool.h>

#include "inc/hw_types.h"
#include "inc/hw_memmap.h"
#include "inc/tm4c123gh6pm.h"
#include "inc/hw_gpio.h"
#include "inc/hw_uart.h"

#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"
#include "driverlib/interrupt.h"
#include "driverlib/pin_map.h"
#include "driverlib/uart.h"
#include "driverlib/qei.h"

#include "utils/uartstdio.h"

#include "Bluetooth.h"

struct {
	volatile char inputData[BT_BUF_SZ] ;
	volatile uint8_t charsReadSz ;
	volatile uint8_t dataAvail : 1 ;
	volatile uint8_t dataOverwrite : 1 ;
} btStruct;

bool btIsDataAvail() {
	return btStruct.dataAvail ;
}

bool btIsDataOverwrite() {
	return btStruct.dataOverwrite ;
}

uint8_t btGetData(uint8_t *destination) {
	uint8_t i ;
	for(i = 0 ; i < btStruct.charsReadSz ; i++)
		destination[i] = btStruct.inputData[i] ;

	uint8_t ret = btStruct.charsReadSz ;

	btStruct.charsReadSz = 0 ;
	btStruct.dataAvail = 0 ;
	btStruct.dataOverwrite = 0 ;

	return ret ;
}

/**
 *	Bluetooth Interrupt. It gets all data from the UART and pushes them to the CycleBuffer.
 */
void btInterrupt() {
	UARTIntClear(BT_UART_BASE, UART_INT_RX) ;

	uint8_t i ;
	for(i = 0 ; UARTCharsAvail(BT_UART_BASE) ; i++)
		btStruct.inputData[i] = UARTCharGetNonBlocking(BT_UART_BASE) ;

	if(btStruct.dataAvail == 1)
		btStruct.dataOverwrite = 1 ;
	btStruct.dataAvail = 1 ;
	btStruct.charsReadSz = i ;
}

/**
 * Bluetooth Init. Enable peripherals, configure UART, UARTStdio, Bluetooth Output Pins
 */
void btInit() {
	// Enable peripherals
	SysCtlPeripheralEnable(BT_SYSCTL_PERIPH_GPIO) ;
	SysCtlDelay(3) ;
	SysCtlPeripheralEnable(BT_SYSCTL_PERIPH_UART) ;
	SysCtlDelay(3) ;

	// Pin configures
	GPIOPinConfigure(BT_GPIO_UX) ;
	GPIOPinConfigure(BT_GPIO_TX) ;
	GPIOPinTypeUART(
			BT_GPIO_PORT_BASE,
			BT_RX_PIN | BT_TX_PIN) ;

	// Uart configuration
	UARTClockSourceSet(
			BT_UART_BASE,
			UART_CLOCK_PIOSC) ;

	UARTConfigSetExpClk(
			BT_UART_BASE,
			16000000,
			BT_BAUD,
			UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE | UART_CONFIG_PAR_NONE ) ;
	UARTEnable(BT_UART_BASE) ;

#ifdef BT_STDIO
	UARTStdioConfig(
			BT_UART_NUMBER,
			BT_BAUD,
			16000000);
#endif

	UARTFIFOEnable(BT_UART_BASE) ;
	UARTFIFOLevelSet(BT_UART_BASE, BT_UART_TX_FIFO, BT_UART_RX_FIFO) ;
	UARTIntRegister(BT_UART_BASE, btInterrupt) ;
	UARTIntEnable(BT_UART_BASE, UART_INT_RX) ; // Receive interrupt
}


