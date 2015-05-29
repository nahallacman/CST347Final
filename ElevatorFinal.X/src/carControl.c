#include "carControl.h"



static void taskCarControl(void *pvParameters)
{
    xTaskParameter_t *pxTaskParameter;
    //portTickType xStartTime;

    /* The parameter points to an xTaskParameters_t structure. */
    pxTaskParameter = (xTaskParameter_t *) pvParameters;
/*
    struct LEDMessage *pxRxedMessage;
    struct LEDMessage pxAllocMessage;
    uint8_t MessageIDtest = 0;

    int delay = 500;

    pxRxedMessage = &pxAllocMessage;
 * */
/*
    while (1)
    {
        if(xLEDQueue[pxTaskParameter->usLEDNumber] != 0) // make sure the queue isn't null
        {
            if( uxQueueMessagesWaiting( xLEDQueue[pxTaskParameter->usLEDNumber] ) != 0 )
            {
                if( xQueueReceive( xLEDQueue[pxTaskParameter->usLEDNumber], ( &pxRxedMessage ), ( TickType_t ) 0 ) )
                {
                    // pcRxedMessage now points to the struct AMessage variable posted
                    // by vATask.
                    MessageIDtest = pxRxedMessage->ucMessageID;
                    //led_test = pxRxedMessage->LEDNum;

                    delay = pxRxedMessage->LEDDelay;
                }
                else
                {
                    //a = 0;
                }
            }
        }
        //no matter what, start out by toggling every 500ms
        toggleLED(pxTaskParameter->usLEDNumber);
        vTaskDelay(delay);
        
    }
*/
    /*
    while(1)
    {
    //no matter what, start out by toggling every 500ms
        toggleLED(0);
        //vTaskDelay(delay);
        vTaskDelay(500);
    }
     */
    
    
    struct CarMessage *pxRxedMessage;
    struct CarMessage Message2;
    pxRxedMessage = &Message2;
    enum ButtonPressed localButton;
    int8_t localVelocity;
    int8_t localAcceleration;
    


    while(1)
    {
        //UART handling code
        if(xUARTQueue != 0) // make sure the task isn't null
        {
            //if( uxQueueMessagesWaiting( xUARTQueue ) != 0 ) // see if there are messages waiting
            //{
                if( xQueueReceive( xCarMessageQueue, ( pxRxedMessage ), portMAX_DELAY ) ) // get the messages
                {
                    localButton = pxRxedMessage->button;
                    localVelocity = pxRxedMessage->Velocity;
                    localAcceleration = pxRxedMessage->Acceleration;
                }
           // }
       }
    }  
    
    
    
}

void carControlInit(void)
{
    UBaseType_t uxQueueLength = 5;
    UBaseType_t uxItemSize;

    uxItemSize = sizeof(xCarMessage);
    
    if( xCarMessageQueue == NULL )
    {
    xCarMessageQueue = xQueueCreate
               (
                  uxQueueLength,
                  uxItemSize
               );
    }
    
    /*
    struct CarMessage *pxTxedMessage;
    struct CarMessage pxAllocMessage;
    pxTxedMessage = &pxAllocMessage;
    pxAllocMessage.button = NONE;
    pxAllocMessage.Velocity = 5;
    pxAllocMessage.Acceleration = 0;
    
    if( xQueueSendToBack(
                       xCarMessageQueue, //QueueHandle_t xQueue,
                       &pxAllocMessage, //const void * pvItemToQueue,
                       0 //TickType_t xTicksToWait
                   ) != pdPASS )
        {
            //task was not able to be created after the xTicksToWait
            //a = 0;
        }
     */

    xTaskCreate(taskCarControl,
        "CarControl",
        configMINIMAL_STACK_SIZE,
        (void *) &xTask0Parameters,
        UARTTXTASKPRIORITY,
        &xCarControlHandle);
   configASSERT( &xCarControlHandle );
    
}
