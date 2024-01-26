#ifndef LED_H
#define LED_H
#include "stm32f10x.h"
#define LED_RCC   			RCC_APB2Periph_GPIOC
#define LED_GPIO				GPIOC
#define LED_GPIO_PIN		GPIO_Pin_13

void LED_GPIO_INIT();
#endif 