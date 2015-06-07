/* 
 * File:   motorControl.h
 * Author: Zachary
 *
 * Created on June 6, 2015, 12:04 PM
 */

#ifndef MOTORCONTROL_H
#define	MOTORCONTROL_H

#ifdef	__cplusplus
extern "C" {
#endif
    
//#include <FreeRTOS.h>
//#include "task.h"
//#include "queue.h"
#include "myTasks.h"
#include "carControl.h"

void motorControlInit(void);

TaskHandle_t xMotorControlHandle;

#ifdef	__cplusplus
}
#endif

#endif	/* MOTORCONTROL_H */

