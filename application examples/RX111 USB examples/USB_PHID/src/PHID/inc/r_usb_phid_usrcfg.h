/******************************************************************************
* File Name     : r_usb_phid_usrcfg.h
* Version       : 2.10
* Description   : USB Peripheral PHID user configuration
******************************************************************************
* History   : DD.MM.YYYY Version Description
*           : 30.11.2012 2.00    First Release
*           : 31.05.2013 2.10    Corresponded to RL78/L1C, RX111
******************************************************************************/
#ifndef __R_USB_CLASS_USRCFG_H__
#define __R_USB_CLASS_USRCFG_H__

/*****************************************************************************
Macro definitions (USER DEFINE)
******************************************************************************/
#define USB_PHID_DEMO_MODE          0           /* Peripheral HID Application demo mode */
#define USB_PHID_KEYBOARD_MODE      1           /* Peripheral HID Application keyboard mode */
#define USB_PHID_MOUSE_MODE         2           /* Peripheral HID Application mouse mode */


/* Select HID Demo sample Application mode  */
//#define USB_PHID_APL_MODE       USB_PHID_DEMO_MODE      /* Peripheral HID Application demo mode */
//#define USB_PHID_APL_MODE       USB_PHID_KEYBOARD_MODE  /* Peripheral HID Application keyboard mode */
#define USB_PHID_APL_MODE       USB_PHID_MOUSE_MODE     /* Peripheral HID Application mouse mode */


#define USB_VENDORID    0x0000
#define USB_BCDNUM          0x0200u        /* bcdUSB */
#define USB_RELEASE         0x0100u        /* Release Number */
#define USB_CONFIGNUM       1u             /* Configuration number */

#ifdef  USB_LSPERI_PP
  #define USB_DCPMAXP       (8u)            /* DCP max packet size */
#endif  /* USB_LSPERI_PP */
#ifdef  USB_FSPERI_PP
  #define USB_DCPMAXP       (64u)           /* DCP max packet size */
#endif  /* USB_FSPERI_PP */

/*  HID Report Descriptor length */
#if USB_PHID_APL_MODE == USB_PHID_DEMO_MODE
  #define USB_PRODUCTID     0x0003
  #define USB_ICONFIGBUM    4
  #define USB_IFPROTOCOL    USB_IFPRO_DEM
  #define USB_INTEPMAXP     USB_PHID_MXPS_DEMO   /* Interrupt pipe max packet size */
  #define USB_SMPLRPRTLEN   34
  #define USB_PHID_SETREPORT_LENGTH 17u          /* Receive data by the class request Set_Report. */
#endif  /* Peripheral HID Application demo mode */
#if USB_PHID_APL_MODE == USB_PHID_KEYBOARD_MODE
  #define USB_PRODUCTID     0x0013
  #define USB_ICONFIGBUM    5
  #define USB_IFPROTOCOL    USB_IFPRO_KBD
  #define USB_INTEPMAXP     USB_PHID_MXPS_KBD    /* Interrupt pipe max packet size */
  #define USB_SMPLRPRTLEN   63
  #define USB_PHID_SETREPORT_LENGTH 1u           /* Receive data by the class request Set_Report. */
#endif  /* Peripheral HID Application keyboard mode */
#if USB_PHID_APL_MODE == USB_PHID_MOUSE_MODE
  #define USB_PRODUCTID     0x0023
  #define USB_ICONFIGBUM    6
  #define USB_IFPROTOCOL    USB_IFPRO_MSE
  #define USB_INTEPMAXP     USB_PHID_MXPS_MSE    /* Interrupt pipe max packet size */
  #define USB_SMPLRPRTLEN   50
  #define USB_PHID_SETREPORT_LENGTH 1u           /* Receive data by the class request Set_Report. */
#endif  /* Peripheral HID Application mouse mode */


#endif  /* __R_USB_CLASS_USRCFG_H__ */
/******************************************************************************
End Of File
******************************************************************************/
