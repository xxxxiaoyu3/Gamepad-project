/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f1xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define KEY1_Input_Pin GPIO_PIN_13
#define KEY1_Input_GPIO_Port GPIOC
#define KEY2_Input_Pin GPIO_PIN_12
#define KEY2_Input_GPIO_Port GPIOB
#define IIC_SCL_Pin GPIO_PIN_13
#define IIC_SCL_GPIO_Port GPIOB
#define IIC_SDA_Pin GPIO_PIN_14
#define IIC_SDA_GPIO_Port GPIOB
#define KEY3_Input_Pin GPIO_PIN_15
#define KEY3_Input_GPIO_Port GPIOB
#define KEY4_Input_Pin GPIO_PIN_8
#define KEY4_Input_GPIO_Port GPIOA
#define Beep_Output_Pin GPIO_PIN_3
#define Beep_Output_GPIO_Port GPIOB
#define KEY6_Input_Pin GPIO_PIN_5
#define KEY6_Input_GPIO_Port GPIOB
#define KEY5_Input_Pin GPIO_PIN_6
#define KEY5_Input_GPIO_Port GPIOB
#define LED1_Out_Pin GPIO_PIN_7
#define LED1_Out_GPIO_Port GPIOB
#define LED2_Out_Pin GPIO_PIN_8
#define LED2_Out_GPIO_Port GPIOB
#define LED3_Out_Pin GPIO_PIN_9
#define LED3_Out_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
