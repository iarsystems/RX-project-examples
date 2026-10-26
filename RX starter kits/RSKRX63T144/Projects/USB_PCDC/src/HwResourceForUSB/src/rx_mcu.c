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
* File Name     : rx_rsk.c
* Version       : 2.00
* Device(s)     : RX-Series
* Tool-Chain    : Renesas RX Standard Toolchain
* OS            : Common to None and uITRON 4.0 Spec
* H/W Platform  : Independent
* Description   : RX63T RSK processing
******************************************************************************
* History : DD.MM.YYYY Version Description
*         : 29.06.2012 0.50    First Release
******************************************************************************/



/******************************************************************************
Includes   <System Includes> , "Project Includes"
******************************************************************************/
#include <machine.h>

/* System definition */
#include "r_usb_usrconfig.h"        /* USB-H/W register set (user define) */
#include "r_usb_ctypedef.h"         /* Type define */
#include "r_usb_cdefusbip.h"        /* USB-FW Library Header */
#include "r_usb_cmacsystemcall.h"   /* System call macro */
#include "r_usb_cextern.h"          /* USB-FW global define */
#include "rx_mcu.h"                 /* RX63xMCU definition */
#include <iorx63t.h>
#include "hw_resource.h"
#include "r_usb_reg_access.h"

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
#endif /* USB_LCD_ENABLE */

#ifdef USB_KEY_ENABLE
extern void usb_cpu_KeyInit(void);
#endif /* USB_KEY_ENABLE */

#ifdef USB_LED_ENABLE
extern void usb_cpu_LedInit(void);
#endif /* USB_LED_ENABLE */

extern void usb_cpu_DmaintInit(void);
extern USB_ER_t R_usb_pstd_PcdChangeDeviceState(USB_UTR_t *ptr, uint16_t state, uint16_t port_no,
    USB_CB_INFO_t complete);
extern void     R_usb_pstd_DeviceInformation(USB_UTR_t *ptr, uint16_t *tbl);
extern void usb_creg_set_bempenb( USB_UTR_t *ptr, uint16_t pipeno );

/******************************************************************************
Section    <Section Definition> , "Project Sections"
******************************************************************************/
#if !defined(__IAR_SYSTEMS_ICC__)
    #pragma address MDEreg=0xffffff80 /* MDE register (Single Chip Mode) */
    #ifdef __BIG
        const unsigned long MDEreg = 0xfffffff8; /* big */
    #else   /* __BIG_ENDIAN__ */
        const unsigned long MDEreg = 0xffffffff; /* little */
    #endif  /* __BIG_ENDIAN__ */
#endif

/******************************************************************************
Typedef definitions
******************************************************************************/

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
Section    <Section Definition> , "Project Sections"
******************************************************************************/
#pragma section="_DTCtable"

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
    (uint32_t)&usb_dtcreg[0],   /* DTC VECTOR 33 (USB0-D0FIFO) */
    (uint32_t)0x00000000,       /* DTC VECTOR 34 (USB0-D1FIFO) */
    (uint32_t)0xFFFFFFFF,       /* DTC VECTOR 35 */
    (uint32_t)0xFFFFFFFF,       /* DTC VECTOR 36 */
    (uint32_t)0xFFFFFFFF,       /* DTC VECTOR 37 */
    (uint32_t)0xFFFFFFFF,       /* DTC VECTOR 38 */
    (uint32_t)0xFFFFFFFF,       /* DTC VECTOR 39 */
    (uint32_t)0xFFFFFFFF,       /* DTC VECTOR 40 */
    (uint32_t)0xFFFFFFFF,       /* DTC VECTOR 41 */
    (uint32_t)0xFFFFFFFF,       /* DTC VECTOR 42 */
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
uint16_t g_usb_D0fifoIER    = 0;        /* D0fifo0 Interrupt Request enable */
uint16_t g_usb_D1fifoIER    = 0;        /* D1fifo0 Interrupt Request enable */

/*=== SYSTEM ================================================================*/
void        usb_cpu_McuInitialize(void);
void        usb_cpu_target_init(void);
void        usb_cpu_FunctionUSB0IP(void);

/*=== USB ===================================================================*/
void        usb_cpu_usbint_init(void);
__interrupt void        usb_cpu_usb_int_hand(void);         /* INT VECTOR 35 */
void        usb_cpu_int_enable(USB_UTR_t *ptr);
void        usb_cpu_int_disable(USB_UTR_t *ptr);
/*=== TIMER =================================================================*/
void        usb_cpu_Delay1us(uint16_t time);
void        usb_cpu_DelayXms(uint16_t time);
/*=== Standby control =======================================================*/
void        usb_cpu_GoWait(void);
void        usb_cpu_RegRecovEnable(USB_UTR_t *ptr);
void        usb_cpu_RegRecovDisable(USB_UTR_t *ptr);
/*=== IRQ ===================================================================*/
void        usb_cpu_IRQ0_Enable(void);
void        usb_cpu_IRQ0_Disable(void);
void        usb_cpu_IRQ1_Enable(USB_UTR_t *ptr);
void        usb_cpu_IRQ1_Disable(void);
void        usb_cpu_IRQ2_Enable(void);
void        usb_cpu_IRQ2_Disable(void);
__interrupt void        usb_cpu_IRQ0Int(void);
__interrupt void        usb_cpu_IRQ1Int(void);
__interrupt void        usb_cpu_IRQ2Int(void);
/*=== DTC ===================================================================*/
void        usb_cpu_DmaintInit(void);
__interrupt void        usb_cpu_d0fifo_int_hand(void);
void        usb_cpu_d0fifo2buf_start_dma(USB_UTR_t *ptr, uint32_t SourceAddr);
void        usb_cpu_buf2d0fifo_start_dma(USB_UTR_t *ptr, uint32_t DistAdr);
void        usb_cpu_d0fifo_restart_dma(USB_UTR_t *ptr);
void        usb_cpu_d0fifo_stop_dma(USB_UTR_t *ptr);
uint16_t    usb_cpu_get_dtc_block_count(USB_UTR_t *ptr);
void        usb_cpu_d0fifo_enable_dma(USB_UTR_t *ptr );
void        usb_cpu_d0fifo_disable_dma(USB_UTR_t *ptr );


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
    uint32_t    i;

    /* protect off */
    SYSTEM.PRCR.WORD        = 0xA503;

    /* Main clock Oscillator register (MOSTP=Not STOP, Default:262144Cycle) */
    /* Sub clock Oscillator control register (SOSTP=STOP) */
    SYSTEM.MOSCCR.BYTE      = 0x00;
    SYSTEM.MOSCWTCR.BYTE    = 0x0E;

    /* PLL control register ( PLIDIV = 12MHz(/1), STC = 192MHz(*16), PLL enable) */
    /* PLL wait control register(Default:4194304cycle) */
    SYSTEM.PLLCR.WORD       = 0x0F00;
    SYSTEM.PLLCR2.BYTE      = 0x00;
    SYSTEM.PLLWTCR.BYTE     = 0x0F;

    /* wait over 11ms */
    for(i = 0;i< 660;i++)
    {
    }

    /* System clock control register(ICK=PLL/2,BCK,FCK,PCK=PLL/4, CKSEL(10-8)=PLL, UCK(7-4)=48MHz(/4)) */
    SYSTEM.SCKCR.LONG       = 0x21862222;
    SYSTEM.SCKCR3.WORD      = 0x0400;
    SYSTEM.SCKCR2.BIT.UCK   = 3;

    /* Module stop control register (Disable ACSE) */
    SYSTEM.MSTPCRA.LONG     = 0x7FFFFFFF;

    /* protect on */
    SYSTEM.PRCR.WORD        = 0xA500;

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
    /* initialized USB interrupt */
    usb_cpu_usbint_init();
    usb_cpu_DmaintInit();

#ifdef  USB_LED_ENABLE
    usb_cpu_LedInit();              /* Initialized RSK LED */
#endif /* USB_LED_ENABLE */

#ifdef USB_KEY_ENABLE
    usb_cpu_KeyInit();              /* Initialized RSK KEY */
#endif /* USB_KEY_ENABLE */

#ifdef USB_LCD_ENABLE
    usb_cpu_LcdInit();
#endif /* USB_LCD_ENABLE */
}
/******************************************************************************
End of function usb_cpu_target_init
******************************************************************************/

/******************************************************************************
Function Name   : usb_cpu_FunctionUSB0IP
Description     : USB0 port mode and Switch mode Initialize
Arguments       : none
Return value    : none
******************************************************************************/
void usb_cpu_FunctionUSB0IP(void)
{
/* Condition compilation by the difference of USB function */
#if USB_FUNCSEL_USBIP0_PP != USB_NOUSE_PP
/* Condition compilation by the difference of USB function */
  #if USB_FUNCSEL_USBIP0_PP == USB_PERI_PP
    /* Set port direction as input(USB0 DPRPU/VBUS) */
    USB0_VBUS_PDR           = 0;
    USB0_VBUS_PMR           = 0;                    /* Port mode register (Disable USB0 VBUS) */

    /* Port mode register write enable */
    MPC.PWPR.BYTE           = 0x00;
    MPC.PWPR.BYTE           = 0x40;

    /* Initial setting of USB0 */
    USB0_DPRPU_PFS          = USB0_DPRPU_ENB;       /* DPUPE output enable */
    USB0_VBUS_PFS           = USB0_VBUS_ENB;        /* VBUS  input  enable */
    /* Enable USB0 DPRPU pullup register */
    MPC.PFUSB0.BIT.PUPHZS   = 1;                    /* DPUPE eneble */

    /* Port mode register write disable */
    MPC.PWPR.BYTE           = 0x80;                 /* PFS register write protect on */
    USB0_VBUS_PMR           = 1;                    /* Port mode register (Enable USB0 VBUS) */
  #endif  /* USB_FUNCSEL_USBIP0_PP == USB_PERI_PP */
/* Condition compilation by the difference of USB function */
  #if USB_FUNCSEL_USBIP0_PP == USB_HOST_PP
    /* Set port direction as input(USB0 OVRCURA) */
    USB0_OVRCURA_PDR        = 0;
    USB0_OVRCURA_PMR        = 0;                    /* Port mode register (Disable USB0 OVRCURA) */
    USB0_VBUSEN_PDR         = 0;
    USB0_VBUSEN_PMR         = 0;                    /* Port mode register (Disable USB0 VBUSEN) */
    USB0_DRPD_PDR           = 0;
    USB0_DRPD_PMR           = 0;                    /* Port mode register (Disable USB0 DRPD) */
    USB0_DPRPD_PDR          = 0;
    USB0_DPRPD_PMR          = 0;                    /* Port mode register (Disable USB0 DPRPD) */

    /* Port mode register write enable */
    MPC.PWPR.BYTE           = 0x00;
    MPC.PWPR.BYTE           = 0x40;

    /* Initial setting of USB0 */
    USB0_OVRCURA_PFS        = USB0_OVRCURA_ENB;     /* OVRCURA input  enable */
    USB0_VBUSEN_PFS         = USB0_VBUSEN_ENB;      /* VBUSEN  output enable */
    USB0_DRPD_PFS           = USB0_DRPD_ENB;        /* DRPD    output enable */
    USB0_DPRPD_PFS          = USB0_DPRPD_ENB;       /* DPRPD   output enable */

    /* Port mode register write disable */
    MPC.PWPR.BYTE           = 0x80;                 /* PFS register write protect on */
    USB0_OVRCURA_PMR        = 1;                    /* Port mode register (Enable USB0 OVRCURA) */
    USB0_VBUSEN_PMR         = 1;                    /* Port mode register (Enable USB0 VBUSEN) */
    USB0_DRPD_PMR           = 1;                    /* Port mode register (Enable USB0 DRPD) */
    USB0_DPRPD_PMR          = 1;                    /* Port mode register (Enable USB0 DPRPD) */
  #endif  /* USB_FUNCSEL_USBIP0_PP == USB_HOST_PP */
#endif  /* USB_FUNCSEL_USBIP0_PP != USB_NOUSE_PP */
}
/******************************************************************************
End of function usb_cpu_FunctionUSB0IP
******************************************************************************/


/******************************************************************************
Function Name   : usb_cpu_usbint_init
Description     : USB interrupt Initialize
Arguments       : void
Return value    : void
******************************************************************************/
void usb_cpu_usbint_init(void)
{
    /* Protect register (protect off) */
    SYSTEM.PRCR.WORD            = 0xA503;   /* protect off */

/* Condition compilation by the difference of USB function */
#if USB_FUNCSEL_USBIP0_PP != USB_NOUSE_PP
    /* Module stop control register (Enable USB0 module(MSTPB19)) */
    /* Deep standby USB monitor register (USB0 Transceiver Output fixed) */
    SYSTEM.MSTPCRB.BIT.MSTPB19  = 0;

    /* USB0 interrupt */
    USB0_D0FIFO_IER             = 0;        /* Disable D0FIFO interrupt(IEN4) */
    USB0_D1FIFO_IER             = 0;        /* Disable D1FIFO interrupt(IEN5) */
    USB0_USBIO_IER              = 1;        /* Enable  USBIO  interrupt(IEN6) */
    USB0_USBR_IER               = 1;        /* Enable Resume interrupt(IEN2) */

    /* USB0 Priority */
    USB0_D0FIFO_IPR             = 0x00;     /* Priority D0FIFO0=0(Disable) */
    USB0_D1FIFO_IPR             = 0x00;     /* Priority D1FIFO0=0(Disable) */
    USB0_USBIO_IPR              = 0x03;     /* Priority USBINT0=3 */
    USB0_USBR_IPR               = 0x00;     /* Priority Resume0=0 */

#endif  /* USB_FUNCSEL_USBIP0_PP != USB_NOUSE_PP */
    usb_cpu_FunctionUSB0IP();
}
/******************************************************************************
End of function usb_cpu_usbint_init
******************************************************************************/

/******************************************************************************
Function Name   : usb_cpu_usb_int_hand
Description     : USB interrupt Handler
Arguments       : void
Return value    : void
******************************************************************************/
#pragma vector=VECT(USB0, USBI0)
__interrupt void usb_cpu_usb_int_hand(void)
{
/* Condition compilation by the difference of USB function */
#if USB_FUNCSEL_USBIP0_PP != USB_NOUSE_PP
    /* Call USB interrupt routine */
    usb_cstd_UsbHandler();      /* Call interrupt routine */
#endif /* USB_FUNCSEL_USBIP0_PP */
}
/******************************************************************************
End of function usb_cpu_usb_int_hand
******************************************************************************/

/******************************************************************************
Function Name   : usb_cpu_d0fifo_int_hand
Description     : D0FIFO interrupt Handler
Arguments       : none
Return value    : none
******************************************************************************/
#pragma vector=VECT(USB0, D0FIFO0)
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
Function Name   : usb_cpu_usb_int_hand_r
Description     : RESUME interrupt Handler
                : IRQ #90 (VECT_USB_USBR0)
Arguments       : none
Return value    : none
******************************************************************************/
#pragma vector=VECT(USB, USBR0)
__interrupt void usb_cpu_usb_int_hand_r(void)
{
    USB_UTR_t       utr;

    utr.ip  = USB_USBIP_0;
    utr.ipp = usb_cstd_GetUsbIpAdr( utr.ip );

/* Condition compilation by the difference of the operating system */
#if USB_FUNCSEL_USBIP0_PP == USB_PERI_PP || USB_FUNCSEL_USBIP1_PP == USB_PERI_PP
    /* Please add the processing for the system. */
    usb_preg_clr_sts_resm( &utr );       /* RESM status clear */
#endif  /* USB_FUNCSEL_USBIP0_PP == USB_PERI_PP || USB_FUNCSEL_USBIP1_PP == USB_PERI_PP */

#if USB_CPU_LPW_PP == USB_LPWR_USE_PP
    USB0_USBR_IPR               = 0x00;     /* Priority Resume0=0 */
#endif /* USB_CPU_LPW_PP == USB_LPWR_USE_PP */

    ICU.IR[IR_USB_USBR0].BIT.IR = 0;    /* Interrupt Request USB_resume USBR0 Clear */
}
/******************************************************************************
End of function usb_cpu_usb_int_hand_r
******************************************************************************/

/******************************************************************************
Renesas Abstracted RSK functions
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
#if USB_FUNCSEL_USBIP0_PP != USB_NOUSE_PP
    /* Interrupt enable register (USB0 USBIO enable(IEN3)) */
    /* Interrupt enable register (USB0 D0/D1FIFO restore (IEN1/IEN2)) */
    USB0_USBIO_IER      = 1;
    USB0_D0FIFO_IER     = g_usb_D0fifoIER;
    USB0_D1FIFO_IER     = g_usb_D1fifoIER;
#endif  /* USB_FUNCSEL_USBIP0_PP != USB_NOUSE_PP */
}   /* eof usb_cpu_int_enable() */

/******************************************************************************
Function Name   : usb_cpu_int_disable
Description     : USB Interrupt disable
Arguments       : USB_UTR_t *ptr  : USB internal structure. Selects USB channel.
Return value    : void
******************************************************************************/
void usb_cpu_int_disable(USB_UTR_t *ptr)
{
/* Condition compilation by the difference of USB function */
#if USB_FUNCSEL_USBIP0_PP != USB_NOUSE_PP
    /* Disable interrupt enable register (USB0 USBIO(IEN3)) */
    /* Backup & disable interrupt enable register (USB0 D0/D1FIFO (IEN1/IEN2)) */
    USB0_USBIO_IER      = 0;
    g_usb_D0fifoIER     = USB0_D0FIFO_IER;
    g_usb_D1fifoIER     = USB0_D1FIFO_IER;
    USB0_D0FIFO_IER     = 0;
#endif  /* USB_FUNCSEL_USBIP0_PP != USB_NOUSE_PP */
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
    volatile register uint32_t  i;

    /* Wait 1ms */
    for( i = 0; i < (6500 * time); ++i )
    {
    };
        /* When "ICLK=48MHz" is set, this code is waiting for 1ms.
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
//                usb_cpu_IRQ2_Enable(ptr);   /* IRQ2(RX63N-RSK_SW1) for Remote wakeup */
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
#endif  /* USB_FUNCSEL_USBIP0_PP == USB_PERI_PP || USB_FUNCSEL_USBIP1_PP == USB_PERI_PP */
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
    USB0_USBR_IPR               = 0x07;     /* Priority Resume0=2 */

    /* Protect register (protect off)
    b0    PRC0     Protect bit0
    b1    PRC1     Protect bit1
    b2    Reserved 0
    b3    PRC3     Protect bit3
    b7-b4 Reserved 0
    b15-b8 PRKEY   PRC Key code bit
    */
    SYSTEM.PRCR.WORD        = 0xA503;   /* protect off */

    /* DTC Transfer disable
    b0    DTCST    DTC module start bit
    b7-b1 Reserved 0
    */
    DTC.DTCST.BIT.DTCST     = 0;

    /* System clock control register3(CKSEL(10-8)=Sub clock)
    b7-b0   Reserved 0
    b10-b8  CKSEL    Clock source select bit
    b15-b11 Reserved 0
    */
    SYSTEM.SCKCR3.BIT.CKSEL         = 3;

    /* Standby control register (Software Standby Shift)
    b7-b0  Reserved 0
    b12-b8 STS      Standby timer select bit
    b13    Reserved 0
    b14    OPE      Output port enable bit
    b15    SSBY     Software standby bit
    */
    SYSTEM.SBYCR.BIT.SSBY       = 1;

    /* Goto WAIT */
    usb_cpu_GoWait();
}
/******************************************************************************
End of function usb_cpu_GoLpwrDeepStby
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
#pragma vector=11
__interrupt void usb_cpu_GoWait_Int(void)
{
    wait();                         /* Wait */
}
/******************************************************************************
End of function usb_cpu_GoWait_Int
******************************************************************************/


/******************************************************************************
IRQ function
******************************************************************************/

/******************************************************************************
Function Name   : usb_cpu_IRQ0_Enable
Description     : IRQ0 Interrupt Enable
Arguments       : none
Return value    : none
******************************************************************************/
void usb_cpu_IRQ0_Enable(void)
{
    /* Interrupt enable register (IEN0 disable)
    b0 IEN0 Interrupt enable bit
    b1 IEN1 Interrupt enable bit
    b2 IEN2 Interrupt enable bit
    b3 IEN3 Interrupt enable bit
    b4 IEN4 Interrupt enable bit
    b5 IEN5 Interrupt enable bit
    b6 IEN6 Interrupt enable bit
    b7 IEN7 Interrupt enable bit
    */
    ICU.IER[8].BIT.IEN0     = 0;        /* Interrupt disable */

    /* IRQ control register
    b1-b0 Reserved 0
    b3-b2 IRQMD    IRQ detect bit
    b7-b4 Reserved 0
    */
    ICU.IRQCR[0].BIT.IRQMD  = 1;        /* Down Edge Set */

    /* Priority Resume0=B
    b3-b0 IPR      Interrupt priority
    b7-b4 Reserved 0
    */
    ICU.IPR[64].BYTE        = 0x0B;     /* Priority Resume0=B */

    /* Interrupt request register
    b0    IR       Interrupt status flag
    b7-b1 Reserved 0
    */
    ICU.IR[64].BIT.IR       = 0;

    /* Interrupt enable register (IEN0 enable)
    b0 IEN0 Interrupt enable bit
    b1 IEN1 Interrupt enable bit
    b2 IEN2 Interrupt enable bit
    b3 IEN3 Interrupt enable bit
    b4 IEN4 Interrupt enable bit
    b5 IEN5 Interrupt enable bit
    b6 IEN6 Interrupt enable bit
    b7 IEN7 Interrupt enable bit
    */
    ICU.IER[8].BIT.IEN0     = 1;
}
/******************************************************************************
End of function usb_cpu_IRQ0_Enable
******************************************************************************/

/******************************************************************************
Function Name   : usb_cpu_IRQ0_Disable
Description     : IRQ0 Interrupt Disable
Arguments       : none
Return value    : none
******************************************************************************/
void usb_cpu_IRQ0_Disable(void)
{
    /* Interrupt request register
    b0    IR       Interrupt status flag
    b7-b1 Reserved 0
    */
    ICU.IR[64].BIT.IR       = 0;

    /* Interrupt enable register
    b0 IEN0 Interrupt enable bit
    b1 IEN1 Interrupt enable bit
    b2 IEN2 Interrupt enable bit
    b3 IEN3 Interrupt enable bit
    b4 IEN4 Interrupt enable bit
    b5 IEN5 Interrupt enable bit
    b6 IEN6 Interrupt enable bit
    b7 IEN7 Interrupt enable bit
    */
    ICU.IER[8].BIT.IEN0     = 0;

    /* Priority Resume0=0
    b3-b0 IPR      Interrupt priority
    b7-b4 Reserved 0
    */
    ICU.IPR[64].BYTE        = 0x00;
}
/******************************************************************************
End of function usb_cpu_IRQ0_Disable
******************************************************************************/

/******************************************************************************
Function Name   : usb_cpu_IRQ1_Enable
Description     : IRQ8 Interrupt Enable
Arguments       : USB_UTR_t *ptr        : USB internal structure. Selects USB channel.
Return value    : none
******************************************************************************/
void usb_cpu_IRQ1_Enable(USB_UTR_t *ptr)
{
/* Condition compilation by the difference of USB function */
#if USB_FUNCSEL_USBIP0_PP == USB_PERI_PP || USB_FUNCSEL_USBIP1_PP == USB_PERI_PP
    uint16_t    dev_info[8];

    if( usb_cstd_is_host_mode(ptr) == USB_NO )
    {

        /* Get USB Device information */
        R_usb_pstd_DeviceInformation(ptr, (uint16_t *)&dev_info );

        /* Remote Wakeup Flag */
        if( dev_info[4] == USB_YES )
        {
            /* Interrupt enable register (IEN1 disable))
            b0 IEN0 Interrupt enable bit
            b1 IEN1 Interrupt enable bit
            b2 IEN2 Interrupt enable bit
            b3 IEN3 Interrupt enable bit
            b4 IEN4 Interrupt enable bit
            b5 IEN5 Interrupt enable bit
            b6 IEN6 Interrupt enable bit
            b7 IEN7 Interrupt enable bit
            */
            ICU.IER[8].BIT.IEN1     = 0u;

            /* IRQ control register
            b1-b0 Reserved 0
            b3-b2 IRQMD    IRQ detect bit
            b7-b4 Reserved 0
            */
            ICU.IRQCR[1].BIT.IRQMD  = 1u;   /* Down Edge Set */

            /* Priority Resume0=B
            b3-b0 IPR      Interrupt priority
            b7-b4 Reserved 0
            */
            ICU.IPR[65].BYTE        = 0x0B;

            /* Interrupt request register
            b0    IR       Interrupt status flag
            b7-b1 Reserved 0
            */
            ICU.IR[65].BIT.IR       = 0u;

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
            ICU.IER[8].BIT.IEN1     = 1u;
        }
    }
#endif  /* USB_FUNCSEL_USBIP0_PP == USB_PERI_PP || USB_FUNCSEL_USBIP1_PP == USB_PERI_PP */

    if( usb_cstd_is_host_mode(ptr) == USB_YES )
    {
        /* Interrupt enable register (IEN1 disable))
        b0 IEN0 Interrupt enable bit
        b1 IEN1 Interrupt enable bit
        b2 IEN2 Interrupt enable bit
        b3 IEN3 Interrupt enable bit
        b4 IEN4 Interrupt enable bit
        b5 IEN5 Interrupt enable bit
        b6 IEN6 Interrupt enable bit
        b7 IEN7 Interrupt enable bit
        */
        ICU.IER[8].BIT.IEN1     = 0u;

        /* IRQ control register
        b1-b0 Reserved 0
        b3-b2 IRQMD    IRQ detect bit
        b7-b4 Reserved 0
        */
        ICU.IRQCR[1].BIT.IRQMD  = 1u;   /* Down Edge Set */

        /* Priority Resume0=B
        b3-b0 IPR      Interrupt priority
        b7-b4 Reserved 0
        */
        ICU.IPR[65].BYTE        = 0x0B;

        /* Interrupt request register
        b0    IR       Interrupt status flag
        b7-b1 Reserved 0
        */
        ICU.IR[65].BIT.IR       = 0u;

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
        ICU.IER[8].BIT.IEN1     = 1u;
    }
}
/******************************************************************************
End of function usb_cpu_IRQ1_Enable
******************************************************************************/

/******************************************************************************
Function Name   : usb_cpu_IRQ1_Disable
Description     : IRQ1 Interrupt Disable
Arguments       : none
Return value    : none
******************************************************************************/
void usb_cpu_IRQ1_Disable(void)
{
    /* Interrupt request register
    b0    IR       Interrupt status flag
    b7-b1 Reserved 0
    */
    ICU.IR[65].BIT.IR           = 0u;

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
    ICU.IER[8].BIT.IEN1         = 0u;

    /* Priority Resume0=0(Disable)
    b3-b0 IPR      Interrupt priority
    b7-b4 Reserved 0
    */
    ICU.IPR[65].BYTE            = 0x00;
}
/******************************************************************************
End of function usb_cpu_IRQ1_Disable
******************************************************************************/

/******************************************************************************
Function Name   : usb_cpu_IRQ2_Enable
Description     : IRQ2 Interrupt Enable
Arguments       : none
Return value    : none
******************************************************************************/
void usb_cpu_IRQ2_Enable(void)
{
    /* Interrupt enable register (IEN2 disable)
    b0 IEN0 Interrupt enable bit
    b1 IEN1 Interrupt enable bit
    b2 IEN2 Interrupt enable bit
    b3 IEN3 Interrupt enable bit
    b4 IEN4 Interrupt enable bit
    b5 IEN5 Interrupt enable bit
    b6 IEN6 Interrupt enable bit
    b7 IEN7 Interrupt enable bit
    */
    ICU.IER[8].BIT.IEN2     = 0;        /* Interrupt disable */

    /* IRQ control register
    b1-b0 Reserved 0
    b3-b2 IRQMD    IRQ detect bit
    b7-b4 Reserved 0
    */
    ICU.IRQCR[2].BIT.IRQMD  = 1;        /* Down Edge Set */

    /* Priority Resume0=B
    b3-b0 IPR      Interrupt priority
    b7-b4 Reserved 0
    */
    ICU.IPR[66].BYTE        = 0x0B;     /* Priority Resume0=B */

    /* Interrupt request register
    b0    IR       Interrupt status flag
    b7-b1 Reserved 0
    */
    ICU.IR[66].BIT.IR       = 0;

    /* Interrupt enable register (IEN2 enable)
    b0 IEN0 Interrupt enable bit
    b1 IEN1 Interrupt enable bit
    b2 IEN2 Interrupt enable bit
    b3 IEN3 Interrupt enable bit
    b4 IEN4 Interrupt enable bit
    b5 IEN5 Interrupt enable bit
    b6 IEN6 Interrupt enable bit
    b7 IEN7 Interrupt enable bit
    */
    ICU.IER[8].BIT.IEN2     = 1;
}
/******************************************************************************
End of function usb_cpu_IRQ2_Enable
******************************************************************************/

/******************************************************************************
Function Name   : usb_cpu_IRQ2_Disable
Description     : IRQ2 Interrupt Disable
Arguments       : none
Return value    : none
******************************************************************************/
void usb_cpu_IRQ2_Disable(void)
{
    /* Interrupt request register
    b0    IR       Interrupt status flag
    b7-b1 Reserved 0
    */
    ICU.IR[66].BIT.IR       = 0;

    /* Interrupt enable register
    b0 IEN0 Interrupt enable bit
    b1 IEN1 Interrupt enable bit
    b2 IEN2 Interrupt enable bit
    b3 IEN3 Interrupt enable bit
    b4 IEN4 Interrupt enable bit
    b5 IEN5 Interrupt enable bit
    b6 IEN6 Interrupt enable bit
    b7 IEN7 Interrupt enable bit
    */
    ICU.IER[8].BIT.IEN2     = 0;

    /* Priority Resume0=0
    b3-b0 IPR      Interrupt priority
    b7-b4 Reserved 0
    */
    ICU.IPR[66].BYTE        = 0x00;
}
/******************************************************************************
End of function usb_cpu_IRQ2_Disable
******************************************************************************/

/******************************************************************************
Function Name   : usb_cpu_IRQ0Int
Description     : IRQ0 Interrupt process
Arguments       : none
Return value    : none
******************************************************************************/
#pragma vector=VECT(ICU, IRQ0)
__interrupt void usb_cpu_IRQ0Int(void)
{
}
/******************************************************************************
End of function usb_cpu_IRQ0Int
******************************************************************************/

/******************************************************************************
Function Name   : usb_cpu_IRQ1Int
Description     : IRQ1 Interrupt process
Arguments       : none
Return value    : none
******************************************************************************/
#pragma vector=VECT(ICU, IRQ1)
__interrupt void usb_cpu_IRQ1Int(void)
{
/* Condition compilation by the difference of USB function */
#if USB_FUNCSEL_USBIP0_PP == USB_PERI_PP || USB_FUNCSEL_USBIP1_PP == USB_PERI_PP

/* Condition compilation by the difference of the devices */
#if (USB_CPU_LPW_PP == USB_LPWR_USE_PP)
    USB_UTR_t utr;
    USB_UTR_t *ptr;

    ptr = (USB_UTR_t *)&utr;
    ptr->ip = USB_PERI_USBIP_NUM;
    ptr->ipp = usb_cstd_GetUsbIpAdr( ptr->ip );

    R_usb_pstd_PcdChangeDeviceState(ptr, USB_DO_REMOTEWAKEUP, (uint16_t)0, (USB_CB_INFO_t)usb_cstd_DummyFunction);
    usb_cpu_IRQ2_Disable();
#endif /* (USB_CPU_LPW_PP == USB_LPWR_USE_PP) */

#endif  /* USB_FUNCSEL_USBIP0_PP == USB_PERI_PP || USB_FUNCSEL_USBIP1_PP == USB_PERI_PP */
}
/******************************************************************************
End of function usb_cpu_IRQ1Int
******************************************************************************/

/******************************************************************************
Function Name   : usb_cpu_IRQ2Int
Description     : IRQ2 Interrupt process
Arguments       : none
Return value    : none
******************************************************************************/
#pragma vector=VECT(ICU, IRQ2)
__interrupt void usb_cpu_IRQ2Int(void)
{
}
/******************************************************************************
End of function usb_cpu_IRQ2Int
******************************************************************************/

/******************************************************************************
Function Name   : usb_cpu_GetDPSRSTF
Description     : Get Deep Software Standby Reset Flag
Arguments       : none
Return value    : Deep Software Standby Reset Flag
******************************************************************************/
uint8_t usb_cpu_GetDPSRSTF(void)
{
    /* Reset status register (Deep software standby reset check)
    b0    POFR     Power on reset Detection flag
    b1    LVD0F    Voltage Watch 0 reset detection flag
    b2    LVD1F    Voltage Watch 1 reset detection flag
    b3    LVD2F    Voltage Watch 2 reset detection flag
    b6-b4 Reserved 0
    b7    DPSRSTF  Deep software standby reset flag
    */
    return (uint8_t)SYSTEM.RSTSR0.BIT.DPSRSTF;
}
/******************************************************************************
End of function usb_cpu_GetDPSRSTF
******************************************************************************/

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    DMA function
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/******************************************************************************
Function Name   : usb_cpu_DmaintInit
Description     : DMA interrupt Initialize
Arguments       : void
Return value    : void
******************************************************************************/
void usb_cpu_DmaintInit(void)
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

    /* Module stop control register (Enable DTC module)
    b3-b0   Reserved 0
    b4      MSTPA4   8bit timer3,2 stop bit
    b5      MSTPA5   8bit timer1,0 stop bit
    b8-b6   Reserved 0
    b9      MSTPA9   Multifunction timer unit0 stop bit
    b10     MSTPA10  Programmable pulse unit1 stop bit
    b11     MSTPA11  Programmable pulse unit0 stop bit
    b12     MSTPA12  16Bit timer pulse unit1 stop bit
    b13     MSTPA13  16Bit timer pulse unit0 stop bit
    b14     MSTPA14  Compare timer unit1 stop bit
    b15     MSTPA15  Compare timer unit0 stop bit
    b16     Reserved 0
    b17     MSTPA17  12bit AD stop bit
    b18     Reserved 0
    b19     MSTPA19  DA stop bit
    b22-b20 Reserved 0
    b23     MSTPA23  10bit AD unit0 stop bit
    b24     MSTPA24  Module stop A24 set bit
    b26-b25 Reserved 0
    b27     MSTPA27  Module stop A27 set bit
    b28     MSTPA28  DMA/DTC stop bit
    b29     MSTPA29  Module stop A29 set bit
    b30     Reserved 0
    b31     ACSE     All clock stop bit
    */
    SYSTEM.MSTPCRA.BIT.MSTPA28  = 0;        /* Enable DMA module(MSTPA28) */

    /* DTC vector register
    b21-b0 DTCVBR Vector table address
    */
    DTC.DTCVBR                  = (void*)&usb_dtcctable;

    /* Protect register
    b0    PRC0     Protect bit0
    b1    PRC1     Protect bit1
    b2    Reserved 0
    b3    PRC3     Protect bit3
    b7-b4 Reserved 0
    b15-b8 PRKEY   PRC Key code bit
    */
    SYSTEM.PRCR.WORD    = 0xA500;   /* Protect on */
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

    tmp                     = ((usb_gcstd_Dma0Fifo[ptr->ip] -1)/ 2) +1;

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

    size    = (uint8_t )(tmp);
    size    += (uint16_t)(tmp << 8);

    /* DTC transfer count registerA
    b15-b0 CRA Transfer count
    */
    usb_dtcreg[ptr->ip].CRA             = (uint16_t)(size);

    /* DTC transfer count registerB (Block count)
    b15-b0 CRB Transfer count
    */
    usb_dtcreg[ptr->ip].CRB = (uint16_t)((usb_gcstd_DataCnt[ptr->ip][usb_gcstd_Dma0Pipe[ptr->ip]] -1)
        / usb_gcstd_Dma0Fifo[ptr->ip]) +1;

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
        /* Priority D0FIFO0=0
        b3-b0 IPR      Interrupt priority
        b7-b4 Reserved 0
        */
        ICU.IPR[33].BYTE            = 0x00;

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
        ICU.IER[0x04].BIT.IEN1      = 1;

        /* DTC start enable register (USB0 D0FIFO transfer)
        b0    DTCE     DTC start enable bit
        b7-b1 Reserved 0
        */
        ICU.DTCER[33].BIT.DTCE      = 1;
    }
    else
    {
        /* Priority D1FIFO0=0
        b3-b0 IPR      Interrupt priority
        b7-b4 Reserved 0
        */
        ICU.IPR[36].BYTE            = 0x00;

        /* Interrupt enable register (USB1 D0FIFO enable(IEN4))
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

        /* DTC start enable register (USB1 D0FIFO transfer)
        b0    DTCE     DTC start enable bit
        b7-b1 Reserved 0
        */
        ICU.DTCER[36].BIT.DTCE      = 1;
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
void usb_cpu_buf2d0fifo_start_dma(USB_UTR_t *ptr, uint32_t DistAddr)
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

    /* DTC source address register (FIFO port address)
    b31-b0 SAR Source address
    */
    usb_dtcreg[ptr->ip].DAR             = (uint32_t)(DistAddr);
    size    = (uint8_t )(tmp);
    size    += (uint16_t)(tmp << 8);

    /* DTC transfer count registerA
    b15-b0 CRA Transfer count
    */
    usb_dtcreg[ptr->ip].CRA             = (uint16_t)(size);

    /* DTC transfer count registerB (Block count)
    b15-b0 CRB Transfer count
    */
    usb_dtcreg[ptr->ip].CRB = (uint16_t)(usb_gcstd_DataCnt[ptr->ip][usb_gcstd_Dma0Pipe[ptr->ip]]
        / usb_gcstd_Dma0Size[ptr->ip]);

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
        /* Priority D0FIFO0=0
        b3-b0 IPR      Interrupt priority
        b7-b4 Reserved 0
        */
        ICU.IPR[33].BYTE            = 0x00;

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
        ICU.IER[0x04].BIT.IEN1      = 1;

        /* DTC start enable register (USB0 D0FIFO transfer)
        b0    DTCE     DTC start enable bit
        b7-b1 Reserved 0
        */
        ICU.DTCER[33].BIT.DTCE      = 1;
    }
    else
    {
        /* Priority D1FIFO0=0
        b3-b0 IPR      Interrupt priority
        b7-b4 Reserved 0
        */
        ICU.IPR[36].BYTE            = 0x00;

        /* Interrupt enable register (USB1 D0FIFO enable(IEN4))
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

        /* DTC start enable register (USB1 D0FIFO transfer)
        b0    DTCE     DTC start enable bit
        b7-b1 Reserved 0
        */
        ICU.DTCER[36].BIT.DTCE      = 1;
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
        ICU.IR[33].BIT.IR           = 0;

        /* Priority D0FIFO0=0
        b3-b0 IPR      Interrupt priority
        b7-b4 Reserved 0
        */
        ICU.IPR[33].BYTE            = 0x00;

        /* Interrupt enable register (USB0 D0FIFO disable(IEN4))
        b0 IEN0 Interrupt enable bit
        b1 IEN1 Interrupt enable bit
        b2 IEN2 Interrupt enable bit
        b3 IEN3 Interrupt enable bit
        b4 IEN4 Interrupt enable bit
        b5 IEN5 Interrupt enable bit
        b6 IEN6 Interrupt enable bit
        b7 IEN7 Interrupt enable bit
        */
        ICU.IER[0x04].BIT.IEN1      = 0;

        /* DTC start enable register (USB0 D0FIFO transfer disable)
        b0    DTCE     DTC start enable bit
        b7-b1 Reserved 0
        */
        ICU.DTCER[33].BIT.DTCE      = 0;
    }
    else
    {
        /* Interrupt request register
        b0    IR       Interrupt status flag
        b7-b1 Reserved 0
        */
        ICU.IR[36].BIT.IR           = 0;

        /* Priority D1FIFO0=0
        b3-b0 IPR      Interrupt priority
        b7-b4 Reserved 0
        */
        ICU.IPR[36].BYTE            = 0x00;

        /* Interrupt enable register (USB1 D0FIFO disable(IEN4))
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

        /* DTC start enable register (USB1 D0FIFO transfer disable)
        b0    DTCE     DTC start enable bit
        b7-b1 Reserved 0
        */
        ICU.DTCER[36].BIT.DTCE      = 0;
    }
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
    size    = (uint8_t )(tmp);
    size    += (uint16_t)(tmp << 8);

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
        ICU.DTCER[33].BIT.DTCE      = 1;
    }
    else
    {
        /* DTC start enable register (USB1 D0FIFO transfer)
        b0    DTCE     DTC start enable bit
        b7-b1 Reserved 0
        */
        ICU.DTCER[36].BIT.DTCE      = 1;
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
        ICU.IPR[33].BYTE    = 0x05; /* USB0 */
    }
    else
    {
        /* Priority D0FIFO0=0(Disable)
        b3-b0 IPR      Interrupt priority
        b7-b4 Reserved 0
        */
        ICU.IPR[36].BYTE    = 0x05; /* USB1 */
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
        ICU.IPR[33].BYTE    = 0x00; /* USB0 */
    }
    else
    {
        /* Priority D0FIFO0=0(Disable)
        b3-b0 IPR      Interrupt priority
        b7-b4 Reserved 0
        */
        ICU.IPR[36].BYTE    = 0x00; /* USB1 */
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
End  Of File
******************************************************************************/
