/******************************************************************************
* DISCLAIMER
* This software is supplied by Renesas Electronics Corporation and is only
* intended for use with Renesas products. No other uses are authorized.
* This software is owned by Renesas Electronics Corporation and is  protected
* under all applicable laws, including copyright laws.
* THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES
* REGARDING THIS SOFTWARE, WHETHER EXPRESS, IMPLIED OR STATUTORY,
* INCLUDING BUT NOT LIMITED TO WARRANTIES OF MERCHANTABILITY, FITNESS FOR  A
* PARTICULAR PURPOSE AND NON-INFRINGEMENT.  ALL SUCH WARRANTIES ARE  EXPRESSLY
* DISCLAIMED.
* TO THE MAXIMUM EXTENT PERMITTED NOT PROHIBITED BY LAW, NEITHER RENESAS
* ELECTRONICS CORPORATION NOR ANY OF ITS AFFILIATED COMPANIES SHALL BE  LIABLE
* FOR ANY DIRECT, INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES
* FOR ANY REASON RELATED TO THIS SOFTWARE, EVEN IF RENESAS OR ITS
* AFFILIATES HAVE BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
* Renesas reserves the right, without notice, to make changes to this
* software and to discontinue the availability of this software.
* By using this software, you agree to the additional terms and
* conditions found by accessing the following link:
* http://www.renesas.com/disclaimer
*******************************************************************************
* Copyright (C) 2010(2011) Renesas Electronics Corpration
* and Renesas Solutions Corp. All rights reserved.
*******************************************************************************
* File Name    : rx_rsk.c
* Version      : 2.00
* Device(s)    : Renesas SH-Series, RX-Series
* Tool-Chain   : Renesas SuperH RISC engine Standard Toolchain
*              : Renesas RX Standard Toolchain
* OS           : Common to None and uITRON 4.0 Spec
* H/W Platform : Independent
* Description  : RX63N RSK A/D Input processing
******************************************************************************
* History : DD.MM.YYYY Version Description
*         : 29.07.2011 0.50    First Release
*         : 06.03.2012         Added #pragma interrupt to ISRs as part of using 
*         :                    C-startup files.
******************************************************************************/



/******************************************************************************
Includes   <System Includes> , "Project Includes"
******************************************************************************/
#include "r_usb_ctypedef.h"         /* Type define */
#include <iorx63n.h>
#include "hw_resource.h"

#ifdef USB_ADC_ENABLE
/******************************************************************************
External variables and functions
******************************************************************************/

/******************************************************************************
Constant macro definitions
******************************************************************************/

/******************************************************************************
Typedef definitions
******************************************************************************/

/******************************************************************************
Section    <Section Definition> , "Project Sections"
******************************************************************************/
#pragma section="_rx_rsk"

/******************************************************************************
Private global variables and functions
******************************************************************************/

/*=== AD ====================================================================*/
void        usb_cpu_AdInit(void);
uint32_t    usb_cpu_AdData(void);

/******************************************************************************
Renesas Abstracted RSK functions
******************************************************************************/

/******************************************************************************
A/D functions
******************************************************************************/

/******************************************************************************
Function Name   : usb_cpu_AdInit
Description     : 12bit A/D initialize
Arguments       : none
Return value    : none
******************************************************************************/
void usb_cpu_AdInit(void)
{
    /* Protect register
    b0    PRC0     Protect bit0
    b1    PRC1     Protect bit1
    b2    Reserved 0
    b3    PRC3     Protect bit3
    b7-b4 Reserved 0
    b15-b8 PRKEY   PRC Key code bit
    */
    SYSTEM.PRCR.WORD    = 0xA503;   /* Protect off */

    /* Enable S12AD module(MSTPA17)
    b3-b0   Reserved 0
    b4      MSTPA4   8bit timer3,2 stop bit
    b5      MSTPA5   8bit timer1,0 stop bit
    b7-b6   Reserved 0
    b8      MSTPA8   Multifunction timer unit1 stop bit
    b9      MSTPA9   Multifunction timer unit0 stop bit
    b10     MSTPA10  Programable pulse unit1 stop bit
    b11     MSTPA11  Programable pulse unit0 stop bit
    b13-12  Reserved 0
    b14     MSTPA14  Compare timer unit1 stop bit
    b15     MSTPA15  Compare timer unit0 stop bit
    b16     Reserved 0
    b17     MSTPA17  12bit AD stop bit
    b18     Reserved 0
    b19     MSTPA19  DA stop bit
    b21-b20 Reserved 0
    b22     MSTPA22  10bit AD unit1 stop bit
    b23     MSTPA23  10bit AD unit0 stop bit
    b26-b24 Reserved 0
    b27     Reserved 0
    b28     MSTPA28  DMA/DTC stop bit
    b29     MSTPA29  EXDMA stop bit
    b30     Reserved 0
    b31     ACSE     All clock stop bit
    */
    SYSTEM.MSTPCRA.BIT.MSTPA17  = 0;

    /* Protect register
    b0    PRC0     Protect bit0
    b1    PRC1     Protect bit1
    b2    Reserved 0
    b3    PRC3     Protect bit3
    b7-b4 Reserved 0
    b15-b8 PRKEY   PRC Key code bit
    */
    SYSTEM.PRCR.WORD    = 0xA500;   /* Protect on */

    /* Data direction register (P40/AN0 in)
    b0 B0 Pn0 in/out select bit
    b1 B1 Pn1 in/out select bit
    b2 B2 Pn2 in/out select bit
    b3 B3 Pn3 in/out select bit
    b4 B4 Pn4 in/out select bit
    b5 B5 Pn5 in/out select bit
    b6 B6 Pn6 in/out select bit
    b7 B7 Pn7 in/out select bit
    */
    PORT4.PDR.BIT.B0    = 0;

    /* A/D channel select register (AN0 select)
    b7-b0  ANS      A/D conversion channel select bit
    b15-b8 Reserved 0
    */
    S12AD.ADANS0.BIT.ANS0 = 0x01;
}
/******************************************************************************
End of function usb_cpu_AdInit
******************************************************************************/


/******************************************************************************
Function Name   : usb_cpu_AdData
Description     : Read A/D data
Arguments       : none
Return value    : uint32_t              : AD data
******************************************************************************/
uint32_t usb_cpu_AdData(void)
{
    uint16_t    dat;

    /* A/D control register (A/D start)
    b0    EXTRG    Trigger select bit
    b1    TRGE     Trigger enable bit
    b3-b2 CKS      Clock select bit
    b4    ADIE     A/D scan interrupt enable bit
    b5    Reserved 0
    b6    ADCS     Scan conversion mode select bit
    b7    ADST     A/D start bit
    */
    S12AD.ADCSR.BIT.ADST = 1;

    /* A/D control register (Wait A/D start bit clear)
    b0    EXTRG    Trigger select bit
    b1    TRGE     Trigger enable bit
    b3-b2 CKS      Clock select bit
    b4    ADIE     A/D scan interrupt enable bit
    b5    Reserved 0
    b6    ADCS     Scan conversion mode select bit
    b7    ADST     A/D start bit
    */
    while( S12AD.ADCSR.BIT.ADST )
    {
        /* Do nothing */
    }

    /* A/D data register (AN0)
    b11-b0           12bit A/D data
    b15-b12 Reserved 0
    */
    dat = S12AD.ADDR0;
    return (uint32_t)dat;
}
/******************************************************************************
End of function usb_cpu_AdData
******************************************************************************/
#endif /* USB_ADC_ENABLE */

/******************************************************************************
End  Of File
******************************************************************************/
