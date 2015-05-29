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
    SW1,
    SW2,
    SW3,
    SW4
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


#ifdef	__cplusplus
}
#endif

#endif	/* CARCONTROL_H */

