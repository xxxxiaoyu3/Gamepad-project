#ifndef __Remote_ADC_H__
#define __Remote_ADC_H__

#include "stm32f1xx_hal.h"
#include "gpio.h"
#include "main.h"
#include "adc.h"
#include <stdint.h>
#include <string.h>


/*********定义遥感模式**********/
//REMOTE_MODE=0:中心模式,以中心点(2048)为零点，向两个方向延伸(-2048~+2047)
//REMOTE_MODE=1:非中心模式,以0为零点，向一个方向延伸(0~4095) 
#define REMOTE_MODE 0

// 校准阈值定义
#define CAL_THRESHOLD_MIN 100     // 最小值阈值
#define CAL_THRESHOLD_MAX 4000    // 最大值阈值
#define CAL_THRESHOLD_CENTER_LOW 1800  // 中心值低阈值
#define CAL_THRESHOLD_CENTER_HIGH 2200 // 中心值高阈值

typedef __packed struct {
  uint8_t  buttons;      // 字节0: 按钮 (bit0-5对应6个按钮)
  uint8_t  left_x;       // 字节1: 左摇杆X (0-255, 128中心)
  uint8_t  left_y;       // 字节2: 左摇杆Y (0-255, 128中心) 
  uint8_t  right_x;      // 字节3: 右摇杆X (0-255, 128中心)
  uint8_t  right_y;      // 字节4: 右摇杆Y (0-255, 128中心)
} Gamepad_Report_t;

// 校准数据结构
typedef struct {
    uint16_t x_min;
    uint16_t x_max;
    uint16_t y_min;
    uint16_t y_max;
    uint16_t x_center;
    uint16_t y_center;
} JoystickCalData;

// 校准状态枚举
typedef enum {
    JOYSTICK_CAL_INACTIVE,
    JOYSTICK_CAL_LEFT_MIN,
    JOYSTICK_CAL_LEFT_MAX,
    JOYSTICK_CAL_LEFT_CENTER,
    JOYSTICK_CAL_RIGHT_MIN,
    JOYSTICK_CAL_RIGHT_MAX,
    JOYSTICK_CAL_RIGHT_CENTER,
    JOYSTICK_CAL_COMPLETE
} JoystickCalState;

extern int16_t Left_x_axis, Left_y_axis, Right_x_axis, Right_y_axis;        //定时转换数据
extern Gamepad_Report_t gamepad_report;

void Remote_ADC_Init(void);
void Read_joystick(Gamepad_Report_t *report);
#endif
