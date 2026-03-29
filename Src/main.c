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
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */



/* Private function prototypes -----------------------------------------------*/
static void SystemClock_Config(void);

uint32_t Cnt_HAL_CAN_RxFifo1FullCallback = 0;
uint32_t Cnt_HAL_CAN_RxFifo1MsgPendingCallback = 0;
uint32_t Cnt_HAL_CAN_RxFifo0FullCallback = 0;
uint32_t Cnt_HAL_CAN_RxFifo0MsgPendingCallback = 0;
uint32_t Cnt_HAL_CAN_TxMailbox2CompleteCallback = 0;
uint32_t Cnt_HAL_CAN_TxMailbox1CompleteCallback = 0;
uint32_t Cnt_HAL_CAN_TxMailbox0CompleteCallback = 0;

/* Private variables ---------------------------------------------------------*/
CAN_HandleTypeDef hcan1;

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_CAN1_Init(void);
/* USER CODE BEGIN PFP */
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


uint8_t magic = 0x55;
void CAN_Send_DemoMessage()
{
  const CAN_TxHeaderTypeDef TxHeader = {
      .StdId=0x555U,
      .ExtId=0x0U,
      .IDE=CAN_ID_STD,
      .RTR = 0x0U,
      .DLC = 0x8U,
      .TransmitGlobalTime = DISABLE};

  #define CAN_TXDATA_BUFFER_SIZE 32
  uint8_t TxData[CAN_TXDATA_BUFFER_SIZE];
  memset(TxData, magic, CAN_TXDATA_BUFFER_SIZE);
  uint32_t TxMailbox = 0;

  HAL_StatusTypeDef can_status = HAL_CAN_AddTxMessage( &hcan1, 
                                                       &TxHeader,
                                                       (const uint8_t *) &TxData, 
                                                       &TxMailbox);
}
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */
uint32_t RxData[32];
HAL_StatusTypeDef rx_status = 0;
/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_CAN1_Init();
  /* USER CODE BEGIN 2 */

const CAN_FilterTypeDef myFlt =
{
  .FilterIdHigh = 0x0000,          /*!< Specifies the filter identification number (MSBs for a 32-bit
                                       configuration, first one for a 16-bit configuration).
                                       This parameter must be a number between
                                       Min_Data = 0x0000 and Max_Data = 0xFFFF. */

  .FilterIdLow = 0x0000,           /*!< Specifies the filter identification number (LSBs for a 32-bit
                                       configuration, second one for a 16-bit configuration).
                                       This parameter must be a number between
                                       Min_Data = 0x0000 and Max_Data = 0xFFFF. */

  .FilterMaskIdHigh = 0x0000,      /*!< Specifies the filter mask number or identification number,
                                       according to the mode (MSBs for a 32-bit configuration,
                                       first one for a 16-bit configuration).
                                       This parameter must be a number between
                                       Min_Data = 0x0000 and Max_Data = 0xFFFF. */

  .FilterMaskIdLow = 0x0000,       /*!< Specifies the filter mask number or identification number,
                                       according to the mode (LSBs for a 32-bit configuration,
                                       second one for a 16-bit configuration).
                                       This parameter must be a number between
                                       Min_Data = 0x0000 and Max_Data = 0xFFFF. */

  .FilterFIFOAssignment = CAN_RX_FIFO0,  /*!< Specifies the FIFO (0 or 1U) which will be assigned to the filter.
                                       This parameter can be a value of @ref CAN_filter_FIFO */

  .FilterBank = 0,            /*!< Specifies the filter bank which will be initialized.
                                       For single CAN instance(14 dedicated filter banks),
                                       this parameter must be a number between Min_Data = 0 and Max_Data = 13.
                                       For dual CAN instances(28 filter banks shared),
                                       this parameter must be a number between Min_Data = 0 and Max_Data = 27. */

  .FilterMode = CAN_FILTERMODE_IDMASK,            /*!< Specifies the filter mode to be initialized.
                                       This parameter can be a value of @ref CAN_filter_mode */

  .FilterScale = CAN_FILTERSCALE_32BIT,          /*!< Specifies the filter scale.
                                       This parameter can be a value of @ref CAN_filter_scale */

  .FilterActivation = ENABLE,     /*!< Enable or disable the filter.
                                       This parameter can be a value of @ref CAN_filter_activation */

  .SlaveStartFilterBank = 0  /*!< Select the start filter bank for the slave CAN instance.
                                       For single CAN instances, this parameter is meaningless.
                                       For dual CAN instances, all filter banks with lower index are assigned to master
                                       CAN instance, whereas all filter banks with greater index are assigned to slave
                                       CAN instance.
                                       This parameter must be a number between Min_Data = 0 and Max_Data = 27. */

};

  HAL_CAN_ConfigFilter(&hcan1, &myFlt);
  HAL_CAN_Start(&hcan1);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */
    /*if(HAL_CAN_GetTxMai lboxesFreeLevel(&hcan1)!=0)
    { CAN_Send_DemoMessage(); }*/
    HAL_Delay(50);

    
  CAN_RxHeaderTypeDef RxHeader = 
{
  .StdId = 0x555,    /*!< Specifies the standard identifier.
                          This parameter must be a number between Min_Data = 0 and Max_Data = 0x7FF. */

  .ExtId = 0,    /*!< Specifies the extended identifier.
                          This parameter must be a number between Min_Data = 0 and Max_Data = 0x1FFFFFFF. */

  .IDE = CAN_ID_STD,      /*!< Specifies the type of identifier for the message that will be transmitted.
                          This parameter can be a value of @ref CAN_identifier_type */

  .RTR = 0,      /*!< Specifies the type of frame for the message that will be transmitted.
                          This parameter can be a value of @ref CAN_remote_transmission_request */

  .DLC = 8,      /*!< Specifies the length of the frame that will be transmitted.
                          This parameter must be a number between Min_Data = 0 and Max_Data = 8. */

  .Timestamp = 0xFFFF, /*!< Specifies the timestamp counter value captured on start of frame reception.
                          @note: Time Triggered Communication Mode must be enabled.
                          This parameter must be a number between Min_Data = 0 and Max_Data = 0xFFFF. */

  .FilterMatchIndex = 0 /*!< Specifies the index of matching acceptance filter element.
                          This parameter must be a number between Min_Data = 0 and Max_Data = 0xFF. */

};
    
    if(HAL_CAN_GetRxFifoFillLevel(&hcan1, 0) > 0)
    { 
      rx_status =  HAL_CAN_GetRxMessage(&hcan1, 0, &RxHeader, RxData); 
    }
    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 8;
  RCC_OscInitStruct.PLL.PLLN = 336;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 7;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief CAN1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_CAN1_Init(void)
{

  /* USER CODE BEGIN CAN1_Init 0 */

  /* USER CODE END CAN1_Init 0 */

  /* USER CODE BEGIN CAN1_Init 1 */

  /* USER CODE END CAN1_Init 1 */
  hcan1.Instance = CAN1;
  hcan1.Init.Prescaler = 14;
  hcan1.Init.Mode = CAN_MODE_NORMAL;
  hcan1.Init.SyncJumpWidth = CAN_SJW_2TQ;
  hcan1.Init.TimeSeg1 = CAN_BS1_2TQ;
  hcan1.Init.TimeSeg2 = CAN_BS2_3TQ;
  hcan1.Init.TimeTriggeredMode = DISABLE;
  hcan1.Init.AutoBusOff = DISABLE;
  hcan1.Init.AutoWakeUp = DISABLE;
  hcan1.Init.AutoRetransmission = DISABLE;
  hcan1.Init.ReceiveFifoLocked = DISABLE;
  hcan1.Init.TransmitFifoPriority = DISABLE;
  if (HAL_CAN_Init(&hcan1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN CAN1_Init 2 */

  /* USER CODE END CAN1_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  /* USER CODE BEGIN MX_GPIO_Init_1 */

  /* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOD, LD4_Pin|LD3_Pin|LD5_Pin|LD6_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin : PDM_OUT_Pin */
  GPIO_InitStruct.Pin = PDM_OUT_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.Alternate = GPIO_AF5_SPI2;
  HAL_GPIO_Init(PDM_OUT_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : BOOT1_Pin */
  GPIO_InitStruct.Pin = BOOT1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(BOOT1_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : LD4_Pin LD3_Pin LD5_Pin LD6_Pin */
  GPIO_InitStruct.Pin = LD4_Pin|LD3_Pin|LD5_Pin|LD6_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

  /* USER CODE BEGIN MX_GPIO_Init_2 */

  /* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}
#ifdef USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
