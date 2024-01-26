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

     RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);   //����GPIOBʱ��
     RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1, ENABLE);    //����I2C1ʱ��

     GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF_OD;            //���ÿ�©����ģʽ
     GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_6 | GPIO_Pin_7;    //I2C1_SCL <--> PB6  I2C1_SDA <--> PB7
     GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
     GPIO_Init(GPIOB, &GPIO_InitStructure);
 
     I2C_DeInit(I2C1);           //��I2C1����Ĵ�����ʼ��Ϊ��Ĭ������ֵ
 
     I2C_InitStructure.I2C_Ack                   = I2C_Ack_Enable;               //ʹ��Ӧ��
     I2C_InitStructure.I2C_AcknowledgedAddress   = I2C_AcknowledgedAddress_7bit; //����Ѱַ��ַΪ7λ��ַģʽ
     I2C_InitStructure.I2C_ClockSpeed            = 400000;                       //����I2C����Ϊ���ֵ400kHz
     I2C_InitStructure.I2C_DutyCycle             = I2C_DutyCycle_2;              //���õͱȸߵ�ƽʱ��Ϊ2:1
     I2C_InitStructure.I2C_Mode                  = I2C_Mode_I2C;                 //����ΪI2Cģʽ
     I2C_InitStructure.I2C_OwnAddress1           = 0x00;                         //���������豸��ַΪ0x30
     I2C_Init(I2C1, &I2C_InitStructure);
     I2C_Cmd(I2C1, ENABLE);                                                      //ʹ��I2C1
 }
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
	while(I2C_GetFlagStatus(I2C1, I2C_FLAG_BUSY));
	
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
//	while(I2C_GetFlagStatus(I2C1, I2C_FLAG_BUSY));
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
//		for(int i=0;i<data_index;i++)
//		{
//			printf("data==%x\r\n",data[i]);
//		
//			printf("data==%x\r\n",*data);
//		}	


		
	
}

