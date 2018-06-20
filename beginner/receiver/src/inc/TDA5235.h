#ifndef _TDA5235_H
#define _TDA5235_H


#define CRC8_CAL_ENABLE
#ifdef CRC8_CAL_ENABLE
#include<CRC.h>
	#define CRC8_CAL_LENGTH 13		//总共发射14个数据	最后一个是CRC
#endif


//NCS  -->	PA4
#define TDA5235_NCS_PORT	GPIOA
#define TDA5235_NCS_PIN 	GPIO_Pin_4

//POWER_ON  -->	PA3
#define TDA5235_POWER_PORT	GPIOA
#define TDA5235_POWER_PIN 	GPIO_Pin_3

//TDA5235 INT  -->	PB0
#define TDA5235_INT_PORT	GPIOB
#define TDA5235_INT_PIN 	GPIO_Pin_0


#define TDA5235_INS_WR  0x02
#define TDA5235_INS_RD  0x03
#define TDA5235_INS_RDF 0x04
#define TDA5235_INS_WRB 0x01
#define TDA5235_INS_RDB 0x05

#define TDA5235_POWER_ON	{GPIO_SetBits(TDA5235_POWER_PORT,TDA5235_POWER_PIN);}
#define TDA5235_POWER_OFF	{GPIO_ResetBits(TDA5235_POWER_PORT,TDA5235_POWER_PIN);}

#define TDA5235_SELECT		{GPIO_ResetBits(TDA5235_NCS_PORT,TDA5235_NCS_PIN);}
#define TDA5235_DISSELECT	{GPIO_SetBits(TDA5235_NCS_PORT,TDA5235_NCS_PIN);}


#define TDA5235_MAX_FIFO_SIZE 64
extern u8 TDA5235_FIFO_NUM;
extern u8 TDA5235_FIFO[TDA5235_MAX_FIFO_SIZE];


void TDA5235_Init(void);
void TDA5235_Receive_INT_Init(void);

void TDA5235_SFR_Init(void);


void TDA5235_Power(FunctionalState NewStatus);
u8 TDA5235_Read_Register(u8 addr);
void TDA5235_Read_Register_Burst(u8 start_addr,u8 *buffer,u8 len);
void TDA5235_Write_Register(u8 addr,u8 data);
void TDA5235_Write_Register_Burst(u8 start_addr,u8 *buffer,u8 len);

u8 TDA5235_Read_FIFO(u8 *FIFO);


#endif


