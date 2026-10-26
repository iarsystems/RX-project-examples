/******************************************************************************
System Includes
******************************************************************************/
#include <iorx63n.h>


/******************************************************************************
User Includes
******************************************************************************/
#include "rskRX63Ndefines.h"
#include "mtu.h"
#include "cgc.h"
#include "intrinsics.h"


/******************************************************************************
 Function Prototypes
******************************************************************************/
extern void HardwareSetup(void);


//
// Address to __iar_program_start imported for the 
// Application.symbols file.
//

extern const unsigned long _iar_program_start;

void user_boot_main(void)
{
  // This is placed in User Boot Area

  // Do things here

  // Get function pointer to __iar_program_start for main application

  void (*application_start)(void) = (void (*)(void)) &_iar_program_start;
  
  // call main application
  //
  // Step into in Dissasembly window to go to first assembler instuction in
  // __iar_program_start for main application.
  //

  application_start();

}
