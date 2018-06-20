#include "sys.h"
#include "iic1.h"
#include "delay.h"
// STM32��Ϊ���豸��OLED����Ϊ���豸
// ��ʼ��IIC1��IIC��Ҫ��Ϊ��OLEDʹ�õģ���Ҫ��Ϊ4����Σ�(1)��ײ���Ǳ�׼�⣬
// (2)֮����IIC����(Ҳ���Ǳ��ļ�)��(3)֮����OLED(��ʾ���֡�bmpͼƬ��)��(4)����ǿͻ�Ҫ��Ӧ�ó���
void IIC1_Init(void)	// ��IIC��Ӧ�Ĵ������ŵĳ�ʼ��
{					     
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE );	
	   
	GPIO_InitStructure.GPIO_Pin = IIC1_SCL_GPIO_PIN|IIC1_SDA_GPIO_PIN;	// PB6/PB7
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD ;   //��©���
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(IIC1_GPIO_PORT, &GPIO_InitStructure);
	GPIO_SetBits(IIC1_GPIO_PORT,IIC1_SCL_GPIO_PIN|IIC1_SDA_GPIO_PIN);
}

// IICʱ��
void IIC1_Start(void)		// �����ź�
{
	IIC1_SDA_OUT();
	IIC1_SDA = 1;
	IIC1_SCL = 1;
	delay_us(2);
	IIC1_SDA = 0;	//ʱ�Ӹ�ʱ�������ź���
	delay_us(2);
	IIC1_SCL = 0;
}

void IIC1_Stop(void)		// �����ź�
{
	IIC1_SDA_OUT();
	IIC1_SDA = 0;
	IIC1_SCL = 0;
	delay_us(2);
	IIC1_SCL = 1;
	IIC1_SDA = 1;	//ʱ�Ӹ�ʱ�������ź���
	delay_us(2);
}

/*
*	�ȴ�ACK
*   ����ֵ1 �ȴ�ʧ��
*   ����ֵ0 �ȴ�ACK�ɹ�
*/
u8 IIC1_Wait_Ack(void)		// �ȴ�����������Ӧ
{
	u8 ucOvertime;
	IIC1_SDA_IN();
	IIC1_SDA = 1;
	IIC1_SCL = 1;
	while(IIC1_READ_SDA)
	{
		ucOvertime++;			// ��ʱ����
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

void IIC1_Send_Ack(void)		// ������Ӧ
{
	IIC1_SDA_OUT();
	IIC1_SCL = 0;
	IIC1_SDA = 0;
	//delay_us(4);
	IIC1_SCL = 1;
	//delay_us(4);
	IIC1_SCL = 0;
}

void IIC1_Send_NAck(void)		// ���͸ߵ͵�ƽ��Ӧ
{
	IIC1_SDA_OUT();
	IIC1_SCL = 0;
	IIC1_SDA = 1;
	//delay_us(4);
	IIC1_SCL = 1;
	//delay_us(4);
	IIC1_SCL = 0;
}


/* IIC1����һ���ֽ� */
void IIC1_Send_Byte(u8 data)		// ����һ���ֽ�
{
	u8 i;
	IIC1_SDA_OUT();
	IIC1_SCL = 0;
	for(i=0;i<8;i++)
	{
		delay_us(4);
		IIC1_SDA = (data & 0x80) >> 7;		// & 0x80�Ǹ�λ��ǰ
		data <<=1;
		IIC1_SCL = 1;
		//delay_us(4);
		IIC1_SCL = 0;
	}
}


/* IIC1��һ���ֽ�  */
u8 IIC1_Read_Byte(unsigned char ack)		// ��һ���ֽ�
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
        IIC1_Send_NAck();//����nACK
    else
        IIC1_Send_Ack(); //����ACK
	return receive_data;
}

