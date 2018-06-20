#include "sys.h"
#include "iic1.h"
#include "delay.h"
// STM32作为主设备，OLED屏作为从设备
// 初始化IIC1；IIC主要是为了OLED使用的；主要分为4个层次，(1)最底层的是标准库，
// (2)之后是IIC驱动(也就是本文件)，(3)之后是OLED(显示数字、bmp图片等)；(4)最后是客户要的应用程序
void IIC1_Init(void)	// 对IIC相应寄存器引脚的初始化
{					     
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE );	
	   
	GPIO_InitStructure.GPIO_Pin = IIC1_SCL_GPIO_PIN|IIC1_SDA_GPIO_PIN;	// PB6/PB7
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD ;   //开漏输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(IIC1_GPIO_PORT, &GPIO_InitStructure);
	GPIO_SetBits(IIC1_GPIO_PORT,IIC1_SCL_GPIO_PIN|IIC1_SDA_GPIO_PIN);
}

// IIC时序
void IIC1_Start(void)		// 发送信号
{
	IIC1_SDA_OUT();
	IIC1_SDA = 1;
	IIC1_SCL = 1;
	delay_us(2);
	IIC1_SDA = 0;	//时钟高时，拉低信号线
	delay_us(2);
	IIC1_SCL = 0;
}

void IIC1_Stop(void)		// 结束信号
{
	IIC1_SDA_OUT();
	IIC1_SDA = 0;
	IIC1_SCL = 0;
	delay_us(2);
	IIC1_SCL = 1;
	IIC1_SDA = 1;	//时钟高时，拉高信号线
	delay_us(2);
}

/*
*	等待ACK
*   返回值1 等待失败
*   返回值0 等待ACK成功
*/
u8 IIC1_Wait_Ack(void)		// 等待主机给我响应
{
	u8 ucOvertime;
	IIC1_SDA_IN();
	IIC1_SDA = 1;
	IIC1_SCL = 1;
	while(IIC1_READ_SDA)
	{
		ucOvertime++;			// 超时设置
		{
			if(ucOvertime>250)
			{
				IIC1_Stop();
				return 1;
			}
		}
	}
	IIC1_SCL = 0;
	return 0;
}

void IIC1_Send_Ack(void)		// 发送响应
{
	IIC1_SDA_OUT();
	IIC1_SCL = 0;
	IIC1_SDA = 0;
	//delay_us(4);
	IIC1_SCL = 1;
	//delay_us(4);
	IIC1_SCL = 0;
}

void IIC1_Send_NAck(void)		// 发送高低电平响应
{
	IIC1_SDA_OUT();
	IIC1_SCL = 0;
	IIC1_SDA = 1;
	//delay_us(4);
	IIC1_SCL = 1;
	//delay_us(4);
	IIC1_SCL = 0;
}


/* IIC1发送一个字节 */
void IIC1_Send_Byte(u8 data)		// 发送一个字节
{
	u8 i;
	IIC1_SDA_OUT();
	IIC1_SCL = 0;
	for(i=0;i<8;i++)
	{
		delay_us(4);
		IIC1_SDA = (data & 0x80) >> 7;		// & 0x80是高位在前
		data <<=1;
		IIC1_SCL = 1;
		//delay_us(4);
		IIC1_SCL = 0;
	}
}


/* IIC1读一个字节  */
u8 IIC1_Read_Byte(unsigned char ack)		// 读一个字节
{
	u8 i,receive_data;
	IIC1_SDA_IN();
	IIC1_SDA = 1;
	for(i=0;i<8;i++)
	{
		IIC1_SCL = 0;
		//delay_us(4);
		IIC1_SCL = 1;
		receive_data <<=1;
		if(IIC1_READ_SDA) receive_data |= 1;
		//delay_us(4);
	}
    if (!ack)
        IIC1_Send_NAck();//发送nACK
    else
        IIC1_Send_Ack(); //发送ACK
	return receive_data;
}

