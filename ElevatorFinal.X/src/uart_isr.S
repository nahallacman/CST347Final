#include "ISR_Support.h"

//We are using UART2 at KFalls campus, but the names of the functions
//Remain UART1 for ease of exchange.

    .set	nomips16
	.set 	noreorder

    .extern vUART1_ISR
	.extern xISRStackTop
    .global vUART1_ISR_Wrapper
    .set noreorder
    .set noat
    .ent vUART1_ISR_Wrapper

vUART1_ISR_Wrapper:
    portSAVE_CONTEXT

    jal vUART1_ISR
    nop

    portRESTORE_CONTEXT

    .end vUART1_ISR_Wrapper
