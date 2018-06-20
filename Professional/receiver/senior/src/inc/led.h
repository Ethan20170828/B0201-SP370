#ifndef __LED_H
#define __LED_H

#include "sys.h"

#define LED_SYSTEM_PORT		GPIOA
#define LED_RECEIVE_PORT	GPIOB


#define LED_SYSTEM_PIN		GPIO_Pin_0
#define LED_RECEIVE_PIN		GPIO_Pin_15

void LED_Init(void);

void LED_SystemCtrl(ONOrOFF status);
void LED_SystemReverse(void);


void LED_ReceiveCtrl(ONOrOFF status);
void LED_ReceiveReverse(void);


#endif


