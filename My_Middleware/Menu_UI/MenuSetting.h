#ifndef MenuSetting_H
#define MenuSetting_H

#include "MenuCore.h"
#include "MenuSelector.h"
extern Node_t* rootNode;
void MenuInit(void);

/***********************/
void myOLEDPrintStringLine(int line, const char* str);
void myOLED_Clear();

#define Simulate_Mode -1
#define RealOLED_Mode 1
/***********************/
#define PrintMode RealOLED_Mode
/***********************/
#if PrintMode == Simulate_Mode
#define PrintStringLine(line, str)   OLEDPrintStringLine(line, str)
#elif PrintMode == RealOLED_Mode
#define PrintStringLine(line, str)   myOLEDPrintStringLine(line, str)
#endif

#endif // MenuSetting_H
