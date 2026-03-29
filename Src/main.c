/**
  ******************************************************************************
  * @file    Demonstrations/Src/main.c 
  * @author  MCD Application Team
  * @brief   This demo describes how to use accelerometer to control mouse on 
  *          PC.
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
#include <string.h>
#include "main.h"
#include "stm32f4xx_hal_pwr.h"

/** @addtogroup STM32F4xx_HAL_Demonstrations
  * @{
  */

/** @addtogroup Demo
  * @{
  */ 

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define KEY_PRESSED     0x01
#define KEY_NOT_PRESSED 0x00

/* TIM4 Autoreload and Capture Compare register values */
#define TIM_ARR        (uint16_t)1999
#define TIM_CCR        (uint16_t)1000

#define CURSOR_STEP     7

/* Private macro -------------------------------------------------------------*/
#define ABS(x)         (x < 0) ? (-x) : x
#define MAX_AB(a,b)       (a < b) ? (b) : a

/* Private variables ---------------------------------------------------------*/
__IO uint8_t SleepModeRequest = 0x00;
__IO uint8_t DemoEnterCondition = 0x00;

/* Variables used for accelerometer */
__IO int16_t X_Offset, Y_Offset;
int16_t Buffer[3];

/* Variables used for timer */
uint16_t PrescalerValue = 0;
TIM_HandleTypeDef htim4;
TIM_OC_InitTypeDef sConfigTim4;

/* Variables used during Systick ISR*/
uint8_t Counter  = 0x00;
__IO uint16_t MaxAcceleration = 0;
volatile uint32_t togglecounter = 0x00;

/* Private function prototypes -----------------------------------------------*/
static void Demo_Exec(void);
static void SystemClock_Config(void);
static void Error_Handler(void);

CAN_HandleTypeDef hcan;

uint32_t Cnt_HAL_CAN_RxFifo1FullCallback = 0;
uint32_t Cnt_HAL_CAN_RxFifo1MsgPendingCallback = 0;
uint32_t Cnt_HAL_CAN_RxFifo0FullCallback = 0;
uint32_t Cnt_HAL_CAN_RxFifo0MsgPendingCallback = 0;
uint32_t Cnt_HAL_CAN_TxMailbox2CompleteCallback = 0;
uint32_t Cnt_HAL_CAN_TxMailbox1CompleteCallback = 0;
uint32_t Cnt_HAL_CAN_TxMailbox0CompleteCallback = 0;

/* Private functions ---------------------------------------------------------*/
void HAL_CAN_TxMailbox0CompleteCallback(CAN_HandleTypeDef *hcan)
{
  if(Cnt_HAL_CAN_TxMailbox0CompleteCallback < UINT32_MAX)
  {
    Cnt_HAL_CAN_TxMailbox0CompleteCallback++;
  }
  else
  {
    Cnt_HAL_CAN_TxMailbox0CompleteCallback = 0;
  }
}
void HAL_CAN_TxMailbox1CompleteCallback(CAN_HandleTypeDef *hcan)
{
  if(Cnt_HAL_CAN_TxMailbox1CompleteCallback < UINT32_MAX)
  {
    Cnt_HAL_CAN_TxMailbox1CompleteCallback++;
  }
  else
  {
    Cnt_HAL_CAN_TxMailbox1CompleteCallback = 0;
  }
}
void HAL_CAN_TxMailbox2CompleteCallback(CAN_HandleTypeDef *hcan)
{
  if(Cnt_HAL_CAN_TxMailbox2CompleteCallback < UINT32_MAX)
  {
    Cnt_HAL_CAN_TxMailbox2CompleteCallback++;
  }
  else
  {
    Cnt_HAL_CAN_TxMailbox2CompleteCallback = 0;
  }
}
void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan)
{
  if(Cnt_HAL_CAN_RxFifo0MsgPendingCallback < UINT32_MAX)
  {
    Cnt_HAL_CAN_RxFifo0MsgPendingCallback++;
  }
  else
  {
    Cnt_HAL_CAN_RxFifo0MsgPendingCallback = 0;
  }
}
void HAL_CAN_RxFifo0FullCallback(CAN_HandleTypeDef *hcan)
{
  if(Cnt_HAL_CAN_RxFifo0FullCallback < UINT32_MAX)
  {
    Cnt_HAL_CAN_RxFifo0FullCallback++;
  }
  else
  {
    Cnt_HAL_CAN_RxFifo0FullCallback = 0;
  }
}
void HAL_CAN_RxFifo1MsgPendingCallback(CAN_HandleTypeDef *hcan)
{
  if(Cnt_HAL_CAN_RxFifo1MsgPendingCallback < UINT32_MAX)
  {
    Cnt_HAL_CAN_RxFifo1MsgPendingCallback++;
  }
  else
  {
    Cnt_HAL_CAN_RxFifo1MsgPendingCallback = 0;
  }
}
void HAL_CAN_RxFifo1FullCallback(CAN_HandleTypeDef *hcan)
{
  if(Cnt_HAL_CAN_RxFifo1FullCallback < UINT32_MAX)
  {
    Cnt_HAL_CAN_RxFifo1FullCallback++;
  }
  else
  {
    Cnt_HAL_CAN_RxFifo1FullCallback = 0;
  }
}

void CAN_Send_DemoMessage()
{
  const CAN_TxHeaderTypeDef TxHeader = {
      .StdId=0x1U,
      .ExtId=0x0U,
      .IDE=CAN_ID_STD,
      .RTR = 0x0U,
      .DLC = 0x8U,
      .TransmitGlobalTime = DISABLE};

  #define CAN_TXDATA_BUFFER_SIZE 32
  uint8_t TxData[CAN_TXDATA_BUFFER_SIZE];
  memset(TxData, 0, CAN_TXDATA_BUFFER_SIZE);
  uint32_t TxMailbox = 0;

  HAL_StatusTypeDef can_status = HAL_CAN_AddTxMessage( &hcan, 
                                                       &TxHeader,
                                                       (const uint8_t *) &TxData, 
                                                       &TxMailbox);
}

/**
  * @brief  Main program
  * @param  None
  * @retval None
  */
int main(void)
{
  /* STM32F4xx HAL library initialization:
       - Configure the Flash prefetch, instruction and Data caches
       - Configure the Systick to generate an interrupt each 1 msec
       - Set NVIC Group Priority to 4
       - Global MSP (MCU Support Package) initialization
     */
  HAL_Init();

  HAL_CAN_Init(&hcan);
  HAL_CAN_Start(&hcan);

  /* Configure LED3, LED4, LED5 and LED6 */
  BSP_LED_Init(LED3);
  BSP_LED_Init(LED4);
  BSP_LED_Init(LED5);
  BSP_LED_Init(LED6);

  /* Configure the system clock to 168 MHz */
  SystemClock_Config();

  /* Configure USER Button */
  BSP_PB_Init(BUTTON_KEY, BUTTON_MODE_EXTI); 

  /* Execute Demo application */
  Demo_Exec();
  
  /* Infinite loop */
  while (1)
  {    
  }
}

/**
  * @brief  Execute the demo application.
  * @param  None
  * @retval None
  */
static void Demo_Exec(void)
{

  
  /* Initialize Accelerometer MEMS */
  if(BSP_ACCELERO_Init() != HAL_OK)
  {
    /* Initialization Error */
    Error_Handler(); 
  }

  /* Reset UserButton_Pressed variable */
  SleepModeRequest = 0x00;

  /* SysTick end of count event each 10ms */
  SystemCoreClock = HAL_RCC_GetHCLKFreq();
  SysTick_Config(SystemCoreClock / 100);
  BSP_LED_Off(LED4);
  BSP_LED_Off(LED3);
  BSP_LED_Off(LED5);
  BSP_LED_Off(LED6);

  while(1)
  {
    if (togglecounter == 100)
    {
      togglecounter = 0x00;
      BSP_LED_Toggle(LED4);
      BSP_LED_Toggle(LED3);
      BSP_LED_Toggle(LED5);
    }
    /* Waiting USER Button is Released */
    if(SleepModeRequest == 0x01)
    {
      while (BSP_PB_GetState(BUTTON_KEY) != KEY_NOT_PRESSED)
      {}
      HAL_SuspendTick();
      HAL_PWR_EnterSLEEPMode(0,PWR_SLEEPENTRY_WFI);
      // Can be used for deeper sleep, but couldn't get to work (most pins hiz)
      // HAL_PWR_EnterSTANDBYMode();
    }
  }
}

/**
  * @brief  Configures the TIM Peripheral.
  * @param  None
  * @retval None
  */
static void TIM4_Config(void)
{
  /* -----------------------------------------------------------------------
  TIM4 Configuration: Output Compare Timing Mode:
  
  In this example TIM4 input clock (TIM4CLK) is set to 2 * APB1 clock (PCLK1), 
  since APB1 prescaler is different from 1 (APB1 Prescaler = 4, see system_stm32f4xx.c file).
  TIM4CLK = 2 * PCLK1  
  PCLK1 = HCLK / 4 
  => TIM4CLK = 2*(HCLK / 4) = HCLK/2 = SystemCoreClock/2
  
  To get TIM4 counter clock at 2 KHz, the prescaler is computed as follows:
  Prescaler = (TIM4CLK / TIM4 counter clock) - 1
  Prescaler = (84 MHz/(2 * 2 KHz)) - 1 = 41999
  
  To get TIM4 output clock at 1 Hz, the period (ARR)) is computed as follows:
  ARR = (TIM4 counter clock / TIM4 output clock) - 1
  = 1999
  
  TIM4 Channel1 duty cycle = (TIM4_CCR1/ TIM4_ARR)* 100 = 50%
  TIM4 Channel2 duty cycle = (TIM4_CCR2/ TIM4_ARR)* 100 = 50%
  TIM4 Channel3 duty cycle = (TIM4_CCR3/ TIM4_ARR)* 100 = 50%
  TIM4 Channel4 duty cycle = (TIM4_CCR4/ TIM4_ARR)* 100 = 50%
  
  ==> TIM4_CCRx = TIM4_ARR/2 = 1000  (where x = 1, 2, 3 and 4).
  ----------------------------------------------------------------------- */ 
  
  /* Compute the prescaler value */
  PrescalerValue = (uint16_t) ((SystemCoreClock /2) / 2000) - 1;
  
  /* Time base configuration */
  htim4.Instance             = TIM4;
  htim4.Init.Period          = TIM_ARR;
  htim4.Init.Prescaler       = PrescalerValue;
  htim4.Init.ClockDivision   = 0;
  htim4.Init.CounterMode     = TIM_COUNTERMODE_UP;
  if(HAL_TIM_PWM_Init(&htim4) != HAL_OK)
  {
    /* Initialization Error */
    Error_Handler();
  }

  /* TIM PWM1 Mode configuration: Channel */
  /* Output Compare Timing Mode configuration: Channel1 */
  sConfigTim4.OCMode = TIM_OCMODE_PWM1;
  sConfigTim4.OCIdleState = TIM_CCx_ENABLE;
  sConfigTim4.Pulse = TIM_CCR;
  sConfigTim4.OCPolarity = TIM_OCPOLARITY_HIGH;
  
  /* Output Compare PWM1 Mode configuration: Channel1 */
  if(HAL_TIM_PWM_ConfigChannel(&htim4, &sConfigTim4, TIM_CHANNEL_1) != HAL_OK)
  {
    /* Initialization Error */
    Error_Handler();
  }

  /* Output Compare PWM1 Mode configuration: Channel2 */
  if(HAL_TIM_PWM_ConfigChannel(&htim4, &sConfigTim4, TIM_CHANNEL_2) != HAL_OK)
  {
    /* Initialization Error */
    Error_Handler();
  }

  /* Output Compare PWM1 Mode configuration: Channel3 */
  if(HAL_TIM_PWM_ConfigChannel(&htim4, &sConfigTim4, TIM_CHANNEL_3) != HAL_OK)
  {
    /* Initialization Error */
    Error_Handler();
  }
  /* Output Compare PWM1 Mode configuration: Channel4 */
  if(HAL_TIM_PWM_ConfigChannel(&htim4, &sConfigTim4, TIM_CHANNEL_4) != HAL_OK)
  {
    /* Initialization Error */
    Error_Handler();
  }
}

/**
  * @brief  SYSTICK callback.
  * @param  None
  * @retval None
  */
void HAL_SYSTICK_Callback(void)
{
  togglecounter++;
}

/**
  * @brief  EXTI line detection callbacks.
  * @param  GPIO_Pin: Specifies the pins connected EXTI line
  * @retval None
  */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
  if(GPIO_Pin == KEY_BUTTON_PIN) 
  {
    if(SleepModeRequest == 0x01)
    {
      HAL_ResumeTick();
      /* Waiting USER Button is Released */
      while (BSP_PB_GetState(BUTTON_KEY) != KEY_NOT_PRESSED)
      {}
      SleepModeRequest = 0x00;
    }
    else
    {
      SleepModeRequest = 0x01;
    }
  }
}

/**
  * @brief  This function is executed in case of error occurrence.
  * @param  None
  * @retval None
  */
static void Error_Handler(void)
{
  /* Turn LED4 on */
  BSP_LED_On(LED4);
  while(1)
  {
  }
}

/**
  * @brief  System Clock Configuration
  *         The system Clock is configured as follow : 
  *            System Clock source            = PLL (HSE)
  *            SYSCLK(Hz)                     = 168000000
  *            HCLK(Hz)                       = 168000000
  *            AHB Prescaler                  = 1
  *            APB1 Prescaler                 = 4
  *            APB2 Prescaler                 = 2
  *            HSE Frequency(Hz)              = 8000000
  *            PLL_M                          = 8
  *            PLL_N                          = 336
  *            PLL_P                          = 2
  *            PLL_Q                          = 7
  *            VDD(V)                         = 3.3
  *            Main regulator output voltage  = Scale1 mode
  *            Flash Latency(WS)              = 5
  * @param  None
  * @retval None
  */
static void SystemClock_Config(void)
{
  RCC_ClkInitTypeDef RCC_ClkInitStruct;
  RCC_OscInitTypeDef RCC_OscInitStruct;

  /* Enable Power Control clock */
  __HAL_RCC_PWR_CLK_ENABLE();
  
  /* The voltage scaling allows optimizing the power consumption when the device is 
     clocked below the maximum system frequency, to update the voltage scaling value 
     regarding system frequency refer to product datasheet.  */
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
  
  /* Enable HSE Oscillator and activate PLL with HSE as source */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 8;
  RCC_OscInitStruct.PLL.PLLN = 336;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 7;
  HAL_RCC_OscConfig(&RCC_OscInitStruct);
  
  /* Select PLL as system clock source and configure the HCLK, PCLK1 and PCLK2 
     clocks dividers */
  RCC_ClkInitStruct.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2);
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;  
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;  
  HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5);

  /* STM32F405x/407x/415x/417x Revision Z and upper devices: prefetch is supported  */
  if (HAL_GetREVID() >= 0x1001)
  {
    /* Enable the Flash prefetch */
    __HAL_FLASH_PREFETCH_BUFFER_ENABLE();
  }
}

#ifdef  USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}
#endif

/**
  * @}
  */

/**
  * @}
  */
