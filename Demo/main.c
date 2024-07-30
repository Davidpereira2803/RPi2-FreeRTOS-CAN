#include <FreeRTOS.h>
#include <task.h>

#include "Drivers/rpi_gpio.h"
#include "Drivers/rpi_irq.h"

#include "Tasks/ledcontrol.h"
#include "Tasks/Bench/epic/epic.h"

#define GPIO_PIN_26 26
#define GPIO_PIN_18 18 

const TickType_t xDelay = 500 * portTICK_PERIOD_MS;

void task1(void *pParam) {
	int i = 0;
	while(1) {
		i++;
		rpi_gpio_set_val(47, 1);
		rpi_gpio_set_val(35, 0);
		vTaskDelay(xDelay);
	}
}

void task2(void *pParam) {
	int i = 0;
	while(1) {
		i++;
		vTaskDelay(xDelay/2);
		rpi_gpio_set_val(47, 0);
		rpi_gpio_set_val(35, 1);
		vTaskDelay(xDelay/2);
	}
}


/**
 *	This is the systems main entry, some call it a boot thread.
 *
 *	-- Absolutely nothing wrong with this being called main(), just it doesn't have
 *	-- the same prototype as you'd see in a linux program.
 **/
int main(void) {
	rpi_cpu_irq_disable();

	rpi_gpio_sel_fun(47, 1);			
	rpi_gpio_sel_fun(35, 1);
	
	uint32_t gpio_pin_26 = GPIO_PIN_26;
    	uint32_t gpio_pin_18 = GPIO_PIN_18;			

	//xTaskCreate(task1, "LED_0", 128, NULL, 0, NULL);
	//xTaskCreate(task2, "LED_1", 128, NULL, 0, NULL);

	xTaskCreate(vLEDON, "LEDyellow", 128, &gpio_pin_26, 1, NULL);
	//xTaskCreate(vLEDON, "LEDred", 128, &gpio_pin_18, 1, NULL);
	xTaskCreate(epic_main_run, "EPICBench", 1024, NULL, 0, NULL);

	vTaskStartScheduler();

	/*
	 *	We should never get here, but just in case something goes wrong,
	 *	we'll place the CPU into a safe loop.
	 */
	while(1) {
		;
	}
}

