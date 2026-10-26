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
* Description  : RX63N RSK processing
******************************************************************************
* History : DD.MM.YYYY Version Description
*         : 29.07.2011 0.50    First Release
*         : 06.03.2012         Added #pragma interrupt to ISRs as part of using
*         :                    C-startup files.
******************************************************************************/



/******************************************************************************
Includes   <System Includes> , "Project Includes"
******************************************************************************/
#include <string.h>
#include "r_usb_ctypedef.h"     /* Type define */
#include <iorx63n.h>
#include "hw_resource.h"

#include "r_usb_ckernelid.h"        /* Kernel ID definition */

/******************************************************************************
External variables and functions
******************************************************************************/

/******************************************************************************
Constant macro definitions
******************************************************************************/

/* Serial port */
/* USB DATA MAX PACKET SIZE define */
#define USB_SCI_MAXPS                   64u
/* Communications Devices Class */
#define USB_SCI_BUFFER_SIZE_MAX         (USB_SCI_MAXPS * 8u)
#define USB_SCI_STX_BUFF_MARGIN         (USB_SCI_MAXPS * 1u)    /* Serial Port Transfer Buffer Margin check define */

#define USB_SCI_MODE_ECHO               1u  /* Echo mode define */
#define USB_SCI_BUFF_BUSY               1u  /* USB Receive data buffer busy */

/* SCI(Serial Communication Interface) Transfer Enable & Receive Enable */
#define USB_SCI_SCI_BYTE_TE_RE          0x30

#define USB_SCI_ENABLE                  1u
#define USB_SCI_DISABLE                 0u

/* SCI Interrupt level */
#define USB_SCI_SCI_INT_LV              5u
/* SCI Data field length */
#define USB_SCI_8BIT_DATA               0u
#define USB_SCI_7BIT_DATA               1u

/* SCI Data Parity setting */
#define USB_SCI_PARITY_ENABLE           1u
#define USB_SCI_PARITY_DISABLE          0u
#define USB_SCI_PARITY_EVEN             0u
#define USB_SCI_PARITY_ODD              1u

/* SCI Data Stop bit length */
#define USB_SCI_1STOP_BIT               0u
#define USB_SCI_2STOP_BIT               1u

#define USB_SCI_CLEAR                   0u
#define USB_SCI_ERR_DETECT              1u

/* CLOCK SELECT for SCI BRG source */
#define USB_SCI_CLK_0DIVID_FREQ         0u
#define USB_SCI_CLK_4DIVID_FREQ         1u
#define USB_SCI_CLK_16DIVID_FREQ        2u
#define USB_SCI_CLK_64DIVID_FREQ        3u

/****************************************/
/* SCI0                                 */
/****************************************/
typedef enum
{
    USB_SCI_DATA_BIT_7  = 7u,   /* Line Coding 7Data bits */
    USB_SCI_DATA_BIT_8  = 8u    /* Line Coding 8Data bits */
} USB_SCI_DATA_BIT_t;

typedef enum
{
    USB_SCI_STOP_BIT_1  = 0u,   /* Line Coding 1Stop bits */
    USB_SCI_STOP_BIT_2  = 2u    /* Line Coding 2Stop bits */
}
USB_SCI_STOT_BIT_t;

typedef enum
{
    USB_SCI_PARITY_BIT_NONE = 0u,   /* Line Coding None Parity */
    USB_SCI_PARITY_BIT_ODD  = 1u,   /* Line Coding Odd Parity */
    USB_SCI_PARITY_BIT_EVEN = 2u    /* Line Coding Even Parity */
}
USB_SCI_PARITY_BIT_t;

typedef enum
{
    USB_SCI_SPEED_1200      = 1200u,    /* Line Coding 1200bps */
    USB_SCI_SPEED_2400      = 2400u,    /* Line Coding 2400bps */
    USB_SCI_SPEED_4800      = 4800u,    /* Line Coding 4800bps */
    USB_SCI_SPEED_9600      = 9600u,    /* Line Coding 9600bps */
    USB_SCI_SPEED_14400     = 14400u,   /* Line Coding 14400bps */
    USB_SCI_SPEED_19200     = 19200u,   /* Line Coding 19200bps */
    USB_SCI_SPEED_38400     = 38400u,   /* Line Coding 38400bps */
    USB_SCI_SPEED_57600     = 57600u,   /* Line Coding 57600bps */
    USB_SCI_SPEED_115200    = 115200u,  /* Line Coding 115200bps */
}
USB_SCI_LINE_SPEED_t;

/* Class Notification Serial State */
typedef struct
{
    /* reserve */
    uint16_t                    rsv:9;
    /* Over Run error */
    uint16_t                    bOverRun:1;
    /* Parity error */
    uint16_t                    bParity:1;
    /* Framing error */
    uint16_t                    bFraming:1;
    /* State of ring signal detection of the device */
    uint16_t                    bRingSignal:1;
    /* State of break detection mechanism of teh device */
    uint16_t                    bBreak:1;
    /* DSR signal */
    uint16_t                    bTxCarrier:1;
    /* DCD signal */
    uint16_t                    bRxCarrier:1;
}
USB_SCI_SerialState_t;

/* Serial Status register */
typedef struct
{
    unsigned char RSV:2;
    unsigned char ORER:1;
    unsigned char FER:1;
    unsigned char PER:1;
    unsigned char TEND:1;
    unsigned char MPB:1;
    unsigned char MPBT:1;
}
USB_SCI_SSR;

/* PCLK=48MHz   */
#define USB_SCI_BRR_1200        77u
#define USB_SCI_BRR_2400        38u
#define USB_SCI_BRR_4800        77u
#define USB_SCI_BRR_9600        155u
#define USB_SCI_BRR_14400       103u
#define USB_SCI_BRR_19200       77u
#define USB_SCI_BRR_38400       38u
#define USB_SCI_BRR_57600       25u
#define USB_SCI_BRR_115200      12u

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
/* global variable prototype definition */

/* Communications Devices Class */
uint8_t     usb_gpcdc_stxdata[USB_SCI_BUFFER_SIZE_MAX+4];   /* Comm Port Tx data buffer */
uint16_t    usb_gpcdc_stx_wp;   /* Comm Port Tx data buffer data count */
uint16_t    usb_gpcdc_stx_rp;   /* Comm Port Tx data buffer read data count */


uint8_t     usb_gpcdc_srxdata[USB_SCI_BUFFER_SIZE_MAX+4];   /* Comm Port Rx data buffer */
uint16_t    usb_gpcdc_srx_wp;   /* Comm Port Rx data buffer Write Pointer */
uint16_t    usb_gpcdc_srx_rp;   /* Comm Port Rx data buffer Read Pointer */
USB_SCI_SSR usb_gpcdc_com_port_ssr; /* Serial Status Register read data */

/*=== Serial port ===========================================================*/
void        usb_cpu_Sci_DataSend(uint16_t mode, void *tranadr, uint16_t length);
uint16_t    usb_cpu_Sci_DataReceive(uint8_t *tranadr,uint16_t receive_length);
uint16_t    usb_cpu_Sci_StxBuffStatus(void);
void        usb_cpu_Sci_Buffer_init(void);
uint16_t    usb_cpu_Sci_CopyData_for_Echo(void);
uint16_t    usb_cpu_Sci_GetSerialState(uint16_t *serial_state);
uint16_t    usb_cpu_Sci_EnableStatus(void);
uint16_t    usb_cpu_Sci_SetDataChk(uint8_t *data_rate, uint8_t stop_bit, uint8_t parity, uint8_t data_bit);

#ifdef USB_UART_ENABLE
uint16_t    usb_cpu_Sci_Set1(uint8_t *data_rate, uint8_t stop_bit, uint8_t parity, uint8_t data_bit);
void        usb_cpu_Sci_HW_init(void);
void        usb_cpu_Sci_enable(void);
void        usb_cpu_Sci_disable(void);

/******************************************************************************
Renesas Abstracted RSK functions
******************************************************************************/

/******************************************************************************
Serial Port function
******************************************************************************/

/******************************************************************************
Function Name   : usb_cpu_Sci_Set1
Description     : Line coding set
Arguments       : data_rate   : Data terminal rate in bits per second
                : stop_bit    : Stop bits 0-1Stopbit 1-1.5Stopbit 2-2Stopbit
                : parity      : Parity 0-None 1-Odd 2-Even 3-Mark 4-Spase
                : data_bit    : Data bits(5,6,7,8 or 16)
Return value    : Error code  : Error code
******************************************************************************/
uint16_t usb_cpu_Sci_Set1(uint8_t *data_rate_top, uint8_t stop_bit, uint8_t parity, uint8_t data_bit)
{
    uint16_t ret_code = USB_OK;
    volatile register uint32_t  tmp;
    uint32_t data_rate;

    /* DATA RATE Set */
    data_rate = (uint32_t)data_rate_top[0];
    data_rate |= (uint32_t)data_rate_top[1] << 8;
    data_rate |= (uint32_t)data_rate_top[2] << 16;
    data_rate |= (uint32_t)data_rate_top[3] << 24;

    /* SCI desable
       CKE:0 Internal baud rate generator
       TIE,RIE,TE,RE,TEIE = 0 */
    /* Serial Control Register
    b0-b1 CKE  Clock enable bit
    b2    TEIE Transmit End Interrupt enable bit
    b3    MPIE Mult Processor Interrupt enable bit
    b4    RE   Receive enable bit
    b5    TE   Transmit enable bit
    b6    RIE  Receive Interrupt enable bit
    b7    TIE  Transmit Interrupt enable bit
    */
    SCI0.SCR.BYTE = 0;

    /* SCI0 TXI Interrypt clear */
    /* Interrupt request register
    b0    IR       Interrupt status flag
    b7-b1 Reserved 0
    */
    ICU.IR[215].BIT.IR = 0;

    /* Serial Mode */
    /* Smart Card Mode Register
    b0    SMIF     Smart Card Interface Mode Select
    b1    Reserved 0
    b2    SINV     Smart Card Data Invert
    b3    SDIR     Smart Card Data Transfer Direction
    b6-b4 Reserved 0
    b7    BCP2     BCP2 Base Clock Pulse 2
    */
    SCI0.SCMR.BIT.SMIF  = 0;

    /* Clock source PCLK */
    /* Serial Mode Register
    b0-b1 CKS  Clock Select bit
    b2    MP   Mult Processor mode bit
    b3    STOP Stop bit length bit
    b4    PM   Parity Mode bit
    b5    PE   Parity Enable bit
    b6    CHR  CHARACTER length bit
    b7    CM   Communication Mode bit
    */
    SCI0.SMR.BIT.CKS = USB_SCI_CLK_0DIVID_FREQ;

    /* DATA RATE Setting */
    switch(data_rate)
    {
    case USB_SCI_SPEED_1200:
        /* Clock source PCLK/4 */
        /* Serial Mode Register
        b0-b1 CKS  Clock Select bit
        b2    MP   Mult Processor mode bit
        b3    STOP Stop bit length bit
        b4    PM   Parity Mode bit
        b5    PE   Parity Enable bit
        b6    CHR  CHARACTER length bit
        b7    CM   Communication Mode bit
        */
        SCI0.SMR.BIT.CKS = USB_SCI_CLK_16DIVID_FREQ;
        /* 48MHz,  1200bps */
        /* Bit Rate Register
        b0-b7 BRR
        */
        SCI0.BRR = USB_SCI_BRR_1200;
        break;

    case USB_SCI_SPEED_2400:
        /* Clock source PCLK/4 */
        /* Serial Mode Register
        b0-b1 CKS  Clock Select bit
        b2    MP   Mult Processor mode bit
        b3    STOP Stop bit length bit
        b4    PM   Parity Mode bit
        b5    PE   Parity Enable bit
        b6    CHR  CHARACTER length bit
        b7    CM   Communication Mode bit
        */
        SCI0.SMR.BIT.CKS = USB_SCI_CLK_16DIVID_FREQ;
        /* 48MHz,  2400bps */
        /* Bit Rate Register
        b0-b7 BRR
        */
        SCI0.BRR = USB_SCI_BRR_2400;
        break;

    case USB_SCI_SPEED_4800:
        /* Clock source PCLK/4 */
        /* Serial Mode Register
        b0-b1 CKS  Clock Select bit
        b2    MP   Mult Processor mode bit
        b3    STOP Stop bit length bit
        b4    PM   Parity Mode bit
        b5    PE   Parity Enable bit
        b6    CHR  CHARACTER length bit
        b7    CM   Communication Mode bit
        */
        SCI0.SMR.BIT.CKS = USB_SCI_CLK_4DIVID_FREQ;
        /* 48MHz,  4800bps */
        /* Bit Rate Register
        b0-b7 BRR
        */
        SCI0.BRR = USB_SCI_BRR_4800;
        break;

    case USB_SCI_SPEED_9600:
        /* 48MHz,  9600bps */
        /* Bit Rate Register
        b0-b7 BRR
        */
        SCI0.BRR = USB_SCI_BRR_9600;
        break;

    case USB_SCI_SPEED_14400:
        /* 48MHz,  14400bps */
        /* Bit Rate Register
        b0-b7 BRR
        */
        SCI0.BRR = USB_SCI_BRR_14400;
        break;

    case USB_SCI_SPEED_19200:
        /* 48MHz,  19200bps */
        /* Bit Rate Register
        b0-b7 BRR
        */
        SCI0.BRR = USB_SCI_BRR_19200;
        break;

    case USB_SCI_SPEED_38400:
        /* 48MHz,  38400bps */
        /* Bit Rate Register
        b0-b7 BRR
        */
        SCI0.BRR = USB_SCI_BRR_38400;
        break;

    case USB_SCI_SPEED_57600:
        /* 48MHz,  57600bps */
        /* Bit Rate Register
        b0-b7 BRR
        */
        SCI0.BRR = USB_SCI_BRR_57600;
        break;

    case USB_SCI_SPEED_115200:
        /* 48MHz,  57600bps */
        /* Bit Rate Register
        b0-b7 BRR
        */
        SCI0.BRR = USB_SCI_BRR_115200;
        break;

    default:
        ret_code = USB_NG;
        break;
    }

    /* STOP BIT Setting */
    if(stop_bit == USB_SCI_STOP_BIT_1)
    {
        /* 1stop bit */
        /* Serial Mode Register
        b0-b1 CKS  Clock Select bit
        b2    MP   Mult Processor mode bit
        b3    STOP Stop bit length bit
        b4    PM   Parity Mode bit
        b5    PE   Parity Enable bit
        b6    CHR  CHARACTER length bit
        b7    CM   Communication Mode bit
        */
        SCI0.SMR.BIT.STOP = USB_SCI_1STOP_BIT;
    }
    else if(stop_bit == USB_SCI_STOP_BIT_2)
    {
        /* 2stop bit */
        /* Serial Mode Register
        b0-b1 CKS  Clock Select bit
        b2    MP   Mult Processor mode bit
        b3    STOP Stop bit length bit
        b4    PM   Parity Mode bit
        b5    PE   Parity Enable bit
        b6    CHR  CHARACTER length bit
        b7    CM   Communication Mode bit
        */
        SCI0.SMR.BIT.STOP = USB_SCI_2STOP_BIT;
    }
    else
    {
        ret_code = USB_NG;
    }

    /* PARITY BIT Setting */
    if(parity == USB_SCI_PARITY_BIT_NONE)
    {
        /* Parity disable */
        /* Serial Mode Register
        b0-b1 CKS  Clock Select bit
        b2    MP   Mult Processor mode bit
        b3    STOP Stop bit length bit
        b4    PM   Parity Mode bit
        b5    PE   Parity Enable bit
        b6    CHR  CHARACTER length bit
        b7    CM   Communication Mode bit
        */
        SCI0.SMR.BIT.PE = USB_SCI_PARITY_DISABLE;
    }
    else if(parity == USB_SCI_PARITY_BIT_ODD)
    {
        /* Parity enable */
        /* Parity mode odd */
        /* Serial Mode Register
        b0-b1 CKS  Clock Select bit
        b2    MP   Mult Processor mode bit
        b3    STOP Stop bit length bit
        b4    PM   Parity Mode bit
        b5    PE   Parity Enable bit
        b6    CHR  CHARACTER length bit
        b7    CM   Communication Mode bit
        */
        SCI0.SMR.BIT.PE = USB_SCI_PARITY_ENABLE;
        SCI0.SMR.BIT.PM = USB_SCI_PARITY_ODD;
    }
    else if(parity == USB_SCI_PARITY_BIT_EVEN)
    {
        /* Parity enable */
        /* Parity mode even */
        /* Serial Mode Register
        b0-b1 CKS  Clock Select bit
        b2    MP   Mult Processor mode bit
        b3    STOP Stop bit length bit
        b4    PM   Parity Mode bit
        b5    PE   Parity Enable bit
        b6    CHR  CHARACTER length bit
        b7    CM   Communication Mode bit
        */
        SCI0.SMR.BIT.PE = USB_SCI_PARITY_ENABLE;
        SCI0.SMR.BIT.PM = USB_SCI_PARITY_EVEN;
    }
    else
    {
        ret_code = USB_NG;
    }

    /* CHARACTER LENGTH Setting */
    if(data_bit == USB_SCI_DATA_BIT_8)
    {
        /* character length 8bit */
        /* Serial Mode Register
        b0-b1 CKS  Clock Select bit
        b2    MP   Mult Processor mode bit
        b3    STOP Stop bit length bit
        b4    PM   Parity Mode bit
        b5    PE   Parity Enable bit
        b6    CHR  CHARACTER length bit
        b7    CM   Communication Mode bit
        */
        SCI0.SMR.BIT.CHR = USB_SCI_8BIT_DATA;
    }
    else if(data_bit == USB_SCI_DATA_BIT_7)
    {
        /* character length 7bit */
        /* Serial Mode Register
        b0-b1 CKS  Clock Select bit
        b2    MP   Mult Processor mode bit
        b3    STOP Stop bit length bit
        b4    PM   Parity Mode bit
        b5    PE   Parity Enable bit
        b6    CHR  CHARACTER length bit
        b7    CM   Communication Mode bit
        */
        SCI0.SMR.BIT.CHR = USB_SCI_7BIT_DATA;
    }
    else
    {
        ret_code = USB_NG;
    }

    /* Wait 1bit time more */
    for( tmp = 0; tmp < 6400; ++tmp ) {
    }

    /* Serial Control Register
    b0-b1 CKE  Clock enable bit
    b2    TEIE Transmit End Interrupt enable bit
    b3    MPIE Mult Processor Interrupt enable bit
    b4    RE   Receive enable bit
    b5    TE   Transmit enable bit
    b6    RIE  Receive Interrupt enable bit
    b7    TIE  Transmit Interrupt enable bit
    */
    SCI0.SCR.BIT.TIE    = USB_SCI_ENABLE;
    SCI0.SCR.BIT.RIE    = USB_SCI_ENABLE;
    SCI0.SCR.BYTE       |= USB_SCI_SCI_BYTE_TE_RE;

    return ret_code;
}
/******************************************************************************
End of function usb_cpu_Sci_Set1
******************************************************************************/


/******************************************************************************
Function Name   : usb_cpu_Sci_ER_Int
Description     : ComPort Receive Error Interrupt.
                : Group 12 SCI0 ERI0 (SCI0 reception error) GEN12.EN0
                : GRP12.IS0 GCR12.CLR0. Interrupt #114.
Arguments       : -
Return value    : -
******************************************************************************/
#pragma vector=VECT_ICU_GROUP12
__interrupt void usb_cpu_Sci_ER_Int(void)
{
    /* Group Interrupt clear register (GROUPL12 SCI0 ERI clear)
    b0 EN0 Interrupt request clear bit
    b1 EN1 Interrupt request clear bit
    b2 EN2 Interrupt request clear bit
    b3 EN3 Interrupt request clear bit
    b4 EN4 Interrupt request clear bit
    b5 EN5 Interrupt request clear bit
    b6 EN6 Interrupt request clear bit
    b7 EN7 Interrupt request clear bit
    b8 EN8 Interrupt request clear bit
    b9 EN9 Interrupt request clear bit
    b10 EN10 Interrupt request clear bit
    b11 EN11 Interrupt request clear bit
    b12 EN12 Interrupt request clear bit
    b13 EN13 Interrupt request clear bit
    b14 EN14 Interrupt request clear bit
    b15 EN15 Interrupt request clear bit
    b31-b16 Reserved 0
    */
    ICU.GCR[12].BIT.CLR0 = 1;

    /* Serial Status Register read */
    /* Serial Status Register
    b0 MPBT Mult Processor Bit Transfer bit
    b1 MPB  Mult Processor Bit
    b2 TEND Transmit End flag
    b3 PER  Parity Error
    b4 FER  Freaming Error
    b5 ORER Over Run Error
    b6-b7 - Reserved
    */
    *(uint8_t *)&usb_gpcdc_com_port_ssr = SCI0.SSR.BYTE;
    SCI0.SSR.BYTE = 0;
}
/******************************************************************************
End of function usb_cpu_Sci_ER_Int
******************************************************************************/


/******************************************************************************
Function Name   : usb_cpu_Sci_RX_Int
Description     : ComPort Receive Interrupt
Arguments       : none
                :
Return value    : none
******************************************************************************/
#pragma vector=VECT(SCI0, RXI0)
__interrupt void usb_cpu_Sci_RX_Int(void)
{
    /* Serial Status error clear */
    *(uint8_t *)&usb_gpcdc_com_port_ssr = 0;

    /* Receive data store */
    /* Receive Data Register
    b0-7 :RDR
    */
    usb_gpcdc_srxdata[usb_gpcdc_srx_wp] = SCI0.RDR;
    /* Receive data write pointer update */
    usb_gpcdc_srx_wp++;
    if(usb_gpcdc_srx_wp >= USB_SCI_BUFFER_SIZE_MAX)
    {
        usb_gpcdc_srx_wp = 0;
    }
}
/******************************************************************************
End of function usb_cpu_Sci_RX_Int
******************************************************************************/

/******************************************************************************
Function Name   : usb_cpu_Sci_TX_Int
Description     : ComPort Transfer Buffer Empty Interrupt
Arguments       : none
                :
Return value    : none
******************************************************************************/
#pragma vector=VECT(SCI0, TXI0)
__interrupt void usb_cpu_Sci_TX_Int(void)
{
    /* Send Data check */
    if(usb_gpcdc_stx_wp != usb_gpcdc_stx_rp)
    {
        /* Data send */
        /* Transmit Data Register
        b0-7 :TDR
        */
        SCI0.TDR =
            usb_gpcdc_stxdata[usb_gpcdc_stx_rp];
        /* Send data pointer update */
        usb_gpcdc_stx_rp++;
        if(usb_gpcdc_stx_rp >= USB_SCI_BUFFER_SIZE_MAX)
        {
            usb_gpcdc_stx_rp = 0;
        }

        if(usb_gpcdc_stx_wp == usb_gpcdc_stx_rp)
        {
            /* Interrupt enable register (IEN0:TXI0 enable)
            b0 IEN0 Interrupt enable bit
            b1 IEN1 Interrupt enable bit
            b2 IEN2 Interrupt enable bit
            b3 IEN3 Interrupt enable bit
            b4 IEN4 Interrupt enable bit
            b5 IEN5 Interrupt enable bit
            b6 IEN6 Interrupt enable bit
            b7 IEN7 Interrupt enable bit
            */
            ICU.IER[0x1a].BIT.IEN7 = USB_SCI_DISABLE;
        }

    }

}
/******************************************************************************
End of function usb_cpu_Sci_TX_Int
******************************************************************************/

/******************************************************************************
Function Name   : usb_cpu_Sci_TE_Int
Description     : ComPort Transfer End Interrupt
Arguments       : none
                :
Return value    : none
******************************************************************************/
#pragma vector=VECT(SCI0, TEI0)
__interrupt void usb_cpu_Sci_TE_Int(void)
{
}
/******************************************************************************
End of function usb_cpu_Sci_TE_Int
******************************************************************************/


/******************************************************************************
Function Name   : usb_cpu_Sci_HW_init
Description     : ComPort Hw Initialize
Argument        : none
Return          : none
******************************************************************************/
void usb_cpu_Sci_HW_init(void)
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

    /* SCI0 Enable */
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
    SYSTEM.MSTPCRB.BIT.MSTPB31 = 0;

    /* Protect register
    b0    PRC0     Protect bit0
    b1    PRC1     Protect bit1
    b2    Reserved 0
    b3    PRC3     Protect bit3
    b7-b4 Reserved 0
    b15-b8 PRKEY   PRC Key code bit
    */
    SYSTEM.PRCR.WORD    = 0xA500;   /* Protect on */

    /* TxD0-A=output */
    /* Data direction register
    b0 B0 Pn0 in/out select bit
    b1 B1 Pn1 in/out select bit
    b2 B2 Pn2 in/out select bit
    b3 B3 Pn3 in/out select bit
    b4 B4 Pn4 in/out select bit
    b5 B5 Pn5 in/out select bit
    b6 B6 Pn6 in/out select bit
    b7 B7 Pn7 in/out select bit
    */
    PORT2.PDR.BIT.B0    = 0;
    PORT2.PDR.BIT.B1    = 0;

    /* Port mode register (Disable TXD0)
    b0 B0 Pn0 pin mode control bit
    b1 B1 Pn1 pin mode control bit
    b2 B2 Pn2 pin mode control bit
    b3 B3 Pn3 pin mode control bit
    b4 B4 Pn4 pin mode control bit
    b5 B5 Pn5 pin mode control bit
    b6 B6 Pn6 pin mode control bit
    b7 B7 Pn7 pin mode control bit
    */
    PORT2.PMR.BIT.B0    = 0;

    /* Port mode register (Disable RXD0)
    b0 B0 Pn0 pin mode control bit
    b1 B1 Pn1 pin mode control bit
    b2 B2 Pn2 pin mode control bit
    b3 B3 Pn3 pin mode control bit
    b4 B4 Pn4 pin mode control bit
    b5 B5 Pn5 pin mode control bit
    b6 B6 Pn6 pin mode control bit
    b7 B7 Pn7 pin mode control bit
    */
    PORT2.PMR.BIT.B1    = 0;

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

    /* TXD0 select */
    /* Port1-bit6 Pin function select register
    b4-b0 PSEL  Pin function select bit
    b5 Reserved 0
    b6 ISEL     Interrupt input function select bit
    b7 Reserved 0
    */
    MPC.P20PFS.BYTE         = 0x0a;

    /* RXD0 select */
    /* Port1-bit6 Pin function select register
    b4-b0 PSEL  Pin function select bit
    b5 Reserved 0
    b6 ISEL     Interrupt input function select bit
    b7 Reserved 0
    */
    MPC.P21PFS.BYTE         = 0x0a;

    /* Port mode register write disable */
    /* PFS register write protect on */
    /* Write protect register
    b5-b0 Reserved 0
    b6 PFSWE    PFS register write enable bit
    b7 BOWI     PFSWE bit write disable bit
    */
    MPC.PWPR.BYTE           = 0x80;

    /* Port mode register (Enable TXD0)
    b0 B0 Pn0 pin mode control bit
    b1 B1 Pn1 pin mode control bit
    b2 B2 Pn2 pin mode control bit
    b3 B3 Pn3 pin mode control bit
    b4 B4 Pn4 pin mode control bit
    b5 B5 Pn5 pin mode control bit
    b6 B6 Pn6 pin mode control bit
    b7 B7 Pn7 pin mode control bit
    */
    PORT2.PMR.BIT.B0    = 1;

    /* Port mode register (Enable RXD0)
    b0 B0 Pn0 pin mode control bit
    b1 B1 Pn1 pin mode control bit
    b2 B2 Pn2 pin mode control bit
    b3 B3 Pn3 pin mode control bit
    b4 B4 Pn4 pin mode control bit
    b5 B5 Pn5 pin mode control bit
    b6 B6 Pn6 pin mode control bit
    b7 B7 Pn7 pin mode control bit
    */
    PORT2.PMR.BIT.B1    = 1;

    /* TxD0-A=output */
    /* Data direction register
    b0 B0 Pn0 in/out select bit
    b1 B1 Pn1 in/out select bit
    b2 B2 Pn2 in/out select bit
    b3 B3 Pn3 in/out select bit
    b4 B4 Pn4 in/out select bit
    b5 B5 Pn5 in/out select bit
    b6 B6 Pn6 in/out select bit
    b7 B7 Pn7 in/out select bit
    */
    PORT2.PDR.BIT.B0    = 1;

    /* RxD0-A=input */
    /* Data direction register
    b0 B0 Pn0 in/out select bit
    b1 B1 Pn1 in/out select bit
    b2 B2 Pn2 in/out select bit
    b3 B3 Pn3 in/out select bit
    b4 B4 Pn4 in/out select bit
    b5 B5 Pn5 in/out select bit
    b6 B6 Pn6 in/out select bit
    b7 B7 Pn7 in/out select bit
    */
    PORT2.PDR.BIT.B1    = 0;

    /* Interrupt enable */
    /* Interrupt enable register (IEN6:RXI0 enable)
    b0 IEN0 Interrupt enable bit
    b1 IEN1 Interrupt enable bit
    b2 IEN2 Interrupt enable bit
    b3 IEN3 Interrupt enable bit
    b4 IEN4 Interrupt enable bit
    b5 IEN5 Interrupt enable bit
    b6 IEN6 Interrupt enable bit
    b7 IEN7 Interrupt enable bit
    */
    ICU.IER[0x1a].BIT.IEN6 = USB_SCI_ENABLE;

    /* Interrupt enable register (IEN7:TXI0 enable)
    b0 IEN0 Interrupt enable bit
    b1 IEN1 Interrupt enable bit
    b2 IEN2 Interrupt enable bit
    b3 IEN3 Interrupt enable bit
    b4 IEN4 Interrupt enable bit
    b5 IEN5 Interrupt enable bit
    b6 IEN6 Interrupt enable bit
    b7 IEN7 Interrupt enable bit
    */
    ICU.IER[0x1a].BIT.IEN7 = USB_SCI_DISABLE;

    /* Interrupt enable register (IEN0:TEI0 enable)
    b0 IEN0 Interrupt enable bit
    b1 IEN1 Interrupt enable bit
    b2 IEN2 Interrupt enable bit
    b3 IEN3 Interrupt enable bit
    b4 IEN4 Interrupt enable bit
    b5 IEN5 Interrupt enable bit
    b6 IEN6 Interrupt enable bit
    b7 IEN7 Interrupt enable bit
    */
    ICU.IER[0x1b].BIT.IEN0 = USB_SCI_ENABLE;

    /* Priority SCI0=5
    b3-b0 IPR      Interrupt priority
    b7-b4 Reserved 0
    */
    ICU.IPR[214].BYTE = USB_SCI_SCI_INT_LV;

    /* Priority GROUPL12=5
    b3-b0 IPR      Interrupt priority
    b7-b4 Reserved 0
    */
    ICU.IPR[114].BYTE = USB_SCI_SCI_INT_LV;

    /* Group Interrupt enable register (GROUPL12 SCI0 ERI enable)
    b0 EN0 Interrupt request enable bit
    b1 EN1 Interrupt request enable bit
    b2 EN2 Interrupt request enable bit
    b3 EN3 Interrupt request enable bit
    b4 EN4 Interrupt request enable bit
    b5 EN5 Interrupt request enable bit
    b6 EN6 Interrupt request enable bit
    b7 EN7 Interrupt request enable bit
    b8 EN8 Interrupt request enable bit
    b9 EN9 Interrupt request enable bit
    b10 EN10 Interrupt request enable bit
    b11 EN11 Interrupt request enable bit
    b12 EN12 Interrupt request enable bit
    b13 EN13 Interrupt request enable bit
    b14 EN14 Interrupt request enable bit
    b15 EN15 Interrupt request enable bit
    b31-b16 Reserved 0
    */
    ICU.GEN[12].BIT.EN0 = USB_SCI_ENABLE;

    /* Interrupt enable register (IEN2:GROUPL12 enable)
    b0 IEN0 Interrupt enable bit
    b1 IEN1 Interrupt enable bit
    b2 IEN2 Interrupt enable bit
    b3 IEN3 Interrupt enable bit
    b4 IEN4 Interrupt enable bit
    b5 IEN5 Interrupt enable bit
    b6 IEN6 Interrupt enable bit
    b7 IEN7 Interrupt enable bit
    */
    ICU.IER[0x0e].BIT.IEN2 = USB_SCI_ENABLE;
}
/******************************************************************************
End of function usb_cpu_Sci_HW_init
******************************************************************************/

/******************************************************************************
Function Name   : usb_cpu_Sci_enable
Description     : Serial port enable (SCI0)
Argument        : none
Return          : none
******************************************************************************/
void usb_cpu_Sci_enable(void)
{
    /* TE = 1, RE = 1 */
    /* Serial Control Register
    b0-b1 CKE  Clock enable bit
    b2    TEIE Transmit End Interrupt enable bit
    b3    MPIE Mult Processor Interrupt enable bit
    b4    RE   Receive enable bit
    b5    TE   Transmit enable bit
    b6    RIE  Receive Interrupt enable bit
    b7    TIE  Transmit Interrupt enable bit
    */
    SCI0.SCR.BYTE       |= USB_SCI_SCI_BYTE_TE_RE;
}
/******************************************************************************
End of function usb_cpu_Sci_enable
******************************************************************************/

/******************************************************************************
Function Name   : usb_cpu_Sci_disable
Description     : Serial port disable (SCI0)
Argument        : none
Return          : none
******************************************************************************/
void usb_cpu_Sci_disable(void)
{

    /* TE = 1, RE = 1 */
    /* Serial Control Register
    b0-b1 CKE  Clock enable bit
    b2    TEIE Transmit End Interrupt enable bit
    b3    MPIE Mult Processor Interrupt enable bit
    b4    RE   Receive enable bit
    b5    TE   Transmit enable bit
    b6    RIE  Receive Interrupt enable bit
    b7    TIE  Transmit Interrupt enable bit
    */
    SCI0.SCR.BYTE       &= 0xcf;
}
/******************************************************************************
End of function usb_cpu_Sci_disable
******************************************************************************/

/******************************************************************************
Function Name   : usb_cpu_Sci_EnableStatus
Description     : Serial Port Enable State Get process
Argument        : none
Return          : 1-Serial port enable 0-Serial port disable
******************************************************************************/
uint16_t usb_cpu_Sci_EnableStatus(void)
{
    uint16_t enable = USB_SCI_DISABLE;

    /* TE & RE check */
    /* Serial Control Register
    b0-b1 CKE  Clock enable bit
    b2    TEIE Transmit End Interrupt enable bit
    b3    MPIE Mult Processor Interrupt enable bit
    b4    RE   Receive enable bit
    b5    TE   Transmit enable bit
    b6    RIE  Receive Interrupt enable bit
    b7    TIE  Transmit Interrupt enable bit
    */
    if((SCI0.SCR.BYTE & USB_SCI_SCI_BYTE_TE_RE)
        == USB_SCI_SCI_BYTE_TE_RE)
    {
        /* TX enable & RX enable */
        enable = USB_SCI_ENABLE;
    }

    return enable;
}
/******************************************************************************
End of function usb_cpu_Sci_EnableStatus
******************************************************************************/
#endif /* USB_UART_ENABLE */

/******************************************************************************
Function Name   : usb_cpu_Sci_Buffer_init
Description     : Serial Tx Rx data buffer clear
Argument        : none
Return          : none
******************************************************************************/
void usb_cpu_Sci_Buffer_init(void)
{
    /* Comm Port Tx data buffer */
    memset((void *)&usb_gpcdc_stxdata[0],0,USB_SCI_BUFFER_SIZE_MAX);
    /* Comm Port Rx data buffer */
    memset((void *)&usb_gpcdc_srxdata[0],0,USB_SCI_BUFFER_SIZE_MAX);
    /* Comm Port Tx data buffer data count */
    usb_gpcdc_stx_wp = 0;
    /* Comm Port Rx data buffer Read Pointer */
    usb_gpcdc_stx_rp = 0;

    /* Comm Port Rx data buffer Write Pointer */
    usb_gpcdc_srx_wp = 0;
    /* Comm Port Rx data buffer Read Pointer */
    usb_gpcdc_srx_rp = 0;
}
/******************************************************************************
End of function usb_cpu_Sci_Buffer_init
******************************************************************************/

/******************************************************************************
Function Name   : usb_cpu_Sci_DataReceive
Description     : Communications Port Data Receive Process
Arguments       : tranadr     : Transfer Data Address
                : length      : Transfer Data Length
Return value    : uint16_t    : Receive data byte count
******************************************************************************/
uint16_t usb_cpu_Sci_DataReceive(uint8_t *tranadr,uint16_t receive_length)
{
    uint16_t length;
    uint16_t wp;
    uint16_t i;

    /* Write pointer read  */
    wp = usb_gpcdc_srx_wp;

    /* Comm Port receive data count check */
    if(wp == usb_gpcdc_srx_rp)
    {
        return 0;   /* No receive data */
    }
    else if(wp > usb_gpcdc_srx_rp)
    {
        /* Read pointer not late */
        length = wp - usb_gpcdc_srx_rp;
    }
    else
    {
        /* Read pointer late */
        length = USB_SCI_BUFFER_SIZE_MAX - usb_gpcdc_srx_rp + wp;
    }

    /* Read length check */
    if( length > receive_length )
    {
        length = receive_length;
    }

    /* Receive data copy */
    for(i =0; i <length; i++)
    {
        tranadr[i] = usb_gpcdc_srxdata[usb_gpcdc_srx_rp];
        usb_gpcdc_srx_rp++;
        if(usb_gpcdc_srx_rp >= USB_SCI_BUFFER_SIZE_MAX)
        {
            usb_gpcdc_srx_rp = 0;
        }
    }

    return length;
}
/******************************************************************************
End of function usb_cpu_Sci_DataReceive
******************************************************************************/

/******************************************************************************
Function Name   : usb_cpu_Sci_DataSend
Description     : Communications Port Data Send Process
Arguments       : tranadr     : Transfer Data Address
                : length      : Transfer Data Length
Return value    : uint16_t    : 1-Echo mode data copy request
******************************************************************************/
void usb_cpu_Sci_DataSend(uint16_t mode, void *tranadr, uint16_t length)
{
    uint16_t    copy_size1,copy_size2;

    /* Buffer Store Data size check */
    if((usb_gpcdc_stx_wp + length) <= USB_SCI_BUFFER_SIZE_MAX)
    {
        /* All data store for Current Page  */
        memcpy(&usb_gpcdc_stxdata[usb_gpcdc_stx_wp], tranadr, length);
        /* Store Data counter update */
        usb_gpcdc_stx_wp += length;
        if(usb_gpcdc_stx_wp >= USB_SCI_BUFFER_SIZE_MAX)
        {
            usb_gpcdc_stx_wp = 0;
        }
    }
    else
    {
        /* Current Page copy size */
        copy_size1 = USB_SCI_BUFFER_SIZE_MAX - usb_gpcdc_stx_wp;
        /* Reverse Page copy size */
        copy_size2 = length - copy_size1;

        /* Data Store Separation */
        memcpy(&usb_gpcdc_stxdata[usb_gpcdc_stx_wp], tranadr, copy_size1);

        /* Data Store Separation */
        memcpy(&usb_gpcdc_stxdata[0], ((uint8_t *)tranadr + copy_size1), copy_size2);

        /* Store Data counter update */
        usb_gpcdc_stx_wp = copy_size2;
    }

#ifdef USB_UART_ENABLE
    /* Echo mode check */
    if(mode != USB_SCI_MODE_ECHO)
    {
        /* Data Send check */
        if(ICU.IER[0x1a].BIT.IEN7 == USB_SCI_DISABLE)
        {
            /* Send Start */
            /* Send Data count check */
            if(usb_gpcdc_stx_wp != usb_gpcdc_stx_rp)
            {
                /* Transmit interrupt enable */
                /* Interrupt enable register (IEN7:TXI0 enable)
                b0 IEN0 Interrupt enable bit
                b1 IEN1 Interrupt enable bit
                b2 IEN2 Interrupt enable bit
                b3 IEN3 Interrupt enable bit
                b4 IEN4 Interrupt enable bit
                b5 IEN5 Interrupt enable bit
                b6 IEN6 Interrupt enable bit
                b7 IEN7 Interrupt enable bit
                */
                ICU.IER[0x1a].BIT.IEN7 = USB_SCI_ENABLE;
            }
        }
    }
#endif /* USB_UART_ENABLE */
}
/******************************************************************************
End of function usb_cpu_Sci_DataSend
******************************************************************************/

/******************************************************************************
Function Name   : usb_cpu_Sci_CopyData_for_Echo
Description     : Echo mode data copy Serial TX -> Serial RX
Argument        : none
Return          : Error code
******************************************************************************/
uint16_t usb_cpu_Sci_CopyData_for_Echo(void)
{
    uint16_t srx_ptr_check;

    while(1)
    {
        /* Send Data check */
        if(usb_gpcdc_stx_wp != usb_gpcdc_stx_rp)
        {
            /* Receive data write pointer update for Copy Stop Check */
            srx_ptr_check = usb_gpcdc_srx_wp + 1;
            if( srx_ptr_check >= USB_SCI_BUFFER_SIZE_MAX )
            {
                srx_ptr_check = 0;
            }
            /* Copy Stop Check */
            if( srx_ptr_check == usb_gpcdc_srx_rp )
            {
                break;
            }

            /* Receive data store */
            usb_gpcdc_srxdata[usb_gpcdc_srx_wp]
                = usb_gpcdc_stxdata[usb_gpcdc_stx_rp];

            /* Receive data write pointer update */
            usb_gpcdc_srx_wp++;
            if(usb_gpcdc_srx_wp >= USB_SCI_BUFFER_SIZE_MAX)
            {
                usb_gpcdc_srx_wp = 0;
            }
            /* Send data pointer update */
            usb_gpcdc_stx_rp++;
            if(usb_gpcdc_stx_rp >= USB_SCI_BUFFER_SIZE_MAX)
            {
                usb_gpcdc_stx_rp = 0;
            }
        }
        else
        {
            /* Transfer end */
            break;
        }
    }

    return USB_OK;
}
/******************************************************************************
End of function usb_cpu_Sci_CopyData_for_Echo
******************************************************************************/

/******************************************************************************
Function Name   : usb_cpu_Sci_GetSerialState
Description     : Serial State Get process
Argument        : SeraalState(CDC PSTN Subclass UART State bitmap) store addr
Return          : 1-Serial error detect
******************************************************************************/
uint16_t usb_cpu_Sci_GetSerialState(uint16_t *serial_state)
{
    USB_SCI_SerialState_t *state;
    uint16_t ret_code = USB_OK;

    state = (USB_SCI_SerialState_t *)serial_state;

    /* State initalize */
    *serial_state = USB_SCI_CLEAR;

    if(usb_gpcdc_com_port_ssr.ORER)
    {
        /* Over Run error */
        state->bOverRun = USB_ON;
        ret_code = 1;
    }
    if(usb_gpcdc_com_port_ssr.FER)
    {
        /* Framing error */
        state->bFraming = USB_ON;
        ret_code = USB_SCI_ERR_DETECT;
    }
    if(usb_gpcdc_com_port_ssr.PER)
    {
        /* Parity error */
        state->bParity = USB_ON;
        ret_code = USB_SCI_ERR_DETECT;
    }

    return ret_code;
}
/******************************************************************************
End of function usb_cpu_Sci_GetSerialState
******************************************************************************/

/******************************************************************************
Function Name   : usb_cpu_Sci_StxBuffStatus
Description     : Serial port send buffer status check
Arguments       : none
                :
Return value    : Serial port send buffer status 1:Busy 0:Not Busy
******************************************************************************/
uint16_t usb_cpu_Sci_StxBuffStatus(void)
{
    uint16_t ret_code = 0;
    uint16_t wp_tmp;
    uint16_t rp,wp;

    rp = usb_gpcdc_stx_rp;
    wp = usb_gpcdc_stx_wp;

    /* Serial Transfer Buffer Space calc */
    if(wp < rp)
    {
        /* Write pointer < Read pointer */
        wp_tmp = rp - wp;
    }
    else if(wp > rp)
    {
        /* Write pointer > Read pointer */
        wp_tmp = rp
            + (USB_SCI_BUFFER_SIZE_MAX- wp);
    }
    else    /* wp = rp */
    {
        /* Write pointer = Read pointer */
        wp_tmp = USB_SCI_BUFFER_SIZE_MAX;
    }

    /* Buffer Space check */
    if( USB_SCI_STX_BUFF_MARGIN >= wp_tmp )
    {
        /* Read pointer Overtake */
        ret_code = USB_SCI_BUFF_BUSY;
    }

    return ret_code;
}
/******************************************************************************
End of function usb_cpu_Sci_StxBuffStatus
******************************************************************************/

/******************************************************************************
Function Name   : usb_cpu_Sci_SetDataChk
Description     : Line coding set data check
Arguments       : data_rate   : Data terminal rate in bits per second
                : stop_bit    : Stop bits 0-1Stopbit 1-1.5Stopbit 2-2Stopbit
                : parity      : Parity 0-None 1-Odd 2-Even 3-Mark 4-Spase
                : data_bit    : Data bits(5,6,7,8 or 16)
Return value    : Error code  : Error code
******************************************************************************/
uint16_t usb_cpu_Sci_SetDataChk(uint8_t *data_rate_top, uint8_t stop_bit, uint8_t parity, uint8_t data_bit)
{
    uint16_t ret_code = USB_OK;
    uint32_t data_rate;

    /* DATA RATE Set */
    data_rate = (uint32_t)data_rate_top[0];
    data_rate |= (uint32_t)data_rate_top[1] << 8;
    data_rate |= (uint32_t)data_rate_top[2] << 16;
    data_rate |= (uint32_t)data_rate_top[3] << 24;

    /* DATA RATE Setting */
    if((data_rate != USB_SCI_SPEED_1200)
        && (data_rate != USB_SCI_SPEED_2400)
        && (data_rate != USB_SCI_SPEED_4800)
        && (data_rate != USB_SCI_SPEED_9600)
        && (data_rate != USB_SCI_SPEED_14400)
        && (data_rate != USB_SCI_SPEED_19200)
        && (data_rate != USB_SCI_SPEED_38400)
        && (data_rate != USB_SCI_SPEED_57600)
        && (data_rate != USB_SCI_SPEED_115200))
    {
        ret_code = USB_NG;
    }

    /* STOP BIT Setting */
    if((stop_bit != USB_SCI_STOP_BIT_1)
        && (stop_bit != USB_SCI_STOP_BIT_2))
    {
        ret_code = USB_NG;
    }

    /* PARITY BIT Setting */
    if((parity != USB_SCI_PARITY_BIT_NONE)
        && (parity != USB_SCI_PARITY_BIT_ODD)
        && (parity != USB_SCI_PARITY_BIT_EVEN))
    {
        ret_code = USB_NG;
    }

    /* CHARACTER LENGTH Setting */
    if((data_bit != USB_SCI_DATA_BIT_8)
        && (data_bit != USB_SCI_DATA_BIT_7))
    {
        ret_code = USB_NG;
    }

    return ret_code;
}
/******************************************************************************
End of function usb_cpu_Sci_SetDataChk
******************************************************************************/

/******************************************************************************
End  Of File
******************************************************************************/
