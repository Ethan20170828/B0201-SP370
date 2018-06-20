#include "delay.h"
#include "sys.h"



static u8  cnt_us = 0;		// us��ʱ������
static u16 cnt_ms = 0;		// ms��ʱ������

//��ʼ���ӳٺ���
//SYSTICK��ʱ�ӹ̶�ΪHCLKʱ�ӵ�1/8
//SYSCLK:ϵͳʱ��
void delay_init(void)	 
{
	SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8);	//ѡ���ⲿʱ��  HCLK/8
	cnt_us = SystemCoreClock/8000000;	// Ϊϵͳʱ�ӵ�1/8  
	cnt_ms = (u16)cnt_us*1000;			// ����ÿ��ms��Ҫ��systickʱ����   
}								    

//��ʱnus
//nusΪҪ��ʱ��us��.	
// us�����ܳ���2^24/8=2097152us
void delay_us(u32 nus)
{		
	u32 temp;	    	 
	SysTick->LOAD = nus * cnt_us; 					// ��ֵ���ص�LOAD��	  		 
	SysTick->VAL = 0x00;        					// ��ռ�������ǰֵ��״̬λ
	SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk ;     	// ������ʱ�� 
	do
	{
		temp = SysTick->CTRL;
	}
	while(temp&0x01 && !(temp&(1<<16)));			// �ȴ�ʱ�䵽��   
	SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;       	// �رռ�����
	SysTick->VAL = 0X00;       						// ��ռ�����	 
}


// ע��SysTick->LOADΪ24λ�Ĵ�������nms����̫��Ȼ�ᳬ��
void delay_ms(u16 nms)
{	 		  	  
	u32 temp;		   
	SysTick->LOAD = (u32)nms * cnt_ms;				// ʱ�����(SysTick->LOADΪ24bit)
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
































