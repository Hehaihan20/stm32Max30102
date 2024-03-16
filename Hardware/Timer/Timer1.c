#include "Timer1.h"
#include "blood.h"
void TIMER1_GPIO_INIT(){
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(TIMER1_RCC , ENABLE);
  GPIO_InitStructure.GPIO_Pin = TIMER1_GPIO_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_10MHz;
  GPIO_Init(TIMER1_GPIO, &GPIO_InitStructure);
	
}
	
//arr=Tclk/Psc*time
void TIMER1_PARAM_CONFIG(){
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);//开启时钟
		
	TIM_InternalClockConfig(TIM2);//选择内部时钟
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseInitStruct;//定义结构体  然后填写参数
	TIM_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1 ;//选择1分频
	TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;//计数器模式 这里选择为向上计数
	TIM_TimeBaseInitStruct.TIM_Period = 10000-1;//arr -1是因为有偏差  自动重装载值
	TIM_TimeBaseInitStruct.TIM_Prescaler = 7200-1;//psc  预分频系数  72M分频7200得到10K的
	TIM_TimeBaseInitStruct.TIM_RepetitionCounter = 0;//重复计数器高级定时器才有，这里直接给0

	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseInitStruct);//配置实基单元
	TIM_ClearITPendingBit(TIM2,TIM_FLAG_Update);//避免刚初始化就进入中断的问题
	TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);//使能中断	 TIM2 向上计数 使能这样就可以了
		
	//配置优先级NVIC
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//配置NVIC分组 这里配置为分组2
	NVIC_InitTypeDef NVIC_InitStruct;//配置NVIC的结构体
	NVIC_InitStruct.NVIC_IRQChannel = TIM2_IRQn;//选择定时器2的中断
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;//使能
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 2;//强占优先级
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 1;//响应优先级
		
	NVIC_Init(&NVIC_InitStruct);
	TIM_Cmd(TIM2, ENABLE);//启动定时器
}


void TIM2_IRQHandler()
{

	if(TIM_GetFlagStatus(TIM2,TIM_FLAG_Update) == SET)//检查一下中断标志位
	{
		printf("123\r\n");
		blood_Loop();
		TIM_ClearITPendingBit(TIM2,TIM_FLAG_Update);//清除中断标志位
	}

}


