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
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);//����ʱ��
		
	TIM_InternalClockConfig(TIM2);//ѡ���ڲ�ʱ��
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseInitStruct;//����ṹ��  Ȼ����д����
	TIM_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1 ;//ѡ��1��Ƶ
	TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;//������ģʽ ����ѡ��Ϊ���ϼ���
	TIM_TimeBaseInitStruct.TIM_Period = 10000-1;//arr -1����Ϊ��ƫ��  �Զ���װ��ֵ
	TIM_TimeBaseInitStruct.TIM_Prescaler = 7200-1;//psc  Ԥ��Ƶϵ��  72M��Ƶ7200�õ�10K��
	TIM_TimeBaseInitStruct.TIM_RepetitionCounter = 0;//�ظ��������߼���ʱ�����У�����ֱ�Ӹ�0

	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseInitStruct);//����ʵ����Ԫ
	TIM_ClearITPendingBit(TIM2,TIM_FLAG_Update);//����ճ�ʼ���ͽ����жϵ�����
	TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);//ʹ���ж�	 TIM2 ���ϼ��� ʹ�������Ϳ�����
		
	//�������ȼ�NVIC
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//����NVIC���� ��������Ϊ����2
	NVIC_InitTypeDef NVIC_InitStruct;//����NVIC�Ľṹ��
	NVIC_InitStruct.NVIC_IRQChannel = TIM2_IRQn;//ѡ��ʱ��2���ж�
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;//ʹ��
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 2;//ǿռ���ȼ�
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 1;//��Ӧ���ȼ�
		
	NVIC_Init(&NVIC_InitStruct);
	TIM_Cmd(TIM2, ENABLE);//������ʱ��
}


void TIM2_IRQHandler()
{

	if(TIM_GetFlagStatus(TIM2,TIM_FLAG_Update) == SET)//���һ���жϱ�־λ
	{
		printf("123\r\n");
		blood_Loop();
		TIM_ClearITPendingBit(TIM2,TIM_FLAG_Update);//����жϱ�־λ
	}

}


