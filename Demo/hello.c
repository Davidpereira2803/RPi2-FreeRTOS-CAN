#include <stdio.h>
#include <time.h>
#include "hello.h"
#include "FreeRTOS.h"
#include "task.h"
#include <stdint.h>

#define GPIO_BASE 0x3F200000 // Base address for GPIO
#define GPIO_SET *(gpio + 7)  // Set GPIO pin high
#define GPIO_CLR *(gpio + 10) // Set GPIO pin low
     // GPIO pin number

volatile unsigned int *gpio = (unsigned int *)GPIO_BASE;

void vBlinkLED(void *pvParameters) {

    uint32_t gpio_pin = *((uint32_t *)pvParameters);
    // Set GPIO pin to output
    *(gpio + ((gpio_pin) / 10)) &= ~(7 << ((gpio_pin % 10) * 3));
    *(gpio + ((gpio_pin) / 10)) |= (1 << ((gpio_pin % 10) * 3));

    while (1) {
        GPIO_SET = 1 << gpio_pin;
        vTaskDelay(pdMS_TO_TICKS(100)); // On duration
        GPIO_CLR = 1 << gpio_pin;
        vTaskDelay(pdMS_TO_TICKS(100)); // Off duration
    }
}




