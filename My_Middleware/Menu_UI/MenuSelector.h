#ifndef MenuSelector_H
#define MenuSelector_H

#include "MenuSetting.h"
#include <stdint.h>  // 添加这个头文件

extern NodeBranch_t* Selector;
typedef void (*FuncPtr)(void);

// 显示状态枚举
typedef enum {
    DISPLAY_MENU = 0,
    DISPLAY_REALTIME,
    DISPLAY_ANIMATION
} DisplayState;

void SelectUP(void);
void SelectDOWN(void);
void SelectINorRUN(void);
void SelectOut(void);

void OLEDPrintStringLine(int line, const char* str);
void PrintSelector(void);

// 控制函数
void EnterRealTimeData(void);
void EnterJoystickAnimation(void);
void ReturnToMainMenu(void);
DisplayState GetDisplayState(void);
uint8_t IsInSubPage(void);

#endif