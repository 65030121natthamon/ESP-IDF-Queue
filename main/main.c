#include <stdio.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include "freertos/queue.h"

// สร้าง handle สำหรับ task และ queue
TaskHandle_t myTaskHandle = NULL;
TaskHandle_t myTaskHandle2 = NULL;
QueueHandle_t queue;

// code ของ task #1
void Task1(void *arg)
{
    // จองพื้นที่สำหรับสร้างข้อมูลที่จะส่งผ่าน queue จำนวน 50 ตัวอักษร
    char txBuffer[50];

    // สร้าง queue แล้วเก็บไว้ในตัวแปร handle
    queue = xQueueCreate(5, sizeof(txBuffer));
    // ทดสอบว่าสร้าง queue สำเร็จหรือไม่
    if (queue == NULL)
    {
        printf("Failed to create queue\n");
        vTaskDelete(NULL);
    }

    // เตรียมข้อมูลและส่งผ่าน queue ทำเป็นจำนวน 3 รอบ
    sprintf(txBuffer, "Hello from Demo_Task 1");
    xQueueSend(queue, (void*)txBuffer, (TickType_t)0);

    sprintf(txBuffer, "Hello from Demo_Task 2");
    xQueueSend(queue, (void*)txBuffer, (TickType_t)0);

    sprintf(txBuffer, "Hello from Demo_Task 3");
    xQueueSend(queue, (void*)txBuffer, (TickType_t)0);

    // ส่งครบ 3 ครั้ง ให้วนลูปไม่รู้จบ
    while(1)
    {
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}

// Code สำหรับ task #2
void Task2(void *arg)
{
    // เตรียมพื้นที่รับข้อมูลผ่าน queue
    char rxBuffer[50];

    // วนลูปรอ
    while (1)
    {
        // ถ้ามีการส่งข้อมูลผ่าน queue handle ที่รอรับ ให้เอาข้อมูลไปแสดงผล
        if (xQueueReceive(queue, &(rxBuffer), (TickType_t) 5))
        {
            printf("Received data from queue == %s\n", rxBuffer);
            vTaskDelay(1000 / portTICK_PERIOD_MS);
        }
    }
}

// Code ของ app_main()
void app_main(void)
{
    xTaskCreate(Task1, "Task_1", 4096, NULL, 10, &myTaskHandle);
    xTaskCreatePinnedToCore(Task2, "Task_2", 4096, NULL, 10, &myTaskHandle2, 1);
}
