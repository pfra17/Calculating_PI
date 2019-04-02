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
void vCalculation(void *pvParameters);

double dPi4 = 1;
uint64_t i=0;
char str[80];

TaskHandle_t InterfaceTask;
TaskHandle_t CalculationTask;
TaskHandle_t ledTask;

EventGroupHandle_t calcEventGroup;
void vApplicationIdleHook( void )
{	
	
}

int main(void)
{
	calcEventGroup = xEventGroupCreate();
	vInitClock();
	vInitDisplay();
	
	xTaskCreate(vInterface, (const char *) "InterfaceTask", configMINIMAL_STACK_SIZE, NULL, 1, NULL);
	xTaskCreate(vCalculation, (const char *) "CalculationTask", configMINIMAL_STACK_SIZE, NULL, 1, NULL);
	
	

	vDisplayClear();
	vDisplayWriteStringAtPos(0, 0, "Calculating Pi v0.1");
	vTaskStartScheduler();
	return 0;
}
	
void vInterface(void *pvParameters) {
	for(;;) {	
		initButtons();
		vTaskDelay(500);
		if (CALC_DONE_EVENT) {
			vDisplayWriteStringAtPos(1, 0, "Iterations: %d", i);
			vDisplayWriteStringAtPos(2, 0, "Pi Value:");
		}
		xEventGroupClearBits(calcEventGroup, CALC_DONE_EVENT);
	}
}

void vCalculation(void *pvParameters) {
	for (;;) {	
		for (i=0; i<=1e6; i++) {
			dPi4 = dPi4 - 1.0/(3+i*4) + 1.0/(5+i*4);
			dPi4 *= 4; //To make Pi out of Pi/4
			xEventGroupSetBits(calcEventGroup, CALC_DONE_EVENT);
		}
	}
}