/* Adapted for use with IAR Embedded Workbench */
/***********************************************************************/
/*                                                                     */
/*  FILE        :hwsetup.c                                             */
/*  DATE        :                                     */
/*  DESCRIPTION :Hardware Setup file                                   */
/*  CPU TYPE    :RX630                                                 */
/*                                                                     */
/*                                                                     */
/***********************************************************************/
/* June 07 , 2011  REA   For RSKRX630 with CAN API.*/

#include "iorx630.h"
#include "rskrx630def.h"
#include "stdint.h"
#include "r_can_api.h"
#include "lcd.h"
  
#ifdef __cplusplus
extern "C" {
#endif
extern void HardwareSetup(void);
#ifdef __cplusplus
}
#endif

extern uint32_t g_can_channel;

/******************************************************************************
Function Name:       HardwareSetup
Parameters:          -
Return value:        -
Description:         IO and peripheral control.
******************************************************************************/
void HardwareSetup(void)
{
    /* Set up RX630 clocks */
  unsigned int i;
  SYSTEM.PRCR.WORD = 0xA503;      /* protect off */
  
  SYSTEM.SOSCCR.BYTE = 0x01;      /* stop sub-clock */
                    /* delete when you use sub-clock */

  SYSTEM.MOSCWTCR.BYTE = 0x0D;    /* 131072 state */
                    /* wait over 10ms  @12MHz */
                    
  SYSTEM.PLLWTCR.BYTE = 0x0F;      /* 4194304 state (default)*/
                    /* wait over 12ms  */
  
  SYSTEM.PLLCR.WORD = 0x0F00;      /* x16 @PLL */

  SYSTEM.MOSCCR.BYTE = 0x00;      /* EXTAL ON */
  SYSTEM.PLLCR2.BYTE = 0x00;      /* PLL ON */

  for(i = 0;i< 0x168;i++){;}      /* wait over 12ms */
  
  SYSTEM.SCKCR.LONG = 0x21821211;
//  SYSTEM.SCKCR.LONG = 0x21023333;    /* ICK=PLL/2,BCK,FCK,PCK=PLL/8 */
  SYSTEM.SCKCR3.WORD = 0x0400;    /* LOCO -> PLL */  
  
  
  /*** SPECIFIC port settings. ***/
  /* Switches are inputs. 
  SW1 is p3_2, SW2 is p4_4, SW3 is p0_7. */
  SW1_DDR = 0;
  SW2_DDR = 0;
  SW3_DDR = 0;
      
  /* Set switch ports to I/O pin mode */
  PORT3.PMR.BIT.B2 = 0; 
  PORT4.PMR.BIT.B4 = 0;
  PORT0.PMR.BIT.B7 = 0;  
  
  /* Set LED ports as outputs. */
  ENABLE_LEDS
    ALL_LEDS_ON
  
  /* For CAN see API. Set default port for CAN so an output high on a 
    CAN pin or low doesn't affect the bus. */
//  R_CAN_PortSet(g_can_channel, ENABLE);
    
}/* HardwareSetup() */


/* EOF */
