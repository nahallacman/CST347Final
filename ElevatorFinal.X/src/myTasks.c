#include "myTasks.h"

void SystemControlSetup()
{
    UBaseType_t uxQueueLength = 5;
    UBaseType_t uxItemSize;

    uxItemSize = sizeof(xLEDMessage);
       
       if( xLEDQueue[currentHandle] == NULL )
       {
       xLEDQueue[currentHandle] = xQueueCreate
                  (
                     uxQueueLength,
                     uxItemSize
                  );
       }

    currentHandle++;
    
       if( xLEDQueue[currentHandle] == NULL )
       {
       xLEDQueue[currentHandle] = xQueueCreate
                  (
                     uxQueueLength,
                     uxItemSize
                  );
       }

        currentHandle++;
        
       if( xLEDQueue[currentHandle] == NULL )
       {
       xLEDQueue[currentHandle] = xQueueCreate
                  (
                     uxQueueLength,
                     uxItemSize
                  );
       }

    //DONT FORGET TO RESET THE HANDLE INDEX -_-
    currentHandle = 0;

        //UART TX and RX Control tasks
        xTaskCreate(taskUARTTXControl,
            "UART-TX",
            configMINIMAL_STACK_SIZE,
            (void *) &xTask0Parameters,
            UARTTXTASKPRIORITY,
            &xUARTTXHandle);
       configASSERT( &xUARTTXHandle );

    xTaskCreate(taskUARTRXControl,
            "UART-RX",
            configMINIMAL_STACK_SIZE,
            (void *) &xTask0Parameters,
            UARTRXTASKPRIORITY,
            &xUARTRXHandle);
       configASSERT( &xUARTRXHandle );

       xTaskCreate(buttonTask,
            "BUTTON",
            configMINIMAL_STACK_SIZE,
            NULL,
            BUTTONTASKPRIORITY,
            NULL);
}


//"driver" function
static void taskToggleAnLED(void *pvParameters)
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
}


static void taskUARTTXControl(void *pvParameters)
{
    xTaskParameter_t *pxTaskParameter;
    portTickType xStartTime;

    /* The parameter points to an xTaskParameters_t structure. */
    pxTaskParameter = (xTaskParameter_t *) pvParameters;

    struct UARTMessage *pxRxedMessage;
    struct UARTMessage Message2;
    pxRxedMessage = &Message2;



    while(1)
    {
        //UART handling code
        if(xUARTQueue != 0) // make sure the task isn't null
        {
            //if( uxQueueMessagesWaiting( xUARTQueue ) != 0 ) // see if there are messages waiting
            //{
                if( xQueueReceive( xUARTQueue, ( pxRxedMessage ), portMAX_DELAY ) ) // get the messages
                {

                    //vUartPutStr(UART2, pxRxedMessage->ucMessage, 50);
                    //void vUartPutStr(UART_MODULE umPortNum, char *pString, int iStrLen);
                    UARTPutString(pxRxedMessage->ucMessage);
                }
           // }
       }
    }

}

#define MAX_INPUT_LENGTH 50
#define MAX_OUTPUT_LENGTH 300
//#define MAX_OUTPUT_LENGTH 50
static const int8_t * const pcWelcomeMessage =
 "FreeRTOS command server.\r\nType Help to view a list of registered commands.\r\n";

static void taskUARTRXControl(void *pvParameters)
{
    struct CarMessage pxAllocMessage;
    int8_t localVelocity = 0;
    int8_t localAccleration = 0 ;
    enum ButtonPressed localButton = NONE;
    pxAllocMessage.Velocity = localVelocity;
    pxAllocMessage.Acceleration = localAccleration;
    
    

    
    
    
    
    
    
    
    xTaskParameter_t *pxTaskParameter;
    portTickType xStartTime;

    /* The parameter points to an xTaskParameters_t structure. */
    pxTaskParameter = (xTaskParameter_t *) pvParameters;

    struct UARTMessage *pxRxedMessage;
    struct UARTMessage Message2;
    pxRxedMessage = &Message2;




    int valid_command = 0;
    //char cRxedChar;
    UART_DATA data;

    //struct UARTMessage *pxRxedMessage;
    //struct UARTMessage Message2;
    //pxRxedMessage = &Message2;

    int j;
    int offset = 0;
    int done = 0;

    //Peripheral_Descriptor_t xConsole;
    int8_t cRxedChar, cInputIndex = 0;
    portBASE_TYPE xMoreDataToFollow;
    /* The input and output buffers are declared static to keep them off the stack. */
    static int8_t pcOutputString[ MAX_OUTPUT_LENGTH ], pcInputString[ MAX_INPUT_LENGTH ]; //= "task-stats\0";//help\0";

    while(1)
    {
        //take the semaphor, wait forever if it is not ready yet.
        xSemaphoreTake(
           InputByteBuffer,
           portMAX_DELAY
        );

        //get data from global variable by calling UARTGetChar()
        cRxedChar = UARTGetChar();

        //echo it on the tx buffer by queueing a message
        Message2.ucMessage[0] = cRxedChar;
        Message2.ucMessage[1] = 0;

        if( xQueueSendToBack(
                       xUARTQueue, //QueueHandle_t xQueue,
                       &Message2, //const void * pvItemToQueue,
                       0 //TickType_t xTicksToWait
                   ) != pdPASS )
        {
            //task was not able to be created after the xTicksToWait
            //a = 0;
        }

        //only one char is recieved at a time, therefore an "enter" press will send a \r\n, but the \n will be cut off. Going to hack it by making it just look for a \r.
         //if( cRxedChar == '\n' )
         if( cRxedChar == '\r' )
         {
             /*
                //echo a newline
                Message2.ucMessage[0] = '\n';
                Message2.ucMessage[1] = 0;

                if( xQueueSendToBack(
                               xUARTQueue, //QueueHandle_t xQueue,
                               &Message2, //const void * pvItemToQueue,
                               0 //TickType_t xTicksToWait
                           ) != pdPASS )
                {
                    //task was not able to be created after the xTicksToWait
                    //a = 0;
                }
              */
             //pcInputString[ cInputIndex ] = '\n';
             //pcInputString[ cInputIndex + 1 ] = '\0';

             /* A newline character was received, so the input command stirng is
             complete and can be processed. Transmit a line separator, just to
             make the output easier to read. */
             //FreeRTOS_write( xConsole, "\r\n", strlen( "\r\n" );


             /* The command interpreter is called repeatedly until it returns
             pdFALSE. See the ?Implementing a command? documentation for an
             exaplanation of why this is. */
             do
             {
                 /* Send the command string to the command interpreter. Any
                 output generated by the command interpreter will be placed in the
                 pcOutputString buffer. */
                 xMoreDataToFollow = FreeRTOS_CLIProcessCommand
                 (
                    pcInputString, /* The command string.*/
                    pcOutputString, /* The output buffer. */
                    MAX_OUTPUT_LENGTH/* The size of the output buffer. */
                 );
                 /* Write the output generated by the command interpreter to the
                 console. */

                    for(j = 0; j < 49 & pcOutputString[j] != 0; j++)
                    {
                        Message2.ucMessage[j] = pcOutputString[j];
                    }
                    Message2.ucMessage[j] = STATICNULL;
                    if( xQueueSendToBack(
                                           xUARTQueue, //QueueHandle_t xQueue,
                                           &Message2, //const void * pvItemToQueue,
                                           0 //TickType_t xTicksToWait
                                       ) != pdPASS )
                            {
                                //task was not able to be created after the xTicksToWait
                                //a = 0;
                            }
                    if(j > 48)
                    {
                        done = 1;
                    }
                    while(done == 1)
                    {
                        offset = offset + 49;
                        for(j = 0; j < 49 & pcOutputString[j+offset] != 0; j++)
                        {
                            Message2.ucMessage[j] = pcOutputString[j+offset];
                        }
                        if(j < 48)
                        {
                            done = 0;
                            for(; j < 49; j++)
                            {
                                Message2.ucMessage[j] = STATICNULL;
                            }
                        }
                        //Message2.ucMessage[j] = STATICNULL;
                            if( xQueueSendToBack(
                                           xUARTQueue, //QueueHandle_t xQueue,
                                           &Message2, //const void * pvItemToQueue,
                                           0 //TickType_t xTicksToWait
                                       ) != pdPASS )
                            {
                                //task was not able to be created after the xTicksToWait
                                //a = 0;
                            }

                    }

                    offset = 0;

                 //FreeRTOS_write( xConsole, pcOutputString, strlen( pcOutputString ) );
             } while( xMoreDataToFollow != pdFALSE );
             /* All the strings generated by the input command have been sent.
             Processing of the command is complete. Clear the input string ready
             to receive the next command. */
             cInputIndex = 0;


             memset( pcInputString, 0x00, MAX_INPUT_LENGTH );
         }
         else
         {
            if( cRxedChar == 'q' ) // Keyboard ?q? ? Emergency Clear inside car
            {
               pxAllocMessage.button = Q; 
               if( xQueueSendToBack(
                                  xCarMessageQueue, //QueueHandle_t xQueue,
                                  &pxAllocMessage, //const void * pvItemToQueue,
                                  0 //TickType_t xTicksToWait
                              ) != pdPASS )
                   {
                       //task was not able to be created after the xTicksToWait
                       //a = 0;
                   }
            }
            else if( cRxedChar == 'w' ) //Keyboard ?w? ? P1Call DN outside car
            {
               pxAllocMessage.button = W; 
               if( xQueueSendToBack(
                                  xCarMessageQueue, //QueueHandle_t xQueue,
                                  &pxAllocMessage, //const void * pvItemToQueue,
                                  0 //TickType_t xTicksToWait
                              ) != pdPASS )
                   {
                       //task was not able to be created after the xTicksToWait
                       //a = 0;
                   }
            }
            else if( cRxedChar == 'e' ) //Keyboard ?e? ? P1Call UP outside car
            {
                pxAllocMessage.button = E; 
                if( xQueueSendToBack(
                                  xCarMessageQueue, //QueueHandle_t xQueue,
                                  &pxAllocMessage, //const void * pvItemToQueue,
                                  0 //TickType_t xTicksToWait
                              ) != pdPASS )
                   {
                       //task was not able to be created after the xTicksToWait
                       //a = 0;
                   }
            }
            else if( cRxedChar == 'r' ) //Keyboard ?r? ? P2 Call outside car
            {
               pxAllocMessage.button = R; 
               if( xQueueSendToBack(
                                  xCarMessageQueue, //QueueHandle_t xQueue,
                                  &pxAllocMessage, //const void * pvItemToQueue,
                                  0 //TickType_t xTicksToWait
                              ) != pdPASS )
                   {
                       //task was not able to be created after the xTicksToWait
                       //a = 0;
                   }
            }
            else if( cRxedChar == 't' ) //Keyboard ?t? ? GD Call button inside car
            {
               pxAllocMessage.button = T; 
               if( xQueueSendToBack(
                                  xCarMessageQueue, //QueueHandle_t xQueue,
                                  &pxAllocMessage, //const void * pvItemToQueue,
                                  0 //TickType_t xTicksToWait
                              ) != pdPASS )
                   {
                       //task was not able to be created after the xTicksToWait
                       //a = 0;
                   }
            }
            else if( cRxedChar == 'y' ) //Keyboard ?y? ? Emergency Stop inside car
            {
               pxAllocMessage.button = Y; 
               if( xQueueSendToBack(
                                  xCarMessageQueue, //QueueHandle_t xQueue,
                                  &pxAllocMessage, //const void * pvItemToQueue,
                                  0 //TickType_t xTicksToWait
                              ) != pdPASS )
                   {
                       //task was not able to be created after the xTicksToWait
                       //a = 0;
                   }
            }
            else if( cRxedChar == 'u' ) //Keyboard ?u? ? Emergency Clear inside car
            {
               pxAllocMessage.button = U; 
               if( xQueueSendToBack(
                                  xCarMessageQueue, //QueueHandle_t xQueue,
                                  &pxAllocMessage, //const void * pvItemToQueue,
                                  0 //TickType_t xTicksToWait
                              ) != pdPASS )
                   {
                       //task was not able to be created after the xTicksToWait
                       //a = 0;
                   }
            }
            else if( cRxedChar == 'i' ) //Keyboard ?i? ? Door Interference
            {
               pxAllocMessage.button = I; 
               if( xQueueSendToBack(
                                  xCarMessageQueue, //QueueHandle_t xQueue,
                                  &pxAllocMessage, //const void * pvItemToQueue,
                                  0 //TickType_t xTicksToWait
                              ) != pdPASS )
                   {
                       //task was not able to be created after the xTicksToWait
                       //a = 0;
                   }
            }
             else if( cRxedChar == '\r' )
             {
                 /* Ignore carriage returns. */
             }
             else if( cRxedChar == '\b' )
             {
                 /* Backspace was pressed. Erase the last character in the input
                 buffer - if there are any. */
                 if( cInputIndex > 0 )
                 {
                     cInputIndex--;
                     pcInputString[ cInputIndex ] = '\0';
                 }
             }
             else
             {
                 /* A character was entered. It was not a new line, backspace
                 or carriage return, so it is accepted as part of the input and
                 placed into the input buffer. When a \n is entered the complete
                 string will be passed to the command interpreter. */
                 if( cInputIndex < MAX_INPUT_LENGTH )
                 {
                     pcInputString[ cInputIndex ] = cRxedChar;
                     cInputIndex++;
                 }
             }
         }
    }
}

static void buttonTask()
{
    //Message to be sent to xCarMessageQueue
    struct CarMessage message;

    /*We are polling values as not all butons have CN support.
    INT_CN and buttondrv.h and .c are not longer needed.*/
    while(1)
    {

         //1st button - Left most 
         if(!(mPORTDRead() & 0x40))
         {
            vTaskDelay(10 / portTICK_PERIOD_MS);

            if( (mPORTDRead() & 0x40) == 0)
            {
                //Lockout until release
                while( (mPORTDRead() & 0x40) == 0)
                    vTaskDelay(10 / portTICK_PERIOD_MS);

                //P1 button message
                message.button = SW1;
                xQueueSendToBack(xCarMessageQueue, &message, 0);
            }
         }
         //2nd button
         else if(!(mPORTDRead() & 0x80))
         {
            vTaskDelay(10 / portTICK_PERIOD_MS);

            if( (mPORTDRead() & 0x80) == 0)
            {
                //Lockout until release
                while( (mPORTDRead() & 0x80) == 0)
                    vTaskDelay(10 / portTICK_PERIOD_MS);

                //P2 button message
                message.button = SW2;
                xQueueSendToBack(xCarMessageQueue, &message, 0);
            }
         }
         //3rd button
         else if(!(mPORTARead() & 0x80))
         {
            vTaskDelay(10 / portTICK_PERIOD_MS);

            if( (mPORTARead() & 0x80) == 0)
            {
                //Lockout until release
                while( (mPORTARead() & 0x80) == 0)
                     vTaskDelay(10 / portTICK_PERIOD_MS);

                //Door open message
                message.button = SW3;
                xQueueSendToBack(xCarMessageQueue, &message, 0);
            }
        }
         //4th button - right most
        else if(!(mPORTDRead() & 0x2000))
        {
            vTaskDelay(10 / portTICK_PERIOD_MS);

            if( (mPORTDRead() & 0x2000) == 0)
            {
                //Lockout until release
                while( (mPORTDRead() & 0x2000) == 0)
                    vTaskDelay(10 / portTICK_PERIOD_MS);

                //Door close message
                message.button = SW4;
                xQueueSendToBack(xCarMessageQueue, &message, 0);
            }
        }
        //small delay to prevent button from triggering
        vTaskDelay(10 / portTICK_PERIOD_MS);
    }
}

