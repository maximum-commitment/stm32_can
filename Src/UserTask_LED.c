/******************************************************************************
 *
 * main() creates all the demo application tasks and software timers, then starts
 * the scheduler. 
 *
 * This file was created based on demo application from FreeRTOS, but most of the
 * content was removed. It is a basic led flasher
 * 
 */

/* Kernel includes. */
#include <string.h>

#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_pwr.h"
#include "stm32f4xx_ll_system.h"
#include "stm32f4xx_hal_exti.h"

#include "FreeRTOS.h"
#include "task.h"

#include "UserTask_LED.h"

/*-----------------------------------------------------------*/

typedef enum 
{
  LED4_GREEN = 0,
  LED3_ORANGE = 1,
  LED5_RED = 2,
  LED6_BLUE = 3,
  MAX_LED
} Led_TypeDef;

/**
  * @}
  */ 

/** @defgroup STM32F4_DISCOVERY_LOW_LEVEL_Exported_Constants STM32F4 DISCOVERY LOW LEVEL Exported Constants
  * @{
  */ 

/** @defgroup STM32F4_DISCOVERY_LOW_LEVEL_LED STM32F4 DISCOVERY LOW LEVEL LED
  * @{
  */

#define LED4_PIN                         GPIO_PIN_12
#define LED4_GPIO_PORT                   GPIOD
#define LED4_GPIO_CLK_ENABLE()           __HAL_RCC_GPIOD_CLK_ENABLE()  
#define LED4_GPIO_CLK_DISABLE()          __HAL_RCC_GPIOD_CLK_DISABLE()  

#define LED3_PIN                         GPIO_PIN_13
#define LED3_GPIO_PORT                   GPIOD
#define LED3_GPIO_CLK_ENABLE()           __HAL_RCC_GPIOD_CLK_ENABLE()  
#define LED3_GPIO_CLK_DISABLE()          __HAL_RCC_GPIOD_CLK_DISABLE()  
  
#define LED5_PIN                         GPIO_PIN_14
#define LED5_GPIO_PORT                   GPIOD
#define LED5_GPIO_CLK_ENABLE()           __HAL_RCC_GPIOD_CLK_ENABLE()  
#define LED5_GPIO_CLK_DISABLE()          __HAL_RCC_GPIOD_CLK_DISABLE()  

#define LED6_PIN                         GPIO_PIN_15
#define LED6_GPIO_PORT                   GPIOD
#define LED6_GPIO_CLK_ENABLE()           __HAL_RCC_GPIOD_CLK_ENABLE()  
#define LED6_GPIO_CLK_DISABLE()          __HAL_RCC_GPIOD_CLK_DISABLE()  

#define LEDx_GPIO_CLK_ENABLE(__INDEX__) do{if((__INDEX__) == 0) LED4_GPIO_CLK_ENABLE(); else \
                                           if((__INDEX__) == 1) LED3_GPIO_CLK_ENABLE(); else \
                                           if((__INDEX__) == 2) LED5_GPIO_CLK_ENABLE(); else \
                                           if((__INDEX__) == 3) LED6_GPIO_CLK_ENABLE(); \
                                           }while(0)

#define LEDx_GPIO_CLK_DISABLE(__INDEX__) do{if((__INDEX__) == 0) LED4_GPIO_CLK_DISABLE(); else \
                                            if((__INDEX__) == 1) LED3_GPIO_CLK_DISABLE(); else \
                                            if((__INDEX__) == 2) LED5_GPIO_CLK_DISABLE(); else \
                                            if((__INDEX__) == 3) LED6_GPIO_CLK_DISABLE(); \
                                            }while(0)

GPIO_TypeDef* GPIO_PORT[MAX_LED] = {GPIOD, 
                                 GPIOD, 
                                 GPIOD,
                                 GPIOD};
 
const uint16_t GPIO_PIN[MAX_LED] = {GPIO_PIN_12, 
                                 GPIO_PIN_13, 
                                 GPIO_PIN_14,
                                 GPIO_PIN_15};


                                 /*
 * Set up the hardware ready to run this demo.
 */

/**
  * @brief  Turns selected LED On.
  * @param  Led: Specifies the Led to be set on. 
  *   This parameter can be one of following parameters:
  *     @arg LED4
  *     @arg LED3
  *     @arg LED5
  *     @arg LED6  
  */
static void prvUserTask_LED_On(Led_TypeDef Led)
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
static void prvUserTask_LED_Off(Led_TypeDef Led)
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
static void prvUserTask_LED_Toggle(Led_TypeDef Led)
{
    HAL_GPIO_TogglePin(GPIO_PORT[Led], GPIO_PIN[Led]);
}

static void prvUserTask_LED_Callback()
{
    prvUserTask_LED_On(LED6_BLUE);
    
    for( ; ; )
    {
        /* Delay for half the flash period then turn the LED on. */
        vTaskDelay( (1000 / portTICK_PERIOD_MS) / ( TickType_t ) 2 );
        prvUserTask_LED_Toggle(LED4_GREEN);

        /* Delay for half the flash period then turn the LED off. */
        vTaskDelay( (1000 / portTICK_PERIOD_MS) / ( TickType_t ) 2 );
        prvUserTask_LED_Toggle(LED4_GREEN);
    }
}

void UserTask_LED_Init()
{
    Led_TypeDef led;

    for(led = 0; led < MAX_LED; led++)
    {
        prvUserTask_LED_Off(led);
    }
}

void UserTask_LEDFlash_Start()
{
    static StackType_t uxLEDTaskStack[ configMINIMAL_STACK_SIZE ];
    static StaticTask_t xLEDTaskTCB;

    xTaskCreateStatic( prvUserTask_LED_Callback,
                   "LED flash task",
             configMINIMAL_STACK_SIZE,
             NULL,
             4,
             (StackType_t * ) &uxLEDTaskStack,
             (StaticTask_t *) &xLEDTaskTCB );
}

