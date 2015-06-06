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
                                    //if target floor not set
                                    if(CarInfo.TargetFloor == NO_FLOOR)
                                    {   //set target floor to ground floor
                                        CarInfo.TargetFloor = GROUND;  
                                    }
                                    else if(CarInfo.TargetFloor != GROUND) //if target floor set
                                    {   //set next floor to ground
                                        CarInfo.NextFloor = GROUND;
                                    }
                                    break;
                                case W://Keyboard ?w? ? P1Call DN outside car
                                    //if target floor not set
                                    if(CarInfo.TargetFloor == NO_FLOOR)
                                    {   //set target floor to ground floor
                                        CarInfo.TargetFloor = P1;  
                                    }
                                    else if(CarInfo.TargetFloor != P1)
                                    {
                                        CarInfo.NextFloor = P1;
                                    }
                                    break;
                                case E://Keyboard ?e? ? P1Call UP outside car
                                    //if target floor not set
                                    if(CarInfo.TargetFloor == NO_FLOOR)
                                    {   //set target floor to ground floor
                                        CarInfo.TargetFloor = P1;  
                                    }
                                    else if(CarInfo.TargetFloor != P1)
                                    {
                                        CarInfo.NextFloor = P1;
                                    }
                                    break;
                                case R: //Keyboard ?r? ? P2 Call outside car
                                    //if target floor not set
                                    if(CarInfo.TargetFloor == NO_FLOOR)
                                    {   //set target floor to ground floor
                                        CarInfo.TargetFloor = P2;  
                                    }
                                    else if(CarInfo.TargetFloor != P2)
                                    {
                                        CarInfo.NextFloor = P2;
                                    }
                                    break;
                                case T: //Keyboard ?t? ? GD Call button inside car
                                    //if target floor not set
                                    if(CarInfo.TargetFloor == NO_FLOOR)
                                    {   //set target floor to ground floor
                                        CarInfo.TargetFloor = GROUND;  
                                    }
                                    else if(CarInfo.TargetFloor != GROUND) //if target floor set
                                    {   //set next floor to ground
                                        CarInfo.NextFloor = GROUND;
                                    }
                                    break;
                                case Y: //Keyboard ?y? ? Emergency Stop inside car
                                    //START THE EMERGENCY!
                                    //set the emergency state to begin the emergency process
                                        CarInfo.EmergencyState = EMERGENCY_START;
                                    break;
                                case I: //Keyboard ?i? ? Door Interference
                                    // interrupt the door closing sequence. If not in either of those states, error? ignore for now
                                    break;
                                case SW1: //SW1 ? P2 Call button inside car
                                    //if target floor not set
                                    if(CarInfo.TargetFloor == NO_FLOOR)
                                    {   //set target floor to ground floor
                                        CarInfo.TargetFloor = P2;  
                                    }
                                    else if(CarInfo.TargetFloor != P2)
                                    {
                                        CarInfo.NextFloor = P2;
                                    }
                                    break;
                                case SW2: //SW2 ? P1 Call button inside car
                                    if(CarInfo.TargetFloor == NO_FLOOR)
                                    {   //set target floor to ground floor
                                        CarInfo.TargetFloor = P1;  
                                    }
                                    else if(CarInfo.TargetFloor != P1)
                                    {
                                        CarInfo.NextFloor = P1;
                                    }
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
                            
                            //since we are in the consumer portion, do no actual code here, this is just a placeholder for notes
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
    //---- variables for update every 500ms -----
    //all of this stuff is for the message every 500ms
        struct UARTMessage *pxRxedMessage;
        struct UARTMessage Message2;
        pxRxedMessage = &Message2;
        int j = 0;
        int k = 0;
        int length = 0;
        int length2 = 0;
        char test;
        char str1 [3];
        char str2 [3];
        static const char mFeet[] = " Feet ";
        static const char mMS[] = " ft/s\r\n\0\0";
        int value = CarInfo.Height;
        int value2 = CarInfo.CurrentVelocity;
    
    //---- end variables for update every 500ms -----
    
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
                //else if traveling up, deaccelerate to 0 and reverse
                if(CarInfo.CurrentVelocity > 0 && CarInfo.Direction == UP )
                {
                    SpeedUpSlowDown = -1;
                }
                //when we stop moving, go the other direction as fast as possible
                else if(CarInfo.CurrentVelocity == 0)
                {
                    CarInfo.TargetFloor = GROUND;
                    //SpeedUpSlowDown = 1;
                }
                //open door when we get to the bottom
                if(CarInfo.Height == targetHeight)
                {
                    //open door
                    //then change state to EMERGENCY_DONE
                    CarInfo.EmergencyState = EMERGENCY_DONE;
                }
                
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
                CarInfo.Direction = DOWN;
                if(CarInfo.Height > DistanceToAccelerateToStop()) // 0 + Distance to negatively accelerate
                //if(CarInfo.Height > 210)
                {
                    SpeedUpSlowDown = 1; //go to max speed while traveling
                    CarInfo.Height -= CarInfo.CurrentVelocity / 2;
                }
                else
                {
                    //finish by reducing acceleration until we hit the ground floor
                    SpeedUpSlowDown = -1;
                    CarInfo.Height -= CarInfo.CurrentVelocity / 2;
                    //done moving when we hit height = 0
                }
                break;
            case P1:
                targetHeight = 500;
                switch(CarInfo.LastFloor)
                {
                    case GROUND:
                        //going UP
                        CarInfo.Direction = UP;
                                                //if(CarInfo.Height < 290)
                        if(CarInfo.Height < (500 - DistanceToAccelerateToStop())) /*&& !(SpeedUpSlowDown == -1)*/ // 500 - Distance to negatively accelerate
                        {
                            CarInfo.Height += CarInfo.CurrentVelocity / 2;
                            SpeedUpSlowDown = 1;
                        }
                        else
                        {
                            //finish by reducing acceleration until we hit the ground floor
                            CarInfo.Height += CarInfo.CurrentVelocity / 2;
                            SpeedUpSlowDown = -1;
                        }
                        break;
                    case P1:
                        //error, last floor and target floor are equal
                        break;
                    case P2:
                        //going down // special case of only moving 1 floor
                        CarInfo.Direction = DOWN;
                        if(CarInfo.Height > ( 500 + DistanceToAccelerateToStop())) // use negative acceleration when halfway to slow down // 505 - Distance to negatively accelerate
                        {
                            CarInfo.Height -= CarInfo.CurrentVelocity / 2;
                            SpeedUpSlowDown = 1;
                        }
                        else
                        {//finish by reducing acceleration until we hit the ground floor
                            CarInfo.Height -= CarInfo.CurrentVelocity / 2;
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
                        CarInfo.Direction = UP;
                        if(CarInfo.Height < 510 - DistanceToAccelerateToStop()) //510 - Distance to negatively accelerate
                        {
                            CarInfo.Height += CarInfo.CurrentVelocity / 2;
                            SpeedUpSlowDown = 1;
                        }
                        else
                        {   //finish by reducing acceleration until we hit the ground floor
                            CarInfo.Height += CarInfo.CurrentVelocity / 2;
                            SpeedUpSlowDown = -1;
                        }
                        //what about when the car is slowing down for the ground floor?
                        break;
                    case P1:
                        //going UP // special case of only moving 1 floor
                        CarInfo.Direction = UP;
                        if(CarInfo.Height < 510 - DistanceToAccelerateToStop()) // 505 - Distance to negatively accelerate
                        {
                            CarInfo.Height += CarInfo.CurrentVelocity / 2;
                            SpeedUpSlowDown = 1;
                        }
                        else
                        {//finish by reducing acceleration until we hit the ground floor
                            CarInfo.Height += CarInfo.CurrentVelocity / 2;
                            SpeedUpSlowDown = -1;
                        }
                        break;
                    case P2:
                        //error, last floor and target floor are equal
                        break;
                }
                break;
            default:
                CarInfo.Direction = NO_DIRECTION;
                SpeedUpSlowDown = 0;
                
        }
        
        if(CarInfo.Direction != NO_DIRECTION) // no height comparisons if we are not moving
        {
            if(CarInfo.Direction == UP)
            {
                if(CarInfo.Height > targetHeight)
                {
                    //we overshot!
                    //make sure we go back to targetHeight
                    SpeedUpSlowDown = -1;
                    CarInfo.Height -= 1;
                    //CarInfo.CurrentVelocity = 1;
                }
                else if(CarInfo.Height == targetHeight)
                {
                    SpeedUpSlowDown = 0;
                    CarInfo.LastFloor = CarInfo.TargetFloor; //set last floor to target floor
                    if(CarInfo.NextFloor != NO_FLOOR) //set target floor to next floor
                    {
                        CarInfo.TargetFloor = CarInfo.NextFloor;
                    }
                    else
                    {
                        CarInfo.TargetFloor = NO_FLOOR;
                    }
                    CarInfo.NextFloor = NO_FLOOR; //set next floor to NO_FLOOR

                    //go through open door process

                    //should wait until door closes before setting TargetFloor
                    //probably requires a state machine here
                }
            }
            else if(CarInfo.Direction == DOWN)
            {
                if(CarInfo.Height < targetHeight)
                {
                    //we overshot!
                    //make sure we go back to targetHeight
                    //make sure we go back to targetHeight
                    SpeedUpSlowDown = -1;
                    CarInfo.Height += 1;
                    //CarInfo.CurrentVelocity = 1;
                }
                else if(CarInfo.Height == targetHeight)
                {
                    CarInfo.LastFloor = CarInfo.TargetFloor; //set last floor to target floor
                    if(CarInfo.NextFloor != NO_FLOOR) //set target floor to next floor
                    {
                        CarInfo.TargetFloor = CarInfo.NextFloor;
                    }
                    else
                    {
                        CarInfo.TargetFloor = NO_FLOOR;
                    }
                    CarInfo.NextFloor = NO_FLOOR; //set next floor to NO_FLOOR

                    //go through open door process

                    //should wait until door closes before setting TargetFloor
                    //probably requires a state machine here
                }
            }
        }
        
        //increase current velocity until max
        //happens once every half second 
        if(SpeedUpSlowDown == 1) //accelerating positively
        {
            if(CarInfo.CurrentVelocity < CarInfo.MaxVelocty)
            {
                CarInfo.CurrentVelocity += (CarInfo.MaxAcceleration / 2); // 1/2 acceleration since 500ms period
            }
        }
        else if(SpeedUpSlowDown == -1) //accelerating negatively to 0
        {
            if(CarInfo.CurrentVelocity > 0)
            {
                CarInfo.CurrentVelocity -= (CarInfo.MaxAcceleration / 2); // 1/2 acceleration since 500ms period
            }
            else // you never accelerate from one direction to the other without giving a break on 0 to switch directions
            {
                CarInfo.CurrentVelocity = 0;
            }
        }
        else if(SpeedUpSlowDown == 0)
        {
            //no acceleration since we are not moving!
            CarInfo.CurrentVelocity = 0;
        }

        value = CarInfo.Height;
        value2 = CarInfo.CurrentVelocity;
        //SEND UPDATE DISTANCE MESSAGE HERE
        itoa(str1, value, 10); 
        itoa(str2, value2, 10);
        for(length = 0; str1[length] != 0; length++)//strlen()
        {
            Message2.ucMessage[length] = str1[length];
        }
        for(j = length; j < length + 6; j++)
        {
            Message2.ucMessage[j] = mFeet[j - length];
        }
        for(length2 = j ; str2[length2 - j] != 0; length2++)
        {
            Message2.ucMessage[length2] = str2[length2 - j];
        }
        for(k = length2; k < length2 + 8; k++)
        {
            Message2.ucMessage[k] = mMS[k - length2];
        }       

        
        if( xQueueSendToBack(
               xUARTQueue, //QueueHandle_t xQueue,
               &Message2, //const void * pvItemToQueue,
               0 //TickType_t xTicksToWait
           ) != pdPASS )
        {
            //task was not able to be created after the xTicksToWait
            //a = 0;
        }
        
        
        
        //execute once every half second to make calculation and output easy
        vTaskDelay(500);
    }
    
    
}

int DistanceToAccelerateToStop()
{
    float localCurVel = CarInfo.CurrentVelocity;
    float localMaxAcc = CarInfo.MaxAcceleration;
    int numSeconds = 0;
    float displacement = 0;
    int avg_speed = 0;
    int ret_val = 0;
    
    numSeconds = localCurVel / localMaxAcc;
    displacement = (((localCurVel /2)  * (localCurVel/2)) ) / 2 * (localMaxAcc); // this may need to use the absolute value instead of possibly negative
    //displacement = ( localCurVel / 2 ) * ( localCurVel / localMaxAcc);
    ret_val = (int) displacement;
    return ret_val;
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
   //CarInfo.CurrentAcceleration = 0;
   CarInfo.MaxVelocty = 20;
   CarInfo.CurrentVelocity = 0;
   CarInfo.Height = 0;
}