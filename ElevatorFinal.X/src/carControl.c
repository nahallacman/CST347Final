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
                            CarInfo.MaxAcceleration = pxRxedMessage->Acceleration;
                        }
                    }
                    else if(localButton == VELOCITY)
                    {
                        //may need to confirm limits here
                        if(pxRxedMessage->Velocity >= 1 && pxRxedMessage->Velocity <= 8)
                        {
                            CarInfo.MaxVelocty = pxRxedMessage->Velocity;
                        }
                    }
                    
                    //this switch takes care of the Y and U keyboard key presses
                    switch(CarInfo.EmergencyState)
                    {
                        case NO_EMERGENCY: // if there is no emergency, interpret the key presses
                            switch(localButton)
                            {
                                case Q: // Keyboard ?q? ? GD Floor Call outside car
                                    //if target floor not ground floor
                                    //  set next floor to ground floor 
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
                                        CarInfo.EmergencyState = EMERGENCY_START;
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
                        case EMERGENCY_START:  // if there is an emergency, ignore all normal key presses
                            // emergency state can not be stopped!
                            //now try to move the car down
                            //if traveling up, deaccelerate
                            //travel to floor 0
                            //open door
                            //then change state to EMERGENCYDONE
                            
                            //since we are in the consumer portion, do nothing
                            break;
                        case EMERGENCY_DONE: //once the emergency procedure is complete, wait for the reset button to be pressed
                            //accept no input besides emergency clear
                            if(localButton == U) //Keyboard ?u? ? Emergency Clear inside car
                            {
                                //set the emergency state to begin the emergency process
                                CarInfo.EmergencyState = NO_EMERGENCY;
                            }
                            break;
                    }

                }
           // }
       }
    }  
}

static void taskCarMotion(void *pvParameters)
{
    int SpeedUpSlowDown = 1; //if this = 1, we are speeding up, if it equals -1 we are slowing down
    int targetHeight = 0;
    while(1)
    {
        //this switch allows for the emergency state to be moved between the CarContol task and this CarMotion task
        //could make it so there is only EMERGENCY_START and default, that way stuff only happens when the emergency is actually activated
        switch(CarInfo.EmergencyState)
        {
            case NO_EMERGENCY:
                //wait for emergency input
                //does nothing here
                break;
            case EMERGENCY_START: // emergency state can not be stopped!
                //if door not closed, close the door
                //else if traveling up, deaccelerate
                //now try to move the car down
                //travel to floor 0
                CarInfo.TargetFloor = GROUND;
                //open door
                //then change state to EMERGENCY_DONE
                break;
            case EMERGENCY_DONE:
                //accept no input besides emergency clear
                //does nothing here
                break;
        }
        
        //if we are moving?
        //takes a target floor, and a last floor, and moves from the last floor to the target floor
        switch(CarInfo.TargetFloor)
        {
            case GROUND: // car is always traveling down
                targetHeight = 0;
                if(CarInfo.Height > 0 + DistanceToAccelerateToStop()) // 0 + Distance to negatively accelerate
                {
                    CarInfo.Height -= CarInfo.CurrentVelocity;
                }
                else
                {
                    //done moving
                }
                break;
            case P1:
                targetHeight = 500;
                switch(CarInfo.LastFloor)
                {
                    case GROUND:
                        //going UP
                        if(CarInfo.Height < 500 - DistanceToAccelerateToStop()) // 500 - Distance to negatively accelerate
                        {
                            CarInfo.Height += CarInfo.CurrentVelocity;
                            SpeedUpSlowDown = 1;
                        }
                        else
                        {
                            //need something for reduction in acceleration to slow down for floor
                            CarInfo.Height += CarInfo.CurrentVelocity;
                            SpeedUpSlowDown = -1;
                        }
                        break;
                    case P1:
                        //error, last floor and target floor are equal
                        break;
                    case P2:
                        //going down // special case of only moving 1 floor
                        if(CarInfo.Height > 505 - DistanceToAccelerateToStop()) // use negative acceleration when halfway to slow down // 505 - Distance to negatively accelerate
                        {
                            CarInfo.Height -= CarInfo.CurrentVelocity;
                            SpeedUpSlowDown = 1;
                        }
                        else
                        {
                            CarInfo.Height -= CarInfo.CurrentVelocity;
                            SpeedUpSlowDown = -1;
                        }
                        break;
                }
                break;
            case P2:
                targetHeight = 510;
                switch(CarInfo.LastFloor)
                {
                    case GROUND:
                        //going UP
                        if(CarInfo.Height < 510 - DistanceToAccelerateToStop()) //510 - Distance to negatively accelerate
                        {
                            CarInfo.Height += CarInfo.CurrentVelocity;
                            SpeedUpSlowDown = 1;
                        }
                        //what about when the car is slowing down for the ground floor?
                        break;
                    case P1:
                        //going UP // special case of only moving 1 floor
                        if(CarInfo.Height < 505 - DistanceToAccelerateToStop()) // 505 - Distance to negatively accelerate
                        {
                            CarInfo.Height += CarInfo.CurrentVelocity;
                            SpeedUpSlowDown = 1;
                        }
                        else
                        {
                            CarInfo.Height += CarInfo.CurrentVelocity;
                            SpeedUpSlowDown = -1;
                        }
                        break;
                    case P2:
                        //error, last floor and target floor are equal
                        break;
                }
                break;
        }
        
        
        //if we reach the target floor
        if(CarInfo.Height == targetHeight)
        {
            CarInfo.LastFloor = CarInfo.TargetFloor; //set last floor to target floor
            if(CarInfo.NextFloor != NO_FLOOR) //set target floor to next floor
            {
                CarInfo.TargetFloor = CarInfo.NextFloor;
                //targetHeight //need to decode the next floor value into a height value
            }
            CarInfo.NextFloor = NO_FLOOR; //set next floor to NO_FLOOR
            
            //go through open door process
            
        }
        
        //increase current velocity until max
        //happens once every second 
        if(SpeedUpSlowDown == 1)
        {
            if(CarInfo.CurrentVelocity < CarInfo.MaxVelocty)
            {
                CarInfo.CurrentVelocity += (CarInfo.MaxAcceleration / 2); // 1/2 acceleration since 500ms period
            }
        }
        else
        {
            if(CarInfo.CurrentVelocity > 0)
            {
                CarInfo.CurrentVelocity -= (CarInfo.MaxAcceleration / 2); // 1/2 acceleration since 500ms period
            }
        }

        struct UARTMessage *pxRxedMessage;
        struct UARTMessage Message2;
        pxRxedMessage = &Message2;
        int j = 0;
        char test;
        char str1 [1000];
        int value = CarInfo.CurrentVelocity;
        //SEND UPDATE DISTANCE MESSAGE HERE
        //itoa(value, str1, 10); //this crashes the system, not sure why
        Message2.ucMessage[0] = '1'; //?n Feet :: m ft/s?
        Message2.ucMessage[1] = ' ';
        Message2.ucMessage[2] = 'F';
        Message2.ucMessage[3] = 'e';
        Message2.ucMessage[4] = 'e';
        Message2.ucMessage[5] = 't';
        Message2.ucMessage[6] = ' ';
        Message2.ucMessage[7] = '1';
        Message2.ucMessage[8] = ' ';
        Message2.ucMessage[9] = 'm';
        Message2.ucMessage[10] = '/';
        Message2.ucMessage[11] = 's';
        Message2.ucMessage[11] = '\r';
        Message2.ucMessage[11] = '\n';
        Message2.ucMessage[12] = '\0';
        
        if( xQueueSendToBack(
               xUARTQueue, //QueueHandle_t xQueue,
               &Message2, //const void * pvItemToQueue,
               0 //TickType_t xTicksToWait
           ) != pdPASS )
        {
            //task was not able to be created after the xTicksToWait
            //a = 0;
        }
        
        //execute once every half second to make calculation easy
        vTaskDelay(500);
    }
    
    
}

int DistanceToAccelerateToStop()
{
    int localCurVel = CarInfo.CurrentVelocity;
    int localMaxAcc = CarInfo.MaxAcceleration;
    int numSeconds = 0;
    int displacement = 0;
    int avg_speed = 0;
    
    numSeconds = localCurVel / localMaxAcc;
    displacement = ( 0 - (localCurVel * localCurVel) ) / 2 * localMaxAcc; // this may need to use the absolute value instead of possibly negative
    return displacement;
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
        CARCONTROLPRIORITY,
        &xCarControlHandle);
    configASSERT( &xCarControlHandle );
 
    xTaskCreate(taskCarMotion,
        "CarMotion",
        configMINIMAL_STACK_SIZE,
        (void *) &xTask0Parameters,
        CARMOTIONPRIORITY,
        &xCarMotionHandle);
    configASSERT( &xCarMotionHandle );
   
   
   CarInfo.EmergencyState = NO_EMERGENCY;
   CarInfo.LastFloor = GROUND;
   CarInfo.TargetFloor = NO_FLOOR; // use this as the next floor to go to
   CarInfo.NextFloor = NO_FLOOR; // use this as a secondary floor to go to AFTER going to target floor. May be unnecessary
   CarInfo.MaxAcceleration = 2;
   CarInfo.CurrentAcceleration = 0;
   CarInfo.MaxVelocty = 20;
   CarInfo.CurrentVelocity = 0;
   CarInfo.Height = 0;
}