#include "sys.h"
#include "led.h"

/**************************************************/
//��������:void Led_Init(void)
//�������:
//�������:
//����:LED��ʼ��
/**************************************************/
// work_led��ӦPA0,  status_led��ӦPB15
void LED_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	// STM32�в���ʹ���ĸ����裬����Ҫ�ȵ���RCC_APB2PeriphClockCmd��������Ҫ���Ƚ�ʱ�Ӵ򿪣����Ǵ򿪵���GPIOA��GPIOB
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);	// STM32��Ĭ�ϵ�ʱ���ǹرյģ���֮ǰҪ����Ӧ�����

	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_OD;	// ����Ϊ�������ģʽ
	GPIO_InitStruct.GPIO_Pin  = LED_SYSTEM_PIN;		// ����Ҫ���õ����ţ�PA0�˿�����
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;	// ����I/O���ٶ�Ϊ50MHz
	GPIO_Init(LED_SYSTEM_PORT,&GPIO_InitStruct);	// SYSTEM��Ӧ����work_led�������趨������ʼ��LED_SYSTEM_PORT

	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_OD;
	GPIO_InitStruct.GPIO_Pin  = LED_RECEIVE_PIN;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(LED_RECEIVE_PORT,&GPIO_InitStruct);	// RECEIVE��Ӧ����status_led

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



