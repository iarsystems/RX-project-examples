/******************************************************************************
* File Name     : r_usb_hmsc_api.h
* Version       : 2.00
* Description   : USB Host MSC Driver API declaration
******************************************************************************
* History   : DD.MM.YYYY Version Description
*           : 30.11.2012 2.00
******************************************************************************/
#ifndef __R_USB_HMSC_API_H__
#define __R_USB_HMSC_API_H__

/*****************************************************************************
Public Functions (API)
******************************************************************************/

uint16_t    R_usb_hmsc_DriveSpeed(USB_UTR_t *ptr, uint16_t side);
uint16_t    R_usb_hmsc_GetDevSts(void);
uint16_t    R_usb_hmsc_Information(uint16_t PipeOffset);
uint16_t    R_usb_hmsc_Inquiry(USB_UTR_t *ptr, uint16_t side, uint8_t *buff);
uint16_t    R_usb_hmsc_ModeSelect6(USB_UTR_t *ptr, uint16_t side, uint8_t *buff);
uint16_t    R_usb_hmsc_ModeSense10(USB_UTR_t *ptr, uint16_t side, uint8_t *buff);
uint16_t    R_usb_hmsc_PreventAllow(USB_UTR_t *ptr, uint16_t side, uint8_t *buff);
uint16_t    R_usb_hmsc_Read10(USB_UTR_t *ptr, uint16_t side, uint8_t *buff,
                uint32_t secno, uint16_t seccnt, uint32_t trans_byte);
uint16_t    R_usb_hmsc_ReadCapacity(USB_UTR_t *ptr, uint16_t side, uint8_t *buff);
uint16_t    R_usb_hmsc_ReadFormatCapacity(USB_UTR_t *ptr, uint16_t side, uint8_t *buff);
uint16_t    R_usb_hmsc_RequestSense(USB_UTR_t *ptr, uint16_t side, uint8_t *buff);
uint16_t    R_usb_hmsc_SetDevSts(uint16_t data);
uint16_t    R_usb_hmsc_StrgDriveClose(uint16_t side);
USB_ER_t    R_usb_hmsc_StrgDriveOpen(USB_UTR_t *ptr, uint16_t side);
uint16_t    R_usb_hmsc_StrgDriveSearch(USB_UTR_t *ptr, uint16_t addr);
uint16_t    R_usb_hmsc_StrgReadSector(USB_UTR_t *ptr, uint16_t side, uint8_t *buff
                , uint32_t secno, uint16_t seccnt, uint32_t trans_byte);
uint16_t    R_usb_hmsc_StrgUserCommand(USB_UTR_t *ptr, uint16_t side, uint16_t command, uint8_t *buff);
uint16_t    R_usb_hmsc_StrgWriteSector(USB_UTR_t *ptr, uint16_t side, uint8_t *buff
                , uint32_t secno, uint16_t seccnt, uint32_t trans_byte);
uint16_t    R_usb_hmsc_TestUnit(USB_UTR_t *ptr, uint16_t side);
uint16_t    R_usb_hmsc_Write10(USB_UTR_t *ptr, uint16_t side, uint8_t *buff
                , uint32_t secno, uint16_t seccnt, uint32_t trans_byte);
void        R_usb_hmsc_ClassCheck(USB_UTR_t *ptr, uint16_t **table);
void        R_usb_hmsc_DriveClose(USB_UTR_t *ptr, uint16_t addr, uint16_t data2);
void        R_usb_hmsc_DriveOpen(USB_UTR_t *ptr, uint16_t addr, uint16_t data2);
void        R_usb_hmsc_Initialized(USB_UTR_t *ptr, uint16_t data1, uint16_t data2);
void        R_usb_hmsc_Release(USB_UTR_t *ptr);
void        R_usb_hmsc_StrgTaskClose(USB_UTR_t *ptr);
void        R_usb_hmsc_StrgTaskOpen(USB_UTR_t *ptr);
void        R_usb_hmsc_TaskClose(USB_UTR_t *ptr);
void        R_usb_hmsc_TaskOpen(USB_UTR_t *ptr, uint16_t data1, uint16_t data2);
void        R_usb_hmsc_driver_start( USB_UTR_t *ptr );

void        R_usb_hmsc_ClearStall(USB_UTR_t *ptr, uint16_t type, uint16_t msgnum, USB_CB_t complete);
USB_ER_t    R_usb_hmsc_MassStorageReset(USB_UTR_t *ptr, uint16_t drvnum, USB_CB_t complete);
USB_ER_t    R_usb_hmsc_GetMaxUnit(USB_UTR_t *ptr, uint16_t addr, USB_CB_t complete);

#endif  /* __R_USB_HMSC_API_H__ */
/******************************************************************************
End  Of File
******************************************************************************/
