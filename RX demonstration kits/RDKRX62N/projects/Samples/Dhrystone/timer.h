/******************************************************************************
* File Name     : 
* Version       : 1.0
* Device(s)     : R5F56108VNFP
* Tool-Chain    : RX Family C Compiler
* OS            : None
* H/W Platform  : 
* Description   : 
* Operation     : 
* Limitations   : None
*******************************************************************************
* History : DD.MM.YYYY    Version   Description
*         : 10.Jul.2010   1.00      First release
*******************************************************************************/

#ifndef _TIMER_H_
#define _TIMER_H_

#include <stdint.h>
/******************************************************************************
 Function prototypes
******************************************************************************/
void TimerInit (void) ;
void TimerStart (void) ;
uint32_t TimerStop (void) ;

#endif
