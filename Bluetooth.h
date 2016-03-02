/*
 * Bluetooth.h
 *
 *  Created on: 3 lut 2016
 *      Author: dicker
 */

#ifndef BLUETOOTH_H_
#define BLUETOOTH_H_

void btTurnOn() ;
void btTurnOff() ;
void btConfigure() ;
void btInit(void) ;
bool btDataAvail() ;
uint8_t btGetData(uint8_t *destination) ;

#define BT_SYSCTL_PERIPH_GPIO	SYSCTL_PERIPH_GPIOC
#define BT_SYSCTL_PERIPH_UART	SYSCTL_PERIPH_UART1
#define BT_GPIO_PORT_BASE		GPIO_PORTC_BASE
#define BT_UART_BASE			UART1_BASE
#define BT_UART_NUMBER			1
#define BT_GPIO_UX				GPIO_PC4_U1RX
#define BT_GPIO_TX				GPIO_PC5_U1TX
#define BT_RX_PIN				GPIO_PIN_4
#define BT_TX_PIN				GPIO_PIN_5
#define BT_POWER_PIN			GPIO_PIN_6

#define BT_BAUD					115200

#define BT_RESPOND_PERIOD_MS	100

#define BT_BUFFER_SZ	32

// #define BT_STDIO

#endif /* BLUETOOTH_H_ */
