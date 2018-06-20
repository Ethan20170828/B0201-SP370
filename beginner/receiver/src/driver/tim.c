#include "sys.h"
#include "tim.h"

#include "led.h"

uint32_t time_1ms = 0;

/**************************************************/
//��������:void Tim3_Init(u16 period_num,u16 prescaler_num)
//�������:period_num:����ֵ
//		   prescaler_num:��Ƶϵ�� >=1
//�������:
//����:���ڻ�����ʱ�������ϼ�������������ж�
/**************************************************/
void Tim3_Init(u16 period_num,u16 prescaler_num)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
	NVIC_InitTypeDef NVIC_InitStruct;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);

	TIM_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInitStruct.TIM_Period = period_num;					// ��������
	TIM_TimeBaseInitStruct.TIM_Prescaler = prescaler_num - 1;
	TIM_TimeBaseInit(TIM3,&TIM_TimeBaseInitStruct);

	NVIC_InitStruct.NVIC_IRQChannel = TIM3_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 14;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0;

	NVIC_Init(&NVIC_InitStruct);

	TIM_ClearFlag(TIM3,TIM_FLAG_Update);
	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE);

}


void TIM3_Cmd(FunctionalState NewState)
{
	TIM_Cmd(TIM3,NewState);
}


// 500ms work_led��˸һ��
void TIM3_IRQHandler(void)	// TIM3���ж���1ms���������������work_ledÿ��500ms��˸һ�Σ����û��500ms��˵��Tim3_Init�������β���
{	
	if(TIM_GetITStatus(TIM3,TIM_IT_Update) != RESET)
	{
		TIM_ClearITPendingBit(TIM3,TIM_IT_Update);
		time_1ms++;
	}

	if(!(time_1ms % 500)){
		LED_SystemReverse();	// time_1ms�ӵ�500����ִ���������
	}
}




