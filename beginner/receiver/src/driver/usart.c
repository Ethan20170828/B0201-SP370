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

	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);	//使能GPIO时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);	//使能USART1时钟

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
	
	//配置中断
	NVIC_InitStruct.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 15;		//这里中断优先级配置为最低
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	
	NVIC_Init(&NVIC_InitStruct);
	USART_ITConfig(USART1,USART_IT_RXNE,ENABLE);	//使能接收中断
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
u16 USART1_RX_NUM=0;       //接收字符数目与回车状态

void USART1_IRQHandler(void)                	//串口1中断服务程序
{
	u8 getchar;	
    
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)//接收到数据
	{	 
		getchar=USART_ReceiveData(USART1); 
		USART1_PUTC(getchar);
		
		if(getchar != 0x0D)
		{
			USART1_RxBuffer[USART1_RX_NUM] = getchar;
			USART1_RX_NUM = (USART1_RX_NUM + 1) % USART1_RxBufferSize;
		}
		else
		{
			USART1_RxBuffer[(USART1_RX_NUM) % USART1_RxBufferSize] = '\0';
			USART1_RX_NUM |= USART1_ENTER_FLAG;		//用于判断回车接收到数据结束,使用时记得清零
			USART1_PUTC(0x0A);
		}		
	}	
}



