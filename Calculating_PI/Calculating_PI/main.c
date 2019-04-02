/*
 * Calculating_PI.c
 *
 * Created: 20.03.2018 18:32:07
 * Author : Philipp
 */ 

//#include <avr/io.h>
#include "avr_compiler.h"
#include "pmic_driver.h"
#include "TC_driver.h"
#include "clksys_driver.h"
#include "sleepConfig.h"
#include "port_driver.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "event_groups.h"
#include "stack_macros.h"
#include "mem_check.h"
#include "init.h"
#include "utils.h"
#include "errorHandler.h"
#include "NHD0420Driver.h"
#include "ButtonHandler.h"

#include <stdio.h>
#include <stdlib.h>

#define CALC_DONE_EVENT 1<<0

extern void vApplicationIdleHook( void );
void vInterface(void *pvParameters);
double vCalculation(void *pvParameters);

double dPi4 = 1;
uint64_t i=0;

//void vLedBlink(void *pvParameters);

TaskHandle_t InterfaceTask;
TaskHandle_t CalculationTask;
TaskHandle_t ledTask;

//calcEventGroup = xEventGroupCreate();
EventGroupHandle_t calcEventGroup;
void vApplicationIdleHook( void )
{	
	
}

int main(void)
{
    resetReason_t reason = getResetReason();

	vInitClock();
	vInitDisplay();
	
	xTaskCreate(vInterface, (const char *) "InterfaceTask", configMINIMAL_STACK_SIZE, NULL, 1, NULL);
	xTaskCreate(vCalculation, (const char *) "CalculationTask", configMINIMAL_STACK_SIZE, NULL, 1, NULL);
	
	

	vDisplayClear();
	vDisplayWriteStringAtPos(0,0,"FreeRTOS 10.0.1");
	vDisplayWriteStringAtPos(1,0,"EDUBoard 1.0");
	vDisplayWriteStringAtPos(2,0,"Template");
	vDisplayWriteStringAtPos(3,0,"ResetReason: %d", reason);
	vTaskStartScheduler();
	return 0;
}

/* void vLedBlink(void *pvParameters) {
	(void) pvParameters;
	PORTF.DIRSET = PIN0_bm; /*LED1
	PORTF.OUT = 0x01;
	for(;;) {
// 		uint32_t stack = get_mem_unused();
// 		uint32_t heap = xPortGetFreeHeapSize();
// 		uint32_t taskStack = uxTaskGetStackHighWaterMark(ledTask);
// 		vDisplayClear();
// 		vDisplayWriteStringAtPos(0,0,"Stack: %d", stack);
// 		vDisplayWriteStringAtPos(1,0,"Heap: %d", heap);
// 		vDisplayWriteStringAtPos(2,0,"TaskStack: %d", taskStack);
// 		vDisplayWriteStringAtPos(3,0,"FreeSpace: %d", stack+heap);
		PORTF.OUTTGL = 0x01;				
		vTaskDelay(100 / portTICK_RATE_MS);
	}
}
*/
	
void vInterface(void *pvParameters) {
	initButtons();
	vTaskDelay(500);
	vDisplayClear();
	vDisplayWriteStringAtPos(0, 0, "Calculating Pi4 v1.0");
	vDisplayWriteStringAtPos(1, 0, "Iterations: %d", i);
	vDisplayWriteStringAtPos(2, 0, "Pi Value:");
	for (;;) {
		
	}

	
}

double vCalculation(void *pvParameters) {
	for (;;) {	
		for (i=0; i<=1e6; i++) {
			dPi4 = dPi4 - 1.0/(3+i*4) + 1.0/(5+i*4);
			dPi4 *= 4; //To make Pi out of Pi/4
			
		}
	}
}