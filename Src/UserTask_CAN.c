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
#include "stm32f4xx_hal_can.h"
#include "FreeRTOS.h"
#include "task.h"

#include "UserTask_CAN.h"

CAN_HandleTypeDef hcan1;

static const CAN_FilterTypeDef DemoRxFilter =
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

CAN_RxHeaderTypeDef DemoRxHeader = 
{
  .StdId = 0x554,    /*!< Specifies the standard identifier.
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

uint8_t DemoRxMessage[8];

static void CAN_Send_Message(const uint8_t *buf)
{
    HAL_StatusTypeDef   HAL_Status;
    const CAN_TxHeaderTypeDef TxHeader = {
        .StdId=0x555U,
        .ExtId=0x0U,
        .IDE=CAN_ID_STD,
        .RTR = 0x0U,
        .DLC = 0x8U,
        .TransmitGlobalTime = DISABLE};

    uint32_t TxMailbox = 0;

    HAL_Status = HAL_CAN_AddTxMessage( &hcan1,
                                       &TxHeader,
                                       (const uint8_t *) buf,
                                       &TxMailbox );
    configASSERT(HAL_Status == HAL_OK);
}

static void CAN_LoopBackDemo()
{
  if(HAL_CAN_GetRxFifoFillLevel(&hcan1, 0) > 0)
  { 
    (void) HAL_CAN_GetRxMessage(&hcan1, 0, &DemoRxHeader, DemoRxMessage);
    if(HAL_CAN_GetTxMailboxesFreeLevel(&hcan1)!=0)
    {
      CAN_Send_Message(DemoRxMessage);
    }
  }
}

static void pUserTask_CAN_Callback()
{   
    for( ; ; )
    {
        CAN_LoopBackDemo();

        vTaskDelay( (5 / portTICK_PERIOD_MS) );
    }
}

void UserTask_CAN_Start()
{
    static StackType_t uxLEDTaskStack[ configMINIMAL_STACK_SIZE ];
    static StaticTask_t xLEDTaskTCB;
    HAL_StatusTypeDef HAL_Status;

    HAL_Status = HAL_CAN_ConfigFilter(&hcan1, &DemoRxFilter);
    configASSERT(HAL_Status == HAL_OK);

    HAL_Status = HAL_CAN_Start(&hcan1);
    configASSERT(HAL_Status == HAL_OK);

    xTaskCreateStatic( pUserTask_CAN_Callback,
                   "CAN loopback task",
             configMINIMAL_STACK_SIZE,
             NULL,
             4,
             (StackType_t * ) &uxLEDTaskStack,
             (StaticTask_t *) &xLEDTaskTCB );
}

void UserTask_CAN_Init()
{
    HAL_StatusTypeDef HAL_Status;

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
    HAL_Status = HAL_CAN_Init(&hcan1);
    configASSERT(HAL_Status == HAL_OK);
    /* USER CODE BEGIN CAN1_Init 2 */

    /* USER CODE END CAN1_Init 2 */
}