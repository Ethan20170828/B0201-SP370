#include "sys.h"
#include "clock.h"


//配置为内部时钟
//倍频到64Mhz

//********************//
//使用内部时钟修改的地方
//1.最大到64M
//2.flash配置
//3.屏蔽掉startup_stm32f10x_md.s --> SystemInit(屏蔽其时钟配置)
//4.修改system_stm32f10x.c       -->n defie SYSCLK_FREQ_72MHz  64000000
//	以适应Delay等函数
//********************//
// 对时钟进行了重新设置
void Clock_Init(void)
{
	RCC_DeInit();//将外设 RCC寄存器重设为缺省值
	
	RCC_HSICmd(ENABLE);//使能HSI ，因为外置的晶振是没有接的，只能用STM32内置的晶振，所以使能HSI
	while(RCC_GetFlagStatus(RCC_FLAG_HSIRDY) == RESET);		//等待HSI使能成功

	FLASH_SetLatency(FLASH_Latency_2);
	FLASH_PrefetchBufferCmd(FLASH_PrefetchBuffer_Enable);

	RCC_PLLConfig(RCC_PLLSource_HSI_Div2,RCC_PLLMul_16);	// 配置的是64MHz；相当于内部的HSI除以2再乘以16
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

