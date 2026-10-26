/******************************************************************************
* File Name     : r_usb_phid_define.h
* Version       : 2.10
* Description   : USB Peripheral PHID Header
******************************************************************************
* History   : DD.MM.YYYY Version Description
*           : 30.11.2012 2.00    First Release
*           : 31.05.2013 2.10    Corresponded to RL78/L1C, RX111
******************************************************************************/
#ifndef __R_USB_PHID_DEFINE_H__
#define __R_USB_PHID_DEFINE_H__


/*****************************************************************************
Macro definitions
******************************************************************************/
//----- HID Mouse Class Request Defines --------
/* HID Class Request codes */
#define USB_GET_REPORT                  0x01u
#define USB_GET_IDLE                    0x02u
#define USB_GET_PROTOCOL                0x03u
#define USB_SET_IDLE                    0x0Au
#define USB_SET_REPORT                  0x09u
#define USB_SET_PROTOCOL                0x0Bu

/* PHID reception message type definition */
#define USB_GET_REPORT_DESCRIPTOR       0x22u
#define USB_GET_HID_DESCRIPTOR          0x21u
#define USB_SET_REPORT_STATUS           0x33u

/* ----- DESCRIPTOR Types ----- */
#define USB_DT_TYPE_GETREPORT           (uint8_t)0x01   /* Report Type Input */
#define USB_DT_TYPE_SETREPORT           (uint8_t)0x02   /* Report Type Output */
#define USB_DT_TYPE_HIDDESCRIPTOR       (uint8_t)0x21   /* HID descriptor type */
#define USB_DT_TYPE_RPTDESCRIPTOR       (uint8_t)0x22   /* Report descriptor type */

/* ----- Subclass Codes ----- */
#define USB_IFSUB_NOBOOT                (uint8_t)0x00   /* No Subclass */
#define USB_IFSUB_BOOT                  (uint8_t)0x01   /* Boot Interface Subclass */

/* ----- Protocol Codes ----- */
#define USB_IFPRO_DEM                   (uint8_t)0x00   /* Demonstration */
#define USB_IFPRO_KBD                   (uint8_t)0x01   /* Keyboard */
#define USB_IFPRO_MSE                   (uint8_t)0x02   /* Mouse */

/* Report Type set */
#define SET_DT_TYPE(v)                  ((v) << 8)

/* Peripheral HID Transfer Pipe */
#define USB_PHID_USE_PIPE               USB_PIPE6

/* Peripheral HID Max Packet Size */
#define USB_PHID_MXPS_DEMO              5u
#define USB_PHID_MXPS_KBD               8u
#define USB_PHID_MXPS_MSE               3u

#define USB_SEND_DATA                   0x11u

/******************************************************************************
Typedef definitions
******************************************************************************/

#endif  /* __R_USB_PHID_DEFINE_H__ */
/******************************************************************************
End  Of File
******************************************************************************/
