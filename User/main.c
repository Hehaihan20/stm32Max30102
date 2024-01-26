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
	u8 data[6];
	u16 dataall[20];
	u8 *p=data;
	uint16_t p_red[1];
	uint16_t p_ir[1];
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);	//设置NVIC中断分组2:2位抢占优先级，2位响应优先级
	systick_config();
	USART_GPIO_INIT();
	USART_PARAM_CONFIG();

	MAX30102_Init();
	printf("start_init\r\n");
	delay_1us(2);
	

//	MAX30102_ReadFifo(data);
//	for(int i=0;i<sizeof(data);i++)
//	{
//		printf("data=%x\r\n",data[i]);
//	}

	
while(1){

	MAX30102_ReadFifo(p_red,p_ir);

//	printf("p_red=%d\r\n",p_red[0]);
//	printf("p_ir=%d\r\n",p_ir[0]);
	
	delay_1ms(1000);
	//printf("%d\r\n",GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_13));
}
}

