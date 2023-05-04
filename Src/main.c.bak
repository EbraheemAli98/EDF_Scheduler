/*
 * FreeRTOS Kernel V10.2.0
 * Copyright (C) 2019 Amazon.com, Inc. or its affiliates.  All Rights Reserved.
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
 * http://www.FreeRTOS.org
 * http://aws.amazon.com/freertos
 *
 * 1 tab == 4 spaces!
 */

/* 
	NOTE : Tasks run in system mode and the scheduler runs in Supervisor mode.
	The processor MUST be in supervisor mode when vTaskStartScheduler is 
	called.  The demo applications included in the FreeRTOS.org download switch
	to supervisor mode prior to main being called.  If you are not using one of
	these demo application projects then ensure Supervisor mode is used.
*/


/*
 * Creates all the demo application tasks, then starts the scheduler.  The WEB
 * documentation provides more details of the demo application tasks.
 * 
 * Main.c also creates a task called "Check".  This only executes every three 
 * seconds but has the highest priority so is guaranteed to get processor time.  
 * Its main function is to check that all the other tasks are still operational.
 * Each task (other than the "flash" tasks) maintains a unique count that is 
 * incremented each time the task successfully completes its function.  Should 
 * any error occur within such a task the count is permanently halted.  The 
 * check task inspects the count of each task to ensure it has changed since
 * the last time the check task executed.  If all the count variables have 
 * changed all the tasks are still executing error free, and the check task
 * toggles the onboard LED.  Should any task contain an error at any time 
 * the LED toggle rate will change from 3 seconds to 500ms.
 *
 */

/* Standard includes. */
#include <stdlib.h>
#include <stdio.h>

/* Scheduler includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "lpc21xx.h"
#include "queue.h"

/* Peripheral includes. */
#include "serial.h"
#include "GPIO.h"


/*-----------------------------------------------------------*/

/* Constants to setup I/O and processor. */
#define mainBUS_CLK_FULL	( ( unsigned char ) 0x01 )

/* Constants for the ComTest demo application tasks. */
#define mainCOM_TEST_BAUD_RATE	( ( unsigned long ) 115200 )

#define NUM_OF_PREEMPTIONS_IN_LOAD_2_SIMULATION_TASK	2
#define SIZE_OF_ITEM_IN_QUEUE sizeof(char[30])
#define NUM_OF_ITEMS_IN_QUEUE	3

#define RISING		1
#define FALLING		0
/*
 * Configure the processor for use with the Keil demo board.  This is very
 * minimal as most of the setup is managed by the settings in the project
 * file.
 */
static void prvSetupHardware( void );
/*-----------------------------------------------------------*/
void Button_1_Monitor (void *pvParameters);
void Button_2_Monitor (void *pvParameters);
void Periodic_Transmitter (void *pvParameters);
void Uart_Receiver (void *pvParameters);
void Load_1_Simulation (void *pvParameters);
void Load_2_Simulation (void *pvParameters);


volatile TickType_t Load_1_TotalExecutionTime =0;
volatile TickType_t Load_2_TotalExecutionTime =0;

volatile TickType_t Task_Load_1_Simulation_StartPoint = 0;
volatile TickType_t Task_Load_1_Simulation_EndPoint = 0;
volatile TickType_t Task_Load_2_Simulation_StartPoint = 0;
volatile TickType_t Task_Load_2_Simulation_EndPoint = 0;

volatile portFLOAT CPU_load = 0;
volatile TickType_t overall_system_time = 0; 

QueueHandle_t Global_Queue_Handle = 0;
/*
 * Application entry point:
 * Starts all the other tasks, then starts the scheduler. 
 */
int main( void )
 {
	/* Setup the hardware for use with the Keil demo board. */
	prvSetupHardware();
	
	Global_Queue_Handle = xQueueCreate(NUM_OF_ITEMS_IN_QUEUE,SIZE_OF_ITEM_IN_QUEUE);
	
	xTaskPeriodicCreate(
											Button_1_Monitor,
											"Button_1_Monitor",
											configMINIMAL_STACK_SIZE,
											NULL,
											1,
											50,
											NULL);
	
	xTaskPeriodicCreate(
											Button_2_Monitor,
											"Button_2_Monitor",
											configMINIMAL_STACK_SIZE,
											NULL,
											1,
											50,
											NULL);
	
  xTaskPeriodicCreate(
											Periodic_Transmitter,
											"Periodic_Transmitter",
											configMINIMAL_STACK_SIZE,
											NULL,
											1,
											100,
											NULL);
											
	xTaskPeriodicCreate(
											Uart_Receiver,
											"Uart_Receiver",
											configMINIMAL_STACK_SIZE,
											NULL,
											1,
											20,
											NULL);
											
	xTaskPeriodicCreate(
											Load_1_Simulation,
											"Load_1_Simulation",
											configMINIMAL_STACK_SIZE,
											NULL,
											1,
											10,
											NULL);
											
	xTaskPeriodicCreate(
											Load_2_Simulation,
											"Load_2_Simulation",
											configMINIMAL_STACK_SIZE,
											NULL,
											1,
											100,
											NULL);
											

	/* Now all the tasks have been started - start the scheduler.

	NOTE : Tasks run in system mode and the scheduler runs in Supervisor mode.
	The processor MUST be in supervisor mode when vTaskStartScheduler is 
	called.  The demo applications included in the FreeRTOS.org download switch
	to supervisor mode prior to main being called.  If you are not using one of
	these demo application projects then ensure Supervisor mode is used here. */
	vTaskStartScheduler();

	/* Should never reach here!  If you do then there was not enough heap
	available for the idle task to be created. */
	for( ;; );
}
/*-----------------------------------------------------------*/
void Button_1_Monitor(void *pvParameters)
{
		TickType_t xLastWakeTime;
		pinState_t CurrentButtonState,PreviousButtonState;
		const TickType_t xFrequency = 50;	
		xLastWakeTime = xTaskGetTickCount ();
		PreviousButtonState = FALLING;
	
		vTaskSetApplicationTaskTag( NULL, ( void * ) 3 );
	
		for(;;)
		{
			CurrentButtonState = GPIO_read(PORT_0,PIN9);
			if( CurrentButtonState != PreviousButtonState )
			{
					if( CurrentButtonState == RISING)
					{
						xQueueSendToBack(
														 Global_Queue_Handle,
														 "Button1_Rising_Edge.\n",
														/* Block time of 0 says don't block if the queue is already full.
															 Check the value returned by xQueueSend() to know if the message
															 was sent to the queue successfully. */
														 0);
					}
					else
					{
						xQueueSendToBack(
															Global_Queue_Handle,
															"Button1_Falling_Edge.\n",
															/* Block time of 0 says don't block if the queue is already full.
																 Check the value returned by xQueueSend() to know if the message
																 was sent to the queue successfully. */
															 0);
					}
			}
			else
			{
			}
			PreviousButtonState = CurrentButtonState;
			
			vTaskDelayUntil(&xLastWakeTime, xFrequency);

		}
		
}
/*-----------------------------------------------------------*/
void Button_2_Monitor(void *pvParameters)
{
		TickType_t xLastWakeTime;
		pinState_t CurrentButtonState,PreviousButtonState;
		const TickType_t xFrequency = 50;
		PreviousButtonState = FALLING;
		xLastWakeTime = xTaskGetTickCount ();
	
		vTaskSetApplicationTaskTag( NULL, ( void * ) 4 );
		
		for(;;)
		{
			CurrentButtonState = GPIO_read(PORT_0,PIN0);
			if(CurrentButtonState != PreviousButtonState)
			{
					if( CurrentButtonState == RISING)
					{
						xQueueSendToBack(
														 Global_Queue_Handle,
														 "Button2_Rising_Edge.\n",
														/* Block time of 0 says don't block if the queue is already full.
															 Check the value returned by xQueueSend() to know if the message
															 was sent to the queue successfully. */
														 0);
					}
					else
					{
						xQueueSendToBack(
															Global_Queue_Handle,
															"Button2_Falling_Edge.\n",
															/* Block time of 0 says don't block if the queue is already full.
																 Check the value returned by xQueueSend() to know if the message
																 was sent to the queue successfully. */
															 0);
					}
			}
			else
			{
			}
			PreviousButtonState = CurrentButtonState;
			

			vTaskDelayUntil(&xLastWakeTime, xFrequency);

		}
		
}
/*-----------------------------------------------------------*/
void Periodic_Transmitter (void *pvParameters)
{
		TickType_t xLastWakeTime;
		//char SendString[50] = "Periodic_Tramsimtter is run.\n";
		const TickType_t xFrequency = 100;
		xLastWakeTime = xTaskGetTickCount ();
	
		vTaskSetApplicationTaskTag( NULL, ( void * ) 5 );
	
		for(;;)
		{
			if(xQueueSendToBack(
												Global_Queue_Handle,
												"Periodic_Tramsimtter is run.\n",
											  /* Block time of 0 says don't block if the queue is already full.
													 Check the value returned by xQueueSendToBack() to know if the message
													 was sent to the queue successfully. */			
												0) != pdTRUE )
			{
				
			}
			else
			{
				
			}
			

			vTaskDelayUntil(&xLastWakeTime, xFrequency);

		}
		
}
/*-----------------------------------------------------------*/
void Uart_Receiver (void *pvParameters)
{
		TickType_t xLastWakeTime;
		signed char ReceiveString[30];
		const TickType_t xFrequency = 20;
		xLastWakeTime = xTaskGetTickCount ();
		
		vTaskSetApplicationTaskTag( NULL, ( void * ) 6 );
	
		for(;;)
		{
			/*
			 * Setting xTicksToWait to 0 will cause the function to return immediately if the queue is empty.
			 */
			if(xQueueReceive(Global_Queue_Handle,ReceiveString,0) == pdTRUE )
			{
				vSerialPutString(ReceiveString,30);
			}
			else
			{
				
			}

			vTaskDelayUntil(&xLastWakeTime, xFrequency);

		}
}
/*-----------------------------------------------------------*/
void Load_1_Simulation (void *pvParameters)
{
		TickType_t xLastWakeTime;
		uint32_t i;
		const TickType_t xFrequency = 10;	
		xLastWakeTime = xTaskGetTickCount ();
	
		vTaskSetApplicationTaskTag( NULL, ( void * ) 1 );
	
	for(;;)
		{

				for(i=0; i<33500; i++)//35000
				{
						i=i;	
				}
				
				// Wait for the next cycle.
				vTaskDelayUntil(&xLastWakeTime, xFrequency);
		}
		
}
/*-----------------------------------------------------------*/
void Load_2_Simulation (void *pvParameters)
{
		TickType_t xLastWakeTime;
		uint32_t i;
		const TickType_t xFrequency = 100;	
		xLastWakeTime = xTaskGetTickCount ();
	
		vTaskSetApplicationTaskTag( NULL, ( void * ) 2 );
		
		for(;;)
		{

				for(i=0; i<77000; i++) //75000
				{
						i=i;
				}

				vTaskDelayUntil(&xLastWakeTime, xFrequency);
		}
		
}
/*-----------------------------------------------------------*/
void vApplicationTickHook(void)
{
    /* This task is going to use floating point operations.  Therefore it calls
    portTASK_USES_FLOATING_POINT() once on task entry, before entering the loop
    that implements its functionality.  From this point on the task has a floating
    point context. */
		//portTASK_USES_FLOATING_POINT();
	
		GPIO_write(PORT_0,PIN1,PIN_IS_HIGH);
		GPIO_write(PORT_0,PIN1,PIN_IS_LOW);
		
		CPU_load = ((float)((Load_1_TotalExecutionTime) + (Load_2_TotalExecutionTime))/(overall_system_time))*100;
}

/*-----------------------------------------------------------*/
void vApplicationIdleHook(void)
{
		GPIO_write(PORT_0,PIN4,PIN_IS_HIGH);
}
/*-----------------------------------------------------------*/
/* Function to reset timer 1 */
void timer1Reset(void)
{
	T1TCR |= 0x2;
	T1TCR &= ~0x2;
}

/* Function to initialize and start timer 1 */
static void configTimer1(void)
{
	T1PR = 1000;
	T1TCR |= 0x1;
}

static void prvSetupHardware( void )
{
	/* Perform the hardware setup required.  This is minimal as most of the
	setup is managed by the settings in the project file. */

	/* Configure UART */
	xSerialPortInitMinimal(mainCOM_TEST_BAUD_RATE);

	/* Configure GPIO */
	GPIO_init();
	
	/* Config trace timer 1 and read T1TC to get current tick */
	configTimer1();

	/* Setup the peripheral bus to be the same as the PLL output. */
	VPBDIV = mainBUS_CLK_FULL;
}
/*-----------------------------------------------------------*/


