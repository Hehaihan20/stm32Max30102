
/****************************************Copyright (c)****************************************************
 
**--------------File Info---------------------------------------------------------------------------------
** File name:    			MAX30102.h             
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

#ifndef __MAX30102_H
#define __MAX30102_H


#include "stm32f10x.h"

#include "I2C.h"
struct compx EE(struct compx a,struct compx b);
/*R=((ir_max+ir_min)(red_max-red_min))/((red_max+red_min)(ir_max-ir_min));
SpO2= ((-45.060)RR + 30.354*R + 94.845).*/
//1=((-9)RR + 6*R
//Heart_Rate = 60.00 * ((100.0 * s1_max_index )/ 512.00)+20;
#define MAX30102_INPUT_PIN_RCC	  RCC_APB2Periph_GPIOC
#define MAX30102_INPUT_GPIO				GPIOC
#define MAX30102_INPUT_PIN      	GPIO_Pin_13

//register addresses
#define Interrupt_Status1_REG			0x00
#define Interrupt_Status2_REG			0x01
#define	Interrupt_Enable1_REG			0x02
#define	Interrupt_Enable2_REG			0x03
#define	FIFOWrite_Pointer_REG			0x04
#define	Overflow__Counter_REG			0x05
#define	FIFO_Read_Pointer_REG			0x06
#define	FIFO_Data_Register_REG		0x07
#define FIFO_Configuration_REG 		0x08
#define	Mode_Configuration_REG 		0x09
#define	SpO2_Configuration_REG 		0x0A
#define LED1Pulse_Amplitude_REG  	0x0C
#define LED2Pulse_Amplitude_REG  	0x0D
#define MulLED_ModeControl1_REG		0x11 
#define MulLED_ModeControl2_REG		0x12
#define Die_TempInteger_REG				0x1F
#define	Die_TempFraction_REG			0x20 
#define Die_Tempe_Config_REG	 		0x21
#define Revision_ID_REG 					0xFE
#define Part_ID_REG 							0xFF
#define REG_PILOT_PA 							0x10
void MAX30102_INPUT_INIT(void);
void MAX30102_Reset(void);
void MAX30102_Init(void);
void MAX30102_I2C_Write(u8 reg_addr,u8 dat);
void MAX30102_I2C_Read(u8 reg_addr,u8 *dat,u8 num);
//void MAX30102_ReadFifo(uint16_t *p_red, uint16_t *p_ir);
void MAX30102_ReadFifo(uint16_t *pdata);
uint16_t MAX30102_GetMax(uint16_t* num,uint16_t size);
uint16_t MAX30102_GetMin(uint16_t* num,uint16_t size);
float MAX30102_GetR(uint16_t *red,uint16_t *ir,uint16_t size );
uint16_t MAX30102_GetHigh();


#endif