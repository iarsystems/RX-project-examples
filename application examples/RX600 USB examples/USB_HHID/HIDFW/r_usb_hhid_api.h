/******************************************************************************
* File Name		: r_usb_hhid_api.h
* Version		: 2.00
* Description	: USB Host HID Driver API declaration
******************************************************************************
* History	: DD.MM.YYYY Version Description
*			: 30.11.2012 2.00
******************************************************************************/
#ifndef __R_USB_HHID_API_H__
#define __R_USB_HHID_API_H__


/*****************************************************************************
Public Functions (API)
******************************************************************************/
void        R_usb_hhid_task(USB_VP_INT_t stacd);
void        R_usb_hhid_class_check(USB_UTR_t *ptr, uint16_t **table);
void        R_usb_hhid_driver_start( USB_UTR_t *ptr );
void        R_usb_hhid_SetPipeRegistration(USB_UTR_t *ptr, uint16_t devadr);
USB_ER_t    R_usb_hhid_PipeTransferExample(USB_UTR_t *ptr, uint8_t *table, uint32_t size, USB_CB_t complete);
USB_ER_t    R_usb_hhid_TransferEnd(USB_UTR_t *ptr);
USB_ER_t    R_usb_hhid_class_request(void *pram);
void        R_usb_hhid_DeviceInformation(USB_UTR_t *ptr, uint16_t *deviceinfo);
void        R_usb_hhid_ChangeDeviceState(USB_UTR_t *ptr, uint16_t msginfo);
uint16_t    R_usb_hhid_GetReportLength(void);
uint8_t     R_usb_hhid_get_hid_protocol(void);

#endif  /* __R_USB_HHID_API_H__ */
/******************************************************************************
End Of File
******************************************************************************/
