#ifndef _USART_H
#define _USART_H
#include  "stm32f10x.h"
#include "stm32f10x_it.h"
#include <stdio.h>
#define USART1_RCC		RCC_APB2Periph_GPIOA|RCC_APB2Periph_USART1
#define USART1_GPIO 			GPIOA
#define USART1_TX_GPIO		GPIO_Pin_9
#define USART1_RX_GPIO		GPIO_Pin_10
void USART_GPIO_INIT();
void USART_PARAM_CONFIG();
void SerialPort_SendByte(uint8_t send_byte);
#endif 