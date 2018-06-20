#include "delay.h"
#include "sys.h"



static u8  cnt_us = 0;		// us延时倍乘数
static u16 cnt_ms = 0;		// ms延时倍乘数

//初始化延迟函数
//SYSTICK的时钟固定为HCLK时钟的1/8
//SYSCLK:系统时钟
void delay_init(void)	 
{
	SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8);	//选择外部时钟  HCLK/8
	cnt_us = SystemCoreClock/8000000;	// 为系统时钟的1/8  
	cnt_ms = (u16)cnt_us*1000;			// 代表每个ms需要的systick时钟数   
}								    

//延时nus
//nus为要延时的us数.	
// us数不能超过2^24/8=2097152us
void delay_us(u32 nus)
{		
	u32 temp;	    	 
	SysTick->LOAD = nus * cnt_us; 					// 初值加载到LOAD中	  		 
	SysTick->VAL = 0x00;        					// 清空计数器当前值和状态位
	SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk ;     	// 开启定时器 
	do
	{
		temp = SysTick->CTRL;
	}
	while(temp&0x01 && !(temp&(1<<16)));			// 等待时间到达   
	SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;       	// 关闭计数器
	SysTick->VAL = 0X00;       						// 清空计数器	 
}


// 注意SysTick->LOAD为24位寄存器所以nms不能太大不然会超出
void delay_ms(u16 nms)
{	 		  	  
	u32 temp;		   
	SysTick->LOAD = (u32)nms * cnt_ms;				// 时间加载(SysTick->LOAD为24bit)
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
































