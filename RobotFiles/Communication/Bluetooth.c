/*
 * Bluetooth.cpp
 *
 *  Created on: 3 lut 2016
 *      Author: dicker
 */

#include "Bluetooth.h"

struct {
	uint8_t array[BT_TASKS_PARAM_NUM + 1] ;
	void (*pFn[BT_TASKS_MAX_NUM])(uint8_t[BT_TASKS_PARAM_NUM]) ;
} btStruct ;

void btAddMessage(uint8_t opCode, void (*pFn)(uint8_t[BT_TASKS_PARAM_NUM])) {
	btStruct.pFn[opCode] = pFn ;
}

void btDeleteMessage(uint8_t opCode) {
	btStruct.pFn[opCode] = 0 ;
}

/**
 *	Bluetooth Interrupt. It gets all data from the UART and pushes them to the CycleBuffer.
 */

void btInterrupt() {
	UARTIntClear(BT_UART_BASE, UART_INT_RX) ;

	uint8_t i ;
	while(1) {
		i = 0 ;
		while(UARTCharsAvail(BT_UART_BASE) && (i < (BT_TASKS_PARAM_NUM + 1))) {
			btStruct.array[i++] = UARTCharGetNonBlocking(BT_UART_BASE);	// without overflow control
		}

		if(i < (BT_TASKS_PARAM_NUM+1)) // nie skompletowano polecenia
			break ;

		if(btStruct.array[0] >= BT_TASKS_MAX_NUM) {
			UARTprintf("Bad command!") ;
			continue ;
		}

		if(btStruct.pFn[btStruct.array[0]] != 0) {
			btStruct.pFn[btStruct.array[0]](btStruct.array+1) ; // wywołanie funkcji z tymi argumentami
		}
	}
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

	uint8_t i ;
	for(i = 0 ; i < BT_TASKS_MAX_NUM ; i++) {
		btStruct.pFn[i] = 0 ;
	}
}


