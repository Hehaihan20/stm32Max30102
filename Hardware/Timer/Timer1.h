#ifndef _TIMER_1_H
#define _TIMER_1_H
#include  "stm32f10x.h"
#include "stm32f10x_it.h"
#include <stdio.h>	
#define TIMER1_RCC						RCC_APB2Periph_GPIOA
#define TIMER1_GPIO 					GPIOA
#define TIMER1_GPIO_PIN				GPIO_Pin_1

void TIMER1_GPIO_INIT();
void TIMER1_PARAM_CONFIG();

#endif 