#ifndef _IIC1_H
#define _IIC1_H

#include "sys.h"


#define IIC1_GPIO_PORT		GPIOB
#define IIC1_SCL_GPIO_PIN	GPIO_Pin_6
#define IIC1_SDA_GPIO_PIN	GPIO_Pin_7


//IO��������
#define IIC1_SDA_IN()  {IIC1_GPIO_PORT->CRL&=0X0FFFFFFF; IIC1_GPIO_PORT->CRL|=4<<28;}
#define IIC1_SDA_OUT() {IIC1_GPIO_PORT->CRL&=0X0FFFFFFF; IIC1_GPIO_PORT->CRL|=3<<28;}

//IO��������	 
#define IIC1_SCL    	PBout(6) //SCL
#define IIC1_SDA    	PBout(7) //SDA	 
#define IIC1_READ_SDA   PBin(7)  //����SDA 

//IIC1���в�������
void IIC1_Init(void);                //��ʼ��IIC1��IO��				 
void IIC1_Start(void);				//����IIC1��ʼ�ź�
void IIC1_Stop(void);	  			//����IIC1ֹͣ�ź�
void IIC1_Send_Byte(u8 txd);		//IIC1����һ���ֽ�
u8 IIC1_Read_Byte(unsigned char ack);	//IIC1��ȡһ���ֽ�
u8 IIC1_Wait_Ack(void); 			//IIC1�ȴ�ACK�ź�
void IIC1_Send_Ack(void);			//IIC1����ACK�ź�
void IIC1_Send_NAck(void);		//IIC1������ACK�ź�
  
#endif



















