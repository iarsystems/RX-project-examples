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
* File Name     : rx_rsk.c
* Version       : 2.00
* Device(s)     : RX-Series
* Tool-Chain    : Renesas RX Standard Toolchain
* OS            : Common to None and uITRON 4.0 Spec
* H/W Platform  : Independent
* Description   : RX62N RSK processing
******************************************************************************
* History : DD.MM.YYYY Version Description
*         : 29.07.2011 0.50    First Release
*         : 06.03.2012         Added #pragma interrupt to ISRs as part of using
*         :                    C-startup files.
******************************************************************************/



/******************************************************************************
Includes   <System Includes> , "Project Includes"
******************************************************************************/
#include <machine.h>
#include "rx_rsk_extern.h"          /* RX62RSK Extern Header */

/* System definition */
#include "r_usb_usrconfig.h"        /* USB-H/W register set (user define) */
#include "r_usb_ctypedef.h"         /* Type define */
#include "r_usb_cdefusbip.h"        /* USB-FW Library Header */
#include "r_usb_cmacprint.h"        /* Standard IO macro */
#include "r_usb_cmacsystemcall.h"   /* System call macro */
#include "r_usb_cextern.h"          /* USB-FW global define */
#include <iorx62n.h>
#include "hw_resource.h"

#include "r_usb_ckernelid.h"        /* Kernel ID definition */

/******************************************************************************
Constant macro definitions
******************************************************************************/
/* Select Use IP */
#define USB_PswIntDisable               (uint32_t)(7 << 24) /* Processer Status Word - IPL(Level7) */
#define USB_PswIntSleep                 (uint32_t)(1 << 24) /* Processer Status Word - IPL(Level1) */

/******************************************************************************
External variables and functions
******************************************************************************/
#ifdef USB_LCD_ENABLE
extern void usb_cpu_LcdInit(void);
extern void usb_cpu_LcdDisp(uint8_t pos, uint8_t *str);
#endif /* USB_LCD_ENABLE */
extern USB_ER_t R_usb_pstd_PcdChangeDeviceState(USB_UTR_t *ptr, uint16_t state, uint16_t port_no,
    USB_CB_INFO_t complete);
extern void     R_usb_pstd_DeviceInformation(USB_UTR_t *ptr, uint16_t *tbl);
extern uint32_t usb_gcstd_dtc_transfer_address[USB_NUM_USBIP];

/******************************************************************************
Section    <Section Definition> , "Project Sections"
******************************************************************************/
#pragma section="_DTCtable"

/******************************************************************************
Bit Order Definition "LEFT"
******************************************************************************/
#pragma bitfields=reversed

/* DTC Vector table */
typedef struct
{
/* Condition compilation by the difference of the endian */
  #if USB_CPUBYTE_PP == USB_BYTE_LITTLE_PP
    unsigned short WORD;
    union
    {
        unsigned char BYTE;
        struct
        {
            unsigned char CHNE:1;
            unsigned char CHNS:1;
            unsigned char DISEL:1;
            unsigned char DTS:1;
            unsigned char DM:2;
            unsigned char :2;
        }
        BIT;
    }
    MRB;                        /* Mode Register B */
    union
    {
        unsigned char BYTE;
        struct
        {
            unsigned char MD:2;
            unsigned char SZ:2;
            unsigned char SM:2;
            unsigned char :2;
        }
        BIT;
    }
    MRA;                        /* Mode Register A */
    unsigned long  SAR;         /* Source Address */
    unsigned long  DAR;         /* Destination Address */
    unsigned short CRB;         /* Count Register A */
    unsigned short CRA;         /* Count Register B */
  #endif /* USB_CPUBYTE_PP == USB_BYTE_LITTLE_PP */

/* Condition compilation by the difference of the endian */
  #if USB_CPUBYTE_PP == USB_BYTE_BIG_PP
    union
    {
        unsigned char BYTE;
        struct
        {
            unsigned char MD:2;
            unsigned char SZ:2;
            unsigned char SM:2;
            unsigned char :2;
        }
        BIT;
    }
    MRA;                        /* Mode Register A */
    union
    {
        unsigned char BYTE;
        struct
        {
            unsigned char CHNE:1;
            unsigned char CHNS:1;
            unsigned char DISEL:1;
            unsigned char DTS:1;
            unsigned char DM:2;
            unsigned char :2;
        }
        BIT;
    }
    MRB;                        /* Mode Register B */
    unsigned short WORD;
    unsigned long  SAR;         /* Source Address */
    unsigned long  DAR;         /* Destination Address */
    unsigned short CRA;         /* Count Register A */
    unsigned short CRB;         /* Count Register B */
  #endif /* USB_CPUBYTE_PP == USB_BYTE_BIG_PP */
} USB_DTC_t;

/******************************************************************************
Private global variables and functions
******************************************************************************/
/*
*  Must be set 0 by the Address Low 2bit. (4byte alignment)
*/
/* DTC Control Register */
USB_DTC_t   usb_dtcreg[2u];

/*
*  Must be set 0 by the Address Low 12bit. (0x???? ?000)
*/
/* DTC VECTOR Table  */
uint32_t    usb_dtcctable[48] =
{
    (uint32_t)0xFFFFFFFF,       /* DTC VECTOR  0 */
    (uint32_t)0xFFFFFFFF,       /* DTC VECTOR  1 */
    (uint32_t)0xFFFFFFFF,       /* DTC VECTOR  2 */
    (uint32_t)0xFFFFFFFF,       /* DTC VECTOR  3 */
    (uint32_t)0xFFFFFFFF,       /* DTC VECTOR  4 */
    (uint32_t)0xFFFFFFFF,       /* DTC VECTOR  5 */
    (uint32_t)0xFFFFFFFF,       /* DTC VECTOR  6 */
    (uint32_t)0xFFFFFFFF,       /* DTC VECTOR  7 */
    (uint32_t)0xFFFFFFFF,       /* DTC VECTOR  8 */
    (uint32_t)0xFFFFFFFF,       /* DTC VECTOR  9 */
    (uint32_t)0xFFFFFFFF,       /* DTC VECTOR 10 */
    (uint32_t)0xFFFFFFFF,       /* DTC VECTOR 11 */
    (uint32_t)0xFFFFFFFF,       /* DTC VECTOR 12 */
    (uint32_t)0xFFFFFFFF,       /* DTC VECTOR 13 */
    (uint32_t)0xFFFFFFFF,       /* DTC VECTOR 14 */
    (uint32_t)0xFFFFFFFF,       /* DTC VECTOR 15 */
    (uint32_t)0xFFFFFFFF,       /* DTC VECTOR 16 */
    (uint32_t)0xFFFFFFFF,       /* DTC VECTOR 17 */
    (uint32_t)0xFFFFFFFF,       /* DTC VECTOR 18 */
    (uint32_t)0xFFFFFFFF,       /* DTC VECTOR 19 */
    (uint32_t)0xFFFFFFFF,       /* DTC VECTOR 20 */
    (uint32_t)0xFFFFFFFF,       /* DTC VECTOR 21 */
    (uint32_t)0xFFFFFFFF,       /* DTC VECTOR 22 */
    (uint32_t)0xFFFFFFFF,       /* DTC VECTOR 23 */
    (uint32_t)0xFFFFFFFF,       /* DTC VECTOR 24 */
    (uint32_t)0xFFFFFFFF,       /* DTC VECTOR 25 */
    (uint32_t)0xFFFFFFFF,       /* DTC VECTOR 26 */
    (uint32_t)0x00000000,       /* DTC VECTOR 27 (SWINT) */
    (uint32_t)0x00000000,       /* DTC VECTOR 28 (CMT0) */
    (uint32_t)0x00000000,       /* DTC VECTOR 29 (CMT1) */
    (uint32_t)0x00000000,       /* DTC VECTOR 30 (CMT2) */
    (uint32_t)0x00000000,       /* DTC VECTOR 31 (CMT3) */
    (uint32_t)0xFFFFFFFF,       /* DTC VECTOR 32 */
    (uint32_t)0xFFFFFFFF,       /* DTC VECTOR 33 */
    (uint32_t)0xFFFFFFFF,       /* DTC VECTOR 34 */
    (uint32_t)0xFFFFFFFF,       /* DTC VECTOR 35 */
    (uint32_t)&usb_dtcreg[0],   /* DTC VECTOR 36 (USB0-D0FIFO) */
    (uint32_t)0x00000000,       /* DTC VECTOR 37 (USB0-D1FIFO) */
    (uint32_t)0xFFFFFFFF,       /* DTC VECTOR 38 */
    (uint32_t)0xFFFFFFFF,       /* DTC VECTOR 39 */
    (uint32_t)&usb_dtcreg[1],   /* DTC VECTOR 40 (USB1-D0FIFO) */
    (uint32_t)0x00000000,       /* DTC VECTOR 41 (USB1-D1FIFO) */
    (uint32_t)0x00000000,       /* DTC VECTOR 42 */
    (uint32_t)0xFFFFFFFF,       /* DTC VECTOR 43 */
    (uint32_t)0xFFFFFFFF,       /* DTC VECTOR 44 */
    (uint32_t)0x00000000,       /* DTC VECTOR 45 (SPRI0) */
    (uint32_t)0x00000000,       /* DTC VECTOR 46 (SPTI1) */
    (uint32_t)0xFFFFFFFF        /* DTC VECTOR 47 */
};

/******************************************************************************
Section    <Section Definition> , "Project Sections"
******************************************************************************/
#pragma section="_hw"

/******************************************************************************
Private global variables and functions
******************************************************************************/
uint16_t usb_gcstd_D0fifo[2u] = {0,0};      /* D0fifo0 Interrupt Request enable */
uint16_t usb_gcstd_D1fifo[2u] = {0,0};      /* D1fifo0 Interrupt Request enable */

/*=== SYSTEM ================================================================*/
void        usb_cpu_McuInitialize(void);
void        usb_cpu_target_init(void);
/*=== Interrupt =============================================================*/
void        usb_cpu_usbint_init(void);
void        usb_cpu_DmaintInit(void);
#pragma vector=VECT_USB0_USBI0
__interrupt void usb_cpu_usb_int_hand(void);
#pragma vector=VECT_USB1_USBI1
__interrupt void usb2_cpu_usb_int_hand(void);
#pragma vector=VECT_USB0_D0FIFO0
__interrupt void usb_cpu_d0fifo_int_hand(void);
#pragma vector=VECT_USB1_D0FIFO1
__interrupt void usb2_cpu_d0fifo_int_hand(void);
void        usb_cpu_int_enable(USB_UTR_t *ptr);
void        usb_cpu_int_disable(USB_UTR_t *ptr);
/*=== TIMER =================================================================*/
void        usb_cpu_Delay1us(uint16_t time);
void        usb_cpu_DelayXms(uint16_t time);
/*=== Standby control =======================================================*/
void        usb_cpu_GoDeepStbyMode(USB_UTR_t *ptr);
void        usb_cpu_GoWait(void);
void        usb_cpu_GoDeepStbyMode0(USB_UTR_t *ptr);
void        usb_cpu_GoDeepStbyMode1(void);
void        usb_cpu_RegRecovEnable(USB_UTR_t *ptr);
void        usb_cpu_RegRecovDisable(USB_UTR_t *ptr);
/*=== IRQ ===================================================================*/
void        usb_cpu_IRQ8_Enable(USB_UTR_t *ptr);
void        usb_cpu_IRQ8_Disable(void);
void        usb_cpu_IRQ9_Enable(void);
void        usb_cpu_IRQ9_Disable(void);
void        usb_cpu_IRQ8Int(void);
#pragma vector=VECT_ICU_IRQ9
__interrupt void usb_cpu_IRQ9Int(void);
/*=== DMA ===================================================================*/
void        usb_cpu_d0fifo2buf_start_dma(USB_UTR_t *ptr, uint32_t SourceAddr);
void        usb_cpu_buf2d0fifo_start_dma(USB_UTR_t *ptr, uint32_t DistAdr);
void        usb_cpu_d0fifo_restart_dma(USB_UTR_t *ptr);
void        usb_cpu_d0fifo_stop_dma(USB_UTR_t *ptr);
uint16_t    usb_cpu_get_dtc_block_count(USB_UTR_t *ptr);
void        usb_cpu_d0fifo_enable_dma(USB_UTR_t *ptr );
void        usb_cpu_d0fifo_disable_dma(USB_UTR_t *ptr );
uint32_t    usb_cpu_get_dtc_Source_address(USB_UTR_t *ptr);
uint32_t    usb_cpu_get_dtc_Destination_address(USB_UTR_t *ptr);

/* Condition compilation by the difference of the devices */
 #if (USB_CPU_LPW_PP == USB_LPWR_USE_PP)
uint16_t    usb_gcpu_RemoteProcess = USB_OFF;
 #endif /* (USB_CPU_LPW_PP == USB_LPWR_USE_PP) */

/******************************************************************************
Renesas Abstracted RSK functions
******************************************************************************/

/******************************************************************************
Function Name   : usb_cpu_McuInitialize
Description     : MCU Initialize
Arguments       : none
Return value    : none
******************************************************************************/
void usb_cpu_McuInitialize(void)
{
    uint8_t stby;

    /* Read Deep software standby flag
    b0    POFR     Deep software standby flag
    b1    LVD1F    LVD1F flag
    b2    LVD2F    LVD2F flag
    b6-b3 Reserved 0
    b7    DPSRSTF  Power on reset flag
    */
    stby = SYSTEM.RSTSR.BIT.DPSRSTF;
    if( stby == 1 )
    {
        /* IO status restore to return */
        /* IO port keep bit
        b0    RAMCUT0  RAM off0 bit
        b3-b1 Reserved 0
        b4    RAMCUT1  RAM off1 bit
        b5    RAMCUT2  RAM off2 bit
        b6    IOKEEP   IO port keep bit
        b7    DPSBY    Deep software standby bit
        */
        SYSTEM.DPSBYCR.BIT.IOKEEP   = 0;

        /* Clear Deep software standby flag
        b0    POFR     Deep software standby flag
        b1    LVD1F    LVD1F flag
        b2    LVD2F    LVD2F flag
        b6-b3 Reserved 0
        b7    DPSRSTF  Power on reset flag
        */
        SYSTEM.RSTSR.BIT.DPSRSTF    = 0;
    }

    /* Clock select ICLK=96MHz/PCLK=48MHz/BCLK=24MHz
    b7-b0   Reserved 0
    b11-b8  PCK      PCLK
    b15-b12 Reserved 0
    b19-b16 BCK      BCLK
    b21-b20 Reserved 0
    b22     PSTOP0   SDCLK control bit
    b23     PSTOP1   BCLK control bit
    b27-b24 ICK      ICLK
    b31-b28 Reserved 0
    */
    SYSTEM.SCKCR.LONG   = 0x00010100;

    /* Disable ACSE & Enable CMT0 module(MSTPA15)
    b3-b0   Reserved 0
    b4      MSTPA4   8bit timer3,2 stop bit
    b5      MSTPA5   8bit timer1,0 stop bit
    b7-b6   Reserved 0
    b8      MSTPA8   Multifunction timer unit1 stop bit
    b9      MSTPA9   Multifunction timer unit0 stop bit
    b10     MSTPA10  Programmable pulse unit1 stop bit
    b11     MSTPA11  Programmable pulse unit0 stop bit
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
    SYSTEM.MSTPCRA.LONG = 0x7FFF7FFF;

#if USB_FUNCSEL_USBIP0_PP != USB_NOUSE_PP
    /* Module stop control register (Enable USB0 module(MSTPB19))
    b0     MSTPB0   CAN stop bit
    b14-b1 Reserved 0
    b15    MSTPB15  Ether DMA stop bit
    b16    MSTPB16  Serial peri1 stop bit
    b17    MSTPB17  Serial peri0 stop bit
    b18    MSTPB18  USB port1 stop bit
    b19    MSTPB19  USB port0 stop bit
    b20    MSTPB20  I2C interface1 stop bit
    b21    MSTPB21  I2C interface0 stop bit
    b22    Reserved 0
    b23    MSTPB23  CRC stop bit
    b24    Reserved 0
    b25    MSTPB25  Serial interface6 stop bit
    b26    MSTPB26  Serial interface5 stop bit
    b27    Reserved 0
    b28    MSTPB28  Serial interface3 stop bit
    b29    MSTPB29  Serial interface2 stop bit
    b30    MSTPB30  Serial interface1 stop bit
    b31    MSTPB31  Serial interface0 stop bit
    */
    SYSTEM.MSTPCRB.BIT.MSTPB19  = 0;
#endif  /* USB_FUNCSEL_USBIP0_PP != USB_NOUSE_PP */

/* Condition compilation by the difference of USB function */
#if USB_FUNCSEL_USBIP1_PP != USB_NOUSE_PP
    /* Module stop control register (Enable USB0 module(MSTPB18))
    b0     MSTPB0   CAN stop bit
    b14-b1 Reserved 0
    b15    MSTPB15  Ether DMA stop bit
    b16    MSTPB16  Serial peri1 stop bit
    b17    MSTPB17  Serial peri0 stop bit
    b18    MSTPB18  USB port1 stop bit
    b19    MSTPB19  USB port0 stop bit
    b20    MSTPB20  I2C interface1 stop bit
    b21    MSTPB21  I2C interface0 stop bit
    b22    Reserved 0
    b23    MSTPB23  CRC stop bit
    b24    Reserved 0
    b25    MSTPB25  Serial interface6 stop bit
    b26    MSTPB26  Serial interface5 stop bit
    b27    Reserved 0
    b28    MSTPB28  Serial interface3 stop bit
    b29    MSTPB29  Serial interface2 stop bit
    b30    MSTPB30  Serial interface1 stop bit
    b31    MSTPB31  Serial interface0 stop bit
    */
    SYSTEM.MSTPCRB.BIT.MSTPB18  = 0;
#endif  /* USB_FUNCSEL_USBIP1_PP != USB_NOUSE_PP */

    /* Enable CMT0 interrupt(IEN5)
    b0 IEN0 Interrupt enable bit
    b1 IEN1 Interrupt enable bit
    b2 IEN2 Interrupt enable bit
    b3 IEN3 Interrupt enable bit
    b4 IEN4 Interrupt enable bit
    b5 IEN5 Interrupt enable bit
    b6 IEN6 Interrupt enable bit
    b7 IEN7 Interrupt enable bit
    */
    ICU.IER[3].BYTE     = 0x20;

    /* Priority System timer(CMT0)
    b3-b0 IPR      Interrupt priority
    b7-b4 Reserved 0
    */
    ICU.IPR[4].BYTE     = 0x02;

    /* BCLK stop
    b7-b0   Reserved 0
    b11-b8  PCK      PCLK
    b15-b12 Reserved 0
    b19-b16 BCK      BCLK
    b21-b20 Reserved 0
    b22     PSTOP0   SDCLK control bit
    b23     PSTOP1   BCLK control bit
    b27-b24 ICK      ICLK
    b31-b28 Reserved 0
    */
    SYSTEM.SCKCR.BIT.PSTOP1 = 1;

    /* Data direction
    b0 Pn0 in/out select bit
    b1 Pn1 in/out select bit
    b2 Pn2 in/out select bit
    b3 Pn3 in/out select bit
    b4 Pn4 in/out select bit
    b5 Pn5 in/out select bit
    b6 Pn6 in/out select bit
    b7 Pn7 in/out select bit
    */
    PORT5.DDR.BIT.B3        = 1;

    /* BCLK start
    b7-b0   Reserved 0
    b11-b8  PCK      PCLK
    b15-b12 Reserved 0
    b19-b16 BCK      BCLK
    b21-b20 Reserved 0
    b22     PSTOP0   SDCLK control bit
    b23     PSTOP1   BCLK control bit
    b27-b24 ICK      ICLK
    b31-b28 Reserved 0
    */
    SYSTEM.SCKCR.BIT.PSTOP1 = 0;

    /* Input buffer control register (SW port)
    b0 B0 Pn0 input buffer control bit
    b1 B1 Pn1 input buffer control bit
    b2 B2 Pn2 input buffer control bit
    b3 B3 Pn3 input buffer control bit
    b4 B4 Pn4 input buffer control bit
    b5 B5 Pn5 input buffer control bit
    b6 B6 Pn6 input buffer control bit
    b7 B7 Pn7 input buffer control bit
    */
    PORT0.ICR.BIT.B0        = 1;    /* SW1 */
    PORT0.ICR.BIT.B1        = 1;    /* SW2 */
    PORT0.ICR.BIT.B7        = 1;    /* SW3 */

    /* 0x5A00 : KEY code */
    /* ROME=1, EXBE=1 (External BUS Enable)
    b0     ROME     ROM enable bit
    b1     EXBE     External bus enable bit
    b7-b2  Reserved 0
    b15-b8 KEY      SYSCR0 key code
    */
    SYSTEM.SYSCR0.WORD = 0x5A00 + 3;
}
/******************************************************************************
End of function usb_cpu_McuInitialize
******************************************************************************/

/******************************************************************************
Function Name   : usb_cpu_target_init
Description     : Target System Initialize
Arguments       : none
Return value    : none
******************************************************************************/
void usb_cpu_target_init( void )
{
    /* Enable Interrupt */
    usb_cpu_usbint_init();           /* Initialized USB interrupt  */
    usb_cpu_DmaintInit();           /* Initialized DMA interrupt  */

#ifdef  USB_LED_ENABLE
    usb_cpu_LedInit();              /* Initialized RSK LED */
#endif /* USB_LED_ENABLE */

#ifdef USB_LCD_ENABLE
    usb_cpu_LcdInit();              /* Initialized RSK LCD */
    usb_cpu_LcdDisp( LCD_POS_U0 , "        " );
    usb_cpu_LcdDisp( LCD_POS_D0 , "        " );
#endif /* USB_LCD_ENABLE */
}
/******************************************************************************
End of function usb_cpu_target_init
******************************************************************************/

/******************************************************************************
Function Name   : usb_cpu_usbint_init
Description     : USB interrupt Initialize
Arguments       : void
Return value    : void
******************************************************************************/
void usb_cpu_usbint_init( void )
{
/* Condition compilation by the difference of USB function */
#if USB_FUNCSEL_USBIP0_PP != USB_NOUSE_PP
    /* Deep standby USB monitor register (USB0 Transceiver Output fixed)
    b0      SRPC0    USB0 single end control
    b3-b1   Reserved 0
    b4      FIXPHY0  USB0 transceiver output fix
    b7-b5   Reserved 0
    b8      SRPC1    USB1 single end control
    b11-b9  Reserved 0
    b12     FIXPHY1  USB1 transceiver output fix
    b15-b13 Reserved 0
    b16     DP0      USB0 DP input
    b17     DM0      USB0 DM input
    b19-b18 Reserved 0
    b20     DOVCA0   USB0 OVRCURA input
    b21     DOVCB0   USB0 OVRCURB input
    b22     Reserved 0
    b23     DVBSTS0  USB1 VBUS input
    b24     DP1      USB1 DP input
    b25     DM1      USB1 DM input
    b27-b26 Reserved 0
    b28     DOVCA1   USB1 OVRCURA input
    b29     DOVCB1   USB1 OVRCURB input
    b30     Reserved 0
    b31     DVBSTS1  USB1 VBUS input
    */
    USB.DPUSR0R.BIT.FIXPHY0     = 0;

    /* Disable USB0 D0FIFO interrupt(IEN4)
    b0 IEN0 Interrupt enable bit
    b1 IEN1 Interrupt enable bit
    b2 IEN2 Interrupt enable bit
    b3 IEN3 Interrupt enable bit
    b4 IEN4 Interrupt enable bit
    b5 IEN5 Interrupt enable bit
    b6 IEN6 Interrupt enable bit
    b7 IEN7 Interrupt enable bit
    */
    ICU.IER[4].BIT.IEN4         = 0;

    /* Disable USB0 D1FIFO interrupt(IEN5)
    b0 IEN0 Interrupt enable bit
    b1 IEN1 Interrupt enable bit
    b2 IEN2 Interrupt enable bit
    b3 IEN3 Interrupt enable bit
    b4 IEN4 Interrupt enable bit
    b5 IEN5 Interrupt enable bit
    b6 IEN6 Interrupt enable bit
    b7 IEN7 Interrupt enable bit
    */
    ICU.IER[4].BIT.IEN5         = 0;

    /* Enable USB0 USBI0  interrupt(IEN6)
    b0 IEN0 Interrupt enable bit
    b1 IEN1 Interrupt enable bit
    b2 IEN2 Interrupt enable bit
    b3 IEN3 Interrupt enable bit
    b4 IEN4 Interrupt enable bit
    b5 IEN5 Interrupt enable bit
    b6 IEN6 Interrupt enable bit
    b7 IEN7 Interrupt enable bit
    */
    ICU.IER[4].BIT.IEN6         = 1;

    /* Disable USB0 Resume interrupt(IEN2)
    b0 IEN0 Interrupt enable bit
    b1 IEN1 Interrupt enable bit
    b2 IEN2 Interrupt enable bit
    b3 IEN3 Interrupt enable bit
    b4 IEN4 Interrupt enable bit
    b5 IEN5 Interrupt enable bit
    b6 IEN6 Interrupt enable bit
    b7 IEN7 Interrupt enable bit
    */
    ICU.IER[0x0B].BIT.IEN2      = 0;

    /* Priority D0FIFO0=0(Disable)
    b3-b0 IPR      Interrupt priority
    b7-b4 Reserved 0
    */
    ICU.IPR[0x0C].BYTE          = 0x00;

    /* Priority D1FIFO0=0(Disable)
    b3-b0 IPR      Interrupt priority
    b7-b4 Reserved 0
    */
    ICU.IPR[0x0D].BYTE          = 0x00;

    /* Priority USBINT0=3
    b3-b0 IPR      Interrupt priority
    b7-b4 Reserved 0
    */
    ICU.IPR[0x0E].BYTE          = 0x03;

    /* Priority Resume0=0(Disable)
    b3-b0 IPR      Interrupt priority
    b7-b4 Reserved 0
    */
    ICU.IPR[0x3A].BYTE          = 0x00;

    /* Enable USB0 port & Pullup & Select Peripheral function
    b1-b0 USBMD    USB mode bit
    b2    PUPHZS   PUPHZ select
    b3    PDHZS    PDHZ select
    b4    USBE     USB enable
    b7-b5 Reserved 0
    */
/* Condition compilation by the difference of USB function */
#if USB_FUNCSEL_USBIP0_PP == USB_HOST_PP
    IOPORT.PFKUSB.BYTE          = 0x11; /* Select host mode for the USB0 pins. */
#else   /* USB_FUNCSEL_USBIP0_PP == USB_HOST_PP */
    IOPORT.PFKUSB.BYTE          = 0x14; /* Select function mode for the USB0 pins. */
#endif  /* USB_FUNCSEL_USBIP0_PP == USB_HOST_PP */

    /* Input buffer control register (Enable USB0 VBUS)
    b0 B0 Pn0 input buffer control bit
    b1 B1 Pn1 input buffer control bit
    b2 B2 Pn2 input buffer control bit
    b3 B3 Pn3 input buffer control bit
    b4 B4 Pn4 input buffer control bit
    b5 B5 Pn5 input buffer control bit
    b6 B6 Pn6 input buffer control bit
    b7 B7 Pn7 input buffer control bit
    */
    PORT1.ICR.BIT.B6            = 1;
#endif  /* USB_FUNCSEL_USBIP0_PP != USB_NOUSE_PP */

/* Condition compilation by the difference of USB function */
#if USB_FUNCSEL_USBIP1_PP != USB_NOUSE_PP
    /* Deep standby USB monitor register (USB0 Transceiver Output fixed)
    b0      SRPC0    USB0 single end control
    b3-b1   Reserved 0
    b4      FIXPHY0  USB0 transceiver output fix
    b7-b5   Reserved 0
    b8      SRPC1    USB1 single end control
    b11-b9  Reserved 0
    b12     FIXPHY1  USB1 transceiver output fix
    b15-b13 Reserved 0
    b16     DP0      USB0 DP input
    b17     DM0      USB0 DM input
    b19-b18 Reserved 0
    b20     DOVCA0   USB0 OVRCURA input
    b21     DOVCB0   USB0 OVRCURB input
    b22     Reserved 0
    b23     DVBSTS0  USB1 VBUS input
    b24     DP1      USB1 DP input
    b25     DM1      USB1 DM input
    b27-b26 Reserved 0
    b28     DOVCA1   USB1 OVRCURA input
    b29     DOVCB1   USB1 OVRCURB input
    b30     Reserved 0
    b31     DVBSTS1  USB1 VBUS input
    */
    USB.DPUSR0R.BIT.FIXPHY1     = 0;

    /* Disable USB1 interrupt
    b0 IEN0 Interrupt enable bit
    b1 IEN1 Interrupt enable bit
    b2 IEN2 Interrupt enable bit
    b3 IEN3 Interrupt enable bit
    b4 IEN4 Interrupt enable bit
    b5 IEN5 Interrupt enable bit
    b6 IEN6 Interrupt enable bit
    b7 IEN7 Interrupt enable bit
    */
    ICU.IER[0x05].BIT.IEN0      = 0;    /* Disable D0FIFO interrupt(IEN0) */
    ICU.IER[0x05].BIT.IEN1      = 0;    /* Disable D1FIFO interrupt(IEN1) */
    ICU.IER[0x05].BIT.IEN2      = 1;    /* Enable  USBI0  interrupt(IEN2) */
    ICU.IER[0x0B].BIT.IEN3      = 0;    /* Disable Resume interrupt(IEN3) */

    /* Priority D0FIFO0=0(Disable)
    b3-b0 IPR      Interrupt priority
    b7-b4 Reserved 0
    */
    ICU.IPR[0x10].BYTE          = 0x00;

    /* Priority D1FIFO0=0(Disable)
    b3-b0 IPR      Interrupt priority
    b7-b4 Reserved 0
    */
    ICU.IPR[0x11].BYTE          = 0x00;

    /* Priority USBINT1=3
    b3-b0 IPR      Interrupt priority
    b7-b4 Reserved 0
    */
    ICU.IPR[0x12].BYTE          = 0x03;

    /* Priority Resume1=0(Disable)
    b3-b0 IPR      Interrupt priority
    b7-b4 Reserved 0
    */
    ICU.IPR[0x3B].BYTE          = 0x00;

    /* Enable USB1 port & Select Host function
    b1-b0 USBMD    USB mode bit
    b2    PUPHZS   PUPHZ select
    b3    PDHZS    PDHZ select
    b4    USBE     USB enable
    b7-b5 Reserved 0
    */
/* Condition compilation by the difference of USB function */
#if USB_FUNCSEL_USBIP1_PP == USB_HOST_PP
    IOPORT.PFLUSB.BYTE          = 0x11; /* Select host mode for the USB1 pins. */
#else   /* USB_FUNCSEL_USBIP1_PP == USB_HOST_PP */
    IOPORT.PFLUSB.BYTE          = 0x14; /* Select function mode for the USB1 pins. */
#endif  /* USB_FUNCSEL_USBIP1_PP == USB_HOST_PP */

    /* Input buffer control register (Enable USB1 VBUS)
    b0 B0 Pn0 input buffer control bit
    b1 B1 Pn1 input buffer control bit
    b2 B2 Pn2 input buffer control bit
    b3 B3 Pn3 input buffer control bit
    b4 B4 Pn4 input buffer control bit
    b5 B5 Pn5 input buffer control bit
    b6 B6 Pn6 input buffer control bit
    b7 B7 Pn7 input buffer control bit
    */
    PORT1.ICR.BIT.B5            = 1;    /* Enable USB0 OVRCURA */
#endif  /* USB_FUNCSEL_USBIP1_PP != USB_NOUSE_PP */
}
/******************************************************************************
End of function usb_cpu_usbint_init
******************************************************************************/

/******************************************************************************
Interrupt function
******************************************************************************/

/******************************************************************************
Function Name   : usb_cpu_usb_int_hand
Description     : USB interrupt Handler
Arguments       : void
Return value    : void
******************************************************************************/
#pragma vector=VECT_USB0_USBI0
__interrupt void usb_cpu_usb_int_hand(void)
{
/* Condition compilation by the difference of USB function */
#if USB_FUNCSEL_USBIP0_PP != USB_NOUSE_PP
    usb_cstd_UsbHandler();      /* Call interrupt routine */
#endif /* USB_FUNCSEL_USBIP0_PP */
}
/******************************************************************************
End of function usb_cpu_usb_int_hand
******************************************************************************/

/******************************************************************************
Function Name   : usb_cpu_d0fifo_int_hand
Description     : D0FIFO interrupt Handler
                : IRQ #36 (VECT_USB0_D0FIFO0)
Arguments       : none
Return value    : none
******************************************************************************/
#pragma vector=VECT_USB0_D0FIFO0
__interrupt void usb_cpu_d0fifo_int_hand(void)
{
/* Condition compilation by the difference of USB function */
#if USB_FUNCSEL_USBIP0_PP != USB_NOUSE_PP
    usb_cstd_DmaHandler();      /* Call interrupt routine */
#endif /* USB_FUNCSEL_USBIP0_PP */
}
/******************************************************************************
End of function usb_cpu_d0fifo_int_hand
******************************************************************************/

/******************************************************************************
Function Name   : usb_cpu_d1fifo_int_hand
Description     : D1FIFO interrupt Handler
                : IRQ #37 (VECT_USB0_D1FIFO0)
Arguments       : none
Return value    : none
******************************************************************************/
#pragma vector=VECT_USB0_D1FIFO0
__interrupt void usb_cpu_d1fifo_int_hand(void)
{
    /* Please add the processing for the system. */
}
/******************************************************************************
End of function usb_cpu_d1fifo_int_hand
******************************************************************************/

/******************************************************************************
Function Name   : usb_cpu_usb_int_hand_r
Description     : RESUME interrupt Handler
                : IRQ #90 (VECT_USB_USBR0)
Arguments       : none
Return value    : none
******************************************************************************/
#pragma vector=VECT_USB_USBR0
__interrupt void usb_cpu_usb_int_hand_r(void)
{
    /* Please add the processing for the system. */
}
/******************************************************************************
End of function usb_cpu_usb_int_hand_r
******************************************************************************/

/******************************************************************************
Function Name   : usb2_cpu_usb_int_hand
Description     : USB interrupt Handler
                : IRQ #42 (VECT_USB1_USBI1)
Arguments       : void
Return value    : void
******************************************************************************/
#pragma vector=VECT_USB1_USBI1
__interrupt void usb2_cpu_usb_int_hand(void)
{
/* Condition compilation by the difference of USB function */
#if USB_FUNCSEL_USBIP1_PP != USB_NOUSE_PP
    usb2_cstd_UsbHandler();     /* Call interrupt routine */
#endif /* USB_FUNCSEL_USBIP1_PP */
}
/******************************************************************************
End of function usb2_cpu_usb_int_hand
******************************************************************************/

/******************************************************************************
Function Name   : usb2_cpu_d0fifo_int_hand
Description     : D0FIFO interrupt Handler
                : IRQ #40 (VECT_USB1_D0FIFO1)
Arguments       : none
Return value    : none
******************************************************************************/
#pragma vector=VECT_USB1_D0FIFO1
__interrupt void usb2_cpu_d0fifo_int_hand(void)
{
/* Condition compilation by the difference of USB function */
#if USB_FUNCSEL_USBIP1_PP != USB_NOUSE_PP
    usb2_cstd_DmaHandler();     /* Call interrupt routine */
#endif /* USB_FUNCSEL_USBIP1_PP */
}
/******************************************************************************
End of function usb2_cpu_d0fifo_int_hand
******************************************************************************/
/******************************************************************************
Function Name   : usb2_cpu_d1fifo_int_hand
Description     : D1FIFO interrupt Handler
                : IRQ #41 (VECT_USB1_D1FIFO1)
Arguments       : none
Return value    : none
******************************************************************************/
#pragma vector=VECT_USB1_D1FIFO1
__interrupt void usb2_cpu_d1fifo_int_hand(void)
{
    /* Please add the processing for the system. */
}
/******************************************************************************
End of function usb2_cpu_d1fifo_int_hand
******************************************************************************/

/******************************************************************************
Function Name   : usb2_cpu_usb_int_hand_r
Description     : RESUME interrupt Handler
                : IRQ #91 (VECT_USB_USBR1)
Arguments       : none
Return value    : none
******************************************************************************/
#pragma vector=VECT_USB_USBR1
__interrupt void usb2_cpu_usb_int_hand_r(void)
{
    /* Please add the processing for the system. */
}
/******************************************************************************
End of function usb2_cpu_usb_int_hand_r
******************************************************************************/

/******************************************************************************
Function Name   : usb_cpu_int_enable
Description     : USB Interrupt Enable
Arguments       : USB_UTR_t *ptr  : USB internal structure. Selects USB channel.
Return value    : void
******************************************************************************/
void usb_cpu_int_enable(USB_UTR_t *ptr)
{
/* Condition compilation by the difference of USB function */
    if (ptr->ip == USB_USBIP_0)
    {
        /* Interrupt enable register (USB0 USBI0 enable(IEN6))
        b0 IEN0 Interrupt enable bit
        b1 IEN1 Interrupt enable bit
        b2 IEN2 Interrupt enable bit
        b3 IEN3 Interrupt enable bit
        b4 IEN4 Interrupt enable bit
        b5 IEN5 Interrupt enable bit
        b6 IEN6 Interrupt enable bit
        b7 IEN7 Interrupt enable bit
        */
        ICU.IER[4].BIT.IEN6     = 1;

        /* Interrupt enable register (Restore bit(IEN4))
        b0 IEN0 Interrupt enable bit
        b1 IEN1 Interrupt enable bit
        b2 IEN2 Interrupt enable bit
        b3 IEN3 Interrupt enable bit
        b4 IEN4 Interrupt enable bit
        b5 IEN5 Interrupt enable bit
        b6 IEN6 Interrupt enable bit
        b7 IEN7 Interrupt enable bit
        */
        ICU.IER[4].BIT.IEN4     = usb_gcstd_D0fifo[ptr->ip];

        /* Interrupt enable register (Restore bit(IEN5))
        b0 IEN0 Interrupt enable bit
        b1 IEN1 Interrupt enable bit
        b2 IEN2 Interrupt enable bit
        b3 IEN3 Interrupt enable bit
        b4 IEN4 Interrupt enable bit
        b5 IEN5 Interrupt enable bit
        b6 IEN6 Interrupt enable bit
        b7 IEN7 Interrupt enable bit
        */
        ICU.IER[4].BIT.IEN5     = usb_gcstd_D1fifo[ptr->ip];
    }

/* Condition compilation by the difference of USB function */
    if (ptr->ip == USB_USBIP_1)
    {
        /* Interrupt enable register (Enable USB1 USBI0  interrupt (IEN2))
        b0 IEN0 Interrupt enable bit
        b1 IEN1 Interrupt enable bit
        b2 IEN2 Interrupt enable bit
        b3 IEN3 Interrupt enable bit
        b4 IEN4 Interrupt enable bit
        b5 IEN5 Interrupt enable bit
        b6 IEN6 Interrupt enable bit
        b7 IEN7 Interrupt enable bit
        */
        ICU.IER[5].BIT.IEN2     = 1;


        /* Interrupt enable register (Restore bit(IEN0))
        b0 IEN0 Interrupt enable bit
        b1 IEN1 Interrupt enable bit
        b2 IEN2 Interrupt enable bit
        b3 IEN3 Interrupt enable bit
        b4 IEN4 Interrupt enable bit
        b5 IEN5 Interrupt enable bit
        b6 IEN6 Interrupt enable bit
        b7 IEN7 Interrupt enable bit
        */
        ICU.IER[5].BIT.IEN0     = usb_gcstd_D0fifo[ptr->ip];

        /* Interrupt enable register (Restore bit(IEN1))
        b0 IEN0 Interrupt enable bit
        b1 IEN1 Interrupt enable bit
        b2 IEN2 Interrupt enable bit
        b3 IEN3 Interrupt enable bit
        b4 IEN4 Interrupt enable bit
        b5 IEN5 Interrupt enable bit
        b6 IEN6 Interrupt enable bit
        b7 IEN7 Interrupt enable bit
        */
        ICU.IER[5].BIT.IEN1     = usb_gcstd_D1fifo[ptr->ip];
    }
}
/******************************************************************************
End of function usb_cpu_int_enable
******************************************************************************/

/******************************************************************************
Function Name   : usb_cpu_int_disable
Description     : USB Interrupt disable
Arguments       : USB_UTR_t *ptr  : USB internal structure. Selects USB channel.
Return value    : void
******************************************************************************/
void usb_cpu_int_disable(USB_UTR_t *ptr)
{
/* Condition compilation by the difference of USB function */
    if (ptr->ip == USB_USBIP_0)
    {
        /* Interrupt enable register (USB0 USBI0 disable(IEN6))
        b0 IEN0 Interrupt enable bit
        b1 IEN1 Interrupt enable bit
        b2 IEN2 Interrupt enable bit
        b3 IEN3 Interrupt enable bit
        b4 IEN4 Interrupt enable bit
        b5 IEN5 Interrupt enable bit
        b6 IEN6 Interrupt enable bit
        b7 IEN7 Interrupt enable bit
        */
        ICU.IER[4].BIT.IEN6     = 0;

        /* Interrupt enable register (Bit save(IEN4))
        b0 IEN0 Interrupt enable bit
        b1 IEN1 Interrupt enable bit
        b2 IEN2 Interrupt enable bit
        b3 IEN3 Interrupt enable bit
        b4 IEN4 Interrupt enable bit
        b5 IEN5 Interrupt enable bit
        b6 IEN6 Interrupt enable bit
        b7 IEN7 Interrupt enable bit
        */
        usb_gcstd_D0fifo[ptr->ip]       = ICU.IER[4].BIT.IEN4;

        /* Interrupt enable register (Disable USB0 D0FIFO (IEN4))
        b0 IEN0 Interrupt enable bit
        b1 IEN1 Interrupt enable bit
        b2 IEN2 Interrupt enable bit
        b3 IEN3 Interrupt enable bit
        b4 IEN4 Interrupt enable bit
        b5 IEN5 Interrupt enable bit
        b6 IEN6 Interrupt enable bit
        b7 IEN7 Interrupt enable bit
        */
        ICU.IER[4].BIT.IEN4     = 0;

        /* Interrupt enable register (Bit save(IEN5))
        b0 IEN0 Interrupt enable bit
        b1 IEN1 Interrupt enable bit
        b2 IEN2 Interrupt enable bit
        b3 IEN3 Interrupt enable bit
        b4 IEN4 Interrupt enable bit
        b5 IEN5 Interrupt enable bit
        b6 IEN6 Interrupt enable bit
        b7 IEN7 Interrupt enable bit
        */
        usb_gcstd_D1fifo[ptr->ip]       = ICU.IER[4].BIT.IEN5;

        /* Interrupt enable register (USB0 D1FIFO disable(IEN5))
        b0 IEN0 Interrupt enable bit
        b1 IEN1 Interrupt enable bit
        b2 IEN2 Interrupt enable bit
        b3 IEN3 Interrupt enable bit
        b4 IEN4 Interrupt enable bit
        b5 IEN5 Interrupt enable bit
        b6 IEN6 Interrupt enable bit
        b7 IEN7 Interrupt enable bit
        */
        ICU.IER[4].BIT.IEN5     = 0;
    }

/* Condition compilation by the difference of USB function */
    if (ptr->ip == USB_USBIP_1)
    {
        /* Interrupt enable register (USB0 USBI0 disable(IEN2))
        b0 IEN0 Interrupt enable bit
        b1 IEN1 Interrupt enable bit
        b2 IEN2 Interrupt enable bit
        b3 IEN3 Interrupt enable bit
        b4 IEN4 Interrupt enable bit
        b5 IEN5 Interrupt enable bit
        b6 IEN6 Interrupt enable bit
        b7 IEN7 Interrupt enable bit
        */
        ICU.IER[5].BIT.IEN2     = 0;


        /* Interrupt enable register (Bit save(IEN0))
        b0 IEN0 Interrupt enable bit
        b1 IEN1 Interrupt enable bit
        b2 IEN2 Interrupt enable bit
        b3 IEN3 Interrupt enable bit
        b4 IEN4 Interrupt enable bit
        b5 IEN5 Interrupt enable bit
        b6 IEN6 Interrupt enable bit
        b7 IEN7 Interrupt enable bit
        */
        usb_gcstd_D0fifo[ptr->ip]       = ICU.IER[5].BIT.IEN0;

        /* Interrupt enable register (Disable USB0 D0FIFO (IEN0))
        b0 IEN0 Interrupt enable bit
        b1 IEN1 Interrupt enable bit
        b2 IEN2 Interrupt enable bit
        b3 IEN3 Interrupt enable bit
        b4 IEN4 Interrupt enable bit
        b5 IEN5 Interrupt enable bit
        b6 IEN6 Interrupt enable bit
        b7 IEN7 Interrupt enable bit
        */
        ICU.IER[5].BIT.IEN0     = 0;

        /* Interrupt enable register (Bit save(IEN1))
        b0 IEN0 Interrupt enable bit
        b1 IEN1 Interrupt enable bit
        b2 IEN2 Interrupt enable bit
        b3 IEN3 Interrupt enable bit
        b4 IEN4 Interrupt enable bit
        b5 IEN5 Interrupt enable bit
        b6 IEN6 Interrupt enable bit
        b7 IEN7 Interrupt enable bit
        */
        usb_gcstd_D1fifo[ptr->ip]       = ICU.IER[5].BIT.IEN1;


        /* Interrupt enable register (USB0 D1FIFO disable(IEN1))
        b0 IEN0 Interrupt enable bit
        b1 IEN1 Interrupt enable bit
        b2 IEN2 Interrupt enable bit
        b3 IEN3 Interrupt enable bit
        b4 IEN4 Interrupt enable bit
        b5 IEN5 Interrupt enable bit
        b6 IEN6 Interrupt enable bit
        b7 IEN7 Interrupt enable bit
        */
        ICU.IER[5].BIT.IEN1     = 0;
    }
}
/******************************************************************************
End of function usb_cpu_int_disable
******************************************************************************/

/******************************************************************************
TIMER function
******************************************************************************/

/******************************************************************************
Function Name   : usb_cpu_Delay1us
Description     : 1us Delay timer
Arguments       : uint16_t  time        ; Delay time(*1us)
Return value    : none
Note            : Please change for your MCU
******************************************************************************/
void usb_cpu_Delay1us(uint16_t time)
{
    volatile register uint16_t  i;

    /* Wait 1us (Please change for your MCU) */
    for( i = 0; i < (5 * time); ++i )
    {
    };
}
/******************************************************************************
End of function usb_cpu_Delay1us
******************************************************************************/

/******************************************************************************
Function Name   : usb_cpu_DelayXms
Description     : xms Delay timer
Arguments       : uint16_t  time        ; Delay time(*1ms)
Return value    : void
Note            : Please change for your MCU
******************************************************************************/
void usb_cpu_DelayXms(uint16_t time)
{
    /* Wait xms (Please change for your MCU) */
    /* Wait xms (Please change for your MCU) */
    volatile register uint32_t  i;

    /* Wait 1ms */
    for( i = 0; i < (6500 * time); ++i )
    {
    };
        /* When "ICLK=96MHz" is set, this code is waiting for 1ms.
          Please change this code with CPU Clock mode. */
}
/******************************************************************************
End of function usb_cpu_DelayXms
******************************************************************************/

/******************************************************************************
SLEEP & STANDBY control function
******************************************************************************/
/******************************************************************************
Function Name   : usb_cpu_GoLpwrSleep
Description     : Go low power sleep mode
Arguments       : USB_UTR_t *ptr        : USB internal structure. Selects USB channel.
Return value    : none
******************************************************************************/
void usb_cpu_GoLpwrSleep(USB_UTR_t *ptr)
{
/* Condition compilation by the difference of the operating system */
#if USB_FUNCSEL_USBIP0_PP == USB_PERI_PP || USB_FUNCSEL_USBIP1_PP == USB_PERI_PP
    uint16_t    dev_info[8];
#endif /* USB_FUNCSEL_USBIP0_PP == USB_PERI_PP || USB_FUNCSEL_USBIP1_PP == USB_PERI_PP */

    if( usb_cstd_is_host_mode(ptr) == USB_YES )
    {
/* Condition compilation by the difference of the devices */
  #if USB_CPU_LPW_PP == USB_LPWR_USE_PP
        if( usb_gcpu_RemoteProcess == USB_OFF )
        {
            /* Standby control register (Software Standby disable)
            b7-b0  Reserved 0
            b12-b8 STS      Standby timer select bit
            b13    Reserved 0
            b14    OPE      Output port enable bit
            b15    SSBY     Software standby bit
            */
            SYSTEM.SBYCR.BIT.SSBY       = 0;

            /* Goto WAIT */
            usb_cpu_GoWait();
        }
  #endif /* (USB_CPU_LPW_PP == USB_LPWR_USE_PP) */
    }
    else
    {
/* Condition compilation by the difference of the operating system */
#if USB_FUNCSEL_USBIP0_PP == USB_PERI_PP || USB_FUNCSEL_USBIP1_PP == USB_PERI_PP

    /* Get USB Device information */
    R_usb_pstd_DeviceInformation(ptr, (uint16_t *)&dev_info );

/* Condition compilation by the difference of the devices */
  #if USB_CPU_LPW_PP == USB_LPWR_USE_PP
        if( usb_gcpu_RemoteProcess == USB_OFF )
        {
            /* Remote Wakeup Flag */
            if( dev_info[4] == USB_YES )
            {
                /* IRQ9(RX62N-RSK_SW2) for Remote wakeup */
                usb_cpu_IRQ9_Enable();
            }

            /* Standby control register (Software Standby disable)
            b7-b0  Reserved 0
            b12-b8 STS      Standby timer select bit
            b13    Reserved 0
            b14    OPE      Output port enable bit
            b15    SSBY     Software standby bit
            */
            SYSTEM.SBYCR.BIT.SSBY       = 0;

            /* Goto WAIT */
            usb_cpu_GoWait();

            /* Remote Wakeup Flag */
            if( dev_info[4] == USB_YES )
            {
                usb_gcpu_RemoteProcess = USB_ON;
            }
        }
  #endif /* (USB_CPU_LPW_PP == USB_LPWR_USE_PP) */
#endif /* USB_FUNCSEL_USBIP0_PP == USB_PERI_PP || USB_FUNCSEL_USBIP1_PP == USB_PERI_PP */
    }
}
/******************************************************************************
End of function usb_cpu_GoLpwrSleep
******************************************************************************/

/******************************************************************************
Function Name   : usb_cpu_GoLpwrDeepStby
Description     : Go Low Power Deep Standby
Arguments       : USB_UTR_t *ptr        : USB internal structure. Selects USB channel.
Return value    : none
******************************************************************************/
void usb_cpu_GoLpwrDeepStby(USB_UTR_t *ptr)
{
    usb_cpu_GoDeepStbyMode(ptr);
    usb_cpu_GoWait();                           /* Goto WAIT */
}
/******************************************************************************
End of function usb_cpu_GoLpwrDeepStby
******************************************************************************/

/******************************************************************************
Function Name   : usb_cpu_GoDeepStbyMode
Description     : Go Deep Standby Mode
Arguments       : USB_UTR_t *ptr        : USB internal structure. Selects USB channel.
Return value    : none
******************************************************************************/
void usb_cpu_GoDeepStbyMode(USB_UTR_t *ptr)
{
/***************************************/
/*  Can not use DTC in Standby Mode !! */
/***************************************/
/*  set_psw(USB_PswIntDisable); */
/*  DTC.DTCST.BIT.DTCST   = 0;  */

    /* Oscillator stable Time 10922.7us(131072/12MHz) */
    /* Deep standby wait control register
    b5-b0 WTSTS    Deep software standby wait time bit
    b7-b6 Reserved 0
    */
    SYSTEM.DPSWCR.BIT.WTSTS     = 9;

    /* Standby control register (Software Standby Shift)
    b7-b0  Reserved 0
    b12-b8 STS      Standby timer select bit
    b13    Reserved 0
    b14    OPE      Output port enable bit
    b15    SSBY     Software standby bit
    */
    SYSTEM.SBYCR.BIT.SSBY       = 1;

    /* Deep Standby control register (Deep Standby Shift)
    b0    RAMCUT0  RAM off0 bit
    b3-b1 Reserved 0
    b4    RAMCUT1  RAM off1 bit
    b5    RAMCUT2  RAM off2 bit
    b6    IOKEEP   IO port keep bit
    b7    DPSBY    Deep standby bit
    */
    SYSTEM.DPSBYCR.BIT.DPSBY    = 1;
    SYSTEM.DPSBYCR.BIT.RAMCUT2  = 0;


    /* Deep Standby control register (Internal RAM0, USB Power supply on)
    b0    RAMCUT0  RAM off0 bit
    b3-b1 Reserved 0
    b4    RAMCUT1  RAM off1 bit
    b5    RAMCUT2  RAM off2 bit
    b6    IOKEEP   IO port keep bit
    b7    DPSBY    Deep standby bit
    */
    SYSTEM.DPSBYCR.BIT.RAMCUT1  = 0;

    /* Deep Standby control register (Internal RAM0, USB Power supply on)
    b0    RAMCUT0  RAM off0 bit
    b3-b1 Reserved 0
    b4    RAMCUT1  RAM off1 bit
    b5    RAMCUT2  RAM off2 bit
    b6    IOKEEP   IO port keep bit
    b7    DPSBY    Deep standby bit
    */
    SYSTEM.DPSBYCR.BIT.RAMCUT0  = 0;

    /* Standby control register (Bus Signal Output status keep)
    b7-b0  Reserved 0
    b12-b8 STS      Standby timer select bit
    b13    Reserved 0
    b14    OPE      Output port enable bit
    b15    SSBY     Software standby bit
    */
    SYSTEM.SBYCR.BIT.OPE        = 1;

    /* Deep Standby control register (IO status keep by the return)
    b0    RAMCUT0  RAM off0 bit
    b3-b1 Reserved 0
    b4    RAMCUT1  RAM off1 bit
    b5    RAMCUT2  RAM off2 bit
    b6    IOKEEP   IO port keep bit
    b7    DPSBY    Deep standby bit
    */
    SYSTEM.DPSBYCR.BIT.IOKEEP   = 1;

    /* Deep standby interrupt edge register (Down Edge Clear)
    b0    DIRQ0EG  IRQ0 edge select bit
    b1    DIRQ1EG  IRQ2 edge select bit
    b2    DIRQ2EG  IRQ3 edge select bit
    b3    DIRQ3EG  IRQ4 edge select bit
    b6-b4 Reserved 0
    b7    DNMIEG   NMI edge select bit
    */
    SYSTEM.DPSIEGR.BYTE         = 0;

    /* Deep standby interrupt enable register (Cancel by USB enable)
    b0 DIRQ0E IRQ0 pin enable bit
    b1 DIRQ1E IRQ1 pin enable bit
    b2 DIRQ2E IRQ2 pin enable bit
    b3 DIRQ3E IRQ3 pin enable bit
    b4 DLVDE  LVD deep standby cancel enable bit
    b5 DRTCE  RTC deep standby cancel enable bit
    b6 DUSBE  USB suspend/resume deep standby cancel enable bit
    b7 DNMIE  NMI deep standby cancel enable bit
    */
    SYSTEM.DPSIER.BIT.DUSBE     = 1;

    /* Deep standby interrupt flag register (Cancel by USB Request clear)
    b0 DIRQ0F IRQ0 pin cancel flag
    b1 DIRQ1F IRQ1 pin cancel flag
    b2 DIRQ2F IRQ2 pin cancel flag
    b3 DIRQ3F IRQ3 pin cancel flag
    b4 DLVDF  LVD deep standby cancel flag
    b5 DRTCF  RTC deep standby cancel flag
    b6 DUSBF  USB suspend/resume deep standby cancel flag
    b7 DNMIF  NMI deep standby cancel flag
    */
    SYSTEM.DPSIFR.BIT.DUSBF     = 0;

    /* Oscillator stop detect control register (disable)
    b5-b0  Reserved 0
    b6     OSTDF    Oscillator stop detect flag
    b7     OSTDE    Oscillator stop detect enable
    b15-b8 KEY      OSTDCR key code
    */
    SYSTEM.OSTDCR.WORD          = 0xAC00;

    usb_cpu_GoDeepStbyMode0(ptr);
    usb_cpu_GoDeepStbyMode1();
}
/******************************************************************************
End of function usb_cpu_GoDeepStbyMode
******************************************************************************/

/******************************************************************************
Function Name   : usb_cpu_GoDeepStbyMode0
Description     : Go Deep Standby Mode 0
Arguments       : USB_UTR_t *ptr        : USB internal structure. Selects USB channel.
Return value    : none
******************************************************************************/
void usb_cpu_GoDeepStbyMode0(USB_UTR_t *ptr)
{
    /* Deep standby USB monitor register (Single end Receiver Control)
    b0      SRPC0    USB0 single end control
    b3-b1   Reserved 0
    b4      FIXPHY0  USB0 transceiver output fix
    b7-b5   Reserved 0
    b8      SRPC1    USB1 single end control
    b11-b9  Reserved 0
    b12     FIXPHY1  USB1 transceiver output fix
    b15-b13 Reserved 0
    b16     DP0      USB0 DP input
    b17     DM0      USB0 DM input
    b19-b18 Reserved 0
    b20     DOVCA0   USB0 OVRCURA input
    b21     DOVCB0   USB0 OVRCURB input
    b22     Reserved 0
    b23     DVBSTS0  USB1 VBUS input
    b24     DP1      USB1 DP input
    b25     DM1      USB1 DM input
    b27-b26 Reserved 0
    b28     DOVCA1   USB1 OVRCURA input
    b29     DOVCB1   USB1 OVRCURB input
    b30     Reserved 0
    b31     DVBSTS1  USB1 VBUS input
    */
    USB.DPUSR0R.BIT.SRPC0       = 1;

    /* Deep standby USB monitor register (Transceiver Output fixed)
    b0      SRPC0    USB0 single end control
    b3-b1   Reserved 0
    b4      FIXPHY0  USB0 transceiver output fix
    b7-b5   Reserved 0
    b8      SRPC1    USB1 single end control
    b11-b9  Reserved 0
    b12     FIXPHY1  USB1 transceiver output fix
    b15-b13 Reserved 0
    b16     DP0      USB0 DP input
    b17     DM0      USB0 DM input
    b19-b18 Reserved 0
    b20     DOVCA0   USB0 OVRCURA input
    b21     DOVCB0   USB0 OVRCURB input
    b22     Reserved 0
    b23     DVBSTS0  USB1 VBUS input
    b24     DP1      USB1 DP input
    b25     DM1      USB1 DM input
    b27-b26 Reserved 0
    b28     DOVCA1   USB1 OVRCURA input
    b29     DOVCB1   USB1 OVRCURB input
    b30     Reserved 0
    b31     DVBSTS1  USB1 VBUS input
    */
    USB.DPUSR0R.BIT.FIXPHY0     = 1;

/* Condition compilation by the difference of USB function */
    if ( usb_cstd_is_host_mode(ptr) == USB_NO )
    {
        /* Deep standby USB interrupt register(VBUS clear)
        b0      DPINTE0   USB0 DP interrupt enable clear bit
        b1      DMINTE0   USB0 DM interrupt enable clear bit
        b3-b2   Reserved  0
        b4      DOVRCRAE0 USB0 OVRCURA interrupt enable clear bit
        b5      DOVRCRBE0 USB0 OVRCURB interrupt enable clear bit
        b6      Reserved  0
        b7      DVBSE0    USB0 VBUS interrupt enable clear bit
        b8      DPINTE1   USB1 DP interrupt enable clear bit
        b9      DMINTE1   USB1 DM interrupt enable clear bit
        b11-b10 Reserved  0
        b12     DOVRCRAE1 USB1 OVRCURA interrupt enable clear bit
        b13     OVRCRBE1  USB1 OVRCURB interrupt enable clear bit
        b14     Reserved  0
        b15     DVBSE1    USB1 VBUS interrupt enable clear bit
        */
        USB.DPUSR1R.BIT.DVBSE0      = 0;

        /* Deep standby USB interrupt register(VBUS set)
        b0      DPINTE0   USB0 DP interrupt enable clear bit
        b1      DMINTE0   USB0 DM interrupt enable clear bit
        b3-b2   Reserved  0
        b4      DOVRCRAE0 USB0 OVRCURA interrupt enable clear bit
        b5      DOVRCRBE0 USB0 OVRCURB interrupt enable clear bit
        b6      Reserved  0
        b7      DVBSE0    USB0 VBUS interrupt enable clear bit
        b8      DPINTE1   USB1 DP interrupt enable clear bit
        b9      DMINTE1   USB1 DM interrupt enable clear bit
        b11-b10 Reserved  0
        b12     DOVRCRAE1 USB1 OVRCURA interrupt enable clear bit
        b13     OVRCRBE1  USB1 OVRCURB interrupt enable clear bit
        b14     Reserved  0
        b15     DVBSE1    USB1 VBUS interrupt enable clear bit
        */
        USB.DPUSR1R.BIT.DVBSE0      = 1;
    }

/* Condition compilation by the difference of USB function */
    if ( usb_cstd_is_host_mode(ptr) == USB_YES )
    {
        /* Deep standby USB interrupt register(OVCR clear)
        b0      DPINTE0   USB0 DP interrupt enable clear bit
        b1      DMINTE0   USB0 DM interrupt enable clear bit
        b3-b2   Reserved  0
        b4      DOVRCRAE0 USB0 OVRCURA interrupt enable clear bit
        b5      DOVRCRBE0 USB0 OVRCURB interrupt enable clear bit
        b6      Reserved  0
        b7      DVBSE0    USB0 VBUS interrupt enable clear bit
        b8      DPINTE1   USB1 DP interrupt enable clear bit
        b9      DMINTE1   USB1 DM interrupt enable clear bit
        b11-b10 Reserved  0
        b12     DOVRCRAE1 USB1 OVRCURA interrupt enable clear bit
        b13     OVRCRBE1  USB1 OVRCURB interrupt enable clear bit
        b14     Reserved  0
        b15     DVBSE1    USB1 VBUS interrupt enable clear bit
        */
        USB.DPUSR1R.BIT.DOVRCRAE0   = 0;

        /* Deep standby USB interrupt register(OVCR clear)
        b0      DPINTE0   USB0 DP interrupt enable clear bit
        b1      DMINTE0   USB0 DM interrupt enable clear bit
        b3-b2   Reserved  0
        b4      DOVRCRAE0 USB0 OVRCURA interrupt enable clear bit
        b5      DOVRCRBE0 USB0 OVRCURB interrupt enable clear bit
        b6      Reserved  0
        b7      DVBSE0    USB0 VBUS interrupt enable clear bit
        b8      DPINTE1   USB1 DP interrupt enable clear bit
        b9      DMINTE1   USB1 DM interrupt enable clear bit
        b11-b10 Reserved  0
        b12     DOVRCRAE1 USB1 OVRCURA interrupt enable clear bit
        b13     OVRCRBE1  USB1 OVRCURB interrupt enable clear bit
        b14     Reserved  0
        b15     DVBSE1    USB1 VBUS interrupt enable clear bit
        */
        USB.DPUSR1R.BIT.DOVRCRBE0   = 0;

        /* Deep standby USB interrupt register(OVCR set)
        b0      DPINTE0   USB0 DP interrupt enable clear bit
        b1      DMINTE0   USB0 DM interrupt enable clear bit
        b3-b2   Reserved  0
        b4      DOVRCRAE0 USB0 OVRCURA interrupt enable clear bit
        b5      DOVRCRBE0 USB0 OVRCURB interrupt enable clear bit
        b6      Reserved  0
        b7      DVBSE0    USB0 VBUS interrupt enable clear bit
        b8      DPINTE1   USB1 DP interrupt enable clear bit
        b9      DMINTE1   USB1 DM interrupt enable clear bit
        b11-b10 Reserved  0
        b12     DOVRCRAE1 USB1 OVRCURA interrupt enable clear bit
        b13     OVRCRBE1  USB1 OVRCURB interrupt enable clear bit
        b14     Reserved  0
        b15     DVBSE1    USB1 VBUS interrupt enable clear bit
        */
        USB.DPUSR1R.BIT.DOVRCRAE0   = 1;

        /* Deep standby USB interrupt register(OVCR set)
        b0      DPINTE0   USB0 DP interrupt enable clear bit
        b1      DMINTE0   USB0 DM interrupt enable clear bit
        b3-b2   Reserved  0
        b4      DOVRCRAE0 USB0 OVRCURA interrupt enable clear bit
        b5      DOVRCRBE0 USB0 OVRCURB interrupt enable clear bit
        b6      Reserved  0
        b7      DVBSE0    USB0 VBUS interrupt enable clear bit
        b8      DPINTE1   USB1 DP interrupt enable clear bit
        b9      DMINTE1   USB1 DM interrupt enable clear bit
        b11-b10 Reserved  0
        b12     DOVRCRAE1 USB1 OVRCURA interrupt enable clear bit
        b13     OVRCRBE1  USB1 OVRCURB interrupt enable clear bit
        b14     Reserved  0
        b15     DVBSE1    USB1 VBUS interrupt enable clear bit
        */
        USB.DPUSR1R.BIT.DOVRCRBE0   = 1;
    }

    /* Deep standby USB interrupt register(DP clear)
    b0      DPINTE0   USB0 DP interrupt enable clear bit
    b1      DMINTE0   USB0 DM interrupt enable clear bit
    b3-b2   Reserved  0
    b4      DOVRCRAE0 USB0 OVRCURA interrupt enable clear bit
    b5      DOVRCRBE0 USB0 OVRCURB interrupt enable clear bit
    b6      Reserved  0
    b7      DVBSE0    USB0 VBUS interrupt enable clear bit
    b8      DPINTE1   USB1 DP interrupt enable clear bit
    b9      DMINTE1   USB1 DM interrupt enable clear bit
    b11-b10 Reserved  0
    b12     DOVRCRAE1 USB1 OVRCURA interrupt enable clear bit
    b13     OVRCRBE1  USB1 OVRCURB interrupt enable clear bit
    b14     Reserved  0
    b15     DVBSE1    USB1 VBUS interrupt enable clear bit
    */
    USB.DPUSR1R.BIT.DPINTE0     = 0;

    /* Deep standby USB interrupt register(DM clear)
    b0      DPINTE0   USB0 DP interrupt enable clear bit
    b1      DMINTE0   USB0 DM interrupt enable clear bit
    b3-b2   Reserved  0
    b4      DOVRCRAE0 USB0 OVRCURA interrupt enable clear bit
    b5      DOVRCRBE0 USB0 OVRCURB interrupt enable clear bit
    b6      Reserved  0
    b7      DVBSE0    USB0 VBUS interrupt enable clear bit
    b8      DPINTE1   USB1 DP interrupt enable clear bit
    b9      DMINTE1   USB1 DM interrupt enable clear bit
    b11-b10 Reserved  0
    b12     DOVRCRAE1 USB1 OVRCURA interrupt enable clear bit
    b13     OVRCRBE1  USB1 OVRCURB interrupt enable clear bit
    b14     Reserved  0
    b15     DVBSE1    USB1 VBUS interrupt enable clear bit
    */
    USB.DPUSR1R.BIT.DMINTE0     = 0;

    /* Deep standby USB interrupt register(DP set)
    b0      DPINTE0   USB0 DP interrupt enable clear bit
    b1      DMINTE0   USB0 DM interrupt enable clear bit
    b3-b2   Reserved  0
    b4      DOVRCRAE0 USB0 OVRCURA interrupt enable clear bit
    b5      DOVRCRBE0 USB0 OVRCURB interrupt enable clear bit
    b6      Reserved  0
    b7      DVBSE0    USB0 VBUS interrupt enable clear bit
    b8      DPINTE1   USB1 DP interrupt enable clear bit
    b9      DMINTE1   USB1 DM interrupt enable clear bit
    b11-b10 Reserved  0
    b12     DOVRCRAE1 USB1 OVRCURA interrupt enable clear bit
    b13     OVRCRBE1  USB1 OVRCURB interrupt enable clear bit
    b14     Reserved  0
    b15     DVBSE1    USB1 VBUS interrupt enable clear bit
    */
    USB.DPUSR1R.BIT.DPINTE0     = 1;

    /* Deep standby USB interrupt register(DM set)
    b0      DPINTE0   USB0 DP interrupt enable clear bit
    b1      DMINTE0   USB0 DM interrupt enable clear bit
    b3-b2   Reserved  0
    b4      DOVRCRAE0 USB0 OVRCURA interrupt enable clear bit
    b5      DOVRCRBE0 USB0 OVRCURB interrupt enable clear bit
    b6      Reserved  0
    b7      DVBSE0    USB0 VBUS interrupt enable clear bit
    b8      DPINTE1   USB1 DP interrupt enable clear bit
    b9      DMINTE1   USB1 DM interrupt enable clear bit
    b11-b10 Reserved  0
    b12     DOVRCRAE1 USB1 OVRCURA interrupt enable clear bit
    b13     OVRCRBE1  USB1 OVRCURB interrupt enable clear bit
    b14     Reserved  0
    b15     DVBSE1    USB1 VBUS interrupt enable clear bit
    */
    USB.DPUSR1R.BIT.DMINTE0     = 1;
}
/******************************************************************************
End of function usb_cpu_GoDeepStbyMode0
******************************************************************************/


/******************************************************************************
Function Name   : usb_cpu_GoDeepStbyMode1
Description     : Go Deep Standby Mode 1
Arguments       : none
Return value    : none
******************************************************************************/
void usb_cpu_GoDeepStbyMode1(void)
{
    /* Deep standby USB monitor register (Single end Receiver Control)
    b0      SRPC0    USB0 single end control
    b3-b1   Reserved 0
    b4      FIXPHY0  USB0 transceiver output fix
    b7-b5   Reserved 0
    b8      SRPC1    USB1 single end control
    b11-b9  Reserved 0
    b12     FIXPHY1  USB1 transceiver output fix
    b15-b13 Reserved 0
    b16     DP0      USB0 DP input
    b17     DM0      USB0 DM input
    b19-b18 Reserved 0
    b20     DOVCA0   USB0 OVRCURA input
    b21     DOVCB0   USB0 OVRCURB input
    b22     Reserved 0
    b23     DVBSTS0  USB1 VBUS input
    b24     DP1      USB1 DP input
    b25     DM1      USB1 DM input
    b27-b26 Reserved 0
    b28     DOVCA1   USB1 OVRCURA input
    b29     DOVCB1   USB1 OVRCURB input
    b30     Reserved 0
    b31     DVBSTS1  USB1 VBUS input
    */
    USB.DPUSR0R.BIT.SRPC1       = 1;

    /* Deep standby USB monitor register (Transceiver Output fixed)
    b0      SRPC0    USB0 single end control
    b3-b1   Reserved 0
    b4      FIXPHY0  USB0 transceiver output fix
    b7-b5   Reserved 0
    b8      SRPC1    USB1 single end control
    b11-b9  Reserved 0
    b12     FIXPHY1  USB1 transceiver output fix
    b15-b13 Reserved 0
    b16     DP0      USB0 DP input
    b17     DM0      USB0 DM input
    b19-b18 Reserved 0
    b20     DOVCA0   USB0 OVRCURA input
    b21     DOVCB0   USB0 OVRCURB input
    b22     Reserved 0
    b23     DVBSTS0  USB1 VBUS input
    b24     DP1      USB1 DP input
    b25     DM1      USB1 DM input
    b27-b26 Reserved 0
    b28     DOVCA1   USB1 OVRCURA input
    b29     DOVCB1   USB1 OVRCURB input
    b30     Reserved 0
    b31     DVBSTS1  USB1 VBUS input
    */
    USB.DPUSR0R.BIT.FIXPHY1     = 1;

    /* Deep standby USB interrupt register(DP clear)
    b0      DPINTE0   USB0 DP interrupt enable clear bit
    b1      DMINTE0   USB0 DM interrupt enable clear bit
    b3-b2   Reserved  0
    b4      DOVRCRAE0 USB0 OVRCURA interrupt enable clear bit
    b5      DOVRCRBE0 USB0 OVRCURB interrupt enable clear bit
    b6      Reserved  0
    b7      DVBSE0    USB0 VBUS interrupt enable clear bit
    b8      DPINTE1   USB1 DP interrupt enable clear bit
    b9      DMINTE1   USB1 DM interrupt enable clear bit
    b11-b10 Reserved  0
    b12     DOVRCRAE1 USB1 OVRCURA interrupt enable clear bit
    b13     OVRCRBE1  USB1 OVRCURB interrupt enable clear bit
    b14     Reserved  0
    b15     DVBSE1    USB1 VBUS interrupt enable clear bit
    */
    USB.DPUSR1R.BIT.DPINTE1     = 0;

    /* Deep standby USB interrupt register(DM clear)
    b0      DPINTE0   USB0 DP interrupt enable clear bit
    b1      DMINTE0   USB0 DM interrupt enable clear bit
    b3-b2   Reserved  0
    b4      DOVRCRAE0 USB0 OVRCURA interrupt enable clear bit
    b5      DOVRCRBE0 USB0 OVRCURB interrupt enable clear bit
    b6      Reserved  0
    b7      DVBSE0    USB0 VBUS interrupt enable clear bit
    b8      DPINTE1   USB1 DP interrupt enable clear bit
    b9      DMINTE1   USB1 DM interrupt enable clear bit
    b11-b10 Reserved  0
    b12     DOVRCRAE1 USB1 OVRCURA interrupt enable clear bit
    b13     OVRCRBE1  USB1 OVRCURB interrupt enable clear bit
    b14     Reserved  0
    b15     DVBSE1    USB1 VBUS interrupt enable clear bit
    */
    USB.DPUSR1R.BIT.DMINTE1     = 0;

    /* Deep standby USB interrupt register(DP set)
    b0      DPINTE0   USB0 DP interrupt enable clear bit
    b1      DMINTE0   USB0 DM interrupt enable clear bit
    b3-b2   Reserved  0
    b4      DOVRCRAE0 USB0 OVRCURA interrupt enable clear bit
    b5      DOVRCRBE0 USB0 OVRCURB interrupt enable clear bit
    b6      Reserved  0
    b7      DVBSE0    USB0 VBUS interrupt enable clear bit
    b8      DPINTE1   USB1 DP interrupt enable clear bit
    b9      DMINTE1   USB1 DM interrupt enable clear bit
    b11-b10 Reserved  0
    b12     DOVRCRAE1 USB1 OVRCURA interrupt enable clear bit
    b13     OVRCRBE1  USB1 OVRCURB interrupt enable clear bit
    b14     Reserved  0
    b15     DVBSE1    USB1 VBUS interrupt enable clear bit
    */
    USB.DPUSR1R.BIT.DPINTE1     = 1;

    /* Deep standby USB interrupt register(DM set)
    b0      DPINTE0   USB0 DP interrupt enable clear bit
    b1      DMINTE0   USB0 DM interrupt enable clear bit
    b3-b2   Reserved  0
    b4      DOVRCRAE0 USB0 OVRCURA interrupt enable clear bit
    b5      DOVRCRBE0 USB0 OVRCURB interrupt enable clear bit
    b6      Reserved  0
    b7      DVBSE0    USB0 VBUS interrupt enable clear bit
    b8      DPINTE1   USB1 DP interrupt enable clear bit
    b9      DMINTE1   USB1 DM interrupt enable clear bit
    b11-b10 Reserved  0
    b12     DOVRCRAE1 USB1 OVRCURA interrupt enable clear bit
    b13     OVRCRBE1  USB1 OVRCURB interrupt enable clear bit
    b14     Reserved  0
    b15     DVBSE1    USB1 VBUS interrupt enable clear bit
    */
    USB.DPUSR1R.BIT.DMINTE1     = 1;
}
/******************************************************************************
End of function usb_cpu_GoDeepStbyMode1
******************************************************************************/


/******************************************************************************
Function Name   : usb_cpu_RegRecovEnable
Description     : Register Recover Enable
Arguments       : USB_UTR_t *ptr        : USB internal structure. Selects USB channel.
Return value    : none
******************************************************************************/
void usb_cpu_RegRecovEnable(USB_UTR_t *ptr)
{
    if( ptr->ip == USB_USBIP_0 )
    {
        /* Device state change register
        b14-b0 Reserved 0
        b15    DVCHG    Device state change bit
        */
        USB0.DVCHGR.BIT.DVCHG = 1;
    }
    else
    {
        /* Device state change register
        b14-b0 Reserved 0
        b15    DVCHG    Device state change bit
        */
        USB1.DVCHGR.BIT.DVCHG = 1;
    }
}
/******************************************************************************
End of function usb_cpu_RegRecovEnable
******************************************************************************/

/******************************************************************************
Function Name   : usb_cpu_RegRecovDisable
Description     : Register Recover Disable
Arguments       : USB_UTR_t *ptr        : USB internal structure. Selects USB channel.
Return value    : none
******************************************************************************/
void usb_cpu_RegRecovDisable(USB_UTR_t *ptr)
{
    if( ptr->ip == USB_USBIP_0 )
    {
        /* Device state change register
        b14-b0 Reserved 0
        b15    DVCHG    Device state change bit
        */
        USB0.DVCHGR.BIT.DVCHG = 0;
    }
    else
    {
        /* Device state change register
        b14-b0 Reserved 0
        b15    DVCHG    Device state change bit
        */
        USB1.DVCHGR.BIT.DVCHG = 0;
    }
}
/******************************************************************************
End of function usb_cpu_RegRecovDisable
******************************************************************************/

/******************************************************************************
Function Name   : usb_cpu_GoWait
Description     : Go Wait
Arguments       : none
Return value    : none
******************************************************************************/
void usb_cpu_GoWait(void)
{
    int_exception(11);              /* Goto usb_cpu_GoWait_Int() */
}
/******************************************************************************
End of function usb_cpu_GoWait
******************************************************************************/

/******************************************************************************
Function Name   : usb_cpu_GoWait_Int
Description     : Go Wait Interrupt
Arguments       : none
Return value    : none
******************************************************************************/
void usb_cpu_GoWait_Int(void)
{
    wait();                         /* Wait */
}
/******************************************************************************
End of function usb_cpu_GoWait_Int
******************************************************************************/

/******************************************************************************
Function Name   : usb_cpu_GetDPSRSTF
Description     : Get Deep Software Standby Reset Flag
Arguments       : none
Return value    : Deep Software Standby Reset Flag
******************************************************************************/
uint8_t usb_cpu_GetDPSRSTF(void)
{
    /* Read Deep software standby flag
    b0    POFR     Deep software standby flag
    b1    LVD1F    LVD1F flag
    b2    LVD2F    LVD2F flag
    b6-b3 Reserved 0
    b7    DPSRSTF  Power on reset flag
    */
    return (uint8_t)SYSTEM.RSTSR.BIT.DPSRSTF;
}
/******************************************************************************
End of function usb_cpu_GetDPSRSTF
******************************************************************************/

/******************************************************************************
IRQ function
******************************************************************************/

/******************************************************************************
Function Name   : usb_cpu_IRQ9_Enable
Description     : IRQ9 Interrupt Enable
Arguments       : none
Return value    : none
******************************************************************************/
void usb_cpu_IRQ9_Enable(void)
{
    /* Interrupt enable register (IEN1 disable)
    b0 IEN0 Interrupt enable bit
    b1 IEN1 Interrupt enable bit
    b2 IEN2 Interrupt enable bit
    b3 IEN3 Interrupt enable bit
    b4 IEN4 Interrupt enable bit
    b5 IEN5 Interrupt enable bit
    b6 IEN6 Interrupt enable bit
    b7 IEN7 Interrupt enable bit
    */
    ICU.IER[9].BIT.IEN1     = 0;        /* Interrupt disable */

    /* Port function register 8
    b0 ITS8     IRQ8 pin select bit
    b1 ITS9     IRQ9 pin select bit
    b2 ITS10    IRQ10 pin select bit
    b3 ITS11    IRQ11 pin select bit
    b4 Reserved 0
    b5 ITS13    IRQ13 pin select bit
    b6 Reserved 0
    b7 ITS15    IRQ15 pin select bit
    */
    IOPORT.PF8IRQ.BIT.ITS9  = 0;

    /* Input buffer control register (Enable USB0 VBUS)
    b0 B0 Pn0 input buffer control bit
    b1 B1 Pn1 input buffer control bit
    b2 B2 Pn2 input buffer control bit
    b3 B3 Pn3 input buffer control bit
    b4 B4 Pn4 input buffer control bit
    b5 B5 Pn5 input buffer control bit
    b6 B6 Pn6 input buffer control bit
    b7 B7 Pn7 input buffer control bit
    */
    PORT0.ICR.BIT.B1        = 1;

    /* IRQ control register
    b1-b0 Reserved 0
    b3-b2 IRQMD    IRQ detect bit
    b7-b4 Reserved 0
    */
    ICU.IRQCR[9].BIT.IRQMD  = 1;        /* Down Edge Set */

    /* Priority Resume0=B
    b3-b0 IPR      Interrupt priority
    b7-b4 Reserved 0
    */
    ICU.IPR[0x29].BYTE      = 0x0B;     /* Priority Resume0=B */

    /* Interrupt request register
    b0    IR       Interrupt status flag
    b7-b1 Reserved 0
    */
    ICU.IR[73].BIT.IR       = 0;

    /* Interrupt enable register (IEN1 enable)
    b0 IEN0 Interrupt enable bit
    b1 IEN1 Interrupt enable bit
    b2 IEN2 Interrupt enable bit
    b3 IEN3 Interrupt enable bit
    b4 IEN4 Interrupt enable bit
    b5 IEN5 Interrupt enable bit
    b6 IEN6 Interrupt enable bit
    b7 IEN7 Interrupt enable bit
    */
    ICU.IER[9].BIT.IEN1     = 1;
}
/******************************************************************************
End of function usb_cpu_IRQ9_Enable
******************************************************************************/

/******************************************************************************
Function Name   : usb_cpu_IRQ9_Disable
Description     : IRQ9 Interrupt Disable
Arguments       : none
Return value    : none
******************************************************************************/
void usb_cpu_IRQ9_Disable(void)
{
    /* Input buffer control register (Enable USB0 VBUS)
    b0 B0 Pn0 input buffer control bit
    b1 B1 Pn1 input buffer control bit
    b2 B2 Pn2 input buffer control bit
    b3 B3 Pn3 input buffer control bit
    b4 B4 Pn4 input buffer control bit
    b5 B5 Pn5 input buffer control bit
    b6 B6 Pn6 input buffer control bit
    b7 B7 Pn7 input buffer control bit
    */
    PORT0.ICR.BIT.B1        = 0;

    /* Interrupt request register
    b0    IR       Interrupt status flag
    b7-b1 Reserved 0
    */
    ICU.IR[73].BIT.IR       = 0;

    /* Interrupt enable register (IEN1 enable)
    b0 IEN0 Interrupt enable bit
    b1 IEN1 Interrupt enable bit
    b2 IEN2 Interrupt enable bit
    b3 IEN3 Interrupt enable bit
    b4 IEN4 Interrupt enable bit
    b5 IEN5 Interrupt enable bit
    b6 IEN6 Interrupt enable bit
    b7 IEN7 Interrupt enable bit
    */
    ICU.IER[9].BIT.IEN1     = 0;

    /* Priority Resume0=0
    b3-b0 IPR      Interrupt priority
    b7-b4 Reserved 0
    */
    ICU.IPR[0x29].BYTE      = 0x00;
}
/******************************************************************************
End of function usb_cpu_IRQ9_Disable
******************************************************************************/

/******************************************************************************
Function Name   : usb_cpu_IRQ9Int
Description     : IRQ9 Interrupt process. RSK SW2.
                : IRQ #73 (VECT_ICU_IRQ9)
Arguments       : none
Return value    : none
******************************************************************************/
#pragma vector=VECT_ICU_IRQ9
__interrupt void usb_cpu_IRQ9Int(void)
{
/* Condition compilation by the difference of USB function */
#if USB_FUNCSEL_USBIP0_PP == USB_PERI_PP || USB_FUNCSEL_USBIP1_PP == USB_PERI_PP
    USB_UTR_t utr0, *ptr0;
    ptr0 = (USB_UTR_t *)&utr0;

    ptr0->ip    = USB_PERI_USBIP_NUM;
    ptr0->ipp   = usb_cstd_GetUsbIpAdr(ptr0->ip);

/* Condition compilation by the difference of the devices */
 #if (USB_CPU_LPW_PP == USB_LPWR_USE_PP)
    R_usb_pstd_PcdChangeDeviceState(ptr0, USB_DO_REMOTEWAKEUP, (uint16_t)0, (USB_CB_INFO_t)usb_cstd_DummyFunction);
    usb_cpu_IRQ9_Disable();
 #endif /* (USB_CPU_LPW_PP == USB_LPWR_USE_PP) */
#endif /* USB_FUNCSEL_USBIP0_PP == USB_PERI_PP || USB_FUNCSEL_USBIP1_PP == USB_PERI_PP */
}
/******************************************************************************
End of function usb_cpu_IRQ9Int
******************************************************************************/

/******************************************************************************
DMA & DTC function
******************************************************************************/

/******************************************************************************
Function Name   : usb_cpu_DmaintInit
Description     : DMA interrupt Initialize
Arguments       : void
Return value    : void
******************************************************************************/
void usb_cpu_DmaintInit(void)
{
/* Condition compilation by the difference of the devices */
#if USB_TRANS_MODE_PP == USB_TRANS_DMA_PP
    /* Input buffer control register (Enable EDREQ) */
    PORT5.ICR.BIT.B5=1;

    /* PORT Function register (DMA)
    b3-b0 Reserved 0
    b5-b4 EXDMAC0 select bit
    b7-b6 EXDMAC1 select bit
    */
    IOPORT.PF7DMA.BIT.EDMA0S=2; /* EDREQ0-C, EDACK0-C */

    /* EXDMAC module stop release (MSTPA29) */
    SYSTEM.MSTPCRA.BIT.MSTPA29=0;

    /* EXDMA Transfer enable register
    b0    DTE       DMA Transfer enable bit
    b7-b1 Reserved 0
    */
    EXDMAC0.EDMCNT.BIT.DTE=0;

    /* EXDMA Output Setting register
    b0    Reserved 0
    b1    DACKW    EDACK Pin Negate Wait
    b2    DACKE    EDACKn Pin Output Enable
    b3    DACKS    EDACKn Pin Output Polarity Select
    b7-b4 Reserved 0
    */
    EXDMAC0.EDMOMD.BIT.DACKS=0;
    EXDMAC0.EDMOMD.BIT.DACKE=1;
    EXDMAC0.EDMOMD.BIT.DACKW=1;

    /* EXDMA External Request Sense Mode Register
    b1-b0 DREQS    Request Input Sense Mode Set
    b7-b2 Reserved 0
    */
    EXDMAC0.EDMRMD.BIT.DREQS=1;

    /* EXDMA Address Mode register
    b4-b0   DARA     Destination Address Extended Repeat Area
    b5      Reserved 0
    b7-b6   DM       Destination Address Update Mode
    b12-b8  SARA     Source Address Extended Repeat Area
    b13     Reserved 0
    b15-b14 SM       Source Address Update Mode
    b16     DIR      Single Address Direction Select
    b17     AMS      Address Mode Select
    b31-b18 Reserved 0
    */
    EXDMAC0.EDMAMD.BIT.DM=0;
    EXDMAC0.EDMAMD.BIT.SM=2;
    EXDMAC0.EDMAMD.BIT.DARA=0;
    EXDMAC0.EDMAMD.BIT.SARA=0;
    EXDMAC0.EDMAMD.BIT.DIR=0;
    EXDMAC0.EDMAMD.BIT.AMS=1;

    /* EXDMA Transfer Mode register
    b1-b0   DCTG     DMA Request Source Select
    b7-b2   Reserved 0
    b9-b8   SZ       Transfer Data Size Select
    b11-b10 Reserved 0
    b13-b12 DTS      Repeat Area Select
    b15-b14 MD       Transfer Mode Select
    */
    EXDMAC0.EDMTMD.BIT.DCTG=2;
    EXDMAC0.EDMTMD.BIT.DTS=0;
    EXDMAC0.EDMTMD.BIT.MD=2;

    /* EXDMA External Request Flag Register
    b0    EREQ     External Request Flag
    b7-b1 Reserved 0
    */
    EXDMAC0.EDMERF.BIT.EREQ=1;

    /* EXDMA Interrupt Setting Register
    b0    DARIE    Destination Address Extended Repeat Area Overflow
                   Interrupt Enable
    b1    SARIE    Source Address Extended Repeat Area Overflow Interrupt Enable
    b2    RPTIE    Repeat Size End Interrupt Enable
    b3    ESIE     Transfer Escape End Interrupt Enable
    b4    DTIE     Transfer End Interrupt Enable
    b7-b5 Reserved 0
    */
    EXDMAC0.EDMINT.BIT.DARIE=0;
    EXDMAC0.EDMINT.BIT.SARIE=0;
    EXDMAC0.EDMINT.BIT.RPTIE=0;
    EXDMAC0.EDMINT.BIT.ESIE=0;
    EXDMAC0.EDMINT.BIT.DTIE=1;
#endif  /* USB_TRANS_MODE_PP == USB_TRANS_DMA_PP */

/* Condition compilation by the difference of the devices */
#if USB_TRANS_MODE_PP == USB_TRANS_DTC_PP
    /* Enable DTC module(MSTPA28)
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
    SYSTEM.MSTPCRA.BIT.MSTPA28  = 0;

    /* DTC vector register
    b21-b0 DTCVBR Vector table address
    */
    DTC.DTCVBR                  = (void *)&usb_dtcctable;

    /* DTC Transfer enable
    b0    DTCST    DTC module start bit
    b7-b1 Reserved 0
    */
    DTC.DTCST.BIT.DTCST         = 1;

    /* DTC address mode register (Full Address Mode)
    b0    SHORT    Short address mode bit
    b7-b1 Reserved 0
    */
    DTC.DTCADMOD.BIT.SHORT      = 0;

#endif  /* USB_TRANS_MODE_PP == USB_TRANS_DTC_PP */
}
/******************************************************************************
End of function usb_cpu_DmaintInit
******************************************************************************/

/******************************************************************************
Function Name   : usb_cpu_d0fifo2buf_start_dma
Description     : FIFO to Buffer data read DMA start
Arguments       : USB_UTR_t *ptr        : USB internal structure. Selects USB channel.
                : uint32_t SourceAddr   : Source address
Return value    : void
******************************************************************************/
void usb_cpu_d0fifo2buf_start_dma(USB_UTR_t *ptr, uint32_t SourceAddr)
{
/* Condition compilation by the difference of the devices */
 #if USB_TRANS_MODE_PP == USB_TRANS_DMA_PP
 #endif /* USB_TRANS_MODE_PP == USB_TRANS_DMA_PP */

/* Condition compilation by the difference of the devices */
 #if USB_TRANS_MODE_PP == USB_TRANS_DTC_PP
    uint16_t    size;
    uint32_t    tmp;

    /* DTC Transfer enable
    b0    DTCST    DTC module start bit
    b7-b1 Reserved 0
    */
    DTC.DTCST.BIT.DTCST         = 1;

    /* DTC control register (Transfer Information Read No Skip)
    b2-b0 Reserved 0
    b3    Reserved 0
    b4    RRS      DTC Transfer Information Read Skip enable bit
    b7-b5 Reserved 0
    */
    DTC.DTCCR.BIT.RRS           = 0;

    /* DTC mode register A (Block Transfer Set)
    b1-b0 Reserved 0
    b3-b2 SM       source address mode bit
    b5-b4 SZ       DTC data transfer size bit
    b7-b6 MD       DTC mode bit
    */
    usb_dtcreg[ptr->ip].MRA.BIT.MD      = 2;

    tmp                     = usb_gcstd_Dma0Fifo[ptr->ip] / 2;

    /* DTC mode register A (Word Size)
    b1-b0 Reserved 0
    b3-b2 SM       source address mode bit
    b5-b4 SZ       DTC data transfer size bit
    b7-b6 MD       DTC mode bit
    */
    usb_dtcreg[ptr->ip].MRA.BIT.SZ  = 1;

    /* DTC mode register A (Source Address fixed)
    b1-b0 Reserved 0
    b3-b2 SM       source address mode bit
    b5-b4 SZ       DTC data transfer size bit
    b7-b6 MD       DTC mode bit
    */
    usb_dtcreg[ptr->ip].MRA.BIT.SM      = 0;

    /* DTC mode register B (Chain Transfer disable)
    b1-b0 Reserved 0
    b3-b2 DM       Destination address mode bit
    b4    DTS      DTC transfer mode select bit
    b5    DISEL    DTC interrupt select bit
    b6    CHNS     DTC chain transfer select bit
    b7    CHNE     DTC chain transfer enable bit
    */
    usb_dtcreg[ptr->ip].MRB.BIT.CHNE        = 0;

    /* DTC mode register B (Select Data Transfer End Interrupt)
    b1-b0 Reserved 0
    b3-b2 DM       Destination address mode bit
    b4    DTS      DTC transfer mode select bit
    b5    DISEL    DTC interrupt select bit
    b6    CHNS     DTC chain transfer select bit
    b7    CHNE     DTC chain transfer enable bit
    */
    usb_dtcreg[ptr->ip].MRB.BIT.DISEL   = 0;

    /* DTC mode register B (Source Side Block Area)
    b1-b0 Reserved 0
    b3-b2 DM       Destination address mode bit
    b4    DTS      DTC transfer mode select bit
    b5    DISEL    DTC interrupt select bit
    b6    CHNS     DTC chain transfer select bit
    b7    CHNE     DTC chain transfer enable bit
    */
    usb_dtcreg[ptr->ip].MRB.BIT.DTS     = 1;

    /* DTC mode register B (Destination Address Increment)
    b1-b0 Reserved 0
    b3-b2 DM       Destination address mode bit
    b4    DTS      DTC transfer mode select bit
    b5    DISEL    DTC interrupt select bit
    b6    CHNS     DTC chain transfer select bit
    b7    CHNE     DTC chain transfer enable bit
    */
    usb_dtcreg[ptr->ip].MRB.BIT.DM      = 2;

    /* DTC source address register (FIFO port address)
    b31-b0 SAR Destination address
    */
    usb_dtcreg[ptr->ip].SAR             = SourceAddr;

    /* DTC source address register (Table address)
    b31-b0 SAR Source address
    */
    usb_dtcreg[ptr->ip].DAR = (uint32_t)(usb_gcstd_DataPtr[ptr->ip][usb_gcstd_Dma0Pipe[ptr->ip]]);

    size            = (uint8_t )(tmp);
    size            += (uint16_t)(tmp << 8);

    /* DTC transfer count registerA
    b15-b0 CRA Transfer count
    */
    usb_dtcreg[ptr->ip].CRA             = (uint16_t)(size);

    /* DTC transfer count registerB (Block count)
    b15-b0 CRB Transfer count
    */
    usb_dtcreg[ptr->ip].CRB = 1;

    /* DTC address mode register (Full Address Mode)
    b0    SHORT    Short address mode bit
    b7-b1 Reserved 0
    */
    DTC.DTCADMOD.BIT.SHORT      = 0;

    /* DTC control register (Transfer Information Read No Skip)
    b2-b0 Reserved 0
    b3    Reserved 0
    b4    RRS      DTC Transfer Information Read Skip enable bit
    b7-b5 Reserved 0
    */
    DTC.DTCCR.BIT.RRS           = 1;

    if( ptr->ip == USB_USBIP_0 )
    {
        /* Priority D0FIFO0=5
        b3-b0 IPR      Interrupt priority
        b7-b4 Reserved 0
        */
        ICU.IPR[0x0C].BYTE          = 0x00;

        /* Interrupt enable register (USB0 D0FIFO enable(IEN4))
        b0 IEN0 Interrupt enable bit
        b1 IEN1 Interrupt enable bit
        b2 IEN2 Interrupt enable bit
        b3 IEN3 Interrupt enable bit
        b4 IEN4 Interrupt enable bit
        b5 IEN5 Interrupt enable bit
        b6 IEN6 Interrupt enable bit
        b7 IEN7 Interrupt enable bit
        */
        ICU.IER[0x04].BIT.IEN4      = 1;

        /* DTC start enable register (USB0 D0FIFO transfer)
        b0    DTCE     DTC start enable bit
        b7-b1 Reserved 0
        */
        ICU.DTCER[36].BIT.DTCE      = 1;
    }
    else
    {
        /* Priority D0FIFO1=5
        b3-b0 IPR      Interrupt priority
        b7-b4 Reserved 0
        */
        ICU.IPR[0x10].BYTE          = 0x00;

        /* Interrupt enable register (USB1 D0FIFO enable(IEN0))
        b0 IEN0 Interrupt enable bit
        b1 IEN1 Interrupt enable bit
        b2 IEN2 Interrupt enable bit
        b3 IEN3 Interrupt enable bit
        b4 IEN4 Interrupt enable bit
        b5 IEN5 Interrupt enable bit
        b6 IEN6 Interrupt enable bit
        b7 IEN7 Interrupt enable bit
        */
        ICU.IER[0x05].BIT.IEN0      = 1;

        /* DTC start enable register (USB1 D0FIFO transfer)
        b0    DTCE     DTC start enable bit
        b7-b1 Reserved 0
        */
        ICU.DTCER[40].BIT.DTCE      = 1;
    }
 #endif /* USB_TRANS_MODE_PP == USB_TRANS_DTC_PP */
}
/******************************************************************************
End of function usb_cpu_d0fifo2buf_start_dma
******************************************************************************/

/******************************************************************************
Function Name   : usb_cpu_buf2d0fifo_start_dma
Description     : Buffer to FIFO data write DMA start
Arguments       : USB_UTR_t *ptr        : USB internal structure. Selects USB channel.
                : uint32_t DistAddr     : Destination address
Return value    : void
******************************************************************************/
void usb_cpu_buf2d0fifo_start_dma(USB_UTR_t *ptr, uint32_t DistAdr)
{
/* Condition compilation by the difference of the devices */
 #if USB_TRANS_MODE_PP == USB_TRANS_DMA_PP
    uint32_t    size;
    uint32_t    tmp;

    if( (usb_gcstd_Dma0Size[ptr->ip] & 0x0001) != 0 )
    {
        /* if count == odd */
        tmp = usb_gcstd_Dma0Size[ptr->ip];
        /* EXDMA transfer mode register (Byte Size) */
        EXDMAC0.EDMTMD.BIT.SZ   = 0;
    }
    else
    {
        tmp = usb_gcstd_Dma0Size[ptr->ip] / 2;
        /* EXDMA transfer mode register A (Word Size) */
        EXDMAC0.EDMTMD.BIT.SZ   = 1;
    }

    /* EXDMA source address register (Buf address)
    b31-b0 SAR Source address
    */
    EXDMAC0.EDMSAR  = (uint8_t*)usb_gcstd_DataPtr[ptr->ip][usb_gcstd_Dma0Pipe[ptr->ip]];

    /* EXDMA destination address register (D0FIFO port address)
    b31-b0 DAR Destination address
    */
    EXDMAC0.EDMDAR  = (uint8_t*)DistAdr;

    size    = (uint32_t )(tmp);
    size    += (uint32_t)(tmp << 16);

    /* EXDMA transfer count registerA
    b15-b0 CRA Transfer count
    */
    EXDMAC0.EDMCRA  = (uint32_t)(size);

    /* EXDMA transfer count registerB (Block count)
    b15-b0 CRB Transfer count
    */
    EXDMAC0.EDMCRB = (uint16_t)(usb_gcstd_DataCnt[ptr->ip][usb_gcstd_Dma0Pipe[ptr->ip]]
        / usb_gcstd_Dma0Size[ptr->ip]);

    /* Priority EXDMACI0=5
    b2-b0 IPR      Interrupt priority
    b7-b3 Reserved 0
    */
    ICU.IPR[0x74].BYTE  = 0x05;

    /* Interrupt enable register (EXDMACI0 enable(IEN2)) */
    ICU.IER[0x19].BIT.IEN2  = 1;

    /* EXDMA module enable register
    b0    DMST     EXDMAC enable bit
    b7-b1 Reserved 0
    */
    EXDMAC.EDMAST.BIT.DMST  = 1;

    /* EXDMA Transfer enable register
    b0    DTE       DMA Transfer enable bit
    b7-b1 Reserved 0
    */
    EXDMAC0.EDMCNT.BIT.DTE  = 1;
 #endif /* USB_TRANS_MODE_PP == USB_TRANS_DMA_PP */

/* Condition compilation by the difference of the devices */
 #if USB_TRANS_MODE_PP == USB_TRANS_DTC_PP
    uint16_t    size;
    uint32_t    tmp;

    /* DTC Transfer enable
    b0    DTCST    DTC module start bit
    b7-b1 Reserved 0
    */
    DTC.DTCST.BIT.DTCST = 1;

    /* DTC control register (Transfer Information Read No Skip)
    b2-b0 Reserved 0
    b3    Reserved 0
    b4    RRS      DTC Transfer Information Read Skip enable bit
    b7-b5 Reserved 0
    */
    DTC.DTCCR.BIT.RRS = 0;

    /* DTC mode register A (Block Transfer Set)
    b1-b0 Reserved 0
    b3-b2 SM       source address mode bit
    b5-b4 SZ       DTC data transfer size bit
    b7-b6 MD       DTC mode bit
    */
    usb_dtcreg[ptr->ip].MRA.BIT.MD = 2;

    if( usb_gcstd_Dma0Size[ptr->ip] >= usb_gcstd_Dma0Fifo[ptr->ip] )
    {
        tmp                     = usb_gcstd_Dma0Size[ptr->ip] / 2;

        /* DTC mode register A (Word Size)
        b1-b0 Reserved 0
        b3-b2 SM       source address mode bit
        b5-b4 SZ       DTC data transfer size bit
        b7-b6 MD       DTC mode bit
        */
        usb_dtcreg[ptr->ip].MRA.BIT.SZ  = 1;
    }
    else
    {
    if( (usb_gcstd_Dma0Size[ptr->ip] & 0x0001) != 0 )
    {
        /* if count == odd */
        tmp = usb_gcstd_Dma0Size[ptr->ip];

        /* DTC mode register A (Byte Size)
        b1-b0 Reserved 0
        b3-b2 SM       source address mode bit
        b5-b4 SZ       DTC data transfer size bit
        b7-b6 MD       DTC mode bit
        */
        usb_dtcreg[ptr->ip].MRA.BIT.SZ  = 0;
    }
    else
    {
        tmp                     = usb_gcstd_Dma0Size[ptr->ip] / 2;

        /* DTC mode register A (Word Size)
        b1-b0 Reserved 0
        b3-b2 SM       source address mode bit
        b5-b4 SZ       DTC data transfer size bit
        b7-b6 MD       DTC mode bit
        */
        usb_dtcreg[ptr->ip].MRA.BIT.SZ  = 1;
    }
    }

    /* DTC mode register A (Source Address Increment)
    b1-b0 Reserved 0
    b3-b2 SM       source address mode bit
    b5-b4 SZ       DTC data transfer size bit
    b7-b6 MD       DTC mode bit
    */
    usb_dtcreg[ptr->ip].MRA.BIT.SM      = 2;

    /* DTC mode register B (Chain Transfer disable)
    b1-b0 Reserved 0
    b3-b2 DM       Destination address mode bit
    b4    DTS      DTC transfer mode select bit
    b5    DISEL    DTC interrupt select bit
    b6    CHNS     DTC chain transfer select bit
    b7    CHNE     DTC chain transfer enable bit
    */
    usb_dtcreg[ptr->ip].MRB.BIT.CHNE        = 0;

    /* DTC mode register B (Select Data Transfer End Interrupt)
    b1-b0 Reserved 0
    b3-b2 DM       Destination address mode bit
    b4    DTS      DTC transfer mode select bit
    b5    DISEL    DTC interrupt select bit
    b6    CHNS     DTC chain transfer select bit
    b7    CHNE     DTC chain transfer enable bit
    */
    usb_dtcreg[ptr->ip].MRB.BIT.DISEL   = 0;

    /* DTC mode register B (Destination Side Block Area)
    b1-b0 Reserved 0
    b3-b2 DM       Destination address mode bit
    b4    DTS      DTC transfer mode select bit
    b5    DISEL    DTC interrupt select bit
    b6    CHNS     DTC chain transfer select bit
    b7    CHNE     DTC chain transfer enable bit
    */
    usb_dtcreg[ptr->ip].MRB.BIT.DTS     = 0;

    /* DTC mode register B (Destination Address fixed)
    b1-b0 Reserved 0
    b3-b2 DM       Destination address mode bit
    b4    DTS      DTC transfer mode select bit
    b5    DISEL    DTC interrupt select bit
    b6    CHNS     DTC chain transfer select bit
    b7    CHNE     DTC chain transfer enable bit
    */
    usb_dtcreg[ptr->ip].MRB.BIT.DM      = 0;

    /* DTC source address register (Table address)
    b31-b0 SAR Destination address
    */
    usb_dtcreg[ptr->ip].SAR = (uint32_t)(usb_gcstd_DataPtr[ptr->ip][usb_gcstd_Dma0Pipe[ptr->ip]]);

    usb_gcstd_dtc_transfer_address[ptr->ip] = usb_dtcreg[ptr->ip].SAR + usb_gcstd_Dma0Size[ptr->ip];


    /* DTC source address register (FIFO port address)
    b31-b0 SAR Source address
    */
    usb_dtcreg[ptr->ip].DAR             = (uint32_t)(DistAdr);
    size                        = (uint8_t )(tmp);
    size                       += (uint16_t)(tmp << 8);

    /* DTC transfer count registerA
    b15-b0 CRA Transfer count
    */
    usb_dtcreg[ptr->ip].CRA             = (uint16_t)(size);

    /* DTC transfer count registerB (Block count)
    b15-b0 CRB Transfer count
    */
    usb_dtcreg[ptr->ip].CRB = 1;

    /* DTC address mode register (Full Address Mode)
    b0    SHORT    Short address mode bit
    b7-b1 Reserved 0
    */
    DTC.DTCADMOD.BIT.SHORT      = 0;

    /* DTC control register (Transfer Information Read No Skip)
    b2-b0 Reserved 0
    b3    Reserved 0
    b4    RRS      DTC Transfer Information Read Skip enable bit
    b7-b5 Reserved 0
    */
    DTC.DTCCR.BIT.RRS           = 1;

    if( ptr->ip == USB_USBIP_0 )
    {
        /* Priority D0FIFO0=5
        b3-b0 IPR      Interrupt priority
        b7-b4 Reserved 0
        */
        ICU.IPR[0x0C].BYTE          = 0x00;

        /* Interrupt enable register (USB0 D0FIFO enable(IEN4))
        b0 IEN0 Interrupt enable bit
        b1 IEN1 Interrupt enable bit
        b2 IEN2 Interrupt enable bit
        b3 IEN3 Interrupt enable bit
        b4 IEN4 Interrupt enable bit
        b5 IEN5 Interrupt enable bit
        b6 IEN6 Interrupt enable bit
        b7 IEN7 Interrupt enable bit
        */
        ICU.IER[0x04].BIT.IEN4      = 1;

        /* DTC start enable register (USB0 D0FIFO transfer)
        b0    DTCE     DTC start enable bit
        b7-b1 Reserved 0
        */
        ICU.DTCER[36].BIT.DTCE      = 1;
    }
    else
    {
        /* Priority D0FIFO1=5
        b3-b0 IPR      Interrupt priority
        b7-b4 Reserved 0
        */
        ICU.IPR[0x10].BYTE          = 0x00;

        /* Interrupt enable register (USB0 D0FIF1 enable(IEN0))
        b0 IEN0 Interrupt enable bit
        b1 IEN1 Interrupt enable bit
        b2 IEN2 Interrupt enable bit
        b3 IEN3 Interrupt enable bit
        b4 IEN4 Interrupt enable bit
        b5 IEN5 Interrupt enable bit
        b6 IEN6 Interrupt enable bit
        b7 IEN7 Interrupt enable bit
        */
        ICU.IER[0x05].BIT.IEN0      = 1;

        /* DTC start enable register (USB1 D0FIFO transfer)
        b0    DTCE     DTC start enable bit
        b7-b1 Reserved 0
        */
        ICU.DTCER[40].BIT.DTCE      = 1;
    }
 #endif /* USB_TRANS_MODE_PP == USB_TRANS_DTC_PP */
}
/******************************************************************************
End of function usb_cpu_buf2d0fifo_start_dma
******************************************************************************/

/******************************************************************************
Function Name   : usb_cpu_d0fifo_stop_dma
Description     : DMA stop
Arguments       : USB_UTR_t *ptr        : USB internal structure. Selects USB channel.
Return value    : void
******************************************************************************/
void usb_cpu_d0fifo_stop_dma(USB_UTR_t *ptr)
{
    if( ptr->ip == USB_USBIP_0 )
    {
        /* Interrupt request register
        b0    IR       Interrupt status flag
        b7-b1 Reserved 0
        */
        ICU.IR[36].BIT.IR           = 0;

        /* Interrupt disable register (USB0 D0FIFO enable(IEN4))
        b0 IEN0 Interrupt enable bit
        b1 IEN1 Interrupt enable bit
        b2 IEN2 Interrupt enable bit
        b3 IEN3 Interrupt enable bit
        b4 IEN4 Interrupt enable bit
        b5 IEN5 Interrupt enable bit
        b6 IEN6 Interrupt enable bit
        b7 IEN7 Interrupt enable bit
        */
        ICU.IER[0x04].BIT.IEN4      = 0;

        /* Priority D0FIFO0=0
        b3-b0 IPR      Interrupt priority
        b7-b4 Reserved 0
        */
        ICU.IPR[0x0C].BYTE          = 0x00;

        /* DTC start disable register (USB0 D0FIFO transfer)
        b0    DTCE     DTC start enable bit
        b7-b1 Reserved 0
        */
        ICU.DTCER[36].BIT.DTCE      = 0;
    }
    else
    {
        /* Interrupt request register
        b0    IR       Interrupt status flag
        b7-b1 Reserved 0
        */
        ICU.IR[40].BIT.IR           = 0;

        /* Interrupt disable register (USB0 D0FIF1 enable(IEN0))
        b0 IEN0 Interrupt enable bit
        b1 IEN1 Interrupt enable bit
        b2 IEN2 Interrupt enable bit
        b3 IEN3 Interrupt enable bit
        b4 IEN4 Interrupt enable bit
        b5 IEN5 Interrupt enable bit
        b6 IEN6 Interrupt enable bit
        b7 IEN7 Interrupt enable bit
        */
        ICU.IER[0x05].BIT.IEN0      = 0;

        /* Priority D0FIFO1=0
        b3-b0 IPR      Interrupt priority
        b7-b4 Reserved 0
        */
        ICU.IPR[0x10].BYTE          = 0x00;

        /* DTC start disable register (USB1 D0FIFO transfer)
        b0    DTCE     DTC start enable bit
        b7-b1 Reserved 0
        */
        ICU.DTCER[40].BIT.DTCE      = 0;
    }

/* Condition compilation by the difference of the devices */
#if USB_TRANS_MODE_PP == USB_TRANS_DMA_PP
    /* Interrupt disable register (EXDMACI0 enable(IEN2)) */
    ICU.IER[0x19].BIT.IEN2  = 0;

    /* Priority EXDMACI0=0
    b2-b0 IPR      Interrupt priority
    b7-b3 Reserved 0
    */
    ICU.IPR[0x74].BYTE  = 0x00;

    /* EXDMA Transfer disable register
    b0    DTE       DMA Transfer enable bit
    b7-b1 Reserved 0
    */
    EXDMAC0.EDMCNT.BIT.DTE  = 0;
#endif  /* USB_TRANS_MODE_PP == USB_TRANS_DMA_PP */
}
/******************************************************************************
End of function usb_cpu_d0fifo_stop_dma
******************************************************************************/

/******************************************************************************
Function Name   : usb_cpu_d0fifo_restart_dma
Description     : DMA Restart
Arguments       : USB_UTR_t *ptr  : USB internal structure. Selects USB channel.
Return value    : none
******************************************************************************/
void usb_cpu_d0fifo_restart_dma( USB_UTR_t *ptr )
{
/* Condition compilation by the difference of the devices */
 #if USB_TRANS_MODE_PP == USB_TRANS_DMA_PP
 #endif /* USB_TRANS_MODE_PP == USB_TRANS_DMA_PP */

/* Condition compilation by the difference of the devices */
 #if USB_TRANS_MODE_PP == USB_TRANS_DTC_PP
    uint16_t    size;
    uint32_t    tmp;

    /* Check direction */
    if( usb_gcstd_Dma0Dir[ptr->ip] == USB_BUF2FIFO )
    {
        if( usb_gcstd_Dma0Size[ptr->ip] >= usb_gcstd_Dma0Fifo[ptr->ip] )
        {
            tmp = usb_gcstd_Dma0Fifo[ptr->ip] / 2;

            /* DTC mode register A (Word Size)
            b1-b0 Reserved 0
            b3-b2 SM       source address mode bit
            b5-b4 SZ       DTC data transfer size bit
            b7-b6 MD       DTC mode bit
            */
            usb_dtcreg[ptr->ip].MRA.BIT.SZ  = 1;
        }
        else
        {
    if( (usb_gcstd_Dma0Size[ptr->ip] & 0x0001u) != 0u )
    {
        /* if count == odd */
        tmp = usb_gcstd_Dma0Size[ptr->ip];

        /* DTC mode register A (Byte Size)
        b1-b0 Reserved 0
        b3-b2 SM       source address mode bit
        b5-b4 SZ       DTC data transfer size bit
        b7-b6 MD       DTC mode bit
        */
        usb_dtcreg[ptr->ip].MRA.BIT.SZ  = 0;
    }
    else
    {
        tmp = usb_gcstd_Dma0Size[ptr->ip] / 2;

        /* DTC mode register A (Word Size)
        b1-b0 Reserved 0
        b3-b2 SM       source address mode bit
        b5-b4 SZ       DTC data transfer size bit
        b7-b6 MD       DTC mode bit
        */
        usb_dtcreg[ptr->ip].MRA.BIT.SZ  = 1;
    }
        }
    }else{
        /* DTC mode register A (Word Size)
        b1-b0 Reserved 0
        b3-b2 SM       source address mode bit
        b5-b4 SZ       DTC data transfer size bit
        b7-b6 MD       DTC mode bit
        */
        usb_dtcreg[ptr->ip].MRA.BIT.SZ  = 1;

        /* DTC transfer count registerA
        b15-b0 CRA Transfer count
        */
        tmp = usb_gcstd_Dma0Fifo[ptr->ip] / 2;
    }
    size                        = (uint8_t )(tmp);
    size                       += (uint16_t)(tmp << 8);

    /* DTC transfer count registerA
    b15-b0 CRA Transfer count
    */
    usb_dtcreg[ptr->ip].CRA             = (uint16_t)(size);

    /* DTC transfer count registerB (Block count)
    b15-b0 CRB Transfer count
    */
    usb_dtcreg[ptr->ip].CRB             = (uint16_t)(1);

    /* DTC Transfer enable
    b0    DTCST    DTC module start bit
    b7-b1 Reserved 0
    */
    DTC.DTCST.BIT.DTCST         = 1;

    if( ptr->ip == USB_USBIP_0 )
    {
        /* DTC start enable register (USB0 D0FIFO transfer)
        b0    DTCE     DTC start enable bit
        b7-b1 Reserved 0
        */
        ICU.DTCER[36].BIT.DTCE      = 1;
    }
    else
    {
        /* DTC start enable register (USB0 D0FIFO transfer)
        b0    DTCE     DTC start enable bit
        b7-b1 Reserved 0
        */
        ICU.DTCER[40].BIT.DTCE      = 1;
    }

 #endif /* USB_TRANS_MODE_PP == USB_TRANS_DTC_PP */
}
/******************************************************************************
End of function usb_cpu_d0fifo_restart_dma
******************************************************************************/

/******************************************************************************
Function Name   : usb_cpu_d0fifo_enable_dma
Description     : DTC(D0FIFO) interrupt enable (Interrupt priority 5 set)
Arguments       : USB_UTR_t *ptr  : USB internal structure. Selects USB channel.
Return value    : none
******************************************************************************/
void usb_cpu_d0fifo_enable_dma(USB_UTR_t *ptr)
{
    if( ptr->ip == USB_USBIP_0 )
    {
        /* Priority D0FIFO0=0(Disable)
        b3-b0 IPR      Interrupt priority
        b7-b4 Reserved 0
        */
        ICU.IPR[0x0C].BYTE          = 0x05;
    }
    else
    {
        /* Priority D0FIFO0=0(Disable)
        b3-b0 IPR      Interrupt priority
        b7-b4 Reserved 0
        */
        ICU.IPR[0x10].BYTE          = 0x05;
    }
}
/******************************************************************************
End of function usb_cpu_d0fifo_enable_dma
******************************************************************************/

/******************************************************************************
Function Name   : usb_cpu_d0fifo_disable_dma
Description     : D0FIFO interrupt disable (Interrupt priority 0 set)
Arguments       : USB_UTR_t *ptr  : USB internal structure. Selects USB channel.
Return value    : none
******************************************************************************/
void usb_cpu_d0fifo_disable_dma(USB_UTR_t *ptr)
{
    if( ptr->ip == USB_USBIP_0 )
    {
        /* Priority D0FIFO0=0(Disable)
        b3-b0 IPR      Interrupt priority
        b7-b4 Reserved 0
        */
        ICU.IPR[0x0C].BYTE          = 0x00;
    }
    else
    {
        /* Priority D0FIFO0=0(Disable)
        b3-b0 IPR      Interrupt priority
        b7-b4 Reserved 0
        */
        ICU.IPR[0x10].BYTE          = 0x00;
    }
}
/******************************************************************************
End of function usb_cpu_d0fifo_disable_dma
******************************************************************************/

/******************************************************************************
Function Name   : usb_cpu_get_dtc_block_count
Description     :
Arguments       : USB_UTR_t *ptr        : USB internal structure. Selects USB channel.
Return value    :
******************************************************************************/
uint16_t usb_cpu_get_dtc_block_count(USB_UTR_t *ptr)
{
    return usb_dtcreg[ptr->ip].CRB;
}   /* eof usb_cpu_get_dtc_block_count() */

/******************************************************************************
Function Name   : usb_cpu_get_dtc_Source_address
Description     : Get DTC Source address
Arguments       : USB_UTR_t *ptr        : USB internal structure. Selects USB channel.
Return value    : DTC Source address
******************************************************************************/
uint32_t usb_cpu_get_dtc_Source_address(USB_UTR_t *ptr)
{
    while( DTC.DTCSTS.BIT.ACT );            /* Wait DTC Transfer */
    return usb_dtcreg[ptr->ip].SAR;
}   /* eof usb_cpu_get_dtc_Source_address() */

/******************************************************************************
Function Name   : usb_cpu_get_dtc_Destination_address
Description     : Get DTC Destination address
Arguments       : USB_UTR_t *ptr        : USB internal structure. Selects USB channel.
Return value    : DTC Destination address
******************************************************************************/
uint32_t usb_cpu_get_dtc_Destination_address(USB_UTR_t *ptr)
{
    while( DTC.DTCSTS.BIT.ACT );            /* Wait DTC Transfer */
    return usb_dtcreg[ptr->ip].DAR;
}   /* eof usb_cpu_get_dtc_Destination_address() */

/******************************************************************************
End  Of File
******************************************************************************/
