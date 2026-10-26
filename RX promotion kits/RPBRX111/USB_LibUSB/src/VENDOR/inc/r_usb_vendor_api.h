/*******************************************************************************
* File Name    : r_usb_vendor_api.h
* Version      : 2.01
* Description  : USB Basic firmware API Hcd function prototype declaration
*******************************************************************************/
/*******************************************************************************
* History   : DD.MM.YYYY Version Description
*           : 31.03.2012 1.80    Basic FW mini Ver2.00(ES Version)
*           : 30.11.2012 2.00    Basic FW mini Ver2.00 Release
*           : 31.01.2013 2.01    Corresponded to RL78/L1C
*******************************************************************************/
#ifndef __R_USB_VENDOR_API_H__
#define __R_USB_VENDOR_API_H__


/******************************************************************************
Macro definitions
******************************************************************************/
#define USB_SMPL_CHECKREQUEST       (0x21u)
#define USB_SMPL_INIT               (0x22u)
#define USB_SMPL_TRANSFER_END       (0x23u)         /* Transfer callback message */
#define USB_SMPL_TRANSFER_START     (0x24u)         /* Data transfer start message */
#define USB_SMPL_TRANSFER_STOP      (0x25u)         /* Data transfer stop message */

/* nonOS Use */
#define USB_SEQ_00                  (0u)
#define USB_SEQ_01                  (1u)
#define USB_SEQ_02                  (2u)
#define USB_SEQ_03                  (3u)
#define USB_SEQ_04                  (4u)
#define USB_SEQ_05                  (5u)
#define USB_SEQ_06                  (6u)
#define USB_SEQ_07                  (7u)
#define USB_SEQ_08                  (8u)
#define USB_SEQ_09                  (9u)
#define USB_SEQ_10                  (10u)
#define USB_SEQ_11                  (11u)
#define USB_SEQ_12                  (12u)
#define USB_SEQ_13                  (13u)
#define USB_SEQ_14                  (14u)
#define USB_SEQ_15                  (15u)
#define USB_SEQ_16                  (16u)
#define USB_SEQ_17                  (17u)
#define USB_SEQ_18                  (18u)
#define USB_SEQ_20                  (20u)
#define USB_SEQ_21                  (21u)
#define USB_SEQ_22                  (22u)
#define USB_SEQ_23                  (23u)
#define USB_SEQ_24                  (24u)
#define USB_SEQ_30                  (30u)
#define USB_SEQ_31                  (31u)
#define USB_SEQ_32                  (32u)
#define USB_SEQ_33                  (33u)
#define USB_SEQ_35                  (35u)
#define USB_SEQ_36                  (36u)
#define USB_SEQ_37                  (37u)
#define USB_SEQ_38                  (38u)
#define USB_SEQ_40                  (40u)
#define USB_SEQ_41                  (41u)
#define USB_SEQ_42                  (42u)
#define USB_SEQ_43                  (43u)
#define USB_SEQ_44                  (44u)


/******************************************************************************
Public Functions (API)
******************************************************************************/
/* Peripheral vendor class driver API function */
void        R_usb_pvndr_Task(void);
usb_er_t    R_usb_pvndr_TransferStart(usb_utr_t *utr_table);
usb_er_t    R_usb_pvndr_TransferStop(usb_pipe_t pipe);
void        R_usb_pvndr_StateCallback(uint16_t data, uint16_t state);
void        R_usb_pvndr_ControlCallback(usb_request_t *request, uint16_t data);
void        R_usb_pvndr_DriverStart(void);
void        R_usb_pvndr_DriverStop(void);

/* Host vendor class driver API function */
void        R_usb_hvndr_Task(void);
usb_er_t    R_usb_hvndr_TransferStart(usb_utr_t *utr_table);
usb_er_t    R_usb_hvndr_TransferStop(usb_pipe_t pipe);
void        R_usb_hvndr_DriverStart(void);
void        R_usb_hvndr_DriverStop(void);
void        R_usb_hvndr_StateCallback(uint16_t data, uint16_t state);
void        R_usb_hvndr_ClassCheck(uint8_t **table);
void        R_usb_hvndr_PipeRegistration(void);
uint8_t     R_usb_hvndr_UsePipe(usb_pipe_t pipenum);



#endif  /* __R_USB_VENDOR_API_H__ */
/******************************************************************************
End of file
******************************************************************************/
