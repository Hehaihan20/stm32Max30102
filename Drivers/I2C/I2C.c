#include "I2C.h"
/****************************************Copyright (c)****************************************************
 
**--------------File Info---------------------------------------------------------------------------------
** File name:    I2C.c             
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
 void I2C_Configuration(void)
 {
     GPIO_InitTypeDef GPIO_InitStructure;
     I2C_InitTypeDef I2C_InitStructure;

     RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);   //开启GPIOB时钟
     RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1, ENABLE);    //开启I2C1时钟

     GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF_OD;            //设置开漏复用模式
     GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_6 | GPIO_Pin_7;    //I2C1_SCL <--> PB6  I2C1_SDA <--> PB7
     GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
     GPIO_Init(GPIOB, &GPIO_InitStructure);
 
     I2C_DeInit(I2C1);           //将I2C1外设寄存器初始化为其默认重置值
 
     I2C_InitStructure.I2C_Ack                   = I2C_Ack_Enable;               //使能应答
     I2C_InitStructure.I2C_AcknowledgedAddress   = I2C_AcknowledgedAddress_7bit; //设置寻址地址为7位地址模式
     I2C_InitStructure.I2C_ClockSpeed            = 400000;                       //设置I2C速率为最大值400kHz
     I2C_InitStructure.I2C_DutyCycle             = I2C_DutyCycle_2;              //设置低比高电平时间为2:1
     I2C_InitStructure.I2C_Mode                  = I2C_Mode_I2C;                 //设置为I2C模式
     I2C_InitStructure.I2C_OwnAddress1           = 0x00;                         //设置自身设备地址为0x30
     I2C_Init(I2C1, &I2C_InitStructure);
     I2C_Cmd(I2C1, ENABLE);                                                      //使能I2C1
 }
u8 I2C_Search_slaveadr(u8 dat){
	u8 addr=0;
	u8 t=0;
	while(I2C_GetFlagStatus(I2C1, I2C_FLAG_BUSY));
	I2C_GenerateSTART(I2C1, ENABLE);//开启I2C1
	while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT));/*EV5,主模式*/
	I2C_Send7bitAddress(I2C1, dat<<1, I2C_Direction_Transmitter);//器件地址 
	while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED))
	{
		t++;
		if(t>11)	//不能<11，stm32f103c8t6 72MHZ
		{
		
			I2C_GenerateSTOP(I2C1, ENABLE);//关闭I2C1总线
			return 0 ;
		}
	}
	I2C_GenerateSTOP(I2C1, ENABLE);//关闭I2C1总线
	return dat;
}



	
void I2C_WriteByte(uint8_t slave_addr,uint8_t addr,uint8_t data)
{
	while(I2C_GetFlagStatus(I2C1, I2C_FLAG_BUSY));
	
	I2C_GenerateSTART(I2C1, ENABLE);//开启I2C1
	while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT));/*EV5,主模式*/
 
	I2C_Send7bitAddress(I2C1, slave_addr<<1, I2C_Direction_Transmitter);//器件地址 -- 默认0xAE
	while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));

	I2C_SendData(I2C1, addr);//寄存器地址
	while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTING));
 
	I2C_SendData(I2C1, data);//发送数据
	while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED));
	
	I2C_GenerateSTOP(I2C1, ENABLE);//关闭I2C1总线
}
void I2C_ReadByte(uint8_t slave_addr,uint8_t addr,uint8_t *data,uint8_t numByte)
{	 

	//u8 data_index=numByte;
	//u8 data1;
	u8 re_slave_addr=((slave_addr<<1)|0x01);
	//产生起始信号，发送设备地址
	while(I2C_GetFlagStatus(I2C1, I2C_FLAG_BUSY));

	I2C_GenerateSTART(I2C1, ENABLE);//开启I2C1
	while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT));/*EV5,主模式*/
 
	I2C_Send7bitAddress(I2C1, slave_addr<<1, I2C_Direction_Transmitter);//器件地址 -- 默认0xAE
	while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));//ev6
	
	I2C_SendData(I2C1, addr);//寄存器地址
	while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED));
	
		//第二次产生起始信号
//	while(I2C_GetFlagStatus(I2C1, I2C_FLAG_BUSY));
	I2C_GenerateSTART(I2C1, ENABLE);//开启I2C1
	while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT));/*EV5,主模式*/

	I2C_Send7bitAddress(I2C1, re_slave_addr, I2C_Direction_Receiver);//器件地址 -- 默认0xAE
	while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED));
	//ev7
//	if(numByte==1)
//	{
//		I2C_AcknowledgeConfig(I2C1,DISABLE);
//		I2C_GenerateSTOP(I2C1, ENABLE);//关闭I2C1总线	
//		while(I2C_GetFlagStatus(I2C1,I2C_FLAG_RXNE) == RESET);
//		data1=I2C_ReceiveData(I2C1);//读取数据
//		printf("numByte==1:data1=0x%x\r\n",data1);
//	}
//		else
		while(numByte--)
		{
	
			
			if(numByte==0)
		{
				I2C_AcknowledgeConfig(I2C1,DISABLE);
				I2C_GenerateSTOP(I2C1, ENABLE);//关闭I2C1总线	
	
		}
				while(I2C_GetFlagStatus(I2C1,I2C_FLAG_RXNE) == RESET);
				*(data++)=I2C_ReceiveData(I2C1);//读取数据=I2C_ReceiveData(I2C1);//读取数据
	

		}
		I2C_AcknowledgeConfig(I2C1,ENABLE);
//		for(int i=0;i<data_index;i++)
//		{
//			printf("data==%x\r\n",data[i]);
//		
//			printf("data==%x\r\n",*data);
//		}	


		
	
}

