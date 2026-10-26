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
* File Name    : rx_mcu.h
* Version      : 0.80
* Device(s)    : Renesas SH-Series, RX-Series
* Tool-Chain   : Renesas SuperH RISC engine Standard Toolchain
*              : Renesas RX Standard Toolchain
* OS           : Common to None and uITRON 4.0 Spec
* H/W Platform : Independent
* Description  : RX63x MCU processing
******************************************************************************
* History : DD.MM.YYYY Version Description
*         : 28.12.2011 0.80    First Release
******************************************************************************/
#ifndef __RX_MCU_H__
#define __RX_MCU_H__

/******************************************************************************
Constant macro definitions
******************************************************************************/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    USB port macro define
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    USB interrupt macro define
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
#define USB0_D0FIFO_NUM         33
#define USB0_D1FIFO_NUM         34
#define USB0_D0FIFO_IER         ICU.IER[0x04].BIT.IEN1
#define USB0_D1FIFO_IER         ICU.IER[0x04].BIT.IEN2
#define USB0_USBIO_IER          ICU.IER[0x04].BIT.IEN3
#define USB0_USBR_IER           ICU.IER[0x0B].BIT.IEN2
#define USB0_D0FIFO_IPR         ICU.IPR[USB0_D0FIFO_NUM].BYTE
#define USB0_D1FIFO_IPR         ICU.IPR[USB0_D1FIFO_NUM].BYTE
#define USB0_USBIO_IPR          ICU.IPR[35].BYTE
#define USB0_USBR_IPR           ICU.IPR[90].BYTE

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    USB interrupt macro define
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
#define USB_PSEL11          0x11

/* VBUS(PE5) */
    #define USB0_VBUS_PDR       PORTE.PDR.BIT.B5
    #define USB0_VBUS_PMR       PORTE.PMR.BIT.B5
    #define USB0_VBUS_PFS       MPC.PE5PFS.BYTE
    #define USB0_VBUS_ENB       USB_PSEL11
/* OVRCURB(PE0) */
    #define USB0_OVRCURB_PDR    PORTE.PDR.BIT.B0
    #define USB0_OVRCURB_PMR    PORTE.PMR.BIT.B0
    #define USB0_OVRCURB_PFS    MPC.PE0PFS.BYTE
    #define USB0_OVRCURB_ENB    USB_PSEL11                  /* PE0PFS = 0x11 */
/* OVRCURA(PE1) */
    #define USB0_OVRCURA_PDR    PORTE.PDR.BIT.B1
    #define USB0_OVRCURA_PMR    PORTE.PMR.BIT.B1
    #define USB0_OVRCURA_PFS    MPC.PE1PFS.BYTE
    #define USB0_OVRCURA_ENB    USB_PSEL11                  /* PE1PFS = 0x11 */
/* DRPD(P01) */
    #define USB0_DRPD_PDR       PORT0.PDR.BIT.B1
    #define USB0_DRPD_PMR       PORT0.PMR.BIT.B1
    #define USB0_DRPD_PFS       MPC.P01PFS.BYTE
    #define USB0_DRPD_ENB       USB_PSEL11
/* DPRPD(P12) */
    #define USB0_DPRPD_PDR      PORT1.PDR.BIT.B2
    #define USB0_DPRPD_PMR      PORT1.PMR.BIT.B2
    #define USB0_DPRPD_PFS      MPC.P12PFS.BYTE
    #define USB0_DPRPD_ENB      USB_PSEL11
/* ID(PD2) */
    #define USB0_ID_PDR         PORTD.PDR.BIT.B2
    #define USB0_ID_PMR         PORTD.PMR.BIT.B2
    #define USB0_ID_PFS         MPC.PD2PFS.BYTE
    #define USB0_ID_ENB         USB_PSEL11
/* EXICEN(PD1) */
    #define USB0_EXICEN_PDR     PORTD.PDR.BIT.B1
    #define USB0_EXICEN_PMR     PORTD.PMR.BIT.B1
    #define USB0_EXICEN_PFS     MPC.PD1PFS.BYTE
    #define USB0_EXICEN_ENB     USB_PSEL11
/* USB0 DPUPE(UDPUPEPFS) */
    #define USB0_DPRPU_PFS      MPC.UDPUPEPFS.BYTE
    #define USB0_DPRPU_ENB      USB_PSEL11
/* VBUSEN(P13) */
    #define USB0_VBUSEN_PDR     PORT1.PDR.BIT.B3
    #define USB0_VBUSEN_PMR     PORT1.PMR.BIT.B3
    #define USB0_VBUSEN_PFS     MPC.P13PFS.BYTE
    #define USB0_VBUSEN_ENB     USB_PSEL11
#endif  /* __RX_MCU_H__ */

/******************************************************************************
End  Of File
******************************************************************************/
