/*
 * UART.h
 *
 *  Created on: 25 окт. 2015 г.
 *      Author: kripton
 */

#ifndef UART_H_
#define UART_H_

#include "stm32f10x.h"


void USART1_init(void);
void USART3_init(void);
void USART_SendString(USART_TypeDef *USART, const char *pucBuffer);

#endif /* UART_H_ */