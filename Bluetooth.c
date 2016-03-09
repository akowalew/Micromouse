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

#ifdef BT_STDIO
#include "utils/uartstdio.h"
#endif

#include "Bluetooth.h"

#include "cycleBuffer.h"

static uint8_t rxBuffer[BT_BUFFER_SZ] ; //! Memory for CycleBuffer called rxCycleBuffer
static CycleBuffer rxCycleBuffer ;	//! Receive Cycle Buffer. Data from interrupt goes here.

/**
 * Function tells us, if there is something to get in rxCycleBuffer.
 * @return
 */
inline bool btDataAvail() {
	return !rxCycleBuffer.empty ;
}

/**
 * Function shifts all data that rxCycleBuffer has to the destination
 * @param destination
 * @return	Function returns how much bytes are shifted to the destination.
 */
uint8_t btGetData(uint8_t *destination) {
	uint8_t howMuch = 0 ;

	while(!rxCycleBuffer.empty) {
		cycleBufferPop(&rxCycleBuffer, destination++) ;
		howMuch ++ ;
	}

	return howMuch ;
}

/**
 *	Bluetooth Interrupt. It gets all data from the UART and pushes them to the CycleBuffer.
 */
void btInterrupt() {
	UARTIntClear(BT_UART_BASE, UART_INT_RX) ;

	while(UARTCharsAvail(BT_UART_BASE))
		cycleBufferPush(&rxCycleBuffer, UARTCharGet(BT_UART_BASE)) ;	// without overflow control
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

	cycleBufferInit(&rxCycleBuffer, rxBuffer, BT_BUFFER_SZ) ;

	UARTFIFODisable(BT_UART_BASE) ;
	//UARTFIFOLevelSet(BT_UART_BASE, UART_FIFO_TX1_8, UART_FIFO_RX1_8) ;
	UARTIntRegister(BT_UART_BASE, btInterrupt) ;
	UARTIntEnable(BT_UART_BASE, UART_INT_RX) ; // Receive interrupt

	btTurnOn() ;

	// Configure the Bluetooth
	// btConfigure() ;
}


