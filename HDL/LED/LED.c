#include "LED.h"

void User_LED_GPIO_Init(void)
{
    //Add User's GPIO code here.
    //The Cubemax I'm using has already been configured, so I won't add it here.
}
void LED_Init(void)
{
    User_LED_GPIO_Init();
}

void LED_Toggle(LED_Pin_t LED_Pin, uint8_t state)
{
    switch (LED_Pin)
    {
    case Pin1:
        HAL_GPIO_WritePin(LED1_Out_GPIO_Port, LED1_Out_Pin, state ? GPIO_PIN_RESET : GPIO_PIN_SET);
        break;

    case Pin2:
        HAL_GPIO_WritePin(LED2_Out_GPIO_Port, LED2_Out_Pin, state ? GPIO_PIN_RESET : GPIO_PIN_SET);
        break;

    case Pin3:
        HAL_GPIO_WritePin(LED3_Out_GPIO_Port, LED3_Out_Pin, state ? GPIO_PIN_RESET : GPIO_PIN_SET);
        break;

    default:
        break;
    }
}
/* 获取LED状态 */
uint8_t LED_GetState(LED_Pin_t LED_Pin)
{
    GPIO_PinState pinState;
    switch (LED_Pin)
    {
    case Pin1:
        pinState = HAL_GPIO_ReadPin(LED1_Out_GPIO_Port, LED1_Out_Pin);
        return (pinState == GPIO_PIN_RESET) ? 1 : 0;

    case Pin2:
        pinState = HAL_GPIO_ReadPin(LED2_Out_GPIO_Port, LED2_Out_Pin);
        return (pinState == GPIO_PIN_RESET) ? 1 : 0;

    case Pin3:
        pinState = HAL_GPIO_ReadPin(LED3_Out_GPIO_Port, LED3_Out_Pin);
        return (pinState == GPIO_PIN_RESET) ? 1 : 0;

    default:
        return 0;
    }
}
void LED_Init_Task(void)        //LED初始化测试任务
{
    for (uint8_t i = 0; i < 3; i++)
    {
        LED_Toggle(i, 1);
        HAL_Delay(50);
    }
    for (uint8_t i = 2; i > 0; i--)
    {
        LED_Toggle(i, 0);
        HAL_Delay(50);
    }
}
