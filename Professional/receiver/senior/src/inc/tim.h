#ifndef _TIM_H
#define _TIM_H

extern uint32_t time_1ms;

void Tim3_Init(u16 period_num,u16 prescaler_num);
void TIM3_Cmd(FunctionalState NewState);


#endif


