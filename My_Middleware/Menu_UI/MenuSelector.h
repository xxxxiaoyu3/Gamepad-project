#ifndef MenuSelector_H
#define MenuSelector_H

#include "MenuSetting.h"

extern NodeBranch_t* Selector;
typedef void (*FuncPtr)(void);

void SelectUP(void);
void SelectDOWN(void);
void SelectINorRUN(void);
void SelectOut(void);			

void OLEDPrintStringLine(int line, const char* str);

void PrintSelector(void);

#endif // MenuSelector_H
