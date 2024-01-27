//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//�о�԰����
//���̵�ַ��http://shop73023976.taobao.com
//
//  �� �� ��   : main.c
//  �� �� ��   : v2.0
//  ��    ��   : HuangKai
//  ��������   : 2018-10-31
//  ����޸�   : 
//  ��������   : OLED I2C�ӿ���ʾ����(STM32F103ϵ��)
//              ˵��: 
//              ----------------------------------------------------------------
//              GND  ��Դ��
//              VCC  3.3v��Դ
//              D0   PA0��SCL��
//              D1   PA1��SDA��
//              RES  PA2��SPIģ��ĳ�IICģ����Ҫ�Ӵ����ţ�IICģ���û�����ԣ�
//              ----------------------------------------------------------------
// �޸���ʷ   :
// ��    ��   : 
// ��    ��   : HuangKai
// �޸�����   : �����ļ�
//��Ȩ���У�����ؾ���
//Copyright(C) �о�԰����2018-10-31
//All rights reserved
//******************************************************************************/
#include	"systick.h"
#include "I2C.h"
#include "MAX30102.h"
#include "oled.h"
#include "bmp.h"
#include "Usart1.h"
void  Find_All_I2c_Salve(){

	u8 t;
	for(t=0x7f;t>0;t--)
	{
		u8 tmp=I2C_Search_slaveadr(t);
		if(tmp)
			{
				printf("addr=%x\r\n",tmp);
			}
	}
}
int main(void)
{

	u8 t=123;
	uint16_t data[2];
	uint16_t red_data[100];
	uint16_t ir_data[100];
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);	//����NVIC�жϷ���2:2λ��ռ���ȼ���2λ��Ӧ���ȼ�
	systick_config();
	USART_GPIO_INIT();
	USART_PARAM_CONFIG();

	MAX30102_Init();
	//I2C_Configuration( );
	printf("start_init\r\n");
	delay_1us(2);
	
	OLED_Init();
	
	OLED_ColorTurn(0);//0������ʾ��1 ��ɫ��ʾ
  OLED_DisplayTurn(0);//0������ʾ 1 ��Ļ��ת��ʾ
	
//	for(int i=0;i<sizeof(data);i++)
//	{
//		printf("data=%x\r\n",data[i]);
//	}
//	for(u8 i=0;i<128;i++)
//	{
//		MAX30102_ReadFifo(data);
//		red_data[i]=data[0];
//		ir_data [i]=data[1];
//	}	

//	printf("p_red=%d\r\n",data[0]);
//	printf("p_ir=%d\r\n",data[1]);
//	u16 r=	MAX30102_GetR(red_data,ir_data,128);
//	printf("r=%d\r\n",r);

while(1){
	for(u8 i=0;i<100;i++)
	{	//printf("p_red=%d\r\n",data[0]);
		MAX30102_ReadFifo(data);
		red_data[i]=data[0];
		ir_data [i]=data[1];
	}	

	printf("p_red=%d\r\n",data[0]);
	printf("p_ir=%d\r\n",data[1]);
	u16 r=	MAX30102_GetR(red_data,ir_data,100);
	printf("r=%d\r\n",r);

	OLED_ShowNum(0,0,r,2,16,1);
	OLED_Refresh();
	delay_1ms(1000);

//	printf("p_red=%d\r\n",p_red[0]);
//	printf("p_ir=%d\r\n",p_ir[0]);
	

	//printf("%d\r\n",GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_13));
}
}

