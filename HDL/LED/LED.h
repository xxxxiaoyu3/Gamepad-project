#ifndef __LED_H__
#define __LED_H__

#include "stm32f1xx_hal.h"
#include "gpio.h"
#include "main.h"

typedef enum
{
    Pin1 = 0, 
    Pin2, 
    Pin3
} LED_Pin_t;

void LED_Init(void);
void LED_Toggle(LED_Pin_t LED_Pin, uint8_t state);
void LED_Init_Task(void);        //LED初始化测试任务
uint8_t LED_GetState(LED_Pin_t LED_Pin);

#endif
