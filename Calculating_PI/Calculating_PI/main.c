/*
 * Calculating_PI.c
 *
 * Created: 20.03.2018 18:32:07
 * Author : Philipp
 */ 

//#include <avr/io.h>
//TEST
//test
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
#include <string.h>
#include <math.h>

#define START_CALC_EVENT 1<<0
#define CALC_DONE_EVENT 1<<1
#define ITERATIONS_DONE_EVENT 1<<2

extern void vApplicationIdleHook( void );
void vInterface(void *pvParameters);
void vCalculation(void *pvParameters);
void vButtonHandler(void *pvParameters);

double dPi4 = 1.0;
double i=0.0;
char i_str[20];
char dPi4_str[20];

TaskHandle_t InterfaceTask;
TaskHandle_t CalculationTask;
TaskHandle_t ButtonHandlerTask;

EventGroupHandle_t egMyEventGroup;
void vApplicationIdleHook( void )
{	
	
}
int main(void)
{
	egMyEventGroup = xEventGroupCreate();	
	xTaskCreate(vInterface, (const char *) "InterfaceTask", configMINIMAL_STACK_SIZE, NULL, 1, &InterfaceTask);
	xTaskCreate(vCalculation, (const char *) "CalculationTask", configMINIMAL_STACK_SIZE, NULL, 1, &CalculationTask);
	xTaskCreate(vButtonHandler, (const char *) "ButtonHandlerTask", configMINIMAL_STACK_SIZE, NULL, 1, &ButtonHandlerTask);
	
	vInitClock();
	vInitDisplay();
	vDisplayClear();
	vTaskStartScheduler();
	return 0;
}
	
void vInterface(void *pvParameters) 
{

	
	for(;;)
	 {	
		vTaskDelay(500 / portTICK_RATE_MS);	
		vDisplayWriteStringAtPos(0, 0, "Calculating Pi v0.8");
		dtostrf(i, 1, 0, i_str);									//Transforming double variable i to a string to be able to print the value correclty on the display.
		vDisplayWriteStringAtPos(1, 0, "Iterations: %s", i_str);	//muss mit sprintf gelöst werden
		vDisplayWriteStringAtPos(2, 0, "Pi Value: ");
		dtostrf(dPi4*4, 1, 7, dPi4_str);							//Transform double variable to a string
		vDisplayWriteStringAtPos(2, 11, "%s", dPi4_str);
	}
}

void vButtonHandler(void *pvParameters)
{
	
	initButtons();
	for (;;)
	{
		updateButtons();
		if (getButtonPress(BUTTON1)==LONG_PRESSED)
		{
			xEventGroupSetBits(egMyEventGroup, START_CALC_EVENT);
		}
		else if (getButtonPress(BUTTON2)==LONG_PRESSED)
		{
			vTaskSuspend(CalculationTask);
		}
		else if (getButtonPress(BUTTON3)==LONG_PRESSED)
		{
			vTaskResume(CalculationTask);
		}
		else if (getButtonPress(BUTTON4)==LONG_PRESSED)
		{
			vTaskSuspend(CalculationTask);
			vDisplayClear();
			i = 0;
			dPi4 = 1.0;
		}
	}
}

void vCalculation(void *pvParameters) 
{
	for (;;)
	{	xEventGroupWaitBits(egMyEventGroup, START_CALC_EVENT, pdFALSE, pdFALSE, portMAX_DELAY);
		for (i=0; i<=1e7; i++) 
		{
			dPi4 = dPi4 - 1.0/(3+i*4) + 1.0/(5+i*4);
			if (i==1e7)
			{
				vTaskSuspend(CalculationTask);
			}
		}
	}
}
