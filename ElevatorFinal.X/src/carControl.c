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
    
    int8_t maxVelocty = 20; //init to 20
    int8_t maxAcceleration = 2; //init to 2
    
    int delete_me;
    

    
    while(1)
    {
        //UART handling code
        if(xUARTQueue != 0) // make sure the task isn't null
        {
            //if( uxQueueMessagesWaiting( xUARTQueue ) != 0 ) // see if there are messages waiting
            //{
                if( xQueueReceive( xCarMessageQueue, ( pxRxedMessage ), portMAX_DELAY ) ) // get the messages
                {
                    //extract message contents
                    localButton = pxRxedMessage->button;
                    localVelocity = pxRxedMessage->Velocity;
                    localAcceleration = pxRxedMessage->Acceleration;
                    
                    //keeping the acceleration and velocity max alterations outside the state machines as their input is always accepted
                    if(localButton == ACCELERATION)
                    {
                        //may need to confirm limits here
                        if(pxRxedMessage->Acceleration >= 1 && pxRxedMessage->Acceleration <= 20 )
                        {
                            maxAcceleration = pxRxedMessage->Acceleration;
                        }
                    }
                    else if(localButton == VELOCITY)
                    {
                        //may need to confirm limits here
                        if(pxRxedMessage->Velocity >= 1 && pxRxedMessage->Velocity <= 8)
                        {
                            maxVelocty = pxRxedMessage->Velocity;
                        }
                    }
                    
                    //this switch takes care of the Y and U keyboard key presses
                    switch(EmergencyState)
                    {
                        case NONE: // if there is no emergency, interpret the key presses
                            switch(localButton)
                            {
                                case Q: // Keyboard ?q? ? GD Floor Call outside car
                                    //if elevator not moving
                                    //  go to floor 
                                    //else do nothing
                                    break;
                                case W://Keyboard ?w? ? P1Call DN outside car
                                    break;
                                case E://Keyboard ?e? ? P1Call UP outside car
                                    break;
                                case R: //Keyboard ?r? ? P2 Call outside car
                                    break;
                                case T: //Keyboard ?t? ? GD Call button inside car
                                    break;
                                case Y: //Keyboard ?y? ? Emergency Stop inside car
                                    //START THE EMERGENCY!
                                    //set the emergency state to begin the emergency process
                                        EmergencyState = EMERGENCYSTART;
                                    break;
                                case I: //Keyboard ?i? ? Door Interference
                                    // interrupt the door open or closing sequence. If not in either of those states, ignore.
                                    break;
                                case SW1: //SW1 ? P2 Call button inside car
                                    //if elevator not moving, go to P2
                                    //else ??
                                    break;
                                case SW2: //SW2 ? P1 Call button inside car
                                    //if elevator not moving, go to P1
                                    //else ??
                                    break;
                                case SW3: //SW3 ? Open Door inside car
                                    //if elevator not moving, 
                                    //      if door not open
                                    //          open door
                                    //      else do nothing
                                    //else do nothing
                                    break;
                                case SW4: //SW4 ? Close Door inside car
                                    //if elevator door open
                                    //      close door
                                    //else do nothing
                                    break;
                                default:
                                    //error? ignore the message
                                    delete_me = 0;
                            }
                            break;
                        case EMERGENCYSTART:  // if there is an emergency, ignore all normal key presses
                            // emergency state can not be stopped!
                            //now try to move the car down
                            //if traveling up, deaccelerate
                            //travel to floor 0
                            //open door
                            //then change state to EMERGENCYDONE
                            
                            //since we are in the consumer portion, do nothing
                            break;
                        case EMERGENCYDONE: //once the emergency procedure is complete, wait for the reset button to be pressed
                            //accept no input besides emergency clear
                            if(localButton == U) //Keyboard ?u? ? Emergency Clear inside car
                            {
                                //set the emergency state to begin the emergency process
                                EmergencyState = NONE;
                            }
                            break;
                    }

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
   
   EmergencyState = NONE;
}

static void taskCarMotion(void *pvParameters)
{
    while(1)
    {
        //this switch allows for the emergency state to be moved between the CarContol task and this CarMotion task
        switch(EmergencyState)
        {
            case NONE:
                //wait for emergency input
                //does nothing here
                break;
            case EMERGENCYSTART: // emergency state can not be stopped!
                //now try to move the car down
                //if traveling up, deaccelerate
                //travel to floor 0
                //open door
                //then change state to EMERGENCYDONE
                break;
            case EMERGENCYDONE:
                //accept no input besides emergency clear
                //does nothing here
                break;
        }
        
        
        
        //execute once every second to make calculation easy
        vTaskDelay(1000);
    }
    
    
}