/*******************************************************************************
* DISCLAIMER
* This software is supplied by Renesas Electronics Corporation and is only
* intended for use with Renesas products. No other uses are authorized. This
* software is owned by Renesas Electronics Corporation and is protected under
* all applicable laws, including copyright laws.
* THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES REGARDING
* THIS SOFTWARE, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING BUT NOT
* LIMITED TO WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE
* AND NON-INFRINGEMENT. ALL SUCH WARRANTIES ARE EXPRESSLY DISCLAIMED.
* TO THE MAXIMUM EXTENT PERMITTED NOT PROHIBITED BY LAW, NEITHER RENESAS
* ELECTRONICS CORPORATION NOR ANY OF ITS AFFILIATED COMPANIES SHALL BE LIABLE
* FOR ANY DIRECT, INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES FOR
* ANY REASON RELATED TO THIS SOFTWARE, EVEN IF RENESAS OR ITS AFFILIATES HAVE
* BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
* Renesas reserves the right, without notice, to make changes to this software
* and to discontinue the availability of this software. By using this software,
* you agree to the additional terms and conditions found by accessing the
* following link:
* http://www.renesas.com/disclaimer *
* Copyright (C) 2012 Renesas Electronics Corporation. All rights reserved.
*******************************************************************************/
/*******************************************************************************
* File Name    : main.c
* Version      : 1.00
* Device(s)    : Renesas
* Tool-Chain   : Renesas
* OS           : Not use
* H/W Platform : Independent
* Description  : USB Host vendor class driver function code.
*******************************************************************************/
/*******************************************************************************
* History : DD.MM.YYYY Version Description
*         : 31.03.2012 1.80    Basic FW mini Ver2.00(ES Version)
*         : 15.08.2012 2.00
*******************************************************************************/

/******************************************************************************
Section definitions
******************************************************************************/
//#pragma SFR
#ifdef  R8CUSB
#pragma section program P_smpl
#pragma section bss B_smpl
#pragma section data R_smpl
#pragma section rom C_smpl
#endif
#ifdef  RX_USB
//#pragma section _smpl
#endif
#ifdef  RL78USB
#endif
#ifdef  RL78USB_SECTION
#pragma section @@CODE   P_SMP
#pragma section @@BASE   T_SMP
#pragma section @@CNST   C_SMP
#pragma section @@R_INIT D_SMP
#pragma section @@DATA   B_SMP
#pragma section @@INIT   R_SMP
#endif


/******************************************************************************
Includes <System Includes> , "Project Includes"
******************************************************************************/
#include "r_usb_ctypedef.h"             /* Type define */
#include "r_usb_ckernelid.h"            /* Kernel ID definition */
#include "r_usb_usrconfig.h"            /* System definition */
//#include "r_usb_cdefusbip.h"            /* USB-FW Library Header */
#include "r_usb_usrconfig.h"            /* System definition, USB-FW Library Header */
#include "r_usb_api.h"                  /* USB API public function header */
#include "hw_resource.h"


#include "r_usb_pcdc_driver.h"
#include "r_usb_pcdc_define.h"
#include "r_usb_pcdc_api.h"
#include "lcd.h"


/******************************************************************************
Macro definitions
******************************************************************************/
/* There is no macro definition. */


/******************************************************************************
Typedef definitions
******************************************************************************/
/* There is no typedef definition. */


/******************************************************************************
Private global variables and functions
******************************************************************************/
/* variables */



/******************************************************************************
Exported global variables and functions (to be accessed by other files)
******************************************************************************/
/* variables */
volatile uint8_t    g_usb_suspend_flag;


/* functions */
void        main(void);
void		usb_smpl_set_suspend_flag(uint8_t data);
uint8_t		usb_smpl_get_suspend_flag(void);



/******************************************************************************
Imported global variables and functions (from other files)
******************************************************************************/
/* variables */

/* functions */
extern void		usb_cpu_target_init(void);
extern void		usb_cstd_task_start(void);


/******************************************************************************
Renesas PCDC Sample
******************************************************************************/

/******************************************************************************
Function Name:  main
Description:    Main Task
Arguments:      void
Return value:   void
******************************************************************************/
void main(void)
{
	usb_cpu_target_init();
	
#ifdef USB_PMOD_LCD_ENABLE
    /* Display instructions on the debug LCD */
    Display_LCD(0, "USB PCDC");
#endif

	usb_smpl_set_suspend_flag( USB_NO );
	
	R_usb_pstd_PcdChangeDeviceState( USB_DO_INITHWFUNCTION );
	
	R_usb_pstd_PcdOpen();
	
    usb_cstd_task_start();
}	/* eof main() */


/******************************************************************************
Function Name   : usb_smpl_set_suspend_flag
Description     : Set resume interruptt flag
Argument        : void
Return          : void
******************************************************************************/
void usb_smpl_set_suspend_flag(uint8_t data)
{
    g_usb_suspend_flag  = data;
}	/* eof usb_smpl_set_suspend_flag() */


/******************************************************************************
Function Name   : usb_smpl_get_suspend_flag
Description     : Get resume interruptt flag
Argument        : void
Return          : void
******************************************************************************/
uint8_t usb_smpl_get_suspend_flag(void)
{
    return  g_usb_suspend_flag;
}	/* eof usb_smpl_get_suspend_flag() */


/******************************************************************************
End  Of File
******************************************************************************/
