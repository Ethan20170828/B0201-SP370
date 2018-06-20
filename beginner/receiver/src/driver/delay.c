#include "delay.h"
#include "sys.h"


static u8  cnt_us = 0;		// us延时倍乘数
static u16 cnt_ms = 0;		// ms延时倍乘数

//初始化延迟函数
//SYSTICK的时钟固定为HCLK时钟的1/8
//SYSCLK:系统时钟
void delay_init(void)	 	// 函数的作用就是计算出cnt_us和cnt_ms的值分别为8和8000
{
	SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8);	//选择外部时钟  HCLK/8 = 64/8 = 8M 
	cnt_us = SystemCoreClock/8000000;	// 为系统时钟的1/8  = 64000000/8000000 = 8；如果延时8us，让SysTick计数器计数8次
	cnt_ms = (u16)cnt_us*1000;			// 代表每个ms需要的systick时钟数   如果是ms，则让SysTick计数器计数8000次
}

// 延时nus
// nus为要延时的us数.	
// us数不能超过2^24/8=2097152us，也就是延时不能超过2s
void delay_us(u32 nus)		// 在STM32中，延时us就是这么做的
{	// 	SysTick->LOAD的地址与数据手册中提供的地址要吻合
	u32 temp;	 	//   SysTick讲解的内容在: Cortex-M3权威指南Cn文档的第八部分的SysTick定时器部分
	SysTick->LOAD = nus * cnt_us; 					// 初值加载到LOAD中	  		 
	SysTick->VAL = 0x00;        					// 清空计数器当前值和状态位
	SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk ;     	// 开启定时器 
	do
	{
		temp = SysTick->CTRL;
	}
	while(temp&0x01 && !(temp&(1<<16)));			// 等待时间到达；CTRL寄存器的bit16表示时间到了或者没到，如果时间到了，则跳出循环
	SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;       // 关闭计数器
	SysTick->VAL = 0X00;       						// 清空计数器	 
}


// 注意SysTick->LOAD为24位寄存器所以nms不能太大不然会超出(不能超过2的24次方)
void delay_ms(u16 nms)
{	 		  	  
	u32 temp;		   
	SysTick->LOAD = (u32)nms * cnt_ms;				// 时间加载(SysTick->LOAD为24bit)；也就是说这个数 (u32)nms * cnt_ms的结果最大不能超过2的24次方
	SysTick->VAL = 0x00;           					// 清空计数器
	SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk ;   	// 开始倒数  
	do
	{
		temp = SysTick->CTRL;
	}
	while(temp&0x01 && !(temp&(1<<16)));			// 等待时间到达   
	SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk; 		// 关闭计数器
	SysTick->VAL = 0X00;       						// 清空计数器	  	    
}



