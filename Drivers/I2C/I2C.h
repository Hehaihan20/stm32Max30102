/****************************************Copyright (c)****************************************************
 
**--------------File Info---------------------------------------------------------------------------------
** File name:    I2C.h             
** Last modified Date:       2024/1/15    
** Last Version:           	 2024/1/15 
** Descriptions:           
**                        
**--------------------------------------------------------------------------------------------------------
** Created by:               Hange
** Created date:             2024/1/15
** Version:                  1.0
** Descriptions:          
**--------------------------------------------------------------------------------------------------------*/



#ifndef _I2C_H
#define _I2C_H
#include  "stm32f10x.h"
#include "stm32f10x_it.h"
#include <stdio.h>
#include "Systick.h"



#define I2C_RCC					RCC_APB2Periph_GPIOB
#define I2C_GPIO 				GPIOB
#define I2C_SCL_GPIO		GPIO_Pin_6
#define I2C_SDA_GPIO		GPIO_Pin_7

#define SCL_SET()		GPIO_WriteBit(I2C_GPIO,I2C_SCL_GPIO,Bit_SET)
#define SCL_CLR()		GPIO_WriteBit(I2C_GPIO,I2C_SCL_GPIO,Bit_RESET)
#define SDA_SET()		GPIO_WriteBit(I2C_GPIO,I2C_SDA_GPIO,Bit_SET)
#define	SDA_CLR()		GPIO_WriteBit(I2C_GPIO,I2C_SDA_GPIO,Bit_RESET)
#define	SDA_READ	GPIO_ReadInputDataBit(I2C_GPIO,I2C_SDA_GPIO)


#define MAX30102_ADDRESS 					0X57
#define MAX30102_I2C_WRIT_ADDR 		0xAE
#define MAX30102_I2C_READ_ADDR 		0xAF
void I2C_Configuration();
void I2C_ReadByte(uint8_t slave_addr,uint8_t addr,uint8_t *data,uint8_t numByte);
void I2C_WriteByte(uint8_t slave_addr,uint8_t addr,uint8_t data);
u8 I2C_Search_slaveadr(u8 dat);
#endif 