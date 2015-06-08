#include "myTasks.h"
#include "myTasks.c"
#include "motorControl.h"

static void taskMotorControl(void *pvParameters)
{
    xTaskParameter_t *pxTaskParameter;
    //portTickType xStartTime;

    /* The parameter points to an xTaskParameters_t structure. */
    pxTaskParameter = (xTaskParameter_t *) pvParameters;
    
    //pxRxedMessage = &Message2;

    uint8_t previousVelocity = 0;
    float blink_rate = 0;

    while (1)
    {
        if ( CarInfo.CurrentVelocity == 0 )     //Elevator not moving (0 ft/s)
        {
            //No blinking
            setLED(4,0);
            while (CarInfo.CurrentVelocity == 0);            
        }
        else
        {      

            if ( previousVelocity != CarInfo.CurrentVelocity )     //Elevator moving
            {
                //Blink according to 1Hz per 10 ft/s
                toggleLED(pxTaskParameter->usLEDNumber);
                previousVelocity = CarInfo.CurrentVelocity;
                blink_rate = (10 / (float)previousVelocity);
                vTaskDelay( blink_rate * 1000 );
            }
            else if ( previousVelocity == CarInfo.MaxVelocty )          //Elevator at max velocity
            {
                toggleLED(pxTaskParameter->usLEDNumber);
                //Maximum blink (may just appear solid)
                vTaskDelay( 500 / portTICK_RATE_MS );
            }
        }
    }
}

void motorControlInit(void)
{
    //Setup Motor task
    xTaskCreate(taskMotorControl,
        "MotorSpeed",
        configMINIMAL_STACK_SIZE,
        (void *) &xTask4Parameters,
        MOTORCONTROLPRIORITY,
        &xMotorControlHandle);
    configASSERT( &xMotorControlHandle );
}
