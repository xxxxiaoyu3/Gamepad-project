#include "Remote_ADC.h"

extern uint16_t ADC_Buf[];//ADC Buffer(DMA传输的地址)
int16_t Left_x_axis, Left_y_axis, Right_x_axis, Right_y_axis;
HID_Gamepad_Report_t gamepad_report;
void User_ADC_Init(void)
{
    //Add User's GPIO code here.
    //The Cubemax I'm using has already been configured, so I won't add it here.
}
void Remote_ADC_Init(void)
{
    User_ADC_Init();
    // 初始化游戏手柄报告
    memset(&gamepad_report, 0, sizeof(gamepad_report));
    gamepad_report.left_x = 128;   // 摇杆居中
    gamepad_report.left_y = 128;
    gamepad_report.right_x = 128;
    gamepad_report.right_y = 128;
}
#if REMOTE_MODE
// 读取摇杆并映射到8位范围
void Read_joystick(HID_Gamepad_Report_t *report)
{
    uint16_t adc_lx, adc_ly, adc_rx, adc_ry;
    
    adc_lx = ADC_Buf[0];
    adc_ly = ADC_Buf[1];
    adc_rx = ADC_Buf[2];
    adc_ry = ADC_Buf[3];

    // 映射到8位范围(0-255)
    report->left_x = (uint8_t)(adc_lx * 255 / 4095);
    report->left_y = (uint8_t)(adc_ly * 255 / 4095);
    report->right_x = (uint8_t)(adc_rx * 255 / 4095);
    report->right_y = (uint8_t)(adc_ry * 255 / 4095);
}
#else
// 读取摇杆并以128为中心点映射的函数
void Read_joystick(HID_Gamepad_Report_t *report)
{
    uint16_t adc_lx, adc_ly, adc_rx, adc_ry;
    
    adc_lx = ADC_Buf[0];
    adc_ly = ADC_Buf[1];
    adc_rx = ADC_Buf[2];
    adc_ry = ADC_Buf[3];

    // 映射到8位范围(0-255)
    report->left_x = (uint8_t)(adc_lx * 255 / 4095);
    report->left_y = (uint8_t)(adc_ly * 255 / 4095);
    report->right_x = (uint8_t)(adc_rx * 255 / 4095);
    report->right_y = (uint8_t)(adc_ry * 255 / 4095);
}
#endif