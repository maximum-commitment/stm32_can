/**
  ******************************************************************************
  * @file    stm32f4_discovery.c
  * @author  MCD Application Team
  * @brief   This file provides set of firmware functions to manage Leds and
  *          push-button available on STM32F4-Discovery Kit from STMicroelectronics.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2017 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* Includes ------------------------------------------------------------------*/
#include "stm32f4_discovery.h"

/** @defgroup BSP BSP
  * @{
  */ 

/** @defgroup STM32F4_DISCOVERY STM32F4 DISCOVERY
  * @{
  */   
    
/** @defgroup STM32F4_DISCOVERY_LOW_LEVEL STM32F4 DISCOVERY LOW LEVEL
  * @brief This file provides set of firmware functions to manage Leds and push-button
  *        available on STM32F4-Discovery Kit from STMicroelectronics.
  * @{
  */ 

/** @defgroup STM32F4_DISCOVERY_LOW_LEVEL_Private_TypesDefinitions STM32F4 DISCOVERY LOW LEVEL Private TypesDefinitions
  * @{
  */ 
/**
  * @}
  */ 

/** @defgroup STM32F4_DISCOVERY_LOW_LEVEL_Private_Defines STM32F4 DISCOVERY LOW LEVEL Private Defines
  * @{
  */
  
  /**
  * @brief STM32F4 DISCO BSP Driver version number V2.1.5
  */
#define __STM32F4_DISCO_BSP_VERSION_MAIN   (0x02) /*!< [31:24] main version */
#define __STM32F4_DISCO_BSP_VERSION_SUB1   (0x01) /*!< [23:16] sub1 version */
#define __STM32F4_DISCO_BSP_VERSION_SUB2   (0x05) /*!< [15:8]  sub2 version */
#define __STM32F4_DISCO_BSP_VERSION_RC     (0x00) /*!< [7:0]  release candidate */ 
#define __STM32F4_DISCO_BSP_VERSION         ((__STM32F4_DISCO_BSP_VERSION_MAIN << 24)\
                                             |(__STM32F4_DISCO_BSP_VERSION_SUB1 << 16)\
                                             |(__STM32F4_DISCO_BSP_VERSION_SUB2 << 8 )\
                                             |(__STM32F4_DISCO_BSP_VERSION_RC)) 
/**
  * @}
  */ 


/** @defgroup STM32F4_DISCOVERY_LOW_LEVEL_Private_Macros STM32F4 DISCOVERY LOW LEVEL Private Macros
  * @{
  */ 
/**
  * @}
  */ 

/** @defgroup STM32F4_DISCOVERY_LOW_LEVEL_Private_Variables STM32F4 DISCOVERY LOW LEVEL Private Variables
  * @{
  */ 
GPIO_TypeDef* GPIO_PORT[LEDn] = {LED4_GPIO_PORT, 
                                 LED3_GPIO_PORT, 
                                 LED5_GPIO_PORT,
                                 LED6_GPIO_PORT};
const uint16_t GPIO_PIN[LEDn] = {LED4_PIN, 
                                 LED3_PIN, 
                                 LED5_PIN,
                                 LED6_PIN};

const uint8_t BUTTON_IRQn[BUTTONn] = {KEY_BUTTON_EXTI_IRQn};

/**
  * @}
  */ 

/** @defgroup STM32F4_DISCOVERY_LOW_LEVEL_Private_FunctionPrototypes STM32F4 DISCOVERY LOW LEVEL Private FunctionPrototypes
  * @{
  */ 
/**
  * @}
  */ 

/** @defgroup STM32F4_DISCOVERY_LOW_LEVEL_Private_Functions STM32F4 DISCOVERY LOW LEVEL Private Functions
  * @{
  */ 

/**
  * @}
  */

/** @defgroup STM32F4_DISCOVERY_LOW_LEVEL_LED_Functions STM32F4 DISCOVERY LOW LEVEL LED Functions
  * @{
  */ 

/**
  * @brief  This method returns the STM32F4 DISCO BSP Driver revision
  * @retval version : 0xXYZR (8bits for each decimal, R for RC)
  */
uint32_t BSP_GetVersion(void)
{
  return __STM32F4_DISCO_BSP_VERSION;
}

/**
  * @brief  Configures LED GPIO.
  * @param  Led: Specifies the Led to be configured. 
  *   This parameter can be one of following parameters:
  *     @arg LED4
  *     @arg LED3
  *     @arg LED5
  *     @arg LED6
  */
void BSP_LED_Init(Led_TypeDef Led)
{
  GPIO_InitTypeDef  GPIO_InitStruct;
  
  /* Enable the GPIO_LED Clock */
  LEDx_GPIO_CLK_ENABLE(Led);

  /* Configure the GPIO_LED pin */
  GPIO_InitStruct.Pin = GPIO_PIN[Led];
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FAST;
  
  HAL_GPIO_Init(GPIO_PORT[Led], &GPIO_InitStruct);
  
  HAL_GPIO_WritePin(GPIO_PORT[Led], GPIO_PIN[Led], GPIO_PIN_RESET); 
}

/**
  * @brief  Turns selected LED On.
  * @param  Led: Specifies the Led to be set on. 
  *   This parameter can be one of following parameters:
  *     @arg LED4
  *     @arg LED3
  *     @arg LED5
  *     @arg LED6  
  */
void BSP_LED_On(Led_TypeDef Led)
{
  HAL_GPIO_WritePin(GPIO_PORT[Led], GPIO_PIN[Led], GPIO_PIN_SET); 
}

/**
  * @brief  Turns selected LED Off.
  * @param  Led: Specifies the Led to be set off. 
  *   This parameter can be one of following parameters:
  *     @arg LED4
  *     @arg LED3
  *     @arg LED5
  *     @arg LED6 
  */
void BSP_LED_Off(Led_TypeDef Led)
{
  HAL_GPIO_WritePin(GPIO_PORT[Led], GPIO_PIN[Led], GPIO_PIN_RESET); 
}

/**
  * @brief  Toggles the selected LED.
  * @param  Led: Specifies the Led to be toggled. 
  *   This parameter can be one of following parameters:
  *     @arg LED4
  *     @arg LED3
  *     @arg LED5
  *     @arg LED6  
  */
void BSP_LED_Toggle(Led_TypeDef Led)
{
  HAL_GPIO_TogglePin(GPIO_PORT[Led], GPIO_PIN[Led]);
}

/**
  * @}
  */ 

/** @defgroup STM32F4_DISCOVERY_LOW_LEVEL_BUTTON_Functions STM32F4 DISCOVERY LOW LEVEL BUTTON Functions
  * @{
  */ 
