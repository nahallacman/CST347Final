#include "carControl.h"



static void taskCarControl(void *pvParameters)
{
    xTaskParameter_t *pxTaskParameter;
    //portTickType xStartTime;

    /* The parameter points to an xTaskParameters_t structure. */
    pxTaskParameter = (xTaskParameter_t *) pvParameters;

    struct LEDMessage *pxRxedMessage;
    struct LEDMessage pxAllocMessage;
    uint8_t MessageIDtest = 0;

    int delay = 500;

    pxRxedMessage = &pxAllocMessage;
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
    while(1)
    {
    //no matter what, start out by toggling every 500ms
        toggleLED(0);
        //vTaskDelay(delay);
        vTaskDelay(500);
    }
        
}

void carControlInit(void)
{
    UBaseType_t uxQueueLength = 5;
    UBaseType_t uxItemSize;

    uxItemSize = sizeof(xSpeedMessage);
    
    if( xSpeedQueue == NULL )
    {
    xSpeedQueue = xQueueCreate
               (
                  uxQueueLength,
                  uxItemSize
               );
    }

    
    xTaskCreate(taskCarControl,
        "CarControl",
        configMINIMAL_STACK_SIZE,
        (void *) &xTask0Parameters,
        UARTTXTASKPRIORITY,
        &xCarControlHandle);
   configASSERT( &xCarControlHandle );
    
}
