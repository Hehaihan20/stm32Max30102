#include "Usart1.h"
void USART_GPIO_INIT(){
		GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(USART1_RCC , ENABLE);

  GPIO_InitStructure.GPIO_Pin = USART1_TX_GPIO;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_10MHz;
  GPIO_Init(USART1_GPIO, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = USART1_RX_GPIO ;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_10MHz;
  GPIO_Init(USART1_GPIO, &GPIO_InitStructure);
}
	
	
void USART_PARAM_CONFIG(){
	NVIC_InitTypeDef NVIC_InitStruct;
  USART_InitTypeDef USART_InitStructure;
	
	//	USART_DeInit(USART1);//��λ����
  USART_InitStructure.USART_BaudRate = 115200;
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;
  USART_InitStructure.USART_StopBits = USART_StopBits_1;
  USART_InitStructure.USART_Parity = USART_Parity_No;
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
  USART_Init(USART1, &USART_InitStructure);
	
	USART_ITConfig(USART1,USART_IT_RXNE,ENABLE);//���ô��ڽ����ж�
	USART_Cmd(USART1,ENABLE);//ʹ�ܴ���
	//�����ж�
	NVIC_InitStruct.NVIC_IRQChannel=USART1_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelCmd=ENABLE;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority=1;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority=1;
	NVIC_Init(&NVIC_InitStruct);


}

//���ڷ���1�ֽ�����
void SerialPort_SendByte(uint8_t send_byte){
    //�������ݼĴ���TDR��д������
    USART_SendData(USART1, send_byte);
    //ȷ�����ݱ�ת�Ƶ�������λ�Ĵ������ȴ���־λTXE��λ��
    while(USART_GetFlagStatus(USART1, USART_FLAG_TXE)==RESET);
}

//��printf�����ض���-��fputc����ԭ���ض��򵽴���
//ע��ptintf���������Ͼ���ѭ������fputc�����ַ�һ��һ�����
int fputc(int ch, FILE *f){
    SerialPort_SendByte(ch);
    return ch;

}
void USART1_IRQHandler(void)                	//����1�жϷ������
	{
	u8 Res;

if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)  //�����ж�(���յ������ݱ�����0x0d 0x0a��β)
		{	Res =USART_ReceiveData(USART1);
	//	Res =USART_ReceiveData(USART1);//(USART1->DR);	//��ȡ���յ�������
		printf("%c\r\n",Res);
		}


} 

