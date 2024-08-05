#include "FreeRTOS.h"
#include "task.h"

#include "binaryblink.h"

#define GPIO_BASE 0x3F200000 		// Base address for GPIO
#define GPIO_High *(blink_gpio + 7)	// GPIO pin high
#define GPIO_Low *(blink_gpio + 10) 	// GPIO pin low
#define GPIO_PIN_18 18 // Pin number 18 -> Red LED
#define GPIO_PIN_26 26 // Pin number 26 -> Yellow LED

volatile unsigned int *blink_gpio = (unsigned int *)GPIO_BASE;

// Default Blink function to make PIN 18 blink
void vBlinkLED() {
    	
    uint32_t gpio_pin = GPIO_PIN_18;
 
    // Set GPIO pin to output
    *(blink_gpio + ((gpio_pin) / 10)) &= ~(7 << ((gpio_pin % 10) * 3));
    *(blink_gpio + ((gpio_pin) / 10)) |= (1 << ((gpio_pin % 10) * 3));

    while (1) {
        GPIO_High = 1 << gpio_pin;
        vTaskDelay(pdMS_TO_TICKS(100)); // On duration
        GPIO_Low = 1 << gpio_pin;
        vTaskDelay(pdMS_TO_TICKS(100)); // Off duration
    }
    
}


// Function to blink Pin 18 and Pin 26 depending on the binary value
void blinkBinary(int num, int bits) {
    // num -> decimal number
    // bits -> # of bytes for binary output
    
    uint32_t gpio_pin_1 = GPIO_PIN_18;
    uint32_t gpio_pin_0 = GPIO_PIN_26; 

    // Set GPIO 18 and GPIO 26 to output
    *(blink_gpio + ((gpio_pin_1) / 10)) &= ~(7 << ((gpio_pin_1 % 10) * 3));
    *(blink_gpio + ((gpio_pin_1) / 10)) |= (1 << ((gpio_pin_1 % 10) * 3));
    
    *(blink_gpio + ((gpio_pin_0) / 10)) &= ~(7 << ((gpio_pin_0 % 10) * 3));
    *(blink_gpio + ((gpio_pin_0) / 10)) |= (1 << ((gpio_pin_0 % 10) * 3));

    for (int i = bits; i >= 0; i--) {
        int bit = (num >> i) & 1;

        if (bit) {
            // Blink once GPIO 18 (RED) for binary '1'
            GPIO_High = 1 << gpio_pin_1;
            vTaskDelay(pdMS_TO_TICKS(150)); // On duration
            GPIO_Low = 1 << gpio_pin_1;
        } else {
            // Blink once GPIO 26 (YELLOW) for binary '0'
            GPIO_High = 1 << gpio_pin_0;
            vTaskDelay(pdMS_TO_TICKS(150)); // On duration
            GPIO_Low = 1 << gpio_pin_0;
        }

        // Delay between blinks
        vTaskDelay(pdMS_TO_TICKS(200)); // Off duration, delay between each blink
    }
}


void vBlinkBinaryTask(void *pParam) {

    int num = *((int *)pParam); // Decimal number to convert and blink
    int bits = 8 - 1;		// # of bytes for binary output	

    while(1) {
        blinkBinary(num, bits);
        vTaskDelay(pdMS_TO_TICKS(3000)); // Delay between full binary displays
    }
}
