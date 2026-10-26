/* Adapted for use with IAR Embedded Workbench */


#include "lcd.h"
#include "crc.h"
#include "async.h"

void main(void)
{
  Init_CRC();
  
  SciInit_CRC();
  

  
  while(1);  
}
