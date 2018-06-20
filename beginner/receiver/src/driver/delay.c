#include "delay.h"
#include "sys.h"


static u8  cnt_us = 0;		// us��ʱ������
static u16 cnt_ms = 0;		// ms��ʱ������

//��ʼ���ӳٺ���
//SYSTICK��ʱ�ӹ̶�ΪHCLKʱ�ӵ�1/8
//SYSCLK:ϵͳʱ��
void delay_init(void)	 	// ���������þ��Ǽ����cnt_us��cnt_ms��ֵ�ֱ�Ϊ8��8000
{
	SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8);	//ѡ���ⲿʱ��  HCLK/8 = 64/8 = 8M 
	cnt_us = SystemCoreClock/8000000;	// Ϊϵͳʱ�ӵ�1/8  = 64000000/8000000 = 8�������ʱ8us����SysTick����������8��
	cnt_ms = (u16)cnt_us*1000;			// ����ÿ��ms��Ҫ��systickʱ����   �����ms������SysTick����������8000��
}

// ��ʱnus
// nusΪҪ��ʱ��us��.	
// us�����ܳ���2^24/8=2097152us��Ҳ������ʱ���ܳ���2s
void delay_us(u32 nus)		// ��STM32�У���ʱus������ô����
{	// 	SysTick->LOAD�ĵ�ַ�������ֲ����ṩ�ĵ�ַҪ�Ǻ�
	u32 temp;	 	//   SysTick�����������: Cortex-M3Ȩ��ָ��Cn�ĵ��ĵڰ˲��ֵ�SysTick��ʱ������
	SysTick->LOAD = nus * cnt_us; 					// ��ֵ���ص�LOAD��	  		 
	SysTick->VAL = 0x00;        					// ��ռ�������ǰֵ��״̬λ
	SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk ;     	// ������ʱ�� 
	do
	{
		temp = SysTick->CTRL;
	}
	while(temp&0x01 && !(temp&(1<<16)));			// �ȴ�ʱ�䵽�CTRL�Ĵ�����bit16��ʾʱ�䵽�˻���û�������ʱ�䵽�ˣ�������ѭ��
	SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;       // �رռ�����
	SysTick->VAL = 0X00;       						// ��ռ�����	 
}


// ע��SysTick->LOADΪ24λ�Ĵ�������nms����̫��Ȼ�ᳬ��(���ܳ���2��24�η�)
void delay_ms(u16 nms)
{	 		  	  
	u32 temp;		   
	SysTick->LOAD = (u32)nms * cnt_ms;				// ʱ�����(SysTick->LOADΪ24bit)��Ҳ����˵����� (u32)nms * cnt_ms�Ľ������ܳ���2��24�η�
	SysTick->VAL = 0x00;           					// ��ռ�����
	SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk ;   	// ��ʼ����  
	do
	{
		temp = SysTick->CTRL;
	}
	while(temp&0x01 && !(temp&(1<<16)));			// �ȴ�ʱ�䵽��   
	SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk; 		// �رռ�����
	SysTick->VAL = 0X00;       						// ��ռ�����	  	    
}



