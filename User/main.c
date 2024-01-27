//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//中景园电子
//店铺地址：http://shop73023976.taobao.com
//
//  文 件 名   : main.c
//  版 本 号   : v2.0
//  作    者   : HuangKai
//  生成日期   : 2018-10-31
//  最近修改   : 
//  功能描述   : OLED I2C接口演示例程(STM32F103系列)
//              说明: 
//              ----------------------------------------------------------------
//              GND  电源地
//              VCC  3.3v电源
//              D0   PA0（SCL）
//              D1   PA1（SDA）
//              RES  PA2（SPI模块改成IIC模块需要接此引脚，IIC模块用户请忽略）
//              ----------------------------------------------------------------
// 修改历史   :
// 日    期   : 
// 作    者   : HuangKai
// 修改内容   : 创建文件
//版权所有，盗版必究。
//Copyright(C) 中景园电子2018-10-31
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
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);	//设置NVIC中断分组2:2位抢占优先级，2位响应优先级
	systick_config();
	USART_GPIO_INIT();
	USART_PARAM_CONFIG();

	MAX30102_Init();
	//I2C_Configuration( );
	printf("start_init\r\n");
	delay_1us(2);
	
	OLED_Init();
	
	OLED_ColorTurn(0);//0正常显示，1 反色显示
  OLED_DisplayTurn(0);//0正常显示 1 屏幕翻转显示
	
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

