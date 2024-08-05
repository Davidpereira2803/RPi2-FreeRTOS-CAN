#include <FreeRTOS.h>
#include <task.h>

#include "Drivers/rpi_gpio.h"
#include "Drivers/rpi_irq.h"

#include "Tasks/ledcontrol.h"
#include "Tasks/binaryblink.h"
#include "Tasks/Bench/epic/epic.h"

	
// Start Default task1 and task2
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
// End Default task1 and task2



// Define the addresses for PMU registers
#define PMUSERENR    (*(volatile uint32_t*)0xF8891C00)
#define PMCR         (*(volatile uint32_t*)0xF8891C04)
#define PMCNTENSET   (*(volatile uint32_t*)0xF8891C20)
#define PMCCNTR      (*(volatile uint32_t*)0xF8891E04)
#define PMOVSR       (*(volatile uint32_t*)0xF8891C40)

// Enable access to PMU registers
void enable_pmu() {
    // Enable user-mode access to the performance counters.
    PMUSERENR = 1;

    // Reset and enable all counters.
    PMCR = (1 << 0) | (1 << 1) | (1 << 2) | (1 << 3);

    // Enable the cycle counter specifically
    PMCNTENSET = 1 << 31;
}

// Read the cycle counter value
uint32_t read_cycle_counter() {
    return PMCCNTR;
}


int num;

int vEpicTask(){
    uint32_t startCycles, endCycles, taskCycles;
        // Get the start cycle count
        startCycles = read_cycle_counter();
	
	epic_main_run();
	
	for(int i = 0; i < 1000; i++) {
 
	}

	
        // Get the end cycle count
        endCycles = read_cycle_counter();

        // Calculate the number of cycles taken for the task to execute
        taskCycles = endCycles - startCycles;
	
	//num = taskCycles/100000;
	
	num = 0;
	
	return 0;
}


/**
 *	This is the systems main entry, some call it a boot thread.
 *
 *	-- Absolutely nothing wrong with this being called main(), just it doesn't have
 *	-- the same prototype as you'd see in a linux program.
 **/
int main(void) {
	//rpi_cpu_irq_disable();

	//rpi_gpio_sel_fun(47, 1);			
	//rpi_gpio_sel_fun(35, 1);
	
	//xTaskCreate(task1, "LED_0", 128, NULL, 0, NULL);
	//xTaskCreate(task2, "LED_1", 128, NULL, 0, NULL);

	//xTaskCreate(vLEDON, "LEDyellow", 128, &gpio_pin_26, 1, NULL);
	//xTaskCreate(vLEDON, "LEDred", 128, &gpio_pin_18, 1, NULL);
	//xTaskCreate(epic_main_run, "EPICBench", 1024, NULL, 2, NULL);
	enable_pmu(); 

	vEpicTask();

	//int num = 0;	
	
	xTaskCreate(vBlinkBinaryTask, "Blink Binary", 128, (void*)&num, 1, NULL);

	vTaskStartScheduler();

	/*
	 *	We should never get here, but just in case something goes wrong,
	 *	we'll place the CPU into a safe loop.
	 */
	while(1) {
		;
	}
}

