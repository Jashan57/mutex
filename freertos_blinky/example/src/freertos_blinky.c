/*
 * @brief FreeRTOS Blinky example
 *
 * @note
 * Copyright(C) NXP Semiconducors, 2014
 * All rights reserved.
 *
 * @par
 * Software that is described herein is for illustrative purposes only
 * which provides customers with programming information regarding the
 * LPC products.  This software is supplied "AS IS" without any warranties of
 * any kind, and NXP Semiconductors and its licensor disclaim any and
 * all warranties, express or implied, including all implied warranties of
 * merchantability, fitness for a particular purpose and non-infringement of
 * intellectual property rights.  NXP Semiconductors assumes no responsibility
 * or liability for the use of the software, conveys no license or rights under any
 * patent, copyright, mask work right, or any other intellectual property rights in
 * or to any products. NXP Semiconductors reserves the right to make changes
 * in the software without notification. NXP Semiconductors also makes no
 * representation or warranty that such application will be suitable for the
 * specified use without further testing or modification.
 *
 * @par
 * Permission to use, copy, modify, and distribute this software and its
 * documentation is hereby granted, under NXP Semiconductors' and its
 * licensor's relevant copyrights in the software, without fee, provided that it
 * is used in conjunction with NXP Semiconductors microcontrollers.  This
 * copyright, permission, and disclaimer notice must appear in all copies of
 * this code.
 */
#include "board.h"
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include "stdint.h"




xSemaphoreHandle mutex = 0;


void TaskRed (void *p)
{
	while(1)
	{
	    if(xSemaphoreTake(mutex,1000))
		{
	    	Board_LED_Set(0, false);
	    	Board_LED_Set(1, true);
			Board_LED_Set(2, true);
			delay(1000);
			Board_LED_Set(0, true);
			//delay(1000);
			xSemaphoreGive(mutex);
		}
	    vTaskDelay(1000);
	}
}
void TaskGreen (void *p)
{

	while(1)
		{
		if(xSemaphoreTake(mutex,1000))
				{
			    	Board_LED_Set(0, true);
			    	Board_LED_Set(1, false);
					Board_LED_Set(2, true);
					delay(1000);
					Board_LED_Set(1, true);
				//	delay(1000);
					xSemaphoreGive(mutex);
				}
			    vTaskDelay(1000);
		}

}


void TaskBlue (void *p)

{
	while(1)
		{
		if(xSemaphoreTake(mutex,1000))
				{
			    	Board_LED_Set(0, true);
			    	Board_LED_Set(1, true);
					Board_LED_Set(2, false);
					delay(1000);
					Board_LED_Set(2, true);
					//delay(1000);
					xSemaphoreGive(mutex);
				}
			    vTaskDelay(1000);
		}
}


/*****************************************************************************
 * Private types/enumerations/variables
 ****************************************************************************/

/*****************************************************************************
 * Public types/enumerations/variables
 ****************************************************************************/

/*****************************************************************************
 * Private functions
 ****************************************************************************/

/* Sets up system hardware */

static void prvSetupHardware(void)
{
	SystemCoreClockUpdate();
	Board_Init();

	/* Initial LED0 state is off */
	Board_LED_Set(0, true);
	Board_LED_Set(1, true);
	Board_LED_Set(2, true);

}




/* UART (or output) thread */
static void vUARTTask(void *pvParameters) {
	int tickCnt = 0;

	while (1) {
		DEBUGOUT("Tick: %d\r\n", tickCnt);
		tickCnt++;

		/* About a 1s delay here */
		vTaskDelay(configTICK_RATE_HZ);
	}
}


/* Delay Function */
void delay()
    {
        for(long int i=0;i<=999;i++)
        {
            for(long int j=0;j<=9999;j++);
        }
    }


/*****************************************************************************
/*****************************************************************************
 * Public functions
 ****************************************************************************/

/**
 * @brief	main routine for FreeRTOS blinky example
 * @return	Nothing, function should not exit
 */
int main(void)
{
	prvSetupHardware();

	mutex = xSemaphoreCreateMutex();




	/* UART output thread, simply counts seconds */
	  xTaskCreate(vUARTTask, (signed char *) "vTaskUart",
				configMINIMAL_STACK_SIZE, NULL, (tskIDLE_PRIORITY + 1UL),
				(xTaskHandle *) NULL);
	  xTaskCreate(TaskRed, (signed char *)"Red", 200, NULL, 1, NULL);
	  xTaskCreate(TaskGreen, (signed char *)"Green", 200, NULL, 1, NULL);
	  xTaskCreate(TaskBlue, (signed char *)"Blue", 200, NULL, 1, NULL);
	  /* Start the scheduler */
	  vTaskStartScheduler();

	/* Should never arrive here */
	return 1;
}

/**
 * @}*/
