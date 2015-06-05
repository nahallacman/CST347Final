/* Note: To use the VTaskList Command you must modify the FreeRTOSConfig.h
 * to change #define configUSE_TRACE_FACILITY	0 to be a 1 */

#include "FreeRTOS.h"

#include "../../FreeRTOS-Plus-CLI/FreeRTOS_CLI.h"
#include "task.h"

#include "myCLICMD.h"
//#include "myTasks.h"
#include "myTasks.c"
#include "myTasks.h"

static const char taskListHdr[] = "\nName\tStat\tPri\tSpace\tTCB\r\n";

/* This function implements the behaviour of a command, so must have the correct
prototype. */
static portBASE_TYPE prvTaskStatsCommand( int8_t *pcWriteBuffer,
 size_t xWriteBufferLen,
const int8_t *pcCommandString )
{
 /* For simplicity, this function assumes the output buffer is large enough
 to hold all the text generated by executing the vTaskList() API function,
 so the xWriteBufferLen parameter is not used. */
 ( void ) xWriteBufferLen;
 /* pcWriteBuffer is used directly as the vTaskList() parameter, so the table
 generated by executing vTaskList() is written directly into the output
 buffer. */
 //vTaskList( pcWriteBuffer );
    sprintf(pcWriteBuffer, taskListHdr);
    pcWriteBuffer += strlen(taskListHdr);
    vTaskList(pcWriteBuffer);

 /* The entire table was written directly to the output buffer. Execution
 of this command is complete, so return pdFALSE. */
 return pdFALSE;
}

static const xCommandLineInput xTaskStatsCommand = {"task-stats",
            "task-stats:\r\n Displays a table of task state information\r\n",
            prvTaskStatsCommand,
            0};


/* This function implements the behaviour of a command, so must have the correct
prototype. */
/**
 * [S n] Change Maximum Speed in ft/s. 
 * @param pcWriteBuffer
 * @param xWriteBufferLen
 * @param pcCommandString
 * @return
 *  
 * The CLI command structure to implement the S command. 
 */
static portBASE_TYPE prvCommandS( int8_t *pcWriteBuffer,
 size_t xWriteBufferLen,
const int8_t *pcCommandString )
{
struct CarMessage pxAllocMessage;
int8_t localVelocity = 0;
int8_t localAccleration = 0 ;
enum ButtonPressed localButton = NONE;

 int8_t *pcParameter1;
portBASE_TYPE xParameter1StringLength;
 /* Obtain the name of the source file, and the length of its name, from
 the command string. The name of the source file is the first parameter. */
 pcParameter1 = FreeRTOS_CLIGetParameter
 (
 /* The command string itself. */
pcCommandString,
 /* Return the first parameter. */
 1,
/* Store the parameter string length. */
&xParameter1StringLength
 );

 localVelocity = atoi(pcParameter1);
 
//check LED range
if(localVelocity >= -20 && localVelocity <= 20)
{
    snprintf( pcWriteBuffer, xWriteBufferLen, "S n successfully ran\r\n\r\n" );
    pxAllocMessage.button = VELOCITY;
    pxAllocMessage.Velocity = localVelocity;
    pxAllocMessage.Acceleration = localAccleration;
    
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
else
{
    snprintf( pcWriteBuffer, xWriteBufferLen, "Error invalid speed argument. Valid range: -20 <= n <= 20 m/s\r\n\r\n" );
}
 /* The entire table was written directly to the output buffer. Execution
 of this command is complete, so return pdFALSE. */
 return pdFALSE;
}

static const xCommandLineInput xCommandS = {"S",
            "S: max_speed \r\n Change Maximum Speed in ft/s \r\n",
            prvCommandS,
            1};

static portBASE_TYPE prvCommandAP( int8_t *pcWriteBuffer,
 size_t xWriteBufferLen,
const int8_t *pcCommandString )
{
struct CarMessage pxAllocMessage;
int8_t localVelocity = 0;
int8_t localAccleration = 0 ;
enum ButtonPressed localButton = NONE;

int8_t *pcParameter1;
portBASE_TYPE xParameter1StringLength;
 /* Obtain the name of the source file, and the length of its name, from
 the command string. The name of the source file is the first parameter. */
 pcParameter1 = FreeRTOS_CLIGetParameter
 (
 /* The command string itself. */
pcCommandString,
 /* Return the first parameter. */
 1,
/* Store the parameter string length. */
&xParameter1StringLength
 );

 
  /* For simplicity, this function assumes the output buffer is large enough
 to hold all the text generated by executing the vTaskList() API function,
 so the xWriteBufferLen parameter is not used. */    
//( void ) xWriteBufferLen;
 
localAccleration = atoi(pcParameter1);
 
//check acceleration range
if(localAccleration >= 1 && localAccleration <= 8)
{
    snprintf( pcWriteBuffer, xWriteBufferLen, "AP n successfully ran\r\n\r\n" );
    pxAllocMessage.button = ACCELERATION;
    pxAllocMessage.Velocity = localVelocity;
    pxAllocMessage.Acceleration = localAccleration;
    
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
else
{
    snprintf( pcWriteBuffer, xWriteBufferLen, "Error invalid acceleration argument. Valid range: 1 <= n <= 8 m/s2\r\n\r\n" );
}
 /* The entire table was written directly to the output buffer. Execution
 of this command is complete, so return pdFALSE. */
 return pdFALSE;
}

static const xCommandLineInput xCommandAP = {"AP",
            "AP: acceleration \r\n Change Max Acceleration in ft/s2 \r\n",
            prvCommandAP,
            1};


static portBASE_TYPE prvCommandSF( int8_t *pcWriteBuffer,
 size_t xWriteBufferLen,
const int8_t *pcCommandString )
{
struct CarMessage pxAllocMessage;
int8_t localVelocity = 0;
int8_t localAccleration = 0 ;
enum ButtonPressed localButton = NONE;
int8_t temp = 0;
 int8_t *pcParameter1;
portBASE_TYPE xParameter1StringLength;
 /* Obtain the name of the source file, and the length of its name, from
 the command string. The name of the source file is the first parameter. */
 pcParameter1 = FreeRTOS_CLIGetParameter
 (
 /* The command string itself. */
pcCommandString,
 /* Return the first parameter. */
 1,
/* Store the parameter string length. */
&xParameter1StringLength
 );

 
  /* For simplicity, this function assumes the output buffer is large enough
 to hold all the text generated by executing the vTaskList() API function,
 so the xWriteBufferLen parameter is not used. */    
//( void ) xWriteBufferLen;
 
    //assign message memory
    //pxRxedMessage2 = &pxAllocMessage;

 temp = atoi(pcParameter1);
 //delayLength = atoi(pcParameter2);
 
//check LED range
//if(LEDnum >= 0 & LEDnum <= 2)
if(temp == 1 || temp == 2 || temp == 3)
{
    snprintf( pcWriteBuffer, xWriteBufferLen, "SF n successfully ran\r\n\r\n" );
    switch(temp)
    {
        case 1: // Keyboard ?t? ? GD Call button inside car
            pxAllocMessage.button = T;
            break;
        case 2: //SW2 ? P1 Call button inside car
            pxAllocMessage.button = SW2;
            break;
        case 3: //SW1 ? P2 Call button inside car
            pxAllocMessage.button = SW1;
            break;
    }
    

    pxAllocMessage.Velocity = localVelocity;
    pxAllocMessage.Acceleration = localAccleration;
    
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
else
{
    snprintf( pcWriteBuffer, xWriteBufferLen, "Error invalid floor argument. Valid range: 1 <= n <= 3\r\n\r\n" );
}
 /* The entire table was written directly to the output buffer. Execution
 of this command is complete, so return pdFALSE. */
 return pdFALSE;
}

static const xCommandLineInput xCommandSF = {"SF",
            "SF: 1/2/3 \r\n Send to floor # \r\n",
            prvCommandSF,
            1};

static portBASE_TYPE prvCommandES( int8_t *pcWriteBuffer,
 size_t xWriteBufferLen,
const int8_t *pcCommandString )
{
struct CarMessage pxAllocMessage;
int8_t localVelocity = 0;
int8_t localAccleration = 0 ;
enum ButtonPressed localButton = NONE;
 
    pxAllocMessage.button = Y; // Keyboard ?y? ? Emergency Stop inside car
    pxAllocMessage.Velocity = localVelocity;
    pxAllocMessage.Acceleration = localAccleration;
    
    if( xQueueSendToBack(
                       xCarMessageQueue, //QueueHandle_t xQueue,
                       &pxAllocMessage, //const void * pvItemToQueue,
                       0 //TickType_t xTicksToWait
                   ) != pdPASS )
        {
            //task was not able to be created after the xTicksToWait
            //a = 0;
        }
    
    snprintf( pcWriteBuffer, xWriteBufferLen, "ES successfully ran\r\n\r\n" );


 /* The entire table was written directly to the output buffer. Execution
 of this command is complete, so return pdFALSE. */
 return pdFALSE;
}

static const xCommandLineInput xCommandES = {"ES",
            "ES: \r\n Emergency Stop (identical to Emergency Stop Button) \r\n",
            prvCommandES,
            0};

static portBASE_TYPE prvCommandER( int8_t *pcWriteBuffer,
 size_t xWriteBufferLen,
const int8_t *pcCommandString )
{
struct CarMessage pxAllocMessage;
int8_t localVelocity = 0;
int8_t localAccleration = 0 ;
enum ButtonPressed localButton = NONE;
 
    pxAllocMessage.button = U; // Keyboard ?u? ? Emergency Clear inside car
    pxAllocMessage.Velocity = localVelocity;
    pxAllocMessage.Acceleration = localAccleration;
    
    if( xQueueSendToBack(
                       xCarMessageQueue, //QueueHandle_t xQueue,
                       &pxAllocMessage, //const void * pvItemToQueue,
                       0 //TickType_t xTicksToWait
                   ) != pdPASS )
        {
            //task was not able to be created after the xTicksToWait
            //a = 0;
        }
    
    snprintf( pcWriteBuffer, xWriteBufferLen, "ER successfully ran\r\n\r\n" );


 /* The entire table was written directly to the output buffer. Execution
 of this command is complete, so return pdFALSE. */
 return pdFALSE;
}

static const xCommandLineInput xCommandER = {"ER",
            "ER: \r\n Emergency Clear (identical to Emergency Clear Button) \r\n",
            prvCommandER,
            0};

void myCLI_init(void)
{
        const CLI_Command_Definition_t * const localCommandTaskStats = &xTaskStatsCommand;
        FreeRTOS_CLIRegisterCommand( localCommandTaskStats );
       
        const CLI_Command_Definition_t * const localCommandS = &xCommandS;
        FreeRTOS_CLIRegisterCommand( localCommandS );
        
        const CLI_Command_Definition_t * const localCommandAP = &xCommandAP;
        FreeRTOS_CLIRegisterCommand( localCommandAP );
        
        const CLI_Command_Definition_t * const localCommandSF = &xCommandSF;
        FreeRTOS_CLIRegisterCommand( localCommandSF );
        
        const CLI_Command_Definition_t * const localCommandES = &xCommandES;
        FreeRTOS_CLIRegisterCommand( localCommandES );
        
        const CLI_Command_Definition_t * const localCommandER = &xCommandER;
        FreeRTOS_CLIRegisterCommand( localCommandER );
        
        //Still need to add the run time stats task
}