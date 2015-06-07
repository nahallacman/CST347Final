/* 
 * File:   carControl.h
 * Author: mainuser
 *
 * Created on May 29, 2015, 2:47 AM
 */

#ifndef CARCONTROL_H
#define	CARCONTROL_H

#ifdef	__cplusplus
extern "C" {
#endif

#include <FreeRTOS.h>
#include "task.h"
#include "queue.h"
#include "myTasks.h"
    
enum ButtonPressed {
    NONE,
    Q,
    W,
    E,
    R,
    T,
    Y,
    U,
    I,
    SW1,
    SW2,
    SW3,
    SW4,
    ACCELERATION,
    VELOCITY
};

enum EMERGENCYSTATE {
    NO_EMERGENCY,
    EMERGENCY_STOP,
    EMERGENCY_START,
    EMERGENCY_DONE
};

enum DIRECTION {
    NO_DIRECTION,
    UP,
    DOWN
};
    
struct /*__attribute__ ((packed)) */CarMessage
{
    uint8_t ucMessageID;
    enum ButtonPressed button;
    int8_t Velocity; // these need to be signed!!!
    int8_t Acceleration;
} xCarMessage;
    
void carControlInit(void);

QueueHandle_t xCarMessageQueue;

TaskHandle_t xCarControlHandle;
TaskHandle_t xCarMotionHandle;

enum FLOOR{
    NO_FLOOR,
    GROUND,
    P1,
    P2
};

struct CARCONTROLINFO
{
    enum EMERGENCYSTATE EmergencyState;
    uint8_t MaxVelocty; // = 20; //init to 20
    int8_t CurrentVelocity;
    uint8_t MaxAcceleration; // = 2; //init to 2
    //int8_t CurrentAcceleration;
    enum FLOOR NextFloor;
    enum FLOOR TargetFloor;
    enum FLOOR LastFloor;
    int Height;
    enum DIRECTION Direction;
};

//struct CARCONTROLINFO
//{
//    enum EMERGENCYSTATE EmergencyState;
//    float MaxVelocty; // = 20; //init to 20
//    float CurrentVelocity;
//    float MaxAcceleration; // = 2; //init to 2
//    //int8_t CurrentAcceleration;
//    enum FLOOR NextFloor;
//    enum FLOOR TargetFloor;
//    enum FLOOR LastFloor;
//    float Height;
//    enum DIRECTION Direction;
//};

static int doorState = 0;
static int openDoor = 0;

struct CARCONTROLINFO CarInfo;

int DistanceToAccelerateToStop();

        static const char mFeet[] = " Feet ";
        static const char mMS[] = " ft/s\r\n\0\0";
        static const char mFloorGD[] = "Floor GD\r\n\0\0";
        static const char mFloorP1[] = "Floor P1\r\n\0\0";
        static const char mFloorP2[] = "Floor P2\r\n\0\0";
        static const char mStopped[] = "Stopped\r\n\0\0";
        static const char mMoving[] = "Moving\r\n\0\0";

#ifdef	__cplusplus
}
#endif

#endif	/* CARCONTROL_H */

