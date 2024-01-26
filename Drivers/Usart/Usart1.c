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
	
	//	USART_DeInit(USART1);//复位串口
  USART_InitStructure.USART_BaudRate = 115200;
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;
  USART_InitStructure.USART_StopBits = USART_StopBits_1;
  USART_InitStructure.USART_Parity = USART_Parity_No;
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
  USART_Init(USART1, &USART_InitStructure);
	
	USART_ITConfig(USART1,USART_IT_RXNE,ENABLE);//设置串口接收中断
	USART_Cmd(USART1,ENABLE);//使能串口
	//设置中断
	NVIC_InitStruct.NVIC_IRQChannel=USART1_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelCmd=ENABLE;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority=1;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority=1;
	NVIC_Init(&NVIC_InitStruct);


}

//串口发送1字节数据
void SerialPort_SendByte(uint8_t send_byte){
    //向发送数据寄存器TDR中写入数据
    USART_SendData(USART1, send_byte);
    //确认数据被转移到发送移位寄存器（等待标志位TXE置位）
    while(USART_GetFlagStatus(USART1, USART_FLAG_TXE)==RESET);
}

//对printf函数重定向-将fputc函数原型重定向到串口
//注：ptintf函数本质上就是循环调用fputc，将字符一个一个输出
int fputc(int ch, FILE *f){
    SerialPort_SendByte(ch);
    return ch;

}
void USART1_IRQHandler(void)                	//串口1中断服务程序
	{
	u8 Res;

if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)  //接收中断(接收到的数据必须是0x0d 0x0a结尾)
		{	Res =USART_ReceiveData(USART1);
	//	Res =USART_ReceiveData(USART1);//(USART1->DR);	//读取接收到的数据
		printf("%c\r\n",Res);
		}


} 

