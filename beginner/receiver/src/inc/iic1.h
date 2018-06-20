#ifndef _IIC1_H
#define _IIC1_H

#include "sys.h"


#define IIC1_GPIO_PORT		GPIOB
#define IIC1_SCL_GPIO_PIN	GPIO_Pin_6
#define IIC1_SDA_GPIO_PIN	GPIO_Pin_7


//IO方向设置
#define IIC1_SDA_IN()  {IIC1_GPIO_PORT->CRL&=0X0FFFFFFF; IIC1_GPIO_PORT->CRL|=4<<28;}
#define IIC1_SDA_OUT() {IIC1_GPIO_PORT->CRL&=0X0FFFFFFF; IIC1_GPIO_PORT->CRL|=3<<28;}

//IO操作函数	 
#define IIC1_SCL    	PBout(6) //SCL
#define IIC1_SDA    	PBout(7) //SDA	 
#define IIC1_READ_SDA   PBin(7)  //输入SDA 

//IIC1所有操作函数
void IIC1_Init(void);                //初始化IIC1的IO口				 
void IIC1_Start(void);				//发送IIC1开始信号
void IIC1_Stop(void);	  			//发送IIC1停止信号
void IIC1_Send_Byte(u8 txd);		//IIC1发送一个字节
u8 IIC1_Read_Byte(unsigned char ack);	//IIC1读取一个字节
u8 IIC1_Wait_Ack(void); 			//IIC1等待ACK信号
void IIC1_Send_Ack(void);			//IIC1发送ACK信号
void IIC1_Send_NAck(void);		//IIC1不发送ACK信号
  
#endif



















