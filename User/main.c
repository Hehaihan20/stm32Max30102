#include "Timer1.h"
#include "systick.h"
#include "I2C.h"
#include "MAX30102.h"
#include "oled.h"
#include "bmp.h"
#include "Usart1.h"
#include "blood.h"
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

	u8 HR_tmp=0;
	u8 r_tmp=0;
	u16 cnt=0;

	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);	//设置NVIC中断分组2:2位抢占优先级，2位响应优先级
	systick_config();
	USART_GPIO_INIT();
	USART_PARAM_CONFIG();
	MAX30102_Init();
	I2C_Configuration( );

	OLED_Init();

	OLED_ShowString(0,0,"HR:",16,1);	
	OLED_ShowString(0,16,"SP02:",16,1);	
	OLED_Refresh();
	TIMER1_PARAM_CONFIG();
	while(1){
	

	}

}
