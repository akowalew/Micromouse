/*
 * Bluetooth.h
 *
 *  Created on: 3 lut 2016
 *      Author: dicker
 */

#ifndef BLUETOOTH_H_
#define BLUETOOTH_H_

#define BT_SYSCTL_PERIPH_GPIO	SYSCTL_PERIPH_GPIOB
#define BT_SYSCTL_PERIPH_UART	SYSCTL_PERIPH_UART1
#define BT_GPIO_PORT_BASE		GPIO_PORTB_BASE
#define BT_UART_BASE			UART1_BASE
#define BT_UART_NUMBER			1
#define BT_GPIO_UX				GPIO_PB0_U1RX
#define BT_GPIO_TX				GPIO_PB1_U1TX
#define BT_RX_PIN				GPIO_PIN_0
#define BT_TX_PIN				GPIO_PIN_1
#define BT_UART_TX_FIFO			UART_FIFO_TX2_8
#define BT_UART_RX_FIFO			UART_FIFO_RX2_8
#define BT_BAUD					115200
#define BT_BUF_SZ				12

#define BT_TASKS_MAX_NUM		0x20
#define BT_TASKS_PARAM_NUM		3

void btInit(void) ;
void btAddMessage(uint8_t opCode, void (*pFn)(uint8_t[BT_TASKS_PARAM_NUM])) ;
void btDeleteMessage(uint8_t opCode) ;

 #define BT_STDIO

#endif /* BLUETOOTH_H_ */
