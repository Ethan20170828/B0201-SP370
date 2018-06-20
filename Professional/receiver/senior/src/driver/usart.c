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

// �����ַ���Ŀ(Ҳ������λ�������ն˷��͵��ֽ���Ŀ)��س�״̬��
// Ĭ����0����ʾ��ͨ����λ�����ַ��͵ĵ�0���ֽ�
u16 USART1_RX_NUM=0;

// ȫ�ֱ����������еı�����ã�Ϊ0��ʾ����ʾ����û�������пط�������һ������
u8 ReceiveState = 0;

// ���ڵ��жϽ��ճ���
// ���ż�ʱ����Ҫ�����������н��������Բ���Ҫ����רҵ����Ҫ��������λ���������ֽ��н�����
// ��������ݣ�������Ҫ�����жϽ��ճ���
// ʵ�ʵ����У���������ǰ���ģ��������ô��ڵײ㹤����ͨ���������ָ�STM32���Ͷ���(Ҳ���Ƿ���Э��)��
// ��STM32�ܽ��յ���λ���������ַ��͵�������(Ҳ���ǽ��յ�Э��)������STM32�ܹ�����λ���������ֻظ�
// ���ݡ�
// ��λ�������������ڷ��͵ģ���STM32���ڽ��յģ�
// ʵ������Ŀ��ʱ��Ҳ�������Ĳ�����еģ�
void USART1_IRQHandler(void)                	//����1�жϷ������
{
	u8 getchar;	
	u8 Clear = Clear;

// ������ͨ�����ڸ��ҷ��Ͷ���ʱ�������ж�
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)//���յ�����
	{
		// ���ն˽��յ����ڷ��͹���һ�ֽ�����
		getchar=USART_ReceiveData(USART1); 
		//USART1_PUTC(getchar);

		// �����ڽ��յ�0x0Dʱ����֪���պ÷���һ֡������
		// ��������0x0Dʱ��֤�������������ڷ���һ֡����
		// 0x0D��ASCII���ʾ�س�����û�����⺬�壬����ѡ��0x0D��Ϊ���͵����ݽ������
		// �жϽ��յ���һ���ֽ��Ƿ����0x0D
		if(getchar != 0x0D)
		{
			// ͨ����λ���������ָ��ҷ���һ֡���ݵ�ʱ�򣬸��ҷ��͹����ĵ�0���ֽڷŵ�����ĵ�0λ
			// �źú��USART1_RX_NUM+1���´η��͹����ĵ�1���ֽڷŵ���1λ�����������
			// Ҳ�������ΰѷ�����������ȷŵ����ڵĻ�������
			USART1_RxBuffer[USART1_RX_NUM] = getchar;
			// ��USART1_RxBufferSizeȡ�࣬�ѷ��Ͷ�����ֽ�ȥ������ֹ�����������֤����׳��
			USART1_RX_NUM = (USART1_RX_NUM + 1) % USART1_RxBufferSize;
		}
		else
		{
			// ������һ֡���ݺ�����������'\0'��'\0'���ַ�����β��Ƿ����������յ�һ֡����������
			// �����Ƹ�ʽ�����ݣ���ʵ����Ҫ�ں������'\0'����Ϊ�ǰ����ֽ��������������ǲ���Ҫ���'\0'
			// ���ı���ʽ�����ݣ���Ҫ�ں������'\0'����Ϊ�ı��������յ��ľ����ı�����Ҫ������н���
			// ������Ҫ�ں������'\0'
			USART1_RxBuffer[(USART1_RX_NUM) % USART1_RxBufferSize] = '\0';
			USART1_RX_NUM |= USART1_ENTER_FLAG;		//�����жϻس����յ����ݽ���,ʹ��ʱ�ǵ�����
			// ������һ֡���ݺ�ͨ�����ڷ���0x0A�����ն˿���0x0A�󣬾�֪������ҷ�������һ������
			// 0x0A��ASCII���ʾ���м���û�����⺬�壬����ѡ��0x0A��Ϊ���͵����ݽ������
			//(1)��λ����������/�п�̨����Э����㷢��һָ֡�����ָ��󣬸���λ����������/�п�̨
			// ����0x0A�����ߴ�������/�п�̨���Ѿ��յ���ղŷ��͵�ָ��
			// �����λ����������/�п�̨û���յ�0x0A��֤��û���յ����ָ��
			//(2)���ô��ڷ���0x0A�󣬾ͻ����USART_IT_IDLE�������жϣ��ͻ���뵽else if��(USART_IT_IDLE)
			USART1_PUTC(0x0A);
		}
	}
	// ͨ�����ڰ����ݷ������ˣ��ͻᴥ������ж�
	else if(USART_GetITStatus(USART1, USART_IT_IDLE) != RESET)
	{
		// �崮���жϣ����ڽ�����Ӳ���Զ���ɵģ���ֹ����֡����֮�䱻������Ӷ���
		Clear=USART1->SR;
		Clear=USART1->DR;
		// ȫ�ֱ������������ã���ʾ��������һ�������ˣ����ǻ�û����
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


