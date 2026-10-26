

/* Device specific SFR definitions */
#include <iorx113.h>

char charget (void);

/******************************************************************************
* Function Name: getchar
* Description  : Gets a character on a serial port
* Arguments    : none
* Return Value : character that was received
******************************************************************************/
int getchar(void)
{
	unsigned char temp;

  temp = charget();

	/* Receive data acquired, send back up */
	return temp;
}

