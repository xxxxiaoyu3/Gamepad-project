#ifndef __BEEP_H__
#define __BEEP_H__

#include "stm32f1xx_hal.h"
#include "gpio.h"
#include "main.h"

typedef enum
{
    OFF = 0, 
    ON  = 1
} Beep_State_t;

void Beep_Init(void);
void Beep_Toggle(Beep_State_t state);
Beep_State_t Beep_Read_State(void);

#endif
