/*
 * FreeRTOS V202212.00
 * Copyright (C) 2020 Amazon.com, Inc. or its affiliates.  All Rights Reserved.
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

#ifndef FREERTOS_CONFIG_H
#define FREERTOS_CONFIG_H

#include <lpc21xx.h>
#include "portmacro.h"
#include "GPIO.h"
/*-----------------------------------------------------------
 * Application specific definitions.
 *
 * These definitions should be adjusted for your particular hardware and
 * application requirements.
 *
 * THESE PARAMETERS ARE DESCRIBED WITHIN THE 'CONFIGURATION' SECTION OF THE
 * FreeRTOS API DOCUMENTATION AVAILABLE ON THE FreeRTOS.org WEB SITE.
 *
 * See http://www.freertos.org/a00110.html
 *----------------------------------------------------------*/

#define configUSE_PREEMPTION		1
#define configUSE_IDLE_HOOK			1
#define configUSE_TICK_HOOK			1
#define configCPU_CLOCK_HZ			( ( unsigned long ) 60000000 )	/* =12.0MHz xtal multiplied by 5 using the PLL. */
#define configTICK_RATE_HZ			( ( TickType_t ) 100 ) /* Tick required freq = 100 Hz ,this gives us Tick time = 1/100 = 10ms */
#define configMAX_PRIORITIES		( 4 )
#define configMINIMAL_STACK_SIZE	( ( unsigned short ) 90 )
#define configTOTAL_HEAP_SIZE		( ( size_t ) 13 * 1024 )
#define configMAX_TASK_NAME_LEN		( 8 )
#define configUSE_TRACE_FACILITY	0
#define configUSE_16_BIT_TICKS		0
#define configIDLE_SHOULD_YIELD		1
#define configUSE_TIME_SLICING    0

#define configQUEUE_REGISTRY_SIZE 	0

/* Macro to Enable or disable the EDF scheduler algorithm */
#define configUSE_EDF_SCHEDULER		1

/* Set the following definitions to 1 to include the API function, or zero
to exclude the API function. */

#define INCLUDE_vTaskPrioritySet			0
#define INCLUDE_uxTaskPriorityGet			1
#define INCLUDE_vTaskDelete						1
#define INCLUDE_vTaskCleanUpResources	0
#define INCLUDE_vTaskSuspend					1
#define INCLUDE_vTaskDelayUntil				1
#define INCLUDE_vTaskDelay						1

#define configUSE_APPLICATION_TASK_TAG	1

extern volatile TickType_t Load_1_TotalExecutionTime;
extern volatile TickType_t Load_2_TotalExecutionTime;

extern volatile TickType_t Task_Load_1_Simulation_StartPoint;
extern volatile TickType_t Task_Load_1_Simulation_EndPoint;
extern volatile TickType_t Task_Load_2_Simulation_StartPoint;
extern volatile TickType_t Task_Load_2_Simulation_EndPoint;


extern volatile TickType_t overall_system_time;


#define traceTASK_SWITCHED_OUT()	do\
{\
	switch((long)pxCurrentTCB->pxTaskTag)\
	{\
	case 1: 	/*** Load_1_Simulation Task ***/\
		GPIO_write(PORT_0,PIN2,PIN_IS_LOW);\
		Task_Load_1_Simulation_EndPoint = T1TC;\
		Load_1_TotalExecutionTime += (Task_Load_1_Simulation_EndPoint - Task_Load_1_Simulation_StartPoint);\
		break;\
	case 2:		/*** Load_2_Simulation Task ***/\
		GPIO_write(PORT_0,PIN3,PIN_IS_LOW);\
		Task_Load_2_Simulation_EndPoint = T1TC;\
		Load_2_TotalExecutionTime += (Task_Load_2_Simulation_EndPoint - Task_Load_2_Simulation_StartPoint);\
		break;\
	case 3:		/*** Button_1_Monitor Task ***/\
		GPIO_write(PORT_0,PIN7,PIN_IS_LOW);\
		break;\
	case 4:		/*** Button_2_Monitor Task ***/\
		GPIO_write(PORT_0,PIN8,PIN_IS_LOW);\
		break;\
	case 5:		/*** Periodic_Transimtter Task ***/\
		GPIO_write(PORT_0,PIN6,PIN_IS_LOW);\
		break;\
	case 6:		/*** Uart_Receiver Task ***/\
		GPIO_write(PORT_0,PIN5,PIN_IS_LOW);\
		break;\
	default:	/*** Idle Task ***/\
		GPIO_write(PORT_0,PIN4,PIN_IS_LOW);\
		break;\
	}\
	overall_system_time = T1TC;\
}while(0)
																	

#define traceTASK_SWITCHED_IN()	do\
{\
	switch((long)pxCurrentTCB->pxTaskTag)\
	{\
	case 1: 	/*** Load_1_Simulation Task ***/\
		GPIO_write(PORT_0,PIN2,PIN_IS_HIGH);\
		Task_Load_1_Simulation_StartPoint = T1TC;\
		break;\
	case 2:		/*** Load_2_Simulation Task ***/\
		GPIO_write(PORT_0,PIN3,PIN_IS_HIGH);\
		Task_Load_2_Simulation_StartPoint = T1TC;\
		break;\
	case 3:		/*** Button_1_Monitor Task ***/\
		GPIO_write(PORT_0,PIN7,PIN_IS_HIGH);\
		break;\
	case 4:		/*** Button_2_Monitor Task ***/\
		GPIO_write(PORT_0,PIN8,PIN_IS_HIGH);\
		break;\
	case 5:		/*** Periodic_Transimtter Task ***/\
		GPIO_write(PORT_0,PIN6,PIN_IS_HIGH);\
		break;\
	case 6:		/*** Uart_Receiver Task ***/\
		GPIO_write(PORT_0,PIN5,PIN_IS_HIGH);\
		break;\
	}\
}while(0)
																	

#endif /* FREERTOS_CONFIG_H */
