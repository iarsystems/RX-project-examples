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
* Copyright (C) 2010(2011) Renesas Electronics Corporation
* and Renesas Solutions Corp. All rights reserved.
*******************************************************************************
* File Name    : rx_rsk.c
* Version      : 2.00
* Device(s)    : Renesas SH-Series, RX-Series
* Tool-Chain   : Renesas SuperH RISC engine Standard Toolchain
*              : Renesas RX Standard Toolchain
* OS           : Common to None and uITRON 4.0 Spec
* H/W Platform : Independent
* Description  : RX62N RSK A/D Input processing
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
#include <iorx63t.h>
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

    /* Enable S12AD control module(MSTPA24) */
    /* Enable S12AD module(MSTPA17)
    b5-b0   Reserved 0
    b6      MSTPA6   PWM timer unit1 stop bit
    b7      MSTPA7   PWM timer unit0 stop bit
    b8      Reserved 0
    b9      MSTPA9   Multifunction timer unit3 stop bit
    b13-10  Reserved 0
    b14     MSTPA14  Compare timer unit1 stop bit
    b15     MSTPA15  Compare timer unit0 stop bit
    b16     MSTPA16  12bit AD unit1 stop bit
    b17     MSTPA17  12bit AD unit0 stop bit
    b18     Reserved 0
    b19     MSTPA19  DA stop bit
    b22-b20 Reserved 0
    b23     MSTPA23  10bit AD stop bit
    b24     MSTPA24  12bit AD control stop bit
    b26-b25 Reserved 0
    b27     Reserved 0
    b28     MSTPA28  DMA/DTC stop bit
    b29     MSTPA29  EXDMA stop bit
    b30     Reserved 0
    b31     ACSE     All clock stop bit
    */
    SYSTEM.MSTPCRA.BIT.MSTPA24  = 0;
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

    /* Port mode register write enable */
    /* PWPR.PFSWE write protect off */
    /* Write protect register
    b5-b0 Reserved 0
    b6 PFSWE    PFS register write enable bit
    b7 BOWI     PFSWE bit write disable bit
    */
    MPC.PWPR.BYTE           = 0x00;

    /* PFS register write protect off */
    /* Write protect register
    b5-b0 Reserved 0
    b6 PFSWE    PFS register write enable bit
    b7 BOWI     PFSWE bit write disable bit
    */
    MPC.PWPR.BYTE           = 0x40;

    /* AN000 select */
    /* Port4-bit0 Pin function select register
    b6-b0 PSEL  Reserved 0
    b7 ASEL     Analog Input Function Select
    */
    MPC.P40PFS.BIT.ASEL = 1;

    /* Port mode register write disable */
    /* PFS register write protect on */
    /* Write protect register
    b5-b0 Reserved 0
    b6 PFSWE    PFS register write enable bit
    b7 BOWI     PFSWE bit write disable bit
    */
    MPC.PWPR.BYTE           = 0x80;

    /* A/D channel select register (AN0 select)
    b7-b0  ANS      A/D conversion channel select bit
    b15-b8 Reserved 0
    */
    S12AD.ADANSA.BIT.ANSA = 0x01;
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
