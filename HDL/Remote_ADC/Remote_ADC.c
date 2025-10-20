#include "Remote_ADC.h"

// 左右摇杆的校准数据
static JoystickCalData left_joystick_cal = {0, 4095, 0, 4095, 2048, 2048};
static JoystickCalData right_joystick_cal = {0, 4095, 0, 4095, 2048, 2048};
static JoystickCalState cal_state = JOYSTICK_CAL_INACTIVE;

// 用于防抖的计数器
static uint16_t cal_stable_counter = 0;
static uint16_t cal_stable_threshold = 50; // 需要稳定的采样次数

/**
 * @brief 开始摇杆校准过程
 */
void Start_Joystick_Calibration(void) {
    cal_state = JOYSTICK_CAL_LEFT_MIN;
    cal_stable_counter = 0;
}

/**
 * @brief 基于摇杆位置自动执行校准步骤
 * @note 此函数应在主循环中定期调用
 */
void Auto_Joystick_Calibration(void) {
    uint16_t adc_lx, adc_ly, adc_rx, adc_ry;
    
    // 读取当前ADC值
    adc_lx = ADC_Buf[0];
    adc_ly = ADC_Buf[1];
    adc_rx = ADC_Buf[2];
    adc_ry = ADC_Buf[3];
    
    // 只有在校准进行中时才处理
    if (cal_state == JOYSTICK_CAL_INACTIVE || cal_state == JOYSTICK_CAL_COMPLETE) {
        return;
    }
    
    uint8_t position_stable = 0;
    
    switch(cal_state) {
        case JOYSTICK_CAL_LEFT_MIN:
            // 检查左摇杆是否处于最小位置
            if (adc_lx < CAL_THRESHOLD_MIN && adc_ly < CAL_THRESHOLD_MIN) {
                cal_stable_counter++;
                if (cal_stable_counter >= cal_stable_threshold) {
                    // 记录左摇杆最小值
                    left_joystick_cal.x_min = adc_lx;
                    left_joystick_cal.y_min = adc_ly;
                    cal_state = JOYSTICK_CAL_LEFT_MAX;
                    cal_stable_counter = 0;
                }
            } else {
                cal_stable_counter = 0; // 重置计数器
            }
            break;
            
        case JOYSTICK_CAL_LEFT_MAX:
            // 检查左摇杆是否处于最大位置
            if (adc_lx > CAL_THRESHOLD_MAX && adc_ly > CAL_THRESHOLD_MAX) {
                cal_stable_counter++;
                if (cal_stable_counter >= cal_stable_threshold) {
                    // 记录左摇杆最大值
                    left_joystick_cal.x_max = adc_lx;
                    left_joystick_cal.y_max = adc_ly;
                    cal_state = JOYSTICK_CAL_LEFT_CENTER;
                    cal_stable_counter = 0;
                }
            } else {
                cal_stable_counter = 0; // 重置计数器
            }
            break;
            
        case JOYSTICK_CAL_LEFT_CENTER:
            // 检查左摇杆是否处于中心位置
            if (adc_lx > CAL_THRESHOLD_CENTER_LOW && adc_lx < CAL_THRESHOLD_CENTER_HIGH &&
                adc_ly > CAL_THRESHOLD_CENTER_LOW && adc_ly < CAL_THRESHOLD_CENTER_HIGH) {
                cal_stable_counter++;
                if (cal_stable_counter >= cal_stable_threshold) {
                    // 记录左摇杆中心值
                    left_joystick_cal.x_center = adc_lx;
                    left_joystick_cal.y_center = adc_ly;
                    cal_state = JOYSTICK_CAL_RIGHT_MIN;
                    cal_stable_counter = 0;
                }
            } else {
                cal_stable_counter = 0; // 重置计数器
            }
            break;
            
        case JOYSTICK_CAL_RIGHT_MIN:
            // 检查右摇杆是否处于最小位置
            if (adc_rx < CAL_THRESHOLD_MIN && adc_ry < CAL_THRESHOLD_MIN) {
                cal_stable_counter++;
                if (cal_stable_counter >= cal_stable_threshold) {
                    // 记录右摇杆最小值
                    right_joystick_cal.x_min = adc_rx;
                    right_joystick_cal.y_min = adc_ry;
                    cal_state = JOYSTICK_CAL_RIGHT_MAX;
                    cal_stable_counter = 0;
                }
            } else {
                cal_stable_counter = 0; // 重置计数器
            }
            break;
            
        case JOYSTICK_CAL_RIGHT_MAX:
            // 检查右摇杆是否处于最大位置
            if (adc_rx > CAL_THRESHOLD_MAX && adc_ry > CAL_THRESHOLD_MAX) {
                cal_stable_counter++;
                if (cal_stable_counter >= cal_stable_threshold) {
                    // 记录右摇杆最大值
                    right_joystick_cal.x_max = adc_rx;
                    right_joystick_cal.y_max = adc_ry;
                    cal_state = JOYSTICK_CAL_RIGHT_CENTER;
                    cal_stable_counter = 0;
                }
            } else {
                cal_stable_counter = 0; // 重置计数器
            }
            break;
            
        case JOYSTICK_CAL_RIGHT_CENTER:
            // 检查右摇杆是否处于中心位置
            if (adc_rx > CAL_THRESHOLD_CENTER_LOW && adc_rx < CAL_THRESHOLD_CENTER_HIGH &&
                adc_ry > CAL_THRESHOLD_CENTER_LOW && adc_ry < CAL_THRESHOLD_CENTER_HIGH) {
                cal_stable_counter++;
                if (cal_stable_counter >= cal_stable_threshold) {
                    // 记录右摇杆中心值
                    right_joystick_cal.x_center = adc_rx;
                    right_joystick_cal.y_center = adc_ry;
                    cal_state = JOYSTICK_CAL_COMPLETE;
                    cal_stable_counter = 0;
                }
            } else {
                cal_stable_counter = 0; // 重置计数器
            }
            break;
            
        default:
            break;
    }
}

/**
 * @brief 获取当前校准步骤的提示信息
 * @return 提示字符串
 */
const char* Get_Calibration_Step_Info(void) {
    switch(cal_state) {
        case JOYSTICK_CAL_LEFT_MIN:
            return "Move LEFT joystick to MIN corner";
        case JOYSTICK_CAL_LEFT_MAX:
            return "Move LEFT joystick to MAX corner";
        case JOYSTICK_CAL_LEFT_CENTER:
            return "Center LEFT joystick";
        case JOYSTICK_CAL_RIGHT_MIN:
            return "Move RIGHT joystick to MIN corner";
        case JOYSTICK_CAL_RIGHT_MAX:
            return "Move RIGHT joystick to MAX corner";
        case JOYSTICK_CAL_RIGHT_CENTER:
            return "Center RIGHT joystick";
        case JOYSTICK_CAL_COMPLETE:
            return "Calibration completed!";
        default:
            return "Press to start calibration";
    }
}

/**
 * @brief 检查校准是否正在进行
 * @return 1-正在进行，0-未进行
 */
uint8_t Is_Joystick_Calibrating(void) {
    return (cal_state != JOYSTICK_CAL_INACTIVE && cal_state != JOYSTICK_CAL_COMPLETE);
}

/**
 * @brief 检查校准是否完成
 * @return 1-已完成，0-未完成
 */
uint8_t Is_Joystick_Calibration_Complete(void) {
    return (cal_state == JOYSTICK_CAL_COMPLETE);
}

/**
 * @brief 重置校准状态
 */
void Reset_Joystick_Calibration_State(void) {
    cal_state = JOYSTICK_CAL_INACTIVE;
    cal_stable_counter = 0;
}

/**
 * @brief 读取摇杆值并应用校准数据
 * @param report 游戏手柄报告结构体指针
 */
void Read_joystick(Gamepad_Report_t *report)
{
    uint16_t adc_lx, adc_ly, adc_rx, adc_ry;
    
    adc_lx = ADC_Buf[0];
    adc_ly = ADC_Buf[1];
    adc_rx = ADC_Buf[2];
    adc_ry = ADC_Buf[3];

    // 应用校准数据并映射到8位范围(0-255)
    // 对于左摇杆X轴
    if (adc_lx <= left_joystick_cal.x_center) {
        if (adc_lx <= left_joystick_cal.x_min) {
            report->left_x = 0;
        } else {
            report->left_x = (uint8_t)(128 - ((left_joystick_cal.x_center - adc_lx) * 128 / 
                                            (left_joystick_cal.x_center - left_joystick_cal.x_min)));
        }
    } else {
        if (adc_lx >= left_joystick_cal.x_max) {
            report->left_x = 255;
        } else {
            report->left_x = (uint8_t)(128 + ((adc_lx - left_joystick_cal.x_center) * 127 / 
                                            (left_joystick_cal.x_max - left_joystick_cal.x_center)));
        }
    }
    
    // 对于左摇杆Y轴
    if (adc_ly <= left_joystick_cal.y_center) {
        if (adc_ly <= left_joystick_cal.y_min) {
            report->left_y = 0;
        } else {
            report->left_y = (uint8_t)(128 - ((left_joystick_cal.y_center - adc_ly) * 128 / 
                                            (left_joystick_cal.y_center - left_joystick_cal.y_min)));
        }
    } else {
        if (adc_ly >= left_joystick_cal.y_max) {
            report->left_y = 255;
        } else {
            report->left_y = (uint8_t)(128 + ((adc_ly - left_joystick_cal.y_center) * 127 / 
                                            (left_joystick_cal.y_max - left_joystick_cal.y_center)));
        }
    }
    
    // 对于右摇杆X轴
    if (adc_rx <= right_joystick_cal.x_center) {
        if (adc_rx <= right_joystick_cal.x_min) {
            report->right_x = 0;
        } else {
            report->right_x = (uint8_t)(128 - ((right_joystick_cal.x_center - adc_rx) * 128 / 
                                             (right_joystick_cal.x_center - right_joystick_cal.x_min)));
        }
    } else {
        if (adc_rx >= right_joystick_cal.x_max) {
            report->right_x = 255;
        } else {
            report->right_x = (uint8_t)(128 + ((adc_rx - right_joystick_cal.x_center) * 127 / 
                                             (right_joystick_cal.x_max - right_joystick_cal.x_center)));
        }
    }
    
    // 对于右摇杆Y轴
    if (adc_ry <= right_joystick_cal.y_center) {
        if (adc_ry <= right_joystick_cal.y_min) {
            report->right_y = 0;
        } else {
            report->right_y = (uint8_t)(128 - ((right_joystick_cal.y_center - adc_ry) * 128 / 
                                             (right_joystick_cal.y_center - right_joystick_cal.y_min)));
        }
    } else {
        if (adc_ry >= right_joystick_cal.y_max) {
            report->right_y = 255;
        } else {
            report->right_y = (uint8_t)(128 + ((adc_ry - right_joystick_cal.y_center) * 127 / 
                                             (right_joystick_cal.y_max - right_joystick_cal.y_center)));
        }
    }
}

/**
 * @brief 重置校准数据为默认值
 */
void Reset_Joystick_Calibration(void) {
    left_joystick_cal.x_min = 0;
    left_joystick_cal.x_max = 4095;
    left_joystick_cal.y_min = 0;
    left_joystick_cal.y_max = 4095;
    left_joystick_cal.x_center = 2048;
    left_joystick_cal.y_center = 2048;
    
    right_joystick_cal.x_min = 0;
    right_joystick_cal.x_max = 4095;
    right_joystick_cal.y_min = 0;
    right_joystick_cal.y_max = 4095;
    right_joystick_cal.x_center = 2048;
    right_joystick_cal.y_center = 2048;
    
    cal_state = JOYSTICK_CAL_INACTIVE;
    cal_stable_counter = 0;
}