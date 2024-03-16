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

	 
     RCC_APB2PeriphClockCmd(I2C_RCC, ENABLE);   //开启GPIOB时钟
     GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_OD;            //设置开漏复用模式
     GPIO_InitStructure.GPIO_Pin   = I2C_SCL_GPIO | I2C_SDA_GPIO;    //I2C1_SCL <--> PB6  I2C1_SDA <--> PB7
     GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
     GPIO_Init(I2C_GPIO, &GPIO_InitStructure);
		 GPIO_WriteBit(I2C_GPIO,I2C_SCL_GPIO|I2C_SDA_GPIO,Bit_SET);
 }
 
void I2C_Delay(void)
{
	uint8_t i=12;  //小于12有问题 stm32f103c8t6
   while(i--);
}
 /*
		Start:  SCL 高电平,SDA下降沿 
		先拉低SDA再拉低SCL
 */
 
 void I2C_Start(void){
	 /* 空闲状态 */
		SDA_SET();
		SCL_SET();
		I2C_Delay();
	 
		SDA_CLR();
		I2C_Delay();
		SCL_CLR();
		I2C_Delay();
 }
 /*
	Stop: SCL高电平时候 ，SDA 上升 
	先拉低SDA再拉高SCL
 */
 
void I2C_Stop(void){
		SDA_CLR();	 
		SCL_SET();
		I2C_Delay();
		SDA_SET();//释放sda控制权
		I2C_Delay();
 }
/*
ACK: SDA 高电平 no_ack 低电平 ack
 */
void I2C_ACK(uint8_t bit){
	if(bit)//no_ack
	{
		SDA_SET();
		I2C_Delay();
		SCL_SET();
		I2C_Delay();
		SCL_CLR();
		I2C_Delay();
	}
	else
	{
		SDA_CLR();
		I2C_Delay();
		SCL_SET();
		I2C_Delay();
		SCL_CLR();
		I2C_Delay();
		SDA_SET();//释放sda控制权
	
	}
		
 }

uint8_t I2C_ACK_WAIT(){
	uint8_t stat;
	SDA_SET();//释放sda控制权
	I2C_Delay();
	SCL_SET(); //SCL = 1, 会返回ACK
	if(SDA_READ)//no_ack
	{
		SCL_CLR();
		return 1;
	}
	else
	{
		SCL_CLR();
		return 0;
	}
		
 }
 
void 	IIC_Send_Byte(uint8_t data){
	 uint8_t i;
	for(i=0;i<8;i++)
	 {
			if((data<<i)&0x80) SDA_SET();
			else				SDA_CLR();
		  
		 I2C_Delay();
		 SCL_SET();//scl 高电读数据
		 I2C_Delay();
		 SCL_CLR();
			I2C_Delay();
		}	

}
static uint8_t data_read;

uint8_t I2C_Read_Byte(void){
	 uint8_t i;
	 data_read=0;
	for(i=0;i<8;i++)
	 {
		 data_read<<=1;
		 SCL_SET();//scl 高电读数据
		 I2C_Delay();
	   data_read=(SDA_READ&1);  
		 SCL_CLR();
		 I2C_Delay();
//		printf("SDA_READ=%d\r\n",SDA_READ);
//		printf("data_read=%d\r\n",data_read);
	 }	
	 
	return data_read;

}

 void I2C_WriteByte(uint8_t slave_addr,uint8_t addr,uint8_t data){
	 
	 I2C_Start();
	 IIC_Send_Byte(slave_addr<<1);
	 while(I2C_ACK_WAIT());
	 IIC_Send_Byte(addr);
	 while(I2C_ACK_WAIT());
	 IIC_Send_Byte(data);
	 while(I2C_ACK_WAIT());
	 I2C_Stop();
 }
 
void I2C_ReadByte(uint8_t slave_addr,uint8_t addr,uint8_t *data,uint8_t numByte)
{
	 u8 re_slave_addr=((slave_addr<<1)|0x01);
	 I2C_Start();
	 IIC_Send_Byte(slave_addr<<1);
	 while(I2C_ACK_WAIT());
	 IIC_Send_Byte(addr);
	 while(I2C_ACK_WAIT());
	 
	
	
	 I2C_Start();
	 IIC_Send_Byte(re_slave_addr);
	while(I2C_ACK_WAIT());
	 while(numByte--){
	 				
		  *(data++)=I2C_Read_Byte();//读取数据=I2C_ReceiveData(I2C1);//读取数据
					
			if(numByte==0)
		{
				I2C_ACK(1);
				I2C_Stop();
	
		}else {	I2C_ACK(0);}
	
		}
			
	 }

u8 I2C_Search_slaveadr(u8 dat){
	u8 addr=0;
	u8 t=0;
	
	I2C_Start();
	IIC_Send_Byte(dat<<1);
	while(I2C_ACK_WAIT())
	{
		t++;
		if(t>11)	//不能<11，stm32f103c8t6 72MHZ 在规定时间内有应答
		{
		
			I2C_Stop();
			return 0 ;
		}
	}
	I2C_Stop();
	return dat;
}

#ifdef hardI2C
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

	//while(I2C_GetFlagStatus(I2C1, I2C_FLAG_BUSY));
	
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
	//	I2C_GenerateSTOP(I2C1, ENABLE);//关闭I2C1总线	
//		for(int i=0;i<data_index;i++)
//		{
//			printf("data==%x\r\n",data[i]);
//		
//			printf("data==%x\r\n",*data);
//		}	


		
	
}
#endif
