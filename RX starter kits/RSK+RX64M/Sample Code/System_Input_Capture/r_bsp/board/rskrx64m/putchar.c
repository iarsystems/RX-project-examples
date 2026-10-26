
/* Device specific SFR definitions */
#include "iorx64m.h"

void charput (char theChar);

/******************************************************************************
* Function Name: putchar
* Description  : Outputs a character on a serial port
* Arguments    : output_char -
*                    character to output
* Return Value : character that was output
******************************************************************************/
int putchar(int output_char)
{
  charput (output_char);

  return output_char;
}

