/******************************************************************************
* File Name     : r_usb_pcdc_api.h
* Version       : 2.00
* Description   : USB Peripheral CDC Driver API declaration
******************************************************************************
* History   : DD.MM.YYYY Version Description
*           : 30.11.2012 2.00
******************************************************************************/
#ifndef __R_USB_PCDC_API_H__
#define __R_USB_PCDC_API_H__

/*****************************************************************************
Public Functions (API)
******************************************************************************/
void R_usb_pcdc_task(USB_VP_INT_t stacd);
void R_usb_pcdc_set_interface(USB_UTR_t *ptr, uint16_t data1, uint16_t data2);
void R_usb_pcdc_descriptor_change(USB_UTR_t *ptr, uint16_t mode, uint16_t data2);
void R_usb_pcdc_SendData(USB_UTR_t *ptr, uint8_t *Table, uint32_t size, USB_CB_t complete);
void R_usb_pcdc_ReceiveData(USB_UTR_t *ptr, uint8_t *Table, uint32_t size, USB_CB_t complete);
void R_usb_pcdc_SerialStateNotification(USB_UTR_t *ptr, uint16_t serial_state, USB_CB_t complete);
void R_usb_pcdc_driver_start( void );
void R_usb_pcdc_usr_ctrl_trans_function(USB_UTR_t *ptr, USB_REQUEST_t *preq, uint16_t ctsq);

#endif  /* __R_USB_PCDC_API_H__ */
/******************************************************************************
End  Of File
******************************************************************************/
