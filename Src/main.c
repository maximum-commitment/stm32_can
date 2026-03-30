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
#include "main.h"

#include "FreeRTOS.h"
#include "task.h"

#include "stm32f4xx_hal_pwr.h"
#include "stm32f4xx_ll_system.h"
#include "stm32f4xx_hal_exti.h"

/*-----------------------------------------------------------*/

/*
 * Set up the hardware ready to run this demo.
 */
static void prvSetupHardware( void );
static void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_CAN1_Init(void);

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
    HAL_StatusTypeDef   Hal_Status;
    const CAN_TxHeaderTypeDef TxHeader = {
        .StdId=0x555U,
        .ExtId=0x0U,
        .IDE=CAN_ID_STD,
        .RTR = 0x0U,
        .DLC = 0x8U,
        .TransmitGlobalTime = DISABLE};

    uint32_t TxMailbox = 0;

    Hal_Status = HAL_CAN_AddTxMessage( &hcan1,
                                       &TxHeader,
                                       (const uint8_t *) buf,
                                       &TxMailbox );
    configASSERT(Hal_Status == HAL_OK);
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

/**
  * @brief CAN1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_CAN1_Init(void)
{
    HAL_StatusTypeDef Hal_Status;

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
    Hal_Status = HAL_CAN_Init(&hcan1);
    configASSERT(Hal_Status == HAL_OK);
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

    /*Configure GPIO pins : LD4_Pin LD3_Pin LD5_Pin LD6_Pin */
    GPIO_InitStruct.Pin = LD4_Pin|LD3_Pin|LD5_Pin|LD6_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

    /* USER CODE BEGIN MX_GPIO_Init_2 */

    /* USER CODE END MX_GPIO_Init_2 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
    RCC_OscInitTypeDef RCC_OscInitStruct = {0};
    RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
    HAL_StatusTypeDef Hal_Status;

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
    Hal_Status = HAL_RCC_OscConfig(&RCC_OscInitStruct);
    configASSERT(Hal_Status == HAL_OK);

    /** Initializes the CPU, AHB and APB buses clocks
     */
    RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                                |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
    RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
    RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
    RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;
    Hal_Status = HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5);
    configASSERT(Hal_Status == HAL_OK);
}

int main( void )
{
    /* Configure the hardware ready to run the test. */
    prvSetupHardware();

    UserTask_LEDFlash_Start();

    vTaskStartScheduler();

    /* If all is well, the scheduler will now be running, and the following line
     * will never be reached.  If the following line does execute, then there was
     * insufficient FreeRTOS heap memory available for the idle and/or timer tasks
     * to be created.  See the memory management section on the FreeRTOS web site
     * for more details. */
    for( ; ; )
    {
    }
}

/* configUSE_STATIC_ALLOCATION is set to 1, so the application must provide an
implementation of vApplicationGetIdleTaskMemory() to provide the memory that is
used by the Idle task. */
void vApplicationGetIdleTaskMemory( StaticTask_t **ppxIdleTaskTCBBuffer, StackType_t **ppxIdleTaskStackBuffer, uint32_t *pulIdleTaskStackSize )
{
    /* example implementation shown here */
    /* If the buffers to be provided to the Idle task are declared inside this
    function then they must be declared static - otherwise they will be allocated on
    the stack and so not exists after this function exits. */
    static StaticTask_t xIdleTaskTCB;
    static StackType_t uxIdleTaskStack[ configMINIMAL_STACK_SIZE ];

    /* Pass out a pointer to the StaticTask_t structure in which the Idle task's
    state will be stored. */
    *ppxIdleTaskTCBBuffer = &xIdleTaskTCB;

    /* Pass out the array that will be used as the Idle task's stack. */
    *ppxIdleTaskStackBuffer = uxIdleTaskStack;

    /* Pass out the size of the array pointed to by *ppxIdleTaskStackBuffer.
    Note that, as the array is necessarily of type StackType_t,
    configMINIMAL_STACK_SIZE is specified in words, not bytes. */
    *pulIdleTaskStackSize = configMINIMAL_STACK_SIZE;
}

/* configUSE_STATIC_ALLOCATION and configUSE_TIMERS are both set to 1, so the
 * application must provide an implementation of vApplicationGetTimerTaskMemory()
 * to provide the memory that is used by the Timer service task. */
void vApplicationGetTimerTaskMemory( StaticTask_t ** ppxTimerTaskTCBBuffer,
                                     StackType_t ** ppxTimerTaskStackBuffer,
                                     uint32_t * pulTimerTaskStackSize )
{
static StaticTask_t xTimerTaskTCB;
static StackType_t uxTimerTaskStack[ configMINIMAL_STACK_SIZE ];

  /* Pass out a pointer to the StaticTask_t structure in which the Idle task's
  state will be stored. */
  *ppxTimerTaskTCBBuffer = &xTimerTaskTCB;

  /* Pass out the array that will be used as the Idle task's stack. */
  *ppxTimerTaskStackBuffer = uxTimerTaskStack;

  /* Pass out the size of the array pointed to by *ppxTimerTaskStackBuffer.
  Note that, as the array is necessarily of type StackType_t,
  configMINIMAL_STACK_SIZE is specified in words, not bytes. */
  *pulTimerTaskStackSize = configMINIMAL_STACK_SIZE;
}

static void prvSetupHardware( void )
{
    HAL_StatusTypeDef   Hal_Status;

    /* Setup STM32 system (clock, PLL and Flash configuration) */
    SystemInit();

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

    /* Ensure all priority bits are assigned as preemption priority bits. */
    HAL_NVIC_SetPriorityGrouping( NVIC_PRIORITYGROUP_4 );

    //Task_LEDFlash_SetupHw();


    Hal_Status = HAL_CAN_ConfigFilter(&hcan1, &DemoRxFilter);
    configASSERT(Hal_Status == HAL_OK);

    Hal_Status = HAL_CAN_Start(&hcan1);
    configASSERT(Hal_Status == HAL_OK);
}
/*-----------------------------------------------------------*/

void vApplicationTickHook( void )
{
}

void EXTI9_5_IRQHandler( void )
{
    long lHigherPriorityTaskWoken = pdFALSE;

    /* Only line 6 is enabled, so there is no need to test which line generated
     * the interrupt. */
    /* Note trigger param appears to be unused by driver, perhaps will be in future update */
    /* Note callback pointer also unused, but this will likely be permanent */
    EXTI_HandleTypeDef hexti = {.Line = LL_SYSCFG_EXTI_LINE6, .PendingCallback = NULL};
    HAL_EXTI_ClearPending(&hexti, EXTI_TRIGGER_NONE /* param unused */);

#ifdef COMPILEOUT_INITIAL_TEST_APP
    /* This interrupt does nothing more than demonstrate how to synchronise a
     * task with an interrupt.  First the handler releases a semaphore.
     * lHigherPriorityTaskWoken has been initialised to zero. */
    xSemaphoreGiveFromISR( xTestSemaphore, &lHigherPriorityTaskWoken );
#endif

    /* If there was a task that was blocked on the semaphore, and giving the
     * semaphore caused the task to unblock, and the unblocked task has a priority
     * higher than the currently executing task (the task that this interrupt
     * interrupted), then lHigherPriorityTaskWoken will have been set to pdTRUE.
     * Passing pdTRUE into the following macro call will cause this interrupt to
     * return directly to the unblocked, higher priority, task. */
    portEND_SWITCHING_ISR( lHigherPriorityTaskWoken );
}

void vApplicationIdleHook( void )
{
    /* vApplicationIdleHook() will only be called if configUSE_IDLE_HOOK is set
     * to 1 in FreeRTOSConfig.h.  It will be called on each iteration of the idle
     * task.  It is essential that code added to this hook function never attempts
     * to block in any way (for example, call xQueueReceive() with a block time
     * specified, or call vTaskDelay()).  If the application makes use of the
     * vTaskDelete() API function (as this demo application does) then it is also
     * important that vApplicationIdleHook() is permitted to return to its calling
     * function, because it is the responsibility of the idle task to clean up
     * memory allocated by the kernel to any task that has since been deleted. */
}
/*-----------------------------------------------------------*/

void vApplicationStackOverflowHook( TaskHandle_t pxTask,
                                    char * pcTaskName )
{
    ( void ) pcTaskName;
    ( void ) pxTask;

    /* Run time stack overflow checking is performed if
     * configCHECK_FOR_STACK_OVERFLOW is defined to 1 or 2.  This hook
     * function is called if a stack overflow is detected. */
    taskDISABLE_INTERRUPTS();

    for( ; ; )
    {
    }
}
