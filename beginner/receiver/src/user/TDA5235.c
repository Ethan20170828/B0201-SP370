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
	//TDA5235 SPI_CLOCK = 2.2Mhz(数据手册Infineon-TDA5235-DS-v01_00-en的145页参数J1)
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

void TDA5235_SFR_Init(void)		// 初始化5235内部的寄存器
{
	u8 i;
	for(i=0;i<init_length;i++)
	{
		// 函数作用就是: 将init_data[i][1]值写到init_data[i][0]寄存器中
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
	SPI1_ReadWriteByte(TDA5235_INS_RD);		// 发送这个命令TDA5235_INS_RD表示开始读寄存器
	SPI1_ReadWriteByte(addr);
	res = SPI1_ReadWriteByte(0xFF);

	TDA5235_DISSELECT;
	return res;
}

//连续读寄存器
//addr:开始地址
//buffer:数据存储指针地址，输出型参数
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
u8 TDA5235_Read_FIFO(u8 * const FIFO)		// FIFO用来给5235存储通过无线接收到的信息
{	// 理解这段代码需要看数据手册Infineon-TDA5235-DS-v01_00-en的2.5.2介绍FIFO部分的SPI Data FIFO Read框图中
	u8 i=0,j=0;
	u8 Status=0,Bits=0;
	TDA5235_SELECT;
	while(1)
	{
		SPI1_ReadWriteByte(TDA5235_INS_RDF);
		for(i=0;i<4;i++)
		{
			FIFO[i+j*4] = SPI1_ReadWriteByte(0xFF);		// 每一次读8位，循环4次，一共读32位，也就是4byte
		}
		Status = SPI1_ReadWriteByte(0xFF);				// status是读出来的第5byte，也就是bit32位
		if(Status & (1<<7)) // 如果bit7为0，则执行return 0
			return 0;		//S7 代表数据出错，或者溢出
		else if(Status>0){ 
			Bits+=Status;	// 计数，
			j++;
		}

		if(Status != 0x20) 	// 一直到status等于0x20(也就是bit5为1)，0x20代表是32位，跳出
			break;
	}

	TDA5235_DISSELECT;
	return (Bits+7)/8;	//以8向上取整
}


u8 TDA5235_FIFO_NUM=0;							// 告诉数组中有多少个有效数据
u8 TDA5235_FIFO[TDA5235_MAX_FIFO_SIZE] = {0};	// 这里定义FIFO数组是给STM32用的，STM32通过硬件读出来的数据后放到这个数组中的

//外部中断0服务程序 
void EXTI0_IRQHandler(void)		// 5235内部的FIFO接收到数据后，发送外部中断，告诉STM32，我这有数据了，快来执行
{
	u8 i;

	if(!TDA5235_FIFO_NUM){		//清零后(被读取后)才会更新 新的数据
		//数据先全部清零
    	for(i=0;i<TDA5235_MAX_FIFO_SIZE;i++){
      		TDA5235_FIFO[i] = 0;  
    	}

	// 读取TDA5235_FIFO数组中的数据
		TDA5235_FIFO_NUM = TDA5235_Read_FIFO(TDA5235_FIFO);
		// 读完数据后，要进行CRC校验
		#ifdef CRC8_CAL_ENABLE

    	// 数据长度太短，直接丢弃
    	// 我们接收端校验的是13个数据
    	if(TDA5235_FIFO_NUM<CRC8_CAL_LENGTH){
        	TDA5235_FIFO_NUM = 0;
    	}

		if( CRC8_Calc(TDA5235_FIFO,CRC8_CAL_LENGTH)!=  TDA5235_FIFO[CRC8_CAL_LENGTH])
			TDA5235_FIFO_NUM = 0;
		#endif
	}

	EXTI_ClearITPendingBit(EXTI_Line0); //清除LINE0上的中断标志位
}


