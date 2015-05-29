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
    
struct /*__attribute__ ((packed)) */SpeedMessage
{
    uint8_t ucMessageID;
    int8_t Velocity; // these need to be signed!!!
    int8_t Acceleration;
} xSpeedMessage;
    
void carControlInit(void);

QueueHandle_t xSpeedQueue;

TaskHandle_t xCarControlHandle;


#ifdef	__cplusplus
}
#endif

#endif	/* CARCONTROL_H */

