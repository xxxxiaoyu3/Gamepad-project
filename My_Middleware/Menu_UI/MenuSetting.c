
// MenuSetting.c - 菜单设置与初始化相关实现
#include "MenuSetting.h"

// 根节点指针，全局变量
Node_t* rootNode;


/* 用户自定义头文件区域，可根据实际需求添加 */
#include <stdio.h>
#include "oled.h"
#include "main.h"
#include "LED.h"
#include "Remote_ADC.h"
#include "My_Display.h"
/* 用户自定义头文件区域结束 */

/**
 * @brief LED 灯翻转函数
 *
 * 切换 GPIOC 13 引脚的电平，实现 LED 灯亮灭切换。
 */
void LED_TurnFunc(void)
{
	uint8_t ledState = LED_GetState(Pin1);
	ledState = !ledState;
	LED_Toggle(Pin1, ledState);
}

void LED1_ON(void)
{
    LED_Toggle(Pin1, 1);
}

void LED1_OFF(void)
{
    LED_Toggle(Pin1, 0);
}

/**
 * @brief 菜单系统初始化
 *
 * 构建菜单树结构，并初始化 Selector 指针。
 */
	void MenuInit() {
	    rootNode = SetNode(DIR_type, "LED", SetBranch(
        SetNode(DIR_type, "LED       ", SetBranch(
            SetNode(EXE_type, "LED_ON", LED1_ON),
            SetNode(EXE_type, "LED_OFF", LED1_OFF)
        )),
        SetNode(DIR_type, "Remote_ADC ", SetBranch(
            SetNode(EXE_type, "Remote_Data", EnterRealTimeData),
            SetNode(EXE_type, "Remote_UI", EnterJoystickAnimation)
        )),
        SetNode(DIR_type, "Speed     ", SetBranch(
            SetNode(EXE_type, "fast", NULL),
            SetNode(EXE_type, "slow", NULL)
        )),
        SetNode(EXE_type, "Root      ", NULL),
        SetNode(EXE_type, "Reset     ", NULL)
    ));// 确保所有括号正确闭合
	
	    Selector = rootNode->pointer;
	}


/* OLED 显示相关自定义函数区域 */
/**
 * @brief OLED 显示一行字符串（自定义实现）
 * @param line 行号
 * @param str  要显示的字符串
 */
void myOLEDPrintStringLine(int line, const char* str) {
	OLED_ShowString(0*16, line*16, (char *)str, OLED_8X8_FULL);
}
void myOLED_Clear() {
	OLED_Clear();
}

/* OLED 显示相关自定义函数区域结束 */
