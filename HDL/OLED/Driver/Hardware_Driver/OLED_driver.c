/*

 * 这个头文件是oled库的 [硬件层] 实现文件，移植的时候需要更改这个文件的内容

*/
#include "main.h"
#include "OLED_driver.h"

uint8_t OLED_DisplayBuf[64 / 8][128]; // 显存
bool OLED_ColorMode = true;
bool spi_busy = 0;
extern SPI_HandleTypeDef hspi1;

/*引脚初始化*/
void OLED_I2C_Init(void)
{
	OLED_W_SCL(1);
	OLED_W_SDA(1);
}

/**
  * @brief  I2C开始
  * @param  无
  * @retval 无
  */
void OLED_I2C_Start(void)
{
	OLED_W_SDA(1);
	OLED_W_SCL(1);
	OLED_W_SDA(0);
	OLED_W_SCL(0);
}

/**
  * @brief  I2C停止
  * @param  无
  * @retval 无
  */
void OLED_I2C_Stop(void)
{
	OLED_W_SDA(0);
	OLED_W_SCL(1);
	OLED_W_SDA(1);
}

/**
  * @brief  I2C发送一个字节
  * @param  Byte 要发送的一个字节
  * @retval 无
  */
void OLED_I2C_SendByte(uint8_t Byte)
{
	uint8_t i;
	for (i = 0; i < 8; i++)
	{
		OLED_W_SDA(Byte & (0x80 >> i));
		OLED_W_SCL(1);
		OLED_W_SCL(0);
	}
	OLED_W_SCL(1);	//额外的一个时钟，不处理应答信号
	OLED_W_SCL(0);
}

/**
  * @brief  OLED写命令
  * @param  Command 要写入的命令
  * @retval 无
  */
void OLED_Write_CMD(uint8_t data)
{
	OLED_I2C_Start();
	OLED_I2C_SendByte(0x78);		//从机地址
	OLED_I2C_SendByte(0x00);		//写命令
	OLED_I2C_SendByte(data);
	OLED_I2C_Stop();
}

/**
  * @brief  OLED写数据
  * @param  Data 要写入的数据
  * @retval 无
  */
void OLED_Write_DATA(uint8_t Data)
{
	OLED_I2C_Start();
	OLED_I2C_SendByte(0x78);		//从机地址
	OLED_I2C_SendByte(0x40);		//写数据
	OLED_I2C_SendByte(Data);
	OLED_I2C_Stop();
}
/**
  * @brief  OLED连续写数组数据
  * @param  *data 要写入的数据数组
  * @param  Size 要写入的数据长度
  * @retval 无
  */
void OLED_WriteDataArr(uint8_t *data, uint16_t size)
{
	OLED_I2C_Start();
	OLED_I2C_SendByte(0x78);		//从机地址
	OLED_I2C_SendByte(0x40);		//写数据
	for (uint16_t i = 0; i < size; i++)
	{
		OLED_I2C_SendByte(data[i]);
	}
	OLED_I2C_Stop();
}

// 反显函数
void OLED_ColorTurn(uint8_t i)
{
	if (i == 0)
	{
		OLED_Write_CMD(0xA6); // 正常显示
	}
	if (i == 1)
	{
		OLED_Write_CMD(0xA7); // 反色显示
	}
}

/**
 * 函    数：控制屏幕显示方向
 * 参    数：Page 0=正常显示，1=屏幕内容反转180度
 * 返 回 值：无
 * 说    明：无
 */
void OLED_DisplayTurn(uint8_t i)
{
	// 如果不关闭屏幕，执行时画面会闪烁两次，先上下镜像再左右镜像
	OLED_Write_CMD(0xAE); // 关闭屏幕
	if (i == 0)
	{
		OLED_Write_CMD(0xC8); // 正常显示
		OLED_Write_CMD(0xA1);
	}
	if (i == 1)
	{
		OLED_Write_CMD(0xC0); // 反转显示
		OLED_Write_CMD(0xA0);
	}
	OLED_Write_CMD(0xAF); // 点亮屏幕
}

// 开启OLED显示
void OLED_DisPlay_On(void)
{
	OLED_Write_CMD(0x8D); // 电荷泵使能
	OLED_Write_CMD(0x14); // 开启电荷泵
	OLED_Write_CMD(0xAF); // 点亮屏幕
}

// 关闭OLED显示
void OLED_DisPlay_Off(void)
{
	OLED_Write_CMD(0x8D); // 电荷泵使能
	OLED_Write_CMD(0x10); // 关闭电荷泵
	OLED_Write_CMD(0xAE); // 关闭屏幕
}
/**
 * 函    数：OLED设置显示光标位置
 * 参    数：Page 指定光标所在的页，范围：0~15
 * 参    数：X 指定光标所在的X轴坐标，范围：0~127
 * 返 回 值：无
 * 说    明：OLED默认的Y轴，只能8个Bit为一组写入，即1页等于8个Y轴坐标
 */
void OLED_SetCursor(uint8_t Page, uint8_t X)
{
	/*如果使用此程序驱动1.3寸的OLED显示屏，则需要解除此注释*/
	/*因为1.3寸的OLED驱动芯片（SH1106）有132列*/
	/*屏幕的起始列接在了第2列，而不是第0列*/
	/*所以需要将X加2，才能正常显示*/
#ifdef SH1106
	X += 2;
#endif

	/*通过指令设置页地址和列地址*/
	OLED_Write_CMD(0xB0 | Page);			  // 设置页位置
	OLED_Write_CMD(0x10 | ((X & 0xF0) >> 4)); // 设置X位置高4位
	OLED_Write_CMD(0x00 | (X & 0x0F));		  // 设置X位置低4位
}

// 更新显存到OLED
void OLED_Update(void)
{
	uint8_t j;
	/*遍历每一页*/
	for (j = 0; j < 8; j++)
	{
		/*设置光标位置为每一页的第一列*/
		OLED_SetCursor(j, 0);
		/*连续写入128个数据，将显存数组的数据写入到OLED硬件*/
		OLED_WriteDataArr(OLED_DisplayBuf[j], 128);
	}
}

/**
 * 函    数：将OLED显存数组部分更新到OLED屏幕
 * 参    数：X 指定区域左上角的横坐标，范围：0~127
 * 参    数：Y 指定区域左上角的纵坐标，范围：0~127
 * 参    数：Width 指定区域的宽度，范围：0~128
 * 参    数：Height 指定区域的高度，范围：0~127
 * 返 回 值：无
 * 说    明：此函数会至少更新参数指定的区域
 *           如果更新区域Y轴只包含部分页，则同一页的剩余部分会跟随一起更新
 * 说    明：所有的显示函数，都只是对OLED显存数组进行读写
 *           随后调用OLED_Update函数或OLED_UpdateArea函数
 *           才会将显存数组的数据发送到OLED硬件，进行显示
 *           故调用显示函数后，要想真正地呈现在屏幕上，还需调用更新函数
 */
void OLED_UpdateArea(uint8_t X, uint8_t Y, uint8_t Width, uint8_t Height)
{
	uint8_t j;

	/*参数检查，保证指定区域不会超出屏幕范围*/
	if (X > 128 - 1)
	{
		return;
	}
	if (Y > 64 - 1)
	{
		return;
	}
	if (X + Width > 128)
	{
		Width = 128 - X;
	}
	if (Y + Height > 64)
	{
		Height = 64 - Y;
	}

	/*遍历指定区域涉及的相关页*/
	/*(Y + Height - 1) / 8 + 1的目的是(Y + Height) / 8并向上取整*/
	for (j = Y / 8; j < (Y + Height - 1) / 8 + 1; j++)
	{
		/*设置光标位置为相关页的指定列*/
		OLED_SetCursor(j, X);
		/*连续写入Width个数据，将显存数组的数据写入到OLED硬件*/
		OLED_WriteDataArr(&OLED_DisplayBuf[j][X], Width);
	}
}

extern void OLED_Clear(void);

// OLED的初始化
void OLED_Init(void)
{
	for (int i = 0; i < 1000; i++)
	{
		for (int j = 0; j < 1000; j++)
		{
		}
	}

	OLED_I2C_Init();

#ifdef SH1106
	OLED_Write_CMD(0xAE); // display off
	OLED_Write_CMD(0x02); // set lower column address
	OLED_Write_CMD(0x10); // set higher column address
	OLED_Write_CMD(0x40); // set display start line
	OLED_Write_CMD(0xB0); // set page address
	//	OLED_Write_CMD(0x81);//设置对比度（亮度）
	//	OLED_Write_CMD(0x00);//0x00-0xFF							//影响亮度：越高越亮
	OLED_Write_CMD(0xA1); // set segment remap
	OLED_Write_CMD(0xA6); // normal / reverse
	OLED_Write_CMD(0xA8); // multiplex ratio
	OLED_Write_CMD(0x3F); // duty = 1/64
	OLED_Write_CMD(0xad); // set charge pump enable
	OLED_Write_CMD(0x8b); // 0x8B 内供 VCC
	OLED_Write_CMD(0x32); // 0X30---0X33 set VPP 电荷泵电压 		//影响亮度：越高越亮
	OLED_Write_CMD(0xC8); // Com scan direction
	OLED_Write_CMD(0xD3); // set display offset
	OLED_Write_CMD(0x00); // 0x20
	OLED_Write_CMD(0xD5); // set osc division
	OLED_Write_CMD(0x80);
	OLED_Write_CMD(0xD9); // set pre-charge period
	OLED_Write_CMD(0x22); // 默认0x22
	OLED_Write_CMD(0xDA); // set COM pins
	OLED_Write_CMD(0x12);
	OLED_Write_CMD(0xdb); // set vcomh
	OLED_Write_CMD(0x35); // 0x00 - 0x40，默认0x35				//影响亮度：越高越亮
#endif

#ifdef SSD1306
	OLED_Write_CMD(0xAE);
	OLED_Write_CMD(0x00);
	OLED_Write_CMD(0x10);
	OLED_Write_CMD(0x40);
	//	OLED_Write_CMD(0x81);//设置对比度（亮度）
	//	OLED_Write_CMD(0xCF);
	OLED_Write_CMD(0xA1);
	OLED_Write_CMD(0xC8);
	OLED_Write_CMD(0xA6);
	OLED_Write_CMD(0xA8);
	OLED_Write_CMD(0x3f);
	OLED_Write_CMD(0xD3);
	OLED_Write_CMD(0x00);
	OLED_Write_CMD(0xd5);
	OLED_Write_CMD(0x80);
	OLED_Write_CMD(0xD9);
	OLED_Write_CMD(0xF1);
	OLED_Write_CMD(0xDA);
	OLED_Write_CMD(0x12);
	OLED_Write_CMD(0xDB);
	OLED_Write_CMD(0x40);
	OLED_Write_CMD(0x20);
	OLED_Write_CMD(0x02);
	OLED_Write_CMD(0x8D);
	OLED_Write_CMD(0x14);
	OLED_Write_CMD(0xA4);
	OLED_Write_CMD(0xA6);
	OLED_Write_CMD(0xAF);
#endif

	OLED_Brightness(150); // 初始化亮度设置函数。设置为-1相当于设置为0
	OLED_Clear();
	OLED_Write_CMD(0xAF); // Display ON
	for (int i = 0; i < 1000; i++)
	{
		for (int j = 0; j < 1000; j++)
		{
		}
	}
}

/**
 * 函    数：OLED设置亮度
 * 参    数：Brightness ，0-255，不同显示芯片效果可能不相同。
 * 返 回 值：无
 * 说    明：不要设置过大或者过小。
 */
void OLED_Brightness(int16_t Brightness)
{

	// 检测亮度设置是否变化，有变化时再发送指令
	static int16_t Last_Brightness;
	if (Brightness == Last_Brightness)
	{
		return;
	}
	else
	{
		Last_Brightness = Brightness;
	}

	if (Brightness > 255)
	{
		Brightness = 255;
	}
	if (Brightness < 0)
	{
		Brightness = 0;
	}
	OLED_Write_CMD(0x81);
	OLED_Write_CMD(Brightness);
}

/**
 * @brief 设置OLED显示模式
 * @param colormode true: 黑色模式，false: 白色模式
 * @note OLED_ColorTurn 0: 正常模式，1: 反转模式
 * @return 无
 */
void OLED_SetColorMode(bool colormode)
{
	OLED_ColorMode = colormode;

	// 检测显示模式设置是否变化，有变化时再发送指令
	static bool Last_OLED_ColorMode;
	if (OLED_ColorMode == Last_OLED_ColorMode)
	{
		return;
	}
	else
	{
		Last_OLED_ColorMode = OLED_ColorMode;
	}

	if (OLED_ColorMode)
		OLED_ColorTurn(0);
	else
		OLED_ColorTurn(1);
}
