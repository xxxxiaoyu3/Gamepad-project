
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
/* 用户自定义头文件区域结束 */

/**
 * @brief 模拟遥控器 ADC 显示函数
 *
 * 模拟显示模拟遥控器的 ADC 值。
 * x128 y64 OLED 显示区域有限，只显示部分信息。
 */
void Remote_ADC_DisplayFunc()
{
	myOLED_Clear();
	char L_X_Str[20];
	char L_Y_Str[20];
	char R_X_Str[20];
	char R_Y_Str[20];
	
	snprintf(L_X_Str, sizeof(L_X_Str), "%d", Left_x_axis);
	snprintf(L_Y_Str, sizeof(L_Y_Str), "%d", Left_y_axis);
	snprintf(R_X_Str, sizeof(R_X_Str), "%d", Right_x_axis);
	snprintf(R_Y_Str, sizeof(R_Y_Str), "%d", Right_y_axis);
	
	OLED_ShowString(1*6, 3*8, "L_Remote:",OLED_8X8_FULL);
	OLED_ShowString(1*6, 4*8, L_X_Str,OLED_8X8_FULL);
	OLED_ShowString((sizeof(L_X_Str)+1)*6, 4*8, L_Y_Str,OLED_8X8_FULL);
	OLED_ShowString(1*6, 5*8, "R_Remote:",OLED_8X8_FULL);
	OLED_ShowString(1*6, 6*8, R_X_Str,OLED_8X8_FULL);
	OLED_ShowString((sizeof(R_X_Str)+1)*6, 6*8, R_Y_Str,OLED_8X8_FULL);
}
/**
 * @brief LED 灯翻转函数
 *
 * 切换 GPIOC 13 引脚的电平，实现 LED 灯亮灭切换。
 */
void LED_TurnFunc(){
	uint8_t ledState = LED_GetState(Pin1);
	ledState = !ledState;
	LED_Toggle(Pin1, ledState);
}

/**
 * @brief 菜单系统初始化
 *
 * 构建菜单树结构，并初始化 Selector 指针。
 */
	void MenuInit() {
	    rootNode = SetNode(DIR_type, "LED", SetBranch(
	        SetNode(EXE_type, "System", LED_TurnFunc),
	        SetNode(DIR_type, "Remote_ADC",SetBranch
			(
				SetNode(EXE_type, "Remote_ADC", Remote_ADC_DisplayFunc)
			)),
	        SetNode(DIR_type, "Speed", SetBranch(
	            SetNode(EXE_type, "fast", NULL),
	            SetNode(EXE_type, "slow", NULL)
	        )),
	        SetNode(EXE_type, "Root", NULL),
	        SetNode(EXE_type, "Reset", NULL)
	    )); // 确保所有括号正确闭合
	
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
