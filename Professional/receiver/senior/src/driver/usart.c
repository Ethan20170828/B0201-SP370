#include "sys.h"
#include "usart.h"


//////////////////////////////////////////////////////////////////
//加入以下代码,支持printf函数,而不需要选择use MicroLIB	  
#if 1
#pragma import(__use_no_semihosting)             
//标准库需要的支持函数                 
struct __FILE 
{ 
	int handle; 
}; 

FILE __stdout;       
//定义_sys_exit()以避免使用半主机模式    
void _sys_exit(int x) 
{ 
	x = x; 
} 
//重定义fputc函数 
int fputc(int ch, FILE *f)
{      
	while(USART_GetFlagStatus(USART1,USART_FLAG_TC)==RESET); 
    USART_SendData(USART1,(uint8_t)ch);   
	return ch;
}
#endif

/**************************************************/
//函数名字:void USART1_Init(uint32_t baudrate)
//输入参数:baudrate->波特率
//输出参数:
//描述:初始化USART1
/**************************************************/
void USART1_Init(uint32_t baudrate)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	USART_InitTypeDef USART_InitStruct;
	NVIC_InitTypeDef NVIC_InitStruct;

	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);	//??GPIO??
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);	//??USART1??

	//USART1_TX		PA9
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStruct.GPIO_Pin  = GPIO_Pin_9;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStruct);
	
	//USART1_RX		PA10
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_InitStruct.GPIO_Pin  = GPIO_Pin_10;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStruct);

	USART_InitStruct.USART_BaudRate = baudrate;
	USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStruct.USART_Mode = USART_Mode_Tx|USART_Mode_Rx;
	USART_InitStruct.USART_Parity = USART_Parity_No;
	USART_InitStruct.USART_StopBits = USART_StopBits_1;
	USART_InitStruct.USART_WordLength = USART_WordLength_8b;

	USART_Init(USART1,&USART_InitStruct);
	
	//????
	NVIC_InitStruct.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 15;		//????????????
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	
	NVIC_Init(&NVIC_InitStruct);
	USART_ITConfig(USART1,USART_IT_IDLE,ENABLE);	//??????
	USART_ITConfig(USART1,USART_IT_RXNE,ENABLE);	//??????
	USART_Cmd(USART1,ENABLE);
}

/**************************************************/
//函数名字:static void USART1_PUTC(uint8_t c)
//输入参数:c:需要打印的字符
//输出参数:
//描述:打印一个字符
/**************************************************/
static void USART1_PUTC(uint8_t c)
{
	while(USART_GetFlagStatus(USART1,USART_FLAG_TC)==RESET); 
    USART_SendData(USART1,(uint8_t)c);
}

//串口1中断服务程序
//注意,读取USARTx->SR能避免莫名其妙的错误   	
u8 USART1_RxBuffer[USART1_RxBufferSize];     //接收缓冲,最大USART_REC_LEN个字节.

// 接收字符数目(也就是上位机给接收端发送的字节数目)与回车状态；
// 默认是0，表示你通过上位机助手发送的第0个字节
u16 USART1_RX_NUM=0;

// 全局变量，程序中的标记作用，为0表示，表示串口没有收完中控发过来的一包数据
u8 ReceiveState = 0;

// 串口的中断接收程序
// 入门级时不需要串口与外界进行交互，所以不需要，而专业级需要串口与上位机串口助手进行交互，
// 进行命令传递，所以需要串口中断接收程序。
// 实际调试中，串口是最前调的，首先是让串口底层工作，通过串口助手给STM32发送东西(也就是发送协议)，
// 让STM32能接收到上位机串口助手发送到的数据(也就是接收到协议)，而且STM32能够给上位机串口助手回复
// 数据。
// 上位机串口助手属于发送的，而STM32属于接收的；
// 实际做项目的时候也是这样的步骤进行的；
void USART1_IRQHandler(void)                	//串口1中断服务程序
{
	u8 getchar;	
	u8 Clear = Clear;

// 当有人通过串口给我发送东西时，触发中断
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)//接收到数据
	{
		// 接收端接收到串口发送过了一字节数据
		getchar=USART_ReceiveData(USART1); 
		//USART1_PUTC(getchar);

		// 当串口接收到0x0D时，就知道刚好发完一帧数据了
		// 当不等于0x0D时，证明我们现在正在发送一帧数据
		// 0x0D的ASCII码表示回车键，没有特殊含义，所以选择0x0D作为发送的数据结束标记
		// 判断接收到的一个字节是否等于0x0D
		if(getchar != 0x0D)
		{
			// 通过上位机串口助手给我发送一帧数据的时候，给我发送过来的第0个字节放到数组的第0位
			// 放好后给USART1_RX_NUM+1，下次发送过来的第1个字节放到第1位，依次往后放
			// 也就是依次把发送这个数据先放到串口的缓冲区中
			USART1_RxBuffer[USART1_RX_NUM] = getchar;
			// 对USART1_RxBufferSize取余，把发送多余的字节去掉，防止数据溢出，保证程序健壮性
			USART1_RX_NUM = (USART1_RX_NUM + 1) % USART1_RxBufferSize;
		}
		else
		{
			// 发送完一帧数据后，在最后添加上'\0'，'\0'是字符串结尾标记符，表明接收到一帧数据完事了
			// 二进制格式的数据，其实不需要在后面添加'\0'；因为是按照字节来解析，所以是不需要添加'\0'
			// 而文本格式的数据，需要在后面添加'\0'，因为文本将来接收到的就是文本，需要对其进行解析
			// 所以需要在后面添加'\0'
			USART1_RxBuffer[(USART1_RX_NUM) % USART1_RxBufferSize] = '\0';
			USART1_RX_NUM |= USART1_ENTER_FLAG;		//用于判断回车接收到数据结束,使用时记得清零
			// 发送完一帧数据后，通过串口发送0x0A，接收端看到0x0A后，就知道你给我发送完了一包数据
			// 0x0A的ASCII码表示换行键，没有特殊含义，所以选择0x0A作为发送的数据结束标记
			//(1)上位机串口助手/中控台按照协议给你发了一帧指令，收完指令后，给上位机串口助手/中控台
			// 发送0x0A，告诉串口助手/中控台我已经收到你刚才发送的指令
			// 如果上位机串口助手/中控台没有收到0x0A，证明没有收到这个指令
			//(2)调用串口发送0x0A后，就会调用USART_IT_IDLE，触发中断，就会进入到else if中(USART_IT_IDLE)
			USART1_PUTC(0x0A);
		}
	}
	// 通过串口把数据发送完了，就会触发这个中断
	else if(USART_GetITStatus(USART1, USART_IT_IDLE) != RESET)
	{
		// 清串口中断，串口接收是硬件自动完成的，防止在两帧数据之间被胡乱添加东西
		Clear=USART1->SR;
		Clear=USART1->DR;
		// 全局变量，起标记作用，表示我们收完一包数据了，但是还没处理
		ReceiveState = 1;
//			USART1_PUTC(USART1_RX_NUM);
//			USART1_PUTC(USART1_RxBuffer[0]);
//			USART1_PUTC(USART1_RxBuffer[1]);
//			USART1_PUTC(USART1_RxBuffer[2]);
//			USART1_PUTC(USART1_RxBuffer[3]);
//			USART1_PUTC(USART1_RxBuffer[4]);
//			USART1_PUTC(USART1_RxBuffer[5]);
//			USART1_PUTC(USART1_RxBuffer[6]);
	}
}


