#include "sys.h"
#include "usart.h"


//////////////////////////////////////////////////////////////////
//�������´���,֧��printf����,������Ҫѡ��use MicroLIB
#if 1
#pragma import(__use_no_semihosting)             
//��׼����Ҫ��֧�ֺ���                 
struct __FILE 
{ 
	int handle; 

}; 

FILE __stdout;       
//����_sys_exit()�Ա���ʹ�ð�����ģʽ
void _sys_exit(int x) 
{ 
	x = x; 
} 
//�ض���fputc���� 
int fputc(int ch, FILE *f)
{      
	while(USART_GetFlagStatus(USART1,USART_FLAG_TC)==RESET); 
    USART_SendData(USART1,(uint8_t)ch);   
	return ch;
}
#endif

/**************************************************/
//��������:void USART1_Init(uint32_t baudrate)
//�������:baudrate->������
//�������:
//����:��ʼ��USART1
/**************************************************/
void USART1_Init(uint32_t baudrate)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	USART_InitTypeDef USART_InitStruct;
	NVIC_InitTypeDef NVIC_InitStruct;

	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);	//ʹ��GPIOʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);	//ʹ��USART1ʱ��

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
	
	//�����ж�
	NVIC_InitStruct.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 15;		//�����ж����ȼ�����Ϊ���
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	
	NVIC_Init(&NVIC_InitStruct);
	USART_ITConfig(USART1,USART_IT_RXNE,ENABLE);	//ʹ�ܽ����ж�
	USART_Cmd(USART1,ENABLE);
}

/**************************************************/
//��������:static void USART1_PUTC(uint8_t c)
//�������:c:��Ҫ��ӡ���ַ�
//�������:
//����:��ӡһ���ַ�
/**************************************************/
static void USART1_PUTC(uint8_t c)
{
	while(USART_GetFlagStatus(USART1,USART_FLAG_TC)==RESET); 
    USART_SendData(USART1,(uint8_t)c);
}

//����1�жϷ������
//ע��,��ȡUSARTx->SR�ܱ���Ī������Ĵ���   	
u8 USART1_RxBuffer[USART1_RxBufferSize];     //���ջ���,���USART_REC_LEN���ֽ�.
u16 USART1_RX_NUM=0;       //�����ַ���Ŀ��س�״̬

void USART1_IRQHandler(void)                	//����1�жϷ������
{
	u8 getchar;	
    
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)//���յ�����
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
			USART1_RX_NUM |= USART1_ENTER_FLAG;		//�����жϻس����յ����ݽ���,ʹ��ʱ�ǵ�����
			USART1_PUTC(0x0A);
		}		
	}	
}



