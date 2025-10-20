#include "My_Display.h"
/**
 * @brief ????? ADC ????
 *
 * ?????????? ADC ??
 * x128 y64 OLED ??????,????????
 */
void Remote_ADC_DisplayFunc(void)
{
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