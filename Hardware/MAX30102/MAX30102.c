
/****************************************Copyright (c)****************************************************
 
**--------------File Info---------------------------------------------------------------------------------
** File name:    			MAX30102.c             
** Last modified Date:           2024/1/26    
** Last Version:           		 		1.0
** Descriptions:           
**                        
**--------------------------------------------------------------------------------------------------------
** Created by:               Hange
** Created date:             2024/1/26
** Version:                  1.0
** Descriptions:          
**--------------------------------------------------------------------------------------------------------*/
#include "MAX30102.h"

static float red_max;
static float red_min;
static float ir_max;
static float ir_min;
static u8 Max_index[2];
static u8 Min_index[2];
static u8 H_FLAG;
static u8 L_FLAG;
static u8 cont;
static u8 finger_flag;
//float Heart_Rate = 60.00 * ((100.0 * s1_max_index )/ 512.00)+20;



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
		printf("MAX30102_INPUT_INIT\r\n");
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
//void MAX30102_ReadFifo(uint16_t *p_red, uint16_t *p_ir){


//	u8 temp;
//  u8 data[6];
//	uint16_t un_temp;

//  

//	MAX30102_I2C_Read(Interrupt_Status1_REG,&temp,1);

//	MAX30102_I2C_Read(Interrupt_Status2_REG,&temp,1);
//	


//	MAX30102_I2C_Read(FIFO_Data_Register_REG,data,6);
//	

//	un_temp	=	data[0];

//	un_temp<<=14;
//	*p_red=un_temp;
////	printf("p_ir\r\n");
//	un_temp	=	data[1];
//	un_temp<<=6;
//	*p_red+=un_temp;
//	un_temp>>=2;
//	un_temp	=	data[2];
//	*p_red+=un_temp;
//		
//	un_temp	=	data[3];
//	un_temp<<=14;
//	*p_ir=un_temp;
//	un_temp	=	data[4];
//	un_temp<<=6;
//	*p_ir+=un_temp;
//	un_temp	=	data[5];
//	un_temp>>=2;
//	*p_ir+=un_temp;

////	 printf("p_ir=%d\r\n",*p_ir);
////	 printf("p_red=%d\r\n",*p_red);
//	*p_red&=0xFFFF;  //Mask MSB [23:18]
//  *p_ir&=0xFFFF;  //Mask MSB [23:18]
//}
uint16_t MAX30102_GetMax(uint16_t* num,uint16_t size){
			uint16_t	tmp=0;
	for(int i=0;i<size;i++)
	{
		if(num[i]>tmp)
		{
			tmp=num[i];
			Max_index[H_FLAG]=i;
		}
	}
	
		return tmp;
}
uint16_t MAX30102_GetMin(uint16_t* num,uint16_t size){

			uint16_t	tmp=num[0];
	for(int i=0;i<size;i++)
	{
		if(tmp>num[i])
		{
			tmp=num[i];
			Min_index[L_FLAG]=i;
		}
	}
	
		return tmp;

}
//void MAX30102_Getthreshold(uint16_t max,uint16_t min){
//			u8 max=
//}



uint16_t MAX30102_GetHigh(){
				uint16_t Heart_Rate;
				uint16_t thrdat1;
				uint16_t thrdat2;
			if(L_FLAG==0)
			{
				
				L_FLAG=1;
				printf("\r\n");
			}
			else if(L_FLAG)
			{
				
				printf("Max_index[1]=%d\r\n",Min_index[1]);
				printf("\r\n");
				
				L_FLAG=0;
				printf("Heart_Rate%d\r\n",Heart_Rate);
			//	return	Heart_Rate = 60000/ (25*((100- Min_index[0])+Min_index[1]))+40;
				Heart_Rate = 60000/ (25*((100- Min_index[0])+Min_index[1]))+40;
//				Heart_Rate = (((500- Min_index[0])+Min_index[1]));
//				thrdat1=(Max_index[0]-Min_index[0]);
				if(Heart_Rate>0){
							return	(Heart_Rate);
				}
				
			}
	return	0;
}
float MAX30102_GetR(uint16_t *red,uint16_t *ir,uint16_t size ){
		uint16_t Heart_Rate;
			float	SpO2;
		red_max=(float)MAX30102_GetMax(red,size);
		red_min=(float)MAX30102_GetMin(red,size);
		ir_max=(float)MAX30102_GetMax(ir,size);
		ir_min=(float)MAX30102_GetMin(ir,size);
//		printf("ir_max+ir_min=%f\r\n",ir_max+ir_min);
//		printf("red_max-red_min=%f\r\n",red_max-red_min);
//		printf("ir_max+ir_min*red_max-red_min=%f\r\n",((ir_max+ir_min)*(red_max-red_min)));
//	
//		printf("ir_max-ir_min=%f\r\n",ir_max-ir_min);
//		printf("red_max+red_min=%f\r\n",red_max+red_min);
//		printf("ir_max-ir_min*red_max+red_min=%f\r\n",((ir_max-ir_min)*(red_max+red_min)));
		//SpO2= ((-45.060)RR + 30.354*R + 94.845).
		float R=((ir_max+ir_min)*(red_max-red_min))/((red_max+red_min)*(ir_max-ir_min));
//		printf("R=%f\r\n",R);
//		printf("(-45.060)*(R*R)=%f\r\n",(-45.060)*(R*R));
//		printf("30.354*R=%f\r\n",30.354*R);
	if(finger_flag){
		SpO2= ((-45.060)*(R*R) + 30.354*R + 94.845);
		printf("SpO2%f\r\n",SpO2);
		
	}
	else {SpO2=0;}
		return SpO2 ;
	
}
void MAX30102_ReadFifo(uint16_t *pdata){


	u8 temp;
  u8 data[6];
	uint16_t un_temp;
	finger_flag=1;
  

	MAX30102_I2C_Read(Interrupt_Status1_REG,&temp,1);

	MAX30102_I2C_Read(Interrupt_Status2_REG,&temp,1);
	


	MAX30102_I2C_Read(FIFO_Data_Register_REG,data,6);
	

	pdata[0]=	((data[0]<<14)|(data[1]<<6)|data[2]>>2);
		
	pdata[1]=	(data[3]<<14)|(data[4]<<6)|(data[5]>>2);

//	printf("data=%d\r\n",	((data[0]<<14)|(data[1]<<6)|data[2]>>2));
//	printf("data2=%d\r\n",	(data[3]<<14)|(data[4]<<6)|(data[5]>>2));
//	printf("p_red=%d\r\n",pdata[0]);
//  printf("p_ir=%d\r\n",pdata[1]);

//	 printf("p_red=%d\r\n",*p_red);
	if(pdata[0]<10000){
		pdata[0]=0;
		finger_flag=0;
	}
	if(pdata[1]<10000){
	pdata[1]=0;
			finger_flag=0;
	}
	pdata[0]&=0xFFFF;  //Mask MSB [23:18]
	pdata[1]&=0xFFFF;  //Mask MSB [23:18]

}
