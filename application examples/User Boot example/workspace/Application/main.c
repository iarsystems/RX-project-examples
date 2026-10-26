/******************************************************************************
System Includes
******************************************************************************/
#include <iorx63n.h>

/******************************************************************************
User Includes
******************************************************************************/
#include "rskRX63Ndefines.h"
#include "main.h"
#include "mtu.h"
#include "cgc.h"
#include "intrinsics.h"


/******************************************************************************
 Function Prototypes
******************************************************************************/
extern void HardwareSetup(void);

/******************************************************************************
Function     : main
Description   : Initialises MTU2_3 peripherals.
Starts the MTU_3 running.
Argument    : none
Return value : none
******************************************************************************/
void main(void)
{
  HardwareSetup();
  __enable_interrupt();

  /* Initialise MTU3.  This timer will toggle LED3 at 50ms rate   */
  /* We can use this to check that board is running and           */
  /* check we have the clocks setup correctly                     */
  InitMTU3();

  /* Start the timer MTU3                                         */
  StartMTU3();

  while(1)
  {
  }
}
