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

	//NCS管脚配置
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;		
	GPIO_InitStruct.GPIO_Pin  = TDA5235_NCS_PIN;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(TDA5235_NCS_PORT,&GPIO_InitStruct);

	//POWER_ON配置
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;		
	GPIO_InitStruct.GPIO_Pin  = TDA5235_POWER_PIN;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(TDA5235_POWER_PORT,&GPIO_InitStruct);	
	
	SPI1_Init();
	//TDA5235 SPI_CLOCK = 2.2Mhz(数据手册145页参数J1)
	//STM32 SPI时钟是64Mhz/2 = 32Mhz
	//所以进行16分频
	SPI1_SetSpeed(SPI_BaudRatePrescaler_16);

	TDA5235_Power(ENABLE);

	//中断初始化
	TDA5235_Receive_INT_Init();
}

//接收中断初始化
void TDA5235_Receive_INT_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;

	EXTI_InitTypeDef EXTI_InitStructure;
 	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);	//使能复用功能时钟

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);//使能GPIOB时钟

	//中断管脚配置
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPU;		//上拉输入
	GPIO_InitStruct.GPIO_Pin  = TDA5235_INT_PIN;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(TDA5235_INT_PORT,&GPIO_InitStruct);	

	//中断配置
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB,GPIO_PinSource0); 

	EXTI_InitStructure.EXTI_Line	=	EXTI_Line0;
	EXTI_InitStructure.EXTI_Trigger = 	EXTI_Trigger_Falling;
	EXTI_InitStructure.EXTI_Mode 	= 	EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_LineCmd = 	ENABLE;
	EXTI_Init(&EXTI_InitStructure);


	NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;			  //使能按键WK_UP所在的外部中断通道
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 13;	  //抢占优先级2， 
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0; 				  //子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;							  //使能外部中断通道
	NVIC_Init(&NVIC_InitStructure); 
}

void TDA5235_SFR_Init(void)
{
	u8 i;
	for(i=0;i<init_length;i++)
	{
		TDA5235_Write_Register(init_data[i][0],init_data[i][1]);
	}
}


void TDA5235_Power(FunctionalState NewStatus)
{
	if(NewStatus==ENABLE){
		TDA5235_POWER_ON;
		//需要延时后，才能正常工作
		delay_ms(200);
	}else{
		TDA5235_POWER_OFF;
	}
}

//读寄存器
//addr:寄存器地址
//返回值:寄存器的值
u8 TDA5235_Read_Register(u8 addr)
{
	u8 res;
	TDA5235_SELECT;
	SPI1_ReadWriteByte(TDA5235_INS_RD);
	SPI1_ReadWriteByte(addr);
	res = SPI1_ReadWriteByte(0xFF);

	TDA5235_DISSELECT;
	return res;
}

//连续读寄存器
//addr:开始地址
//buffer:数据存储指针地址
//len:要读取的数据长度
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

//写寄存器
//addr:地址
//data:数据
void TDA5235_Write_Register(u8 addr,u8 data)
{
	TDA5235_SELECT;
	SPI1_ReadWriteByte(TDA5235_INS_WR);
	SPI1_ReadWriteByte(addr);
	SPI1_ReadWriteByte(data);
	TDA5235_DISSELECT;
}

//连续写寄存器
//start_addr:开始地址
//buffer:数据指针
//len:数据长度
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

//读FIFO
//FIFO:数据指针
//返回值:收到的字节数
//返回值:0代表没有收到数据，或者数据溢出错误
u8 TDA5235_Read_FIFO(u8 * const FIFO)
{
	u8 i=0,j=0;
	u8 Status=0,Bits=0;
	TDA5235_SELECT;
	while(1)
	{
		SPI1_ReadWriteByte(TDA5235_INS_RDF);
		for(i=0;i<4;i++)
		{
			FIFO[i+j*4] = SPI1_ReadWriteByte(0xFF);
		}
		Status = SPI1_ReadWriteByte(0xFF);
		if(Status & (1<<7)) 
			return 0;		//S7 代表数据出错，或者溢出
		else if(Status>0){ 
			Bits+=Status;
			j++;
		}

		if(Status != 0x20) break;
	}

	TDA5235_DISSELECT;
	return (Bits+7)/8;	//以8向上取整
}

// TDA5235_FIFO_NUM全局变量为0，表示发射端没有给接收端发送数据
u8 TDA5235_FIFO_NUM=0;
// TDA5235_FIFO接收端的接收数据缓冲区，将发射端发送过来的数据存储到这个缓冲区中
u8 TDA5235_FIFO[TDA5235_MAX_FIFO_SIZE] = {0};

//外部中断0服务程序 
void EXTI0_IRQHandler(void)
{
	u8 i;

	if(!TDA5235_FIFO_NUM){		//清零后(被读取后)才会更新 新的数据
		//数据先全部清零
    	for(i=0;i<TDA5235_MAX_FIFO_SIZE;i++){
      		TDA5235_FIFO[i] = 0;  
    	}
	
		TDA5235_FIFO_NUM = TDA5235_Read_FIFO(TDA5235_FIFO);
		
		#ifdef CRC8_CAL_ENABLE

    	//数据长度太短，直接丢弃
    	if(TDA5235_FIFO_NUM<CRC8_CAL_LENGTH){
        	TDA5235_FIFO_NUM = 0;
    	}

		if( CRC8_Calc(TDA5235_FIFO,CRC8_CAL_LENGTH)!=  TDA5235_FIFO[CRC8_CAL_LENGTH])
			TDA5235_FIFO_NUM = 0;
		#endif
	}

	EXTI_ClearITPendingBit(EXTI_Line0); //清除LINE0上的中断标志位  
}


