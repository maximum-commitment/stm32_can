/*
 * FreeRTOS V202212.00
 * Copyright (C) 2020 Amazon.com, Inc. or its affiliates. All Rights Reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 * the Software, and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 * https://www.FreeRTOS.org
 * https://github.com/FreeRTOS
 *
 */

/******************************************************************************
 * >>>>>> NOTE 1: <<<<<<
 *
 * main() can be configured to create either a very simple LED flasher demo, or
 * a more comprehensive test/demo application.
 *
 * To create a very simple LED flasher example, set the
 * mainCREATE_SIMPLE_LED_FLASHER_DEMO_ONLY constant (defined below) to 1.  When
 * this is done, only the standard demo flash tasks are created.  The standard
 * demo flash example creates three tasks, each of which toggle an LED at a
 * fixed but different frequency.
 *
 * To create a more comprehensive test and demo application, set
 * mainCREATE_SIMPLE_LED_FLASHER_DEMO_ONLY to 0.
 *
 * >>>>>> NOTE 2: <<<<<<
 *
 * In addition to the normal set of standard demo tasks, the comprehensive test
 * makes heavy use of the floating point unit, and forces floating point
 * instructions to be used from interrupts that nest three deep.  The nesting
 * starts from the tick hook function, resulting is an abnormally long context
 * switch time.  This is done purely to stress test the FPU context switching
 * implementation, and that part of the test can be removed by setting
 * configUSE_TICK_HOOK to 0 in FreeRTOSConfig.h.
 ******************************************************************************
 *
 * main() creates all the demo application tasks and software timers, then starts
 * the scheduler.  The web documentation provides more details of the standard
 * demo application tasks, which provide no particular functionality, but do
 * provide a good example of how to use the FreeRTOS API.
 *
 * In addition to the standard demo tasks, the following tasks and tests are
 * defined and/or created within this file:
 *
 * "Reg test" tasks - These fill both the core and floating point registers with
 * known values, then check that each register maintains its expected value for
 * the lifetime of the task.  Each task uses a different set of values.  The reg
 * test tasks execute with a very low priority, so get preempted very
 * frequently.  A register containing an unexpected value is indicative of an
 * error in the context switching mechanism.
 *
 * "Check" timer - The check software timer period is initially set to three
 * seconds.  The callback function associated with the check software timer
 * checks that all the standard demo tasks, and the register check tasks, are
 * not only still executing, but are executing without reporting any errors.  If
 * the check software timer discovers that a task has either stalled, or
 * reported an error, then it changes its own execution period from the initial
 * three seconds, to just 200ms.  The check software timer callback function
 * also toggles an LED each time it is called.  This provides a visual
 * indication of the system status:  If the LED toggles every three seconds,
 * then no issues have been discovered.  If the LED toggles every 200ms, then
 * an issue has been discovered with at least one task.
 *
 * Tick hook - The application tick hook is called from the schedulers tick
 * interrupt service routine when configUSE_TICK_HOOK is set to 1 in
 * FreeRTOSConfig.h.  In this example, the tick hook is used to test the kernels
 * handling of the floating point units (FPU) context, both at the task level
 * and when nesting interrupts access the floating point unit registers.  The
 * tick hook function first fills the FPU registers with a known value, it
 * then triggers a medium priority interrupt.  The medium priority interrupt
 * fills the FPU registers with a different value, and triggers a high priority
 * interrupt.  The high priority interrupt once again fills the the FPU
 * registers with a known value before returning to the medium priority
 * interrupt.  The medium priority interrupt checks that the FPU registers
 * contain the values that it wrote to them, then returns to the tick hook
 * function.  Finally, the tick hook function checks that the FPU registers
 * contain the values that it wrote to them, before it too returns.
 *
 * Button interrupt - The button marked "USER" on the starter kit is used to
 * demonstrate how to write an interrupt service routine, and how to synchronise
 * a task with an interrupt.  A task is created that blocks on a test semaphore.
 * When the USER button is pressed, the button interrupt handler gives the
 * semaphore, causing the task to unblock.  When the task unblocks, it simply
 * increments an execution count variable, then returns to block on the
 * semaphore again.
 */

/* Kernel includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "timers.h"
#include "semphr.h"

/* Demo application includes. */
#ifdef FREERTOS_TESTCODE
#include "partest.h"
#include "flash.h"
#include "flop.h"
#include "integer.h"
#include "PollQ.h"
#include "semtest.h"
#include "dynamic.h"
#include "BlockQ.h"
#include "blocktim.h"
#include "countsem.h"
#include "GenQTest.h"
#include "recmutex.h"
#include "death.h"
#endif

/* Hardware and starter kit includes. */
#ifdef FREERTOS_TESTCODE
#include "stm32f4xx.h"
#include "stm32f4xx_conf.h"
#endif

#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_cortex.h"
#include "stm32f4xx_ll_system.h"
#include "stm32f4xx_hal_exti.h"
#include "stm32f4_discovery.h"

/* Priorities for the demo application tasks. */
#define mainFLASH_TASK_PRIORITY                    ( tskIDLE_PRIORITY + 1UL )
#define mainQUEUE_POLL_PRIORITY                    ( tskIDLE_PRIORITY + 2UL )
#define mainSEM_TEST_PRIORITY                      ( tskIDLE_PRIORITY + 1UL )
#define mainBLOCK_Q_PRIORITY                       ( tskIDLE_PRIORITY + 2UL )
#define mainCREATOR_TASK_PRIORITY                  ( tskIDLE_PRIORITY + 3UL )
#define mainFLOP_TASK_PRIORITY                     ( tskIDLE_PRIORITY )

/* The LED used by the check timer. */
#define mainCHECK_LED                              ( 3UL )

/* A block time of zero simply means "don't block". */
#define mainDONT_BLOCK                             ( 0UL )

/* The period after which the check timer will expire, in ms, provided no errors
 * have been reported by any of the standard demo tasks.  ms are converted to the
 * equivalent in ticks using the portTICK_PERIOD_MS constant. */
#define mainCHECK_TIMER_PERIOD_MS                  ( 3000UL / portTICK_PERIOD_MS )

/* The period at which the check timer will expire, in ms, if an error has been
 * reported in one of the standard demo tasks.  ms are converted to the equivalent
 * in ticks using the portTICK_PERIOD_MS constant. */
#define mainERROR_CHECK_TIMER_PERIOD_MS            ( 200UL / portTICK_PERIOD_MS )

/* Set mainCREATE_SIMPLE_LED_FLASHER_DEMO_ONLY to 1 to create a simple demo.
 * Set mainCREATE_SIMPLE_LED_FLASHER_DEMO_ONLY to 0 to create a much more
 * comprehensive test application.  See the comments at the top of this file, and
 * the documentation page on the http://www.FreeRTOS.org web site for more
 * information. */
#define mainCREATE_SIMPLE_LED_FLASHER_DEMO_ONLY    1

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

#ifdef COMPILEOUT_INITIAL_TEST_APP
    /* The following function will only create more tasks and timers if
     * mainCREATE_SIMPLE_LED_FLASHER_DEMO_ONLY is set to 0 (at the top of this
     * file).  See the comments at the top of this file for more information. */
    prvOptionallyCreateComprehensveTestApplication();

    /* Start the scheduler. */

#endif

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
    //EXTI_ClearITPendingBit( EXTI_Line6 );
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
