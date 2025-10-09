#ifndef __OLED_DRIVER_H
#define __OLED_DRIVER_H

#include <string.h>
#include <math.h>
#include <stdbool.h>
#include <stdarg.h>
#include "stdio.h"
#include "main.h"

// 主控型号选择
#define SSD1306
// #define SH1106

#define OLED_CMD 0  // 写命令
#define OLED_DATA 1 // 写数据

/*引脚配置*/
#define OLED_W_SCL(x)		HAL_GPIO_WritePin(IIC_SCL_GPIO_Port, IIC_SCL_Pin, (x) ? GPIO_PIN_SET : GPIO_PIN_RESET)
#define OLED_W_SDA(x)		HAL_GPIO_WritePin(IIC_SDA_GPIO_Port, IIC_SDA_Pin, (x) ? GPIO_PIN_SET : GPIO_PIN_RESET)

//	oled初始化函数
void OLED_Init(void);
//	oled全局刷新函数
void OLED_Update(void);
//	oled局部刷新函数
void OLED_UpdateArea(uint8_t X, uint8_t Y, uint8_t Width, uint8_t Height);
// 设置颜色模式
void OLED_SetColorMode(bool colormode);
// OLED 设置亮度函数
void OLED_Brightness(int16_t Brightness);

#endif
