#ifndef __USART_H
#define __USART_H
#include "stdio.h"

#define USART1_RxBufferSize		(1<<7)  	//定义最大接收字节数 64
	  	
extern u8 USART1_RxBuffer[USART1_RxBufferSize];     //接收缓冲,最大USART_REC_LEN个字节

extern u16 USART1_RX_NUM;       //接收数据大小，回车将在USART1_RxBufferSize前一位至1
#define USART1_ENTER_FLAG USART1_RxBufferSize<<1	//回车标记

void USART1_Init(uint32_t baudrate);

#endif

