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

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);

	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_OD;
	GPIO_InitStruct.GPIO_Pin  = LED_SYSTEM_PIN;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(LED_SYSTEM_PORT,&GPIO_InitStruct);

	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_OD;
	GPIO_InitStruct.GPIO_Pin  = LED_RECEIVE_PIN;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(LED_RECEIVE_PORT,&GPIO_InitStruct);	

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



