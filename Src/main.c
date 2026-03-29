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
#include "FreeRTOS.h"
#include "task.h"

#include "stm32f4xx_ll_system.h"
#include "stm32f4xx_hal_exti.h"
#include "stm32f4_discovery.h"

/*-----------------------------------------------------------*/

/*
 * Set up the hardware ready to run this demo.
 */
static void prvSetupHardware( void );

void Task_LEDFlash_Callback()
{
    /* Queue a message for printing to say the task has started. */
    //vPrintDisplayMessage( &pcTaskStartMsg );

    for( ; ; )
    {
        /* Delay for half the flash period then turn the LED on. */
        vTaskDelay( (1000 / portTICK_PERIOD_MS) / ( TickType_t ) 2 );
        BSP_LED_Toggle(LED4);

        /* Delay for half the flash period then turn the LED off. */
        vTaskDelay( (1000 / portTICK_PERIOD_MS) / ( TickType_t ) 2 );
        BSP_LED_Toggle(LED4);;
    }
}

void Task_LEDFlash_StartTask()
{
    static StackType_t uxLEDTaskStack[ configMINIMAL_STACK_SIZE ];
    static StaticTask_t xLEDTaskTCB;

    xTaskCreateStatic( Task_LEDFlash_Callback,
    		           "LED flash task",
					   configMINIMAL_STACK_SIZE,
					   NULL,
					   4,
					   (StackType_t * ) &uxLEDTaskStack,
					   (StaticTask_t *) &xLEDTaskTCB );
}

void Task_LEDFlash_SetupHw()
{
    BSP_LED_Init(LED3);
    BSP_LED_Init(LED4);
    BSP_LED_Init(LED5);
    BSP_LED_Init(LED6);
}

int main( void )
{
    /* Configure the hardware ready to run the test. */
    prvSetupHardware();

    BSP_LED_On(LED4);
    BSP_LED_On(LED3);
    BSP_LED_On(LED5);
    BSP_LED_On(LED6);

    Task_LEDFlash_StartTask();

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
    /* Setup STM32 system (clock, PLL and Flash configuration) */
    SystemInit();

    /* Ensure all priority bits are assigned as preemption priority bits. */
    HAL_NVIC_SetPriorityGrouping( NVIC_PRIORITYGROUP_4 );

    Task_LEDFlash_SetupHw();
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
/*-----------------------------------------------------------*/
