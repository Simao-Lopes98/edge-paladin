#include <stdio.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

#include <network.h>

void app_main() {
    while (1)
    {
        printf ("Heelo\n");
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
    
}