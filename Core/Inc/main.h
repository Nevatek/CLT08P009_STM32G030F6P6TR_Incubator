/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
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
#include "stm32g0xx_hal.h"

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
I2C_HandleTypeDef* GetInstance_I2C1(void);
ADC_HandleTypeDef* GetInstance_ADC1(void);
#if (PELTIER_PID_CONTROL)
TIM_HandleTypeDef* GetInstance_Timer16(void);
TIM_HandleTypeDef* GetInstance_Timer1_P0(void);
TIM_HandleTypeDef* GetInstance_Timer3_P1(void);
#endif
/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define NC_Pin GPIO_PIN_9
#define NC_GPIO_Port GPIOB
#define NCC15_Pin GPIO_PIN_15
#define NCC15_GPIO_Port GPIOC
#define NTC_ADC_0_Pin GPIO_PIN_0
#define NTC_ADC_0_GPIO_Port GPIOA
#define NCA1_Pin GPIO_PIN_1
#define NCA1_GPIO_Port GPIOA
#define NCA2_Pin GPIO_PIN_2
#define NCA2_GPIO_Port GPIOA
#define NCA3_Pin GPIO_PIN_3
#define NCA3_GPIO_Port GPIOA
#define NCA4_Pin GPIO_PIN_4
#define NCA4_GPIO_Port GPIOA
#define PELTIER_SIG_CH0_Pin GPIO_PIN_5
#define PELTIER_SIG_CH0_GPIO_Port GPIOA
#define FAN_ENABLE_Pin GPIO_PIN_6
#define FAN_ENABLE_GPIO_Port GPIOA
#define PELTIER_SIG_CH1_Pin GPIO_PIN_7
#define PELTIER_SIG_CH1_GPIO_Port GPIOA
#define PELTIER_DIR_CH1_Pin GPIO_PIN_8
#define PELTIER_DIR_CH1_GPIO_Port GPIOA
#define PELTIER_DIR_CH0_Pin GPIO_PIN_11
#define PELTIER_DIR_CH0_GPIO_Port GPIOA
#define NCA12_Pin GPIO_PIN_12
#define NCA12_GPIO_Port GPIOA

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
