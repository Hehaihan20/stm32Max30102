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

	 
     RCC_APB2PeriphClockCmd(I2C_RCC, ENABLE);   //����GPIOBʱ��
     GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_OD;            //���ÿ�©����ģʽ
     GPIO_InitStructure.GPIO_Pin   = I2C_SCL_GPIO | I2C_SDA_GPIO;    //I2C1_SCL <--> PB6  I2C1_SDA <--> PB7
     GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
     GPIO_Init(I2C_GPIO, &GPIO_InitStructure);
		 GPIO_WriteBit(I2C_GPIO,I2C_SCL_GPIO|I2C_SDA_GPIO,Bit_SET);
 }
 
void I2C_Delay(void)
{
	uint8_t i=12;  //С��12������ stm32f103c8t6
   while(i--);
}
 /*
		Start:  SCL �ߵ�ƽ,SDA�½��� 
		������SDA������SCL
 */
 
 void I2C_Start(void){
	 /* ����״̬ */
		SDA_SET();
		SCL_SET();
		I2C_Delay();
	 
		SDA_CLR();
		I2C_Delay();
		SCL_CLR();
		I2C_Delay();
 }
 /*
	Stop: SCL�ߵ�ƽʱ�� ��SDA ���� 
	������SDA������SCL
 */
 
void I2C_Stop(void){
		SDA_CLR();	 
		SCL_SET();
		I2C_Delay();
		SDA_SET();//�ͷ�sda����Ȩ
		I2C_Delay();
 }
/*
ACK: SDA �ߵ�ƽ no_ack �͵�ƽ ack
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
		SDA_SET();//�ͷ�sda����Ȩ
	
	}
		
 }

uint8_t I2C_ACK_WAIT(){
	uint8_t stat;
	SDA_SET();//�ͷ�sda����Ȩ
	I2C_Delay();
	SCL_SET(); //SCL = 1, �᷵��ACK
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
		 SCL_SET();//scl �ߵ������
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
		 SCL_SET();//scl �ߵ������
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
	 				
		  *(data++)=I2C_Read_Byte();//��ȡ����=I2C_ReceiveData(I2C1);//��ȡ����
					
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
		if(t>11)	//����<11��stm32f103c8t6 72MHZ �ڹ涨ʱ������Ӧ��
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
	I2C_GenerateSTART(I2C1, ENABLE);//����I2C1
	while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT));/*EV5,��ģʽ*/
	I2C_Send7bitAddress(I2C1, dat<<1, I2C_Direction_Transmitter);//������ַ 
	while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED))
	{
		t++;
		if(t>11)	//����<11��stm32f103c8t6 72MHZ
		{
		
			I2C_GenerateSTOP(I2C1, ENABLE);//�ر�I2C1����
			return 0 ;
		}
	}
	I2C_GenerateSTOP(I2C1, ENABLE);//�ر�I2C1����
	return dat;
}




void I2C_WriteByte(uint8_t slave_addr,uint8_t addr,uint8_t data)
{

	//while(I2C_GetFlagStatus(I2C1, I2C_FLAG_BUSY));
	
	I2C_GenerateSTART(I2C1, ENABLE);//����I2C1
	while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT));/*EV5,��ģʽ*/
 
	I2C_Send7bitAddress(I2C1, slave_addr<<1, I2C_Direction_Transmitter);//������ַ -- Ĭ��0xAE
	while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));

	I2C_SendData(I2C1, addr);//�Ĵ�����ַ
	while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTING));
 
	I2C_SendData(I2C1, data);//��������
	while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED));
	
	I2C_GenerateSTOP(I2C1, ENABLE);//�ر�I2C1����
}
void I2C_ReadByte(uint8_t slave_addr,uint8_t addr,uint8_t *data,uint8_t numByte)
{	 

	//u8 data_index=numByte;
	//u8 data1;
	u8 re_slave_addr=((slave_addr<<1)|0x01);
	//������ʼ�źţ������豸��ַ
	while(I2C_GetFlagStatus(I2C1, I2C_FLAG_BUSY));

	I2C_GenerateSTART(I2C1, ENABLE);//����I2C1
	while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT));/*EV5,��ģʽ*/
 
	I2C_Send7bitAddress(I2C1, slave_addr<<1, I2C_Direction_Transmitter);//������ַ -- Ĭ��0xAE
	while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));//ev6
	
	I2C_SendData(I2C1, addr);//�Ĵ�����ַ
	while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED));
	
		//�ڶ��β�����ʼ�ź�
	I2C_GenerateSTART(I2C1, ENABLE);//����I2C1
	while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT));/*EV5,��ģʽ*/

	I2C_Send7bitAddress(I2C1, re_slave_addr, I2C_Direction_Receiver);//������ַ -- Ĭ��0xAE
	while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED));
	//ev7
//	if(numByte==1)
//	{
//		I2C_AcknowledgeConfig(I2C1,DISABLE);
//		I2C_GenerateSTOP(I2C1, ENABLE);//�ر�I2C1����	
//		while(I2C_GetFlagStatus(I2C1,I2C_FLAG_RXNE) == RESET);
//		data1=I2C_ReceiveData(I2C1);//��ȡ����
//		printf("numByte==1:data1=0x%x\r\n",data1);
//	}
//		else
		while(numByte--)
		{
	
			
			if(numByte==0)
		{
				I2C_AcknowledgeConfig(I2C1,DISABLE);
				I2C_GenerateSTOP(I2C1, ENABLE);//�ر�I2C1����	
	
		}
				while(I2C_GetFlagStatus(I2C1,I2C_FLAG_RXNE) == RESET);
				*(data++)=I2C_ReceiveData(I2C1);//��ȡ����=I2C_ReceiveData(I2C1);//��ȡ����

		}
		I2C_AcknowledgeConfig(I2C1,ENABLE);
	//	I2C_GenerateSTOP(I2C1, ENABLE);//�ر�I2C1����	
//		for(int i=0;i<data_index;i++)
//		{
//			printf("data==%x\r\n",data[i]);
//		
//			printf("data==%x\r\n",*data);
//		}	


		
	
}
#endif
