/******************************************************************************
 * NOTE: Windows will not be running the FreeRTOS demo threads continuously, so
 * do not expect to get real time behaviour from the FreeRTOS Windows port, or
 * this demo application.  Also, the timing information in the FreeRTOS+Trace
 * logs have no meaningful units.  See the documentation page for the Windows
 * port for further information:
 * http://www.freertos.org/FreeRTOS-Windows-Simulator-Emulator-for-Visual-Studio-and-Eclipse-MingW.html

/* Standard includes. */
#include <stdio.h>
#include <assert.h>
#include <limits.h>
#include <stdlib.h>

/* Kernel includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "timers.h"
#include "semphr.h"
#include "event_groups.h"              //Event Group

/*Clock Frequencies*/
# define sensor01_ms 200 
# define sensor2A_ms 500  
# define sensor2B_ms 1400  

/*Priorities*/
#define SENSING_TASK (tskIDLE_PRIORITY)
#define CNTRLING_TASK (tskIDLE_PRIORITY + 2)
#define BKUPCNTRLING_TASK (tskIDLE_PRIORITY + 1)
/*Data Structures*/

#define ANSI_COLOR_RED     "\x1b[31m" //01
#define ANSI_COLOR_GREEN   "\x1b[32m" //2A
#define ANSI_COLOR_BLUE    "\x1b[34m" //2B
#define ANSI_COLOR_CYAN    "\x1b[36m" //Computation
#define ANSI_COLOR_RESET   "\x1b[0m"
/*Message Queue*/
// A structure to represent a queue Credit: Introduction and Array Implementation of Queue Geeks for Geeks
struct Queue {
	uint16_t front, rear, size;
	uint8_t capacity;
	char identifier[10];
	int* array;
};

/*Structure for the Sensors*/
struct Sensor_t {
	uint8_t writtenToQueue; //Boolean Functionality
	uint16_t coutStart;
	uint16_t countStop;
	uint16_t countFreq;
	struct Queue* messageQueue;
};

// Structure of Sensors
struct Sensors {
   struct Sensor_t* sensor01;
   struct Sensor_t* sensor2A;
   struct Sensor_t* sensor2B;
};

/* C function (prototype) for task */
 void sensor01TaskFunction(void *pvParameters);
 void sensor2ATaskFunction(void *pvParameters);
 void sensor2BTaskFunction(void *pvParameters);
 void controlerTaskFunction(void *pvParameters);
 void backupControlerTaskFunction(void *pvParameters);


/*Task Handles*/
TaskHandle_t sensor01TaskHandle;        // Handle for Sensor 1
TaskHandle_t sensor2ATaskHandle;        // Handle for Sensor 2A
TaskHandle_t sensor2BTaskHandle;        // Handle for Sensor 2B
TaskHandle_t controlerTaskHandle;       // Handle for Controller
TaskHandle_t backupControlerTaskHandle; // Handle for Backup Controller

/*Function Prototypes for Task Functionality*/
/// @fn      createQueue()
/// @brief   A Simple Queue Creator
/// @details Creates a queue of Capacity and initializes size of queue as 0
struct Queue* createQueue(uint8_t capacity);

/// @fn      isFull()
/// @brief   Queue is full when size becomes equal to the capacity (Boolean Check)
int isFull(struct Queue* queue);

/// @fn      isEmpty()
/// @brief   Queue is empty when size is 0 (Boolean Check)
int isEmpty(struct Queue* queue);

/// @fn      enqueue()
/// @brief   Function to add an item to the queue.
void enqueue(struct Queue* queue, int item);

/// @fn      dequeue()
/// @brief   Function to get first item from the queue.
int dequeue(struct Queue* queue);

/// @fn      front()
/// @brief   Function to get front of queue //First Element
int front(struct Queue* queue);

/// @fn      rear()
/// @brief   Function to get rear of queue //Last Element
int rear(struct Queue* queue);

/*Global Variables*/
uint64_t queueCapacity = 1000U;

/*Semaphores for each sensor*/
SemaphoreHandle_t sensor1Semaphore;
SemaphoreHandle_t sensor2ASemaphore;
SemaphoreHandle_t sensor2BSemaphore;

//Event Group
EventGroupHandle_t xControllerEventGroup;

const EventBits_t uxBitsToSet = 0x01;     // Controller 1 running flag

/*Handling of Controller Failure and Activation of backup Controller
- Controller 1 will be setting the above flag when it is operational
- If the flag is not set within a certain timeout, Controller 2 kicks in
- After 2000ms, bit setting is stopped and deleted*/

void main_exercise( void )
{    
	//Taskgroup Creation
	xControllerEventGroup = xEventGroupCreate();

    // Check if the event group was created successfully.
    if (xControllerEventGroup == NULL) 
    {
        printf(ANSI_COLOR_RED "Task Group Creation Failed!" ANSI_COLOR_RESET "\n");
    }

	/*Message queue initialization*/
	struct Queue* queue01 = createQueue(queueCapacity);
	struct Queue* queue2A = createQueue(queueCapacity);
	struct Queue* queue2B = createQueue(queueCapacity);
	//Message Queue Identifier
	strcpy(queue01->identifier, "01");
	strcpy(queue2A->identifier, "2A");
	strcpy(queue2B->identifier, "2B");

	/*Structure for Sensors initialization*/
	struct Sensor_t* sensor01 = malloc(sizeof(struct Sensor_t));
	struct Sensor_t* sensor2A = malloc(sizeof(struct Sensor_t));
	struct Sensor_t* sensor2B = malloc(sizeof(struct Sensor_t));

	//As defined in the problem
	sensor01->coutStart = 100U;
	sensor01->countStop = 199U;
	sensor01->writtenToQueue = 0U;
	sensor01->countFreq = 200U;
	sensor01->messageQueue = queue01;

	sensor2A->coutStart = 200U;
	sensor2A->countStop = 249U;
	sensor2A->writtenToQueue = 0U;
	sensor2A->countFreq = 500U;
	sensor2A->messageQueue = queue2A;

	sensor2B->coutStart = 250U;
	sensor2B->countStop = 299U;
	sensor2B->writtenToQueue = 0U;
	sensor2B->countFreq = 1400U;
	sensor2B->messageQueue = queue2B;

	// Create the semaphores
    sensor1Semaphore = xSemaphoreCreateBinary();
    sensor2ASemaphore = xSemaphoreCreateBinary();
    sensor2BSemaphore = xSemaphoreCreateBinary();

	/*Task Creation*/
 	xTaskCreate(sensor01TaskFunction, "Sensor01", configMINIMAL_STACK_SIZE, sensor01, SENSING_TASK, &sensor01TaskHandle); 
	xTaskCreate(sensor2ATaskFunction, "Sensor2A", configMINIMAL_STACK_SIZE, sensor2A, SENSING_TASK, &sensor2ATaskHandle);
	xTaskCreate(sensor2BTaskFunction, "Sensor2B", configMINIMAL_STACK_SIZE, sensor2B, SENSING_TASK, &sensor2BTaskHandle);


    // Allocate memory for the structure and set the pointers
    struct Sensors* sensors = malloc(sizeof(struct Sensors));
    sensors->sensor01 = sensor01;
    sensors->sensor2A = sensor2A;
    sensors->sensor2B = sensor2B;

    //Controller
    xTaskCreate(controlerTaskFunction, "Controller", configMINIMAL_STACK_SIZE, (void *)sensors, CNTRLING_TASK, &controlerTaskHandle);
    xTaskCreate(backupControlerTaskFunction, "BackupController", configMINIMAL_STACK_SIZE, (void *)sensors, BKUPCNTRLING_TASK, &backupControlerTaskHandle); //Controller has High Priority

    vTaskStartScheduler();
    printf("After starting the scheduler (this point should not be reached)\n");

    for (;;)
    {
        printf("Inside infinite loop (this point should not be reached)\n");
    }
}


/*-------------------------TASKS------------------------------*/

void sensor01TaskFunction(void *pvParameters)
{
	struct Sensor_t* sensor01Param = (struct Sensor_t*)pvParameters;
	/*Synthetic Sensor Value Generation using a Cyclic Counter*/
	uint32_t count = sensor01Param->coutStart;
	while(1)
	{
		count +=1;
		enqueue(sensor01Param->messageQueue, count);
		sensor01Param->writtenToQueue = 1U;
		if (count>=sensor01Param->countStop)
		{
			count = sensor01Param->coutStart;
		}
		// Signalling that Sensor 1 Data has reached the queue and is available for access
     	xSemaphoreGive(sensor1Semaphore);
		vTaskDelay(sensor01Param->countFreq);
		sensor01Param->writtenToQueue = 0U;
	};

}

void sensor2ATaskFunction(void *pvParameters)
{
	struct Sensor_t* sensor2AParam = (struct Sensor_t*)pvParameters;
	uint32_t count = sensor2AParam->coutStart;
	while(1)
	{
		count +=1;
		enqueue(sensor2AParam->messageQueue, count);
		sensor2AParam->writtenToQueue = 1U;
		if (count>=sensor2AParam->countStop)
		{
			count = sensor2AParam->coutStart;
		}
		// Signalling that Sensor 1 Data has reached the queue and is available for access
     	xSemaphoreGive(sensor2ASemaphore);
		vTaskDelay(sensor2AParam->countFreq);
		sensor2AParam->writtenToQueue = 0U;
	};

}

void sensor2BTaskFunction(void *pvParameters)
{
	struct Sensor_t* sensor2BParam = (struct Sensor_t*)pvParameters;
	uint32_t count = sensor2BParam->coutStart;
	while(1)
	{
		count +=1;
		enqueue(sensor2BParam->messageQueue, count);
		sensor2BParam->writtenToQueue = 1U;
		if (count>=sensor2BParam->countStop)
		{
			count = sensor2BParam->coutStart;
		}
		// Signalling that Sensor 1 Data has reached the queue and is available for access
     	xSemaphoreGive(sensor2BSemaphore);
		vTaskDelay(sensor2BParam->countFreq);
		sensor2BParam->writtenToQueue = 0U;
	};

}

void controlerTaskFunction(void *pvParameters)
{   
    struct Sensors* sensors = (struct Sensors*)pvParameters;
    TickType_t xLastWakeTime;
    const TickType_t xFrequency = pdMS_TO_TICKS(20); 
    /* Because: the execution time of controller task job is less than the waiting time between the generation of sensor values for the sensor
    task with the highest value generation frequency.*/ 

    // Initialize the last wake time with the current time.
    xLastWakeTime = xTaskGetTickCount();

    while(1)
    {   
	   // Set the bit to signal that Controller 1 is operational
        xEventGroupSetBits(xControllerEventGroup, uxBitsToSet);

        // Delay for the frequency period.
        vTaskDelayUntil(&xLastWakeTime, xFrequency);

        // Wait for Sensor 01 data
        if (xSemaphoreTake(sensor1Semaphore, portMAX_DELAY) == pdTRUE)
        {
           //printf(ANSI_COLOR_RED "Sensor 1 Data Received: %d" ANSI_COLOR_RESET "\n",dequeue(sensors->sensor01->messageQueue));
		 int sensor01Data = dequeue(sensors->sensor01->messageQueue);
		 printf(ANSI_COLOR_RED "Sensor 1 Data Received: %d" ANSI_COLOR_RESET "\n",sensor01Data);

		 if (sensor01Data != INT_MIN) 
		 {
                // Attempt to take data from either Sensor 2A or 2B
                if (xSemaphoreTake(sensor2ASemaphore, 0) == pdTRUE) 
			 {
				//printf(ANSI_COLOR_GREEN "Sensor 2A Data Received: %d" ANSI_COLOR_RESET "\n", sensors->sensor2A->messageQueue);
                    int sensor2AData = dequeue(sensors->sensor2A->messageQueue);
				printf(ANSI_COLOR_GREEN "Sensor 2A Data Received: %d" ANSI_COLOR_RESET "\n", sensor2AData);
                    if (sensor2AData != INT_MIN) 
				{
                        printf(ANSI_COLOR_CYAN "Controller1 has received sensor data; Sensor1: %d; Sensor2: %d" ANSI_COLOR_RESET "\n", sensor01Data, sensor2AData);
                    }
                } 
			 else if (xSemaphoreTake(sensor2BSemaphore, 0) == pdTRUE) 
			 {
                    //printf(ANSI_COLOR_BLUE "Sensor 2B Data Received: %d" ANSI_COLOR_RESET "\n", sensors->sensor2B->messageQueue);
                    int sensor2BData = dequeue(sensors->sensor2B->messageQueue);
				printf(ANSI_COLOR_BLUE "Sensor 2B Data Received: %d" ANSI_COLOR_RESET "\n", sensor2BData);
                    if (sensor2BData != INT_MIN)
				{
                        printf(ANSI_COLOR_CYAN "Controller1 has received sensor data; Sensor1: %d; Sensor2: %d"ANSI_COLOR_RESET "\n", sensor01Data, sensor2BData);
                    }
                }

        	 }
    	   }
	   if (xTaskGetTickCount() - xLastWakeTime > pdMS_TO_TICKS(2000))
	   {
            // Stop setting the bit to simulate failure
	       // Delete this task
		  printf(ANSI_COLOR_RED ">>>>>>>>>>>>>>>>>%d ms Crossed, Emulating Controller Failure<<<<<<<<<<<<<<<<<<" ANSI_COLOR_RESET "\n", 2000U);
            vTaskDelete(NULL);
		  //vTaskSuspend(NULL);
            break;
        }
     }
}

void backupControlerTaskFunction(void *pvParameters)
{
    struct Sensors* sensors = (struct Sensors*)pvParameters;
    EventBits_t uxBits;
    const TickType_t xTicksToWait = pdMS_TO_TICKS(250);
    /* Wait for the bit to be cleared or for 250 .*/ 

    while(1)
    {   
	   uxBits = xEventGroupWaitBits(
            xControllerEventGroup,   // The event group being tested.
            uxBitsToSet,             // The bits within the event group to wait for.
            pdTRUE,                  // uxBitsToSet should be cleared before returning.
            pdFALSE,                 // Don't wait for both bits, either bit will do.
            xTicksToWait );          // Wait a maximum of 250ms for either bit to be set.

	   // If the bit was not set within the timeout, assume Controller 1 has failed
        if ((uxBits & uxBitsToSet) == 0) 
	   {
        	// Wait for Sensor 01 data
        	if (xSemaphoreTake(sensor1Semaphore, portMAX_DELAY) == pdTRUE)
        	{
          	//printf(ANSI_COLOR_RED "Sensor 1 Data Received: %d" ANSI_COLOR_RESET "\n",sensors->sensor01->messageQueue);
		 	int sensor01Data = dequeue(sensors->sensor01->messageQueue);
			printf(ANSI_COLOR_RED "Sensor 1 Data Received: %d" ANSI_COLOR_RESET "\n",sensor01Data);

		 	if (sensor01Data != INT_MIN) 
		 	{
                	// Attempt to take data from either Sensor 2A or 2B
                	if (xSemaphoreTake(sensor2ASemaphore, 0) == pdTRUE) 
			 	{
					//printf(ANSI_COLOR_GREEN "Sensor 2A Data Received: %d" ANSI_COLOR_RESET "\n", sensors->sensor2A->messageQueue);
                    	int sensor2AData = dequeue(sensors->sensor2A->messageQueue);
					printf(ANSI_COLOR_GREEN "Sensor 2A Data Received: %d" ANSI_COLOR_RESET "\n", sensor2AData);
                    	if (sensor2AData != INT_MIN) 
					{
                        		printf(ANSI_COLOR_CYAN "Backup Controller / Controller 2 has received sensor data; Sensor1: %d; Sensor2: %d" ANSI_COLOR_RESET "\n", sensor01Data, sensor2AData);
                    	}
                	} 
			 	else if (xSemaphoreTake(sensor2BSemaphore, 0) == pdTRUE) 
			 	{
                    	//printf(ANSI_COLOR_BLUE "Sensor 2B Data Received: %d" ANSI_COLOR_RESET "\n", sensors->sensor2B->messageQueue);
                    	int sensor2BData = dequeue(sensors->sensor2B->messageQueue);
					printf(ANSI_COLOR_BLUE "Sensor 2B Data Received: %d" ANSI_COLOR_RESET "\n", sensor2BData);
                    	if (sensor2BData != INT_MIN)
					{
                        		printf(ANSI_COLOR_CYAN "Backup Controller / Controller 2 has received sensor data; Sensor1: %d; Sensor2: %d"ANSI_COLOR_RESET "\n", sensor01Data, sensor2BData);
                    	}
                	}

        	 	}
    	   	}
	   }
     }
}

/*------------------------------------------------------------*/

struct Queue* createQueue(uint8_t capacity)
{
	struct Queue* queue = (struct Queue*)malloc(sizeof(struct Queue));
	queue->capacity = capacity;
	queue->front = queue->size = 0;

	queue->rear = capacity - 1;
	queue->array = (int*)malloc(queue->capacity * sizeof(int));
	return queue;
}

// Check if queue is full
int isFull(struct Queue* queue)
{
	return (queue->size == queue->capacity);
}

// Check if queue is empty
int isEmpty(struct Queue* queue)
{
	return (queue->size == 0);
}

// Enqueue Function
void enqueue(struct Queue* queue, int item)
{
	if (isFull(queue))
		return;
	queue->rear = (queue->rear + 1)
				% queue->capacity;
	queue->array[queue->rear] = item;
	queue->size = queue->size + 1;
	//printf("%d enqueued to queue %s\n", item, queue->identifier);
	//printf("Setting Written to Queue to capture write in queue %s\n", queue->identifier);
}

// Dequeue Function
int dequeue(struct Queue* queue)
{
	if (isEmpty(queue))
		return INT_MIN;
	int item = queue->array[queue->front];
	queue->front = (queue->front + 1)
				% queue->capacity;
	queue->size = queue->size - 1;
	return item;
}

// Function to get front of queue
int front(struct Queue* queue)
{
	if (isEmpty(queue))
		return INT_MIN;
	return queue->array[queue->front];
}

// Function to get rear of queue
int rear(struct Queue* queue)
{
	if (isEmpty(queue))
		return INT_MIN;
	return queue->array[queue->rear];
}
