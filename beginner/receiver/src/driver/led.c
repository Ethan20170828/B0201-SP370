#include "sys.h"
#include "led.h"

/**************************************************/
//函数名字:void Led_Init(void)
//输入参数:
//输出参数:
//描述:LED初始化
/**************************************************/
// work_led对应PA0,  status_led对应PB15
void LED_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	// STM32中不管使用哪个外设，都需要先调用RCC_APB2PeriphClockCmd函数，主要是先将时钟打开，这是打开的是GPIOA和GPIOB
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);	// STM32中默认的时钟是关闭的，用之前要把相应外设打开

	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_OD;	// 配置为推挽输出模式
	GPIO_InitStruct.GPIO_Pin  = LED_SYSTEM_PIN;		// 设置要配置的引脚，PA0端口配置
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;	// 配置I/O口速度为50MHz
	GPIO_Init(LED_SYSTEM_PORT,&GPIO_InitStruct);	// SYSTEM对应的是work_led；根据设定参数初始化LED_SYSTEM_PORT

	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_OD;
	GPIO_InitStruct.GPIO_Pin  = LED_RECEIVE_PIN;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(LED_RECEIVE_PORT,&GPIO_InitStruct);	// RECEIVE对应的是status_led

	GPIO_SetBits(LED_SYSTEM_PORT,LED_SYSTEM_PIN);
	
	GPIO_SetBits(LED_RECEIVE_PORT,LED_RECEIVE_PIN);
}


void LED_SystemCtrl(ONOrOFF status)
{
	if(status == ON){
		GPIO_ResetBits(LED_SYSTEM_PORT,LED_SYSTEM_PIN);
	}else{
		GPIO_SetBits(LED_SYSTEM_PORT,LED_SYSTEM_PIN);
	}
}

void LED_SystemReverse(void)
{
	uint8_t status;
	status = GPIO_ReadOutputDataBit(LED_SYSTEM_PORT,LED_SYSTEM_PIN);
	if(status == Bit_SET){
		GPIO_ResetBits(LED_SYSTEM_PORT,LED_SYSTEM_PIN);
	}else{
		GPIO_SetBits(LED_SYSTEM_PORT,LED_SYSTEM_PIN);
	}
}


void LED_ReceiveCtrl(ONOrOFF status)
{
	if(status == ON){
		GPIO_ResetBits(LED_RECEIVE_PORT,LED_RECEIVE_PIN);
	}else{
		GPIO_SetBits(LED_RECEIVE_PORT,LED_RECEIVE_PIN);
	}
}

void LED_ReceiveReverse(void)
{
	uint8_t status;
	status = GPIO_ReadOutputDataBit(LED_RECEIVE_PORT,LED_RECEIVE_PIN);
	if(status == Bit_SET){
		GPIO_ResetBits(LED_RECEIVE_PORT,LED_RECEIVE_PIN);
	}else{
		GPIO_SetBits(LED_RECEIVE_PORT,LED_RECEIVE_PIN);
	}
}



