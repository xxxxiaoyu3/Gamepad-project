#include "My_Display.h"
/**
 * @brief 显示遥控器ADC数值
 *
 * 在OLED屏幕上显示左右摇杆的ADC数值
 * 适用于128x64分辨率的OLED屏幕
 */
void Remote_ADC_DisplayFunc(void)
{
	// 清屏
	OLED_Clear();
	
	OLED_ShowString(0, 0*8, "L_Remote:",OLED_8X8_FULL);
  	OLED_ShowString(0, 2*8, "X:",OLED_8X8_FULL);
 	 OLED_ShowString(8*8, 2*8, "Y:",OLED_8X8_FULL);
  	OLED_ShowSignedNum(2*8, 2*8,gamepad_report.left_x,5,OLED_8X8_FULL);
	OLED_ShowSignedNum(10*8, 2*8,gamepad_report.left_y,5,OLED_8X8_FULL);
 	 OLED_ShowString(0, 4*8, "R_Remote:",OLED_8X8_FULL);
  	OLED_ShowString(0, 6*8, "X:",OLED_8X8_FULL);
  	OLED_ShowString(8*8, 6*8, "Y:",OLED_8X8_FULL);
  	OLED_ShowSignedNum(2*8, 6*8,gamepad_report.right_x,5,OLED_8X8_FULL);
	OLED_ShowSignedNum(10*8, 6*8,gamepad_report.right_y,5,OLED_8X8_FULL);
}
/* 	* @brief 校准Remote UI界面
	* 绘制一个圆形遥控器UI，并显示左右摇杆的UI
	* 适用于128x64分辨率的OLED屏幕
*/
void Remote_ADC_Joystick_Dis(void)
{
	// 清屏
	OLED_Clear();
	
	// 绘制左侧摇杆的圆形背景 (在屏幕左半部分)
	// 圆心坐标 (32, 32)，半径 20
	OLED_DrawCircle(32, 43, 20,OLED_UNFILLED);
	
	// 绘制右侧摇杆的圆形背景 (在屏幕右半部分)
	// 圆心坐标 (96, 32)，半径 20
	OLED_DrawCircle(96, 43, 20,OLED_UNFILLED);
	
	// 计算左摇杆点的位置 (基于gamepad_report.left_x和gamepad_report.left_y)
	// 假设ADC值范围为-128到127，需要映射到圆形区域内
	int8_t left_x_pos = 32 + ((int8_t)(gamepad_report.left_x-128) * 20) / 127;
	int8_t left_y_pos = 43 + ((int8_t)(gamepad_report.left_y-128) * 20) / 127;

	/* 如果左摇杆在圆形区域外，则将左摇杆点设置为圆形区域边缘*/
	// 计算相对于圆心的位置
	int8_t left_dx = left_x_pos - 32;
	int8_t left_dy = left_y_pos - 43;
	// 计算距离的平方
	int16_t left_dist_sq = left_dx * left_dx + left_dy * left_dy;
	// 如果距离大于半径的平方(20*20=400)，则限制在圆周上
	if (left_dist_sq > 400) {
		// 计算缩放因子以将点投影到圆周上
		float scale = 20.0f / sqrtf((float)left_dist_sq);
		left_x_pos = 32 + (int8_t)(left_dx * scale);
		left_y_pos = 43 + (int8_t)(left_dy * scale);
	}
	
	// 计算右摇杆点的位置 (基于gamepad_report.right_x和gamepad_report.right_y)
	int8_t right_x_pos = 96 + ((int8_t)(gamepad_report.right_x-128) * 20) / 127;
	int8_t right_y_pos = 43 + ((int8_t)(gamepad_report.right_y-128) * 20) / 127;
	
	/* 如果右摇杆在圆形区域外，则将右摇杆点设置为圆形区域边缘*/
	// 计算相对于圆心的位置
	int8_t right_dx = right_x_pos - 96;
	int8_t right_dy = right_y_pos - 43;
	// 计算距离的平方
	int16_t right_dist_sq = right_dx * right_dx + right_dy * right_dy;
	// 如果距离大于半径的平方(20*20=400)，则限制在圆周上
	if (right_dist_sq > 400) {
		// 计算缩放因子以将点投影到圆周上
		float scale = 20.0f / sqrtf((float)right_dist_sq);
		right_x_pos = 96 + (int8_t)(right_dx * scale);
		right_y_pos = 43 + (int8_t)(right_dy * scale);
	}

	// 绘制左摇杆点
	OLED_DrawPoint(left_x_pos, left_y_pos);
	OLED_DrawPoint(left_x_pos-1, left_y_pos);
	OLED_DrawPoint(left_x_pos+1, left_y_pos);
	OLED_DrawPoint(left_x_pos, left_y_pos-1);
	OLED_DrawPoint(left_x_pos, left_y_pos+1);
	
	// 绘制右摇杆点
	OLED_DrawPoint(right_x_pos, right_y_pos);
	OLED_DrawPoint(right_x_pos-1, right_y_pos);
	OLED_DrawPoint(right_x_pos+1, right_y_pos);
	OLED_DrawPoint(right_x_pos, right_y_pos-1);
	OLED_DrawPoint(right_x_pos, right_y_pos+1);
	
	switch (Get_Calibration_State())
    {
	case JOYSTICK_CAL_LEFT_MIN:
      /* code */
		OLED_ShowString(6*8,0*8,(char *)"FIX",OLED_8X8_FULL);
      break;
    case JOYSTICK_CAL_LEFT_MAX:
      /* code */
		OLED_ShowString(0*8,0*8,(char *)"Left Min OK    ",OLED_8X8_FULL);
      
      break;
    case JOYSTICK_CAL_LEFT_CENTER:
      /* code */
		OLED_ShowString(0*8,0*8,(char *)"Left Max OK    ",OLED_8X8_FULL);
      
      break;
    case JOYSTICK_CAL_RIGHT_MIN:
		OLED_ShowString(0*8,0*8,(char *)"Left Center OK    ",OLED_8X8_FULL);
		break;
	case JOYSTICK_CAL_RIGHT_MAX:
		OLED_ShowString(0*8,0*8,(char *)"Right Min OK    ",OLED_8X8_FULL);
		break;
	case JOYSTICK_CAL_RIGHT_CENTER:
		OLED_ShowString(0*8,0*8,(char *)"Right Max OK    ",OLED_8X8_FULL);
		break;
//	case JOYSTICK_CAL_COMPLETE:
//		OLED_ShowString(0*8,0*8,(char *)"Right Center OK    ",OLED_8X8_FULL);
//		break;
    default:
      break;
    }
	
	OLED_Update();
}