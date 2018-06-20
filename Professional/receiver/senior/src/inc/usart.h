#ifndef __USART_H
#define __USART_H
#include "stdio.h"

//�����������ֽ��� 64(�������пظ����ն˷��͵�Э������ֽ�Ҳ����7�ֽ�)
#define USART1_RxBufferSize		(1<<7)
	  	
extern u8 USART1_RxBuffer[USART1_RxBufferSize];     //���ջ���,���USART_REC_LEN���ֽ�

extern u16 USART1_RX_NUM;       //�������ݴ�С���س�����USART1_RxBufferSizeǰһλ��1
#define USART1_ENTER_FLAG USART1_RxBufferSize<<1	//�س����

void USART1_Init(uint32_t baudrate);
extern u8 ReceiveState;
#endif

