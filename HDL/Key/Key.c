#include "Key.h"

// 按键端口数组，依次对应四个按键
static GPIO_TypeDef* const Key_PORTS[KEY_COUNT] = {
    Key1_PORT, Key2_PORT, Key3_PORT, Key4_PORT, Key5_PORT, Key6_PORT
};

// 按键引脚数组，依次对应四个按键
static const uint16_t Key_PINS[KEY_COUNT] = {
    Key1_LINE, Key2_LINE, Key3_LINE, Key4_LINE, Key5_LINE, Key6_LINE
};

// 简单按键状态机的状态定义
typedef enum {
    KEY_IDLE = 0,     // 空闲状态
    KEY_DEBOUNCE,     // 消抖状态
    KEY_PRESSED       // 已按下状态
} KeyState;

//

// 每个按键的当前状态
static KeyState key_state[KEY_COUNT] = {KEY_IDLE};
/*************外部变量声明*************/

// 按键初始化，配置GPIO
void User_Key_GPIO_Init(void)
{
    //Add User's code here.
    //The Cubemax I'm using has already been configured, so I won't add it here.
}
void Key_Init(void)
{
    User_Key_GPIO_Init();
}

/**
 * 邱Sir
 * @brief  Key_Scan函数实现原理：
 * 采用简单状态机方式对每个按键进行扫描和消抖
 * 每个按键有独立的状态(KEY_IDLE/KEY_DEBOUNCE/KEY_PRESSED)
 * 确认按键按下后传递给数组Key_States_Flag
 * Key_States_Flag数组用于存储每个按键的状态，1表示按下，0表示未按下
 * 注意：Key_States_Flag数组在置1后需要软件清零(本函数不会自己清零)
 */
//i=0:Key_UP
//i=1:Key_LEFT
//i=2:Key_DOWN
//i=3:Key_RIGHT
KeyType Key_Scan(void)
{
    KeyType act_key = None; // 默认返回None
    for (uint8_t i = 0; i < KEY_COUNT; i++)
    {
        // 扫描每个按键当前电平
        uint8_t pinState = HAL_GPIO_ReadPin(Key_PORTS[i], Key_PINS[i]);

        switch (key_state[i])
        {
            case KEY_IDLE:
                // 检测到按下，进入消抖
                if (pinState == GPIO_PIN_RESET) {
                    key_state[i] = KEY_DEBOUNCE;
                }
                break;
            case KEY_DEBOUNCE:
				if (pinState == GPIO_PIN_RESET) {
					// 按键确认按下
					switch (i)
					{
                        /* 在这里添加按键x按键对应的操作，如按键1为向上按键，按键2为左按键，按键3为下按键，按键4为右按键 */
						case 0: act_key = KEY_UP; break;
						case 1: act_key = KEY_DOWN; break;
						case 2: act_key = KEY_CANCEL; break;
						case 3: act_key = KEY_ENTER; break;
                        case 4: act_key = KEY_LEFT; break;
                        case 5: act_key = KEY_RIGHT; break;
					}
					key_state[i] = KEY_PRESSED;  //在这里进入 PRESSED 状态
				} else {
					key_state[i] = KEY_IDLE; // 抖动误触
				}
				break;
            case KEY_PRESSED:
                // 松开后回到空闲
                if (pinState == GPIO_PIN_SET) {
                    key_state[i] = KEY_IDLE;
                }
                break;
        }
    }
    return act_key; // 返回当前按键状态
}

// 按键任务处理函数(这里处理按键按下后要做什么)
void Key_Handler(KeyType Read_key)
{
    switch (Read_key) // 根据按键类型执行不同操作
    {
    case KEY_UP:
        /* code */
        SelectUP();
        break;
    case KEY_DOWN:
        /* code */
        SelectDOWN();
        break;
    case KEY_LEFT:
        /* code */

        break;
    case KEY_RIGHT:
        /* code */

        break;
    case KEY_ENTER:
        /* code */
        SelectINorRUN();
        break;
	case KEY_CANCEL:
        /* code */
        SelectOut();
        break;
    default:
        break;
    }
}


void Key_Task(KeyType Read_key)     //按键测试任务
{
    switch (Read_key) // 根据按键类型执行不同操作
    {
    case KEY_UP:
        /* code */
        printf("KEY_UP");
		printf(" Read_key: %d\r\n", Read_key);
        SelectUP();
		LED_Toggle(Pin1,1);
        break;
    case KEY_DOWN:
        /* code */
		printf("KEY_DOWN");
		printf(" Read_key: %d\r\n", Read_key);
        SelectDOWN();
        break;
    case KEY_LEFT:
        /* code */
		printf("KEY_LEFT");
		printf(" Read_key: %d\r\n", Read_key);
        SelectINorRUN();
        break;
    case KEY_RIGHT:
        /* code */
		printf("KEY_RIGHT");
		printf(" Read_key: %d\r\n", Read_key);
        SelectOut();
        break;
    case KEY_ENTER:
        /* code */
        printf("KEY_ENTER");
        printf(" Read_key: %d\r\n", Read_key);
        break;
	case KEY_CANCEL:
        /* code */
        printf("KEY_CANCEL");
        printf(" Read_key: %d\r\n", Read_key);
        break;
    default:
        break;
    }
}

