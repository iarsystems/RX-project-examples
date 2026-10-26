/******************************************************************************
* File Name     : r_usb_pmsc_api.h
* Version       : 2.00
* Description   : USB Peripheral MSC Driver API declaration
******************************************************************************
* History   : DD.MM.YYYY Version Description
*           : 30.11.2012 2.00
******************************************************************************/
#ifndef __R_USB_PMSC_API_H__
#define __R_USB_PMSC_API_H__

/*****************************************************************************
Public Functions (API)
******************************************************************************/
uint16_t    R_usb_pmsc_ProcessWaitTmo(uint16_t tmo);
USB_ER_t    R_usb_pmsc_Close(USB_UTR_t *ptr, uint16_t data1, uint16_t data2);
USB_ER_t    R_usb_pmsc_Open(USB_UTR_t *ptr, uint16_t data1, uint16_t data2);
void        R_usb_pmsc_ControlTrans0(USB_UTR_t *ptr, USB_REQUEST_t *req);
void        R_usb_pmsc_ControlTrans1(USB_UTR_t *ptr, USB_REQUEST_t *req);
void        R_usb_pmsc_ControlTrans2(USB_UTR_t *ptr, USB_REQUEST_t *req);
void        R_usb_pmsc_ControlTrans3(USB_UTR_t *ptr, USB_REQUEST_t *req);
void        R_usb_pmsc_ControlTrans4(USB_UTR_t *ptr, USB_REQUEST_t *req);
void        R_usb_pmsc_ControlTrans5(USB_UTR_t *ptr, USB_REQUEST_t *req);
void        R_usb_pmsc_DataTrans(USB_UTR_t *ptr, uint16_t pipe, uint32_t size, uint8_t *table, USB_CB_t complete);
void        R_usb_pmsc_DescriptorChange(USB_UTR_t *ptr, uint16_t mode, uint16_t data2);
void        R_usb_pmsc_GetMaxLun(USB_UTR_t *ptr, USB_CB_INFO_t complete);
void        R_usb_pmsc_MassStrageReset(USB_UTR_t *ptr, USB_CB_INFO_t complete);
void        R_usb_pmsc_Registration(USB_UTR_t *ptr);
void        R_usb_pmsc_SetConfig(USB_UTR_t *ptr);
void        R_usb_pmsc_SetInterface(USB_UTR_t *ptr, uint16_t data1, uint16_t data2);
void        R_usb_pmsc_StallClearResult(USB_UTR_t *ptr, uint16_t data,uint16_t dummy);
void        R_usb_pmsc_StrgTaskClose(USB_UTR_t *ptr);
void        R_usb_pmsc_StrgTaskOpen(USB_UTR_t *ptr);
void        R_usb_pmsc_TransResult(USB_UTR_t *mess, uint16_t data1, uint16_t data2 );

#endif  /* __R_USB_PMSC_API_H__ */
/******************************************************************************
End  Of File
******************************************************************************/
