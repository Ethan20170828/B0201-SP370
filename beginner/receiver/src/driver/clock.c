#include "sys.h"
#include "clock.h"


//����Ϊ�ڲ�ʱ��
//��Ƶ��64Mhz

//********************//
//ʹ���ڲ�ʱ���޸ĵĵط�
//1.���64M
//2.flash����
//3.���ε�startup_stm32f10x_md.s --> SystemInit(������ʱ������)
//4.�޸�system_stm32f10x.c       -->n defie SYSCLK_FREQ_72MHz  64000000
//	����ӦDelay�Ⱥ���
//********************//
// ��ʱ�ӽ�������������
void Clock_Init(void)
{
	RCC_DeInit();//������ RCC�Ĵ�������Ϊȱʡֵ
	
	RCC_HSICmd(ENABLE);//ʹ��HSI ����Ϊ���õľ�����û�нӵģ�ֻ����STM32���õľ�������ʹ��HSI
	while(RCC_GetFlagStatus(RCC_FLAG_HSIRDY) == RESET);		//�ȴ�HSIʹ�ܳɹ�

	FLASH_SetLatency(FLASH_Latency_2);
	FLASH_PrefetchBufferCmd(FLASH_PrefetchBuffer_Enable);

	RCC_PLLConfig(RCC_PLLSource_HSI_Div2,RCC_PLLMul_16);	// ���õ���64MHz���൱���ڲ���HSI����2�ٳ���16
	RCC_PLLCmd(ENABLE);
	while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY) != SET);

	RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);
	while(RCC_GetSYSCLKSource() != 0x08);
											// sysclk = 64Mhz
	RCC_HCLKConfig(RCC_SYSCLK_Div1);		// hclk = 64M
	RCC_PCLK1Config(RCC_HCLK_Div1);			// pclk1 = 64M
	RCC_PCLK2Config(RCC_HCLK_Div2);			// pclk2 = 32M
	RCC_ADCCLKConfig(RCC_PCLK2_Div2);		// ADCCLK = 32M
}

