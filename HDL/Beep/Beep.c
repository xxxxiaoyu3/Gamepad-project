#include "Beep.h"

void User_Beep_GPIO_Init(void)
{
    //Add User's GPIO code here.
    //The Cubemax I'm using has already been configured, so I won't add it here.
}
void Beep_Init(void)
{
    User_Beep_GPIO_Init();
}
/*
    * state: ON or OFF
    * ON: Beep on
    * OFF: Beep off
*/
void Beep_Toggle(Beep_State_t state)
{
    if (state == ON)
    {
        HAL_GPIO_WritePin(Beep_Output_GPIO_Port, Beep_Output_Pin, GPIO_PIN_SET);
    }
    else
    {
        HAL_GPIO_WritePin(Beep_Output_GPIO_Port, Beep_Output_Pin, GPIO_PIN_RESET);
    }
}
/** 获取蜂鸣器的状态 Get the state of Beep
 * @return Beep_State_t 蜂鸣器状态，ON或OFF @return Beep_State_t The state of Beep, ON or OFF
 * 
 */
Beep_State_t Beep_Read_State(void)
{
    //Add User's code here.
    if (HAL_GPIO_ReadPin(Beep_Output_GPIO_Port, Beep_Output_Pin) == GPIO_PIN_SET)
    {
        // Beep is ON
        return ON;
    }
    else
    {
        // Beep is OFF
        return OFF;
    }
}

