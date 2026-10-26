/******************************************************************************
* File Name     : r_usb_phid_api.h
* Version       : 2.10
* Description   : USB Peripheral PHID Header
******************************************************************************
* History   : DD.MM.YYYY Version Description
*           : 30.11.2012 2.00    First Release
*           : 31.05.2013 2.10    Corresponded to RL78/L1C, RX111
******************************************************************************/
#ifndef __R_USB_PHID_API_H__
#define __R_USB_PHID_API_H__


/*****************************************************************************
Public Functions (API)
******************************************************************************/
void        R_usb_phid_task(void);
void        R_usb_phid_DriverRelease(void);
void        R_usb_phid_SetTaskPri(void);
void        R_usb_phid_TransferEnd(void);
void        R_usb_phid_DeviceInformation(uint16_t *deviceinfo);
void        R_usb_phid_ChangeDeviceState(uint16_t msginfo);
void        R_usb_phid_driver_start(void);
void        R_usb_phid_control_callback(usb_request_t *request, uint16_t data);
void        R_usb_phid_send_data( uint8_t *Table, usb_leng_t size, usb_cb_t complete );

#endif  /* __R_USB_PHID_API_H__ */
/******************************************************************************
End Of File
******************************************************************************/
