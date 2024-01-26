#include "MAX30102.h"
void MAX30102_INPUT_INIT(void){

		 GPIO_InitTypeDef GPIO_InitStructure;
  
     RCC_APB2PeriphClockCmd(MAX30102_INPUT_PIN_RCC, ENABLE);   //开启GPIOB时钟
     GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IN_FLOATING;            //设置开漏复用模式
     GPIO_InitStructure.GPIO_Pin   = MAX30102_INPUT_PIN ;    //I2C1_SCL <--> PB6  I2C1_SDA <--> PB7
     GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
     GPIO_Init(MAX30102_INPUT_GPIO, &GPIO_InitStructure);
	   GPIO_SetBits(GPIOC,GPIO_Pin_13);
		 
 
}

void MAX30102_I2C_Write(u8 reg_addr,u8 dat){
		I2C_WriteByte(MAX30102_ADDRESS, reg_addr,dat);
}
void MAX30102_I2C_Read(u8 reg_addr,u8 *dat,u8 num){
		
		I2C_ReadByte(MAX30102_ADDRESS, reg_addr,dat,num);
}

void MAX30102_Init(void)
{
		MAX30102_INPUT_INIT();
		I2C_Configuration( );
		delay_1ms( 200 );
		MAX30102_I2C_Write(Interrupt_Enable1_REG,0xc0);
		MAX30102_I2C_Write(Interrupt_Enable2_REG,0x00);
		MAX30102_I2C_Write(FIFOWrite_Pointer_REG,0x00);	
		MAX30102_I2C_Write(Overflow__Counter_REG,0x00);	
		MAX30102_I2C_Write(FIFO_Read_Pointer_REG,0x00);

		MAX30102_I2C_Write(FIFO_Configuration_REG,0x0f);	
		MAX30102_I2C_Write(Mode_Configuration_REG,0x03);
		MAX30102_I2C_Write(SpO2_Configuration_REG,0x27);//adc range 4096na spo2 sample rate =100hz Led_pluse-width=411us
		MAX30102_I2C_Write(LED1Pulse_Amplitude_REG,0x24);
		MAX30102_I2C_Write(LED2Pulse_Amplitude_REG,0x24);
		MAX30102_I2C_Write (REG_PILOT_PA,0x7f);
		MAX30102_I2C_Write(Revision_ID_REG,0x24);				



}
void MAX30102_Shutdown(){

MAX30102_I2C_Write(Mode_Configuration_REG,0x80);
}
void MAX30102_Reset(void){
MAX30102_I2C_Write(Mode_Configuration_REG,0x40);

}
void MAX30102_ReadFifo(uint16_t *p_red, uint16_t *p_ir){


	u8 temp;
  u8 data[6];
	uint16_t un_temp;

  

	MAX30102_I2C_Read(Interrupt_Status1_REG,&temp,1);

	MAX30102_I2C_Read(Interrupt_Status2_REG,&temp,1);
	


	MAX30102_I2C_Read(FIFO_Data_Register_REG,data,6);
	

	un_temp	=	data[0];

	un_temp<<=14;
	*p_red=un_temp;
//	printf("p_ir\r\n");
	un_temp	=	data[1];
	un_temp<<=6;
	*p_red+=un_temp;
	un_temp>>=2;
	un_temp	=	data[2];
	*p_red+=un_temp;
		
	un_temp	=	data[3];
	un_temp<<=14;
	*p_ir=un_temp;
	un_temp	=	data[4];
	un_temp<<=6;
	*p_ir+=un_temp;
	un_temp	=	data[5];
	un_temp>>=2;
	*p_ir+=un_temp;

//	 printf("p_ir=%d\r\n",*p_ir);
//	 printf("p_red=%d\r\n",*p_red);
	*p_red&=0xFFFF;  //Mask MSB [23:18]
  *p_ir&=0xFFFF;  //Mask MSB [23:18]
}

