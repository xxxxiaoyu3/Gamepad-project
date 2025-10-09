#ifndef __KEY_H
#define __KEY_H

#include <stdio.h>
#include "main.h"
#include "led.h"
#include "Beep.h"
#include "MenuSelector.h"

// 按键数量 Key Count
#define KEY_COUNT 6

//按键相关定义(有多少按键就定义多少组)  Key Port and Line Definitions

#define Key1_PORT KEY1_Input_GPIO_Port                       // Key1 对应端口
#define Key1_LINE KEY1_Input_Pin                            // Key1 对应引脚

#define Key2_PORT KEY2_Input_GPIO_Port                       // Key2 对应端口
#define Key2_LINE KEY2_Input_Pin                            // Key2 对应引脚

#define Key3_PORT KEY3_Input_GPIO_Port                       // Key3 对应端口
#define Key3_LINE KEY3_Input_Pin                            // Key3 对应引脚

#define Key4_PORT KEY4_Input_GPIO_Port                       // Key4 对应端口
#define Key4_LINE KEY4_Input_Pin                            // Key4 对应引脚

#define Key5_PORT KEY5_Input_GPIO_Port                       // Key5 对应端口
#define Key5_LINE KEY5_Input_Pin                            // Key5 对应引脚

#define Key6_PORT KEY6_Input_GPIO_Port                       // Key6 对应端口
#define Key6_LINE KEY6_Input_Pin                            // Key6 对应引脚

typedef enum {
    KEY_UP = 0,     // 上键 
    KEY_DOWN,       // 下键
    KEY_LEFT,      // 左键
    KEY_RIGHT,     // 右键
    KEY_ENTER,     // 确认键
    KEY_CANCEL,    // 取消键
    None,         // 无按键
} KeyType;

/**
 * @brief  按键初始化函数，配置相关GPIO为输入模式
 */
void Key_Init(void);

/**
 * @brief  按键扫描函数，带消抖和状态机
 * @param  Key_States_Flag 按键状态标志数组指针，按下时对应元素被置1，未按下时对应元素为0
 * @note   注意：Key_States_Flag数组在置1后需要软件清零(本函数不会自己清零)
 */
KeyType Key_Scan(void);
void Key_Handler(KeyType Read_key);
void Key_Task(KeyType Read_key);		//测试常用函数

#endif
