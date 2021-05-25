/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
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
#include "stm32f7xx_hal.h"

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
#define ICM_42605_INT1_Pin GPIO_PIN_8
#define ICM_42605_INT1_GPIO_Port GPIOI
#define TCS34725FN_INT_Pin GPIO_PIN_13
#define TCS34725FN_INT_GPIO_Port GPIOC
#define ICM_42605_INT1I9_Pin GPIO_PIN_9
#define ICM_42605_INT1I9_GPIO_Port GPIOI
#define LPS22HBTR_INT_Pin GPIO_PIN_10
#define LPS22HBTR_INT_GPIO_Port GPIOI
#define RV_3028_C7_INT_Pin GPIO_PIN_11
#define RV_3028_C7_INT_GPIO_Port GPIOI
#define TC78H660FTG_MODE_Pin GPIO_PIN_12
#define TC78H660FTG_MODE_GPIO_Port GPIOI
#define TC78H660FTG_ERR_Pin GPIO_PIN_13
#define TC78H660FTG_ERR_GPIO_Port GPIOI
#define TC78H660FTG_STBY_Pin GPIO_PIN_14
#define TC78H660FTG_STBY_GPIO_Port GPIOI
#define GPIO2_Pin GPIO_PIN_0
#define GPIO2_GPIO_Port GPIOB
#define GPIO3_Pin GPIO_PIN_1
#define GPIO3_GPIO_Port GPIOB
#define SUB_NRST_Pin GPIO_PIN_15
#define SUB_NRST_GPIO_Port GPIOI
#define LD0_Pin GPIO_PIN_0
#define LD0_GPIO_Port GPIOJ
#define LD1_Pin GPIO_PIN_1
#define LD1_GPIO_Port GPIOJ
#define LD2_Pin GPIO_PIN_2
#define LD2_GPIO_Port GPIOJ
#define LD3_Pin GPIO_PIN_3
#define LD3_GPIO_Port GPIOJ
#define SEG_A_Pin GPIO_PIN_0
#define SEG_A_GPIO_Port GPIOG
#define SEG_B_Pin GPIO_PIN_1
#define SEG_B_GPIO_Port GPIOG
#define uSDDETECT_Pin GPIO_PIN_5
#define uSDDETECT_GPIO_Port GPIOJ
#define CAN_FD_STBY_Pin GPIO_PIN_8
#define CAN_FD_STBY_GPIO_Port GPIOD
#define APA102_EN_Pin GPIO_PIN_9
#define APA102_EN_GPIO_Port GPIOD
#define DSI_TE_Pin GPIO_PIN_10
#define DSI_TE_GPIO_Port GPIOD
#define DSI_RESET_Pin GPIO_PIN_11
#define DSI_RESET_GPIO_Port GPIOD
#define SEG_C_Pin GPIO_PIN_2
#define SEG_C_GPIO_Port GPIOG
#define SEG_D_Pin GPIO_PIN_3
#define SEG_D_GPIO_Port GPIOG
#define SEG_E_Pin GPIO_PIN_4
#define SEG_E_GPIO_Port GPIOG
#define SEG_F_Pin GPIO_PIN_5
#define SEG_F_GPIO_Port GPIOG
#define SEG_G_Pin GPIO_PIN_6
#define SEG_G_GPIO_Port GPIOG
#define SEG_DP_Pin GPIO_PIN_7
#define SEG_DP_GPIO_Port GPIOG
#define GPIO1_Pin GPIO_PIN_6
#define GPIO1_GPIO_Port GPIOC
#define GPIO0_Pin GPIO_PIN_7
#define GPIO0_GPIO_Port GPIOC
#define HEARTBEAT_Pin GPIO_PIN_10
#define HEARTBEAT_GPIO_Port GPIOA
#define SPI2_CE_Pin GPIO_PIN_0
#define SPI2_CE_GPIO_Port GPIOI
#define SEG_SEL0_Pin GPIO_PIN_12
#define SEG_SEL0_GPIO_Port GPIOJ
#define SEG_SEL1_Pin GPIO_PIN_13
#define SEG_SEL1_GPIO_Port GPIOJ
#define SEG_SEL2_Pin GPIO_PIN_14
#define SEG_SEL2_GPIO_Port GPIOJ
#define SEG_SEL3_Pin GPIO_PIN_15
#define SEG_SEL3_GPIO_Port GPIOJ
#define BTN_JS0_Pin GPIO_PIN_10
#define BTN_JS0_GPIO_Port GPIOG
#define BTN_JS1_Pin GPIO_PIN_11
#define BTN_JS1_GPIO_Port GPIOG
#define ENC_INT_Pin GPIO_PIN_3
#define ENC_INT_GPIO_Port GPIOK
#define BTN0_Pin GPIO_PIN_4
#define BTN0_GPIO_Port GPIOK
#define BTN1_Pin GPIO_PIN_5
#define BTN1_GPIO_Port GPIOK
#define BTN2_Pin GPIO_PIN_6
#define BTN2_GPIO_Port GPIOK
#define BTN3_Pin GPIO_PIN_7
#define BTN3_GPIO_Port GPIOK
#define JACK_Pin GPIO_PIN_5
#define JACK_GPIO_Port GPIOB
#define LCD_BL_CTRL_Pin GPIO_PIN_9
#define LCD_BL_CTRL_GPIO_Port GPIOB
#define ENC_CHA_Pin GPIO_PIN_5
#define ENC_CHA_GPIO_Port GPIOI
#define ENC_CHB_Pin GPIO_PIN_6
#define ENC_CHB_GPIO_Port GPIOI
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
