#include "sys.h"
#include "delay.h"
#include "spi.h"

#include "TDA5235_SFR_INIT.h"
#include "TDA5235.h"

//NCS  -->	PA3
void TDA5235_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);

	//NCS�ܽ�����
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;		
	GPIO_InitStruct.GPIO_Pin  = TDA5235_NCS_PIN;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(TDA5235_NCS_PORT,&GPIO_InitStruct);

	//POWER_ON����
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;		
	GPIO_InitStruct.GPIO_Pin  = TDA5235_POWER_PIN;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(TDA5235_POWER_PORT,&GPIO_InitStruct);	

	SPI1_Init();
	//TDA5235 SPI_CLOCK = 2.2Mhz(�����ֲ�Infineon-TDA5235-DS-v01_00-en��145ҳ����J1)
	//STM32 SPIʱ����64Mhz/2 = 32Mhz
	//���Խ���16��Ƶ
	SPI1_SetSpeed(SPI_BaudRatePrescaler_16);

	TDA5235_Power(ENABLE);

	//�жϳ�ʼ��
	TDA5235_Receive_INT_Init();
}

//�����жϳ�ʼ��
void TDA5235_Receive_INT_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;

	EXTI_InitTypeDef EXTI_InitStructure;
 	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);	//ʹ�ܸ��ù���ʱ��

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);//ʹ��GPIOBʱ��

	//�жϹܽ�����
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPU;		//��������
	GPIO_InitStruct.GPIO_Pin  = TDA5235_INT_PIN;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(TDA5235_INT_PORT,&GPIO_InitStruct);	

	//�ж�����
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB,GPIO_PinSource0); 

	EXTI_InitStructure.EXTI_Line	=	EXTI_Line0;
	EXTI_InitStructure.EXTI_Trigger = 	EXTI_Trigger_Falling;
	EXTI_InitStructure.EXTI_Mode 	= 	EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_LineCmd = 	ENABLE;
	EXTI_Init(&EXTI_InitStructure);


	NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;			  //ʹ�ܰ���WK_UP���ڵ��ⲿ�ж�ͨ��
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 13;	  //��ռ���ȼ�2�� 
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0; 				  //�����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;							  //ʹ���ⲿ�ж�ͨ��
	NVIC_Init(&NVIC_InitStructure); 
}

void TDA5235_SFR_Init(void)		// ��ʼ��5235�ڲ��ļĴ���
{
	u8 i;
	for(i=0;i<init_length;i++)
	{
		// �������þ���: ��init_data[i][1]ֵд��init_data[i][0]�Ĵ�����
		TDA5235_Write_Register(init_data[i][0],init_data[i][1]);
	}
}


void TDA5235_Power(FunctionalState NewStatus)
{
	if(NewStatus==ENABLE){
		TDA5235_POWER_ON;
		//��Ҫ��ʱ�󣬲�����������
		delay_ms(200);
	}else{
		TDA5235_POWER_OFF;
	}
}

//���Ĵ���
//addr:�Ĵ�����ַ
//����ֵ:�Ĵ�����ֵ
u8 TDA5235_Read_Register(u8 addr)
{
	u8 res;
	TDA5235_SELECT;
	SPI1_ReadWriteByte(TDA5235_INS_RD);		// �����������TDA5235_INS_RD��ʾ��ʼ���Ĵ���
	SPI1_ReadWriteByte(addr);
	res = SPI1_ReadWriteByte(0xFF);

	TDA5235_DISSELECT;
	return res;
}

//�������Ĵ���
//addr:��ʼ��ַ
//buffer:���ݴ洢ָ���ַ������Ͳ���
//len:Ҫ��ȡ�����ݳ���
void TDA5235_Read_Register_Burst(u8 start_addr,u8 *buffer,u8 len)
{
	u8 i;
	TDA5235_SELECT;
	SPI1_ReadWriteByte(TDA5235_INS_RDB);
	SPI1_ReadWriteByte(start_addr);

	for(i=0;i<len;i++)
	{
		*buffer = SPI1_ReadWriteByte(0xFF);
		buffer++;
	}
	
	TDA5235_DISSELECT;
}

//д�Ĵ���
//addr:��ַ
//data:����
void TDA5235_Write_Register(u8 addr,u8 data)
{
	TDA5235_SELECT;
	SPI1_ReadWriteByte(TDA5235_INS_WR);
	SPI1_ReadWriteByte(addr);
	SPI1_ReadWriteByte(data);
	TDA5235_DISSELECT;
}

//����д�Ĵ���
//start_addr:��ʼ��ַ
//buffer:����ָ��
//len:���ݳ���
void TDA5235_Write_Register_Burst(u8 start_addr,u8 *buffer,u8 len)
{
	u8 i;
	TDA5235_SELECT;
	SPI1_ReadWriteByte(TDA5235_INS_WRB);
	SPI1_ReadWriteByte(start_addr);

	for(i=0;i<len;i++)
	{
		SPI1_ReadWriteByte(*buffer);
		buffer++;
	}
	
	TDA5235_DISSELECT;
}

//��FIFO
//FIFO:����ָ��
//����ֵ:�յ����ֽ���
//����ֵ:0����û���յ����ݣ����������������
u8 TDA5235_Read_FIFO(u8 * const FIFO)		// FIFO������5235�洢ͨ�����߽��յ�����Ϣ
{	// �����δ�����Ҫ�������ֲ�Infineon-TDA5235-DS-v01_00-en��2.5.2����FIFO���ֵ�SPI Data FIFO Read��ͼ��
	u8 i=0,j=0;
	u8 Status=0,Bits=0;
	TDA5235_SELECT;
	while(1)
	{
		SPI1_ReadWriteByte(TDA5235_INS_RDF);
		for(i=0;i<4;i++)
		{
			FIFO[i+j*4] = SPI1_ReadWriteByte(0xFF);		// ÿһ�ζ�8λ��ѭ��4�Σ�һ����32λ��Ҳ����4byte
		}
		Status = SPI1_ReadWriteByte(0xFF);				// status�Ƕ������ĵ�5byte��Ҳ����bit32λ
		if(Status & (1<<7)) // ���bit7Ϊ0����ִ��return 0
			return 0;		//S7 �������ݳ����������
		else if(Status>0){ 
			Bits+=Status;	// ������
			j++;
		}

		if(Status != 0x20) 	// һֱ��status����0x20(Ҳ����bit5Ϊ1)��0x20������32λ������
			break;
	}

	TDA5235_DISSELECT;
	return (Bits+7)/8;	//��8����ȡ��
}


u8 TDA5235_FIFO_NUM=0;							// �����������ж��ٸ���Ч����
u8 TDA5235_FIFO[TDA5235_MAX_FIFO_SIZE] = {0};	// ���ﶨ��FIFO�����Ǹ�STM32�õģ�STM32ͨ��Ӳ�������������ݺ�ŵ���������е�

//�ⲿ�ж�0������� 
void EXTI0_IRQHandler(void)		// 5235�ڲ���FIFO���յ����ݺ󣬷����ⲿ�жϣ�����STM32�������������ˣ�����ִ��
{
	u8 i;

	if(!TDA5235_FIFO_NUM){		//�����(����ȡ��)�Ż���� �µ�����
		//������ȫ������
    	for(i=0;i<TDA5235_MAX_FIFO_SIZE;i++){
      		TDA5235_FIFO[i] = 0;  
    	}

	// ��ȡTDA5235_FIFO�����е�����
		TDA5235_FIFO_NUM = TDA5235_Read_FIFO(TDA5235_FIFO);
		// �������ݺ�Ҫ����CRCУ��
		#ifdef CRC8_CAL_ENABLE

    	// ���ݳ���̫�̣�ֱ�Ӷ���
    	// ���ǽ��ն�У�����13������
    	if(TDA5235_FIFO_NUM<CRC8_CAL_LENGTH){
        	TDA5235_FIFO_NUM = 0;
    	}

		if( CRC8_Calc(TDA5235_FIFO,CRC8_CAL_LENGTH)!=  TDA5235_FIFO[CRC8_CAL_LENGTH])
			TDA5235_FIFO_NUM = 0;
		#endif
	}

	EXTI_ClearITPendingBit(EXTI_Line0); //���LINE0�ϵ��жϱ�־λ
}


