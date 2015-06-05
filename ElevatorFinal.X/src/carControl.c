#include "carControl.h"

static void taskCarControl(void *pvParameters)
{
    xTaskParameter_t *pxTaskParameter;
    //portTickType xStartTime;

    /* The parameter points to an xTaskParameters_t structure. */
    pxTaskParameter = (xTaskParameter_t *) pvParameters;
    
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
    
    xTaskCreate(taskCarControl,
        "CarControl",
        configMINIMAL_STACK_SIZE,
        (void *) &xTask0Parameters,
        UARTTXTASKPRIORITY,
        &xCarControlHandle);
   configASSERT( &xCarControlHandle );    
}

