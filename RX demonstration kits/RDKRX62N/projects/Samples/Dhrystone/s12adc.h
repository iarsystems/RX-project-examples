/******************************************************************************
* File Name     : S12ADC.H
* Version       : 1.0
* Device(s)     : R5F56108VNFP
* Tool-Chain    : RX Family C Compiler
* OS            : None
* H/W Platform  : RSK/RDK RX62N
* Description   : Header file for 12-bit ADC routines
* Limitations   : None
*******************************************************************************
* History : DD.MM.YYYY    Version   Description
*         : 10.Jul.2010   1.00      First release
*******************************************************************************/

/******************************************************************************
 Include files
******************************************************************************/
#ifndef _S12ADC_H_
#define _S12ADC_H_

#include <stdint.h>

#define BUFFER_PING   2
#define BUFFER_PONG   1
#define BUFFER_NONE   0

void S12ADC_Init (uint16_t *pPing, uint16_t *pPong, uint32_t size) ; /* Sets up the ADC */
void S12ADC_Start(void ) ;    /* Starts the conversion process */

void S12ADC_Release (void) ;  /* Allows forground tasks to release ADC buffer after processing */
uint32_t S12ADC_BufferReady (void) ;  /* Allows foreground to check if a buffer is ready */

#endif
