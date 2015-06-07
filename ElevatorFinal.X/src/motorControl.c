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
        else if ( previousVelocity != CarInfo.CurrentVelocity )     //Elevator moving
        {
            //Blink according to 1Hz per 10 ft/s
            previousVelocity = CarInfo.CurrentVelocity;
            blink_rate = ( (10 / previousVelocity) * 1000);
            vTaskDelay( blink_rate / portTICK_PERIOD_MS );
        }
        else if ( previousVelocity == CarInfo.MaxVelocty )          //Elevator at max velocity
        {
            //Maximum blink (may just appear solid)
            blink_rate = ( (10 / CarInfo.MaxVelocty) * 1000);
            vTaskDelay( blink_rate / portTICK_PERIOD_MS );
        }

            toggleLED(pxTaskParameter->usLEDNumber);
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
