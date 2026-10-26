/***********************************************************************************************************************
* DISCLAIMER
* This software is supplied by Renesas Electronics Corporation and is only intended for use with Renesas products. No
* other uses are authorized. This software is owned by Renesas Electronics Corporation and is protected under all
* applicable laws, including copyright laws.
* THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES REGARDING
* THIS SOFTWARE, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING BUT NOT LIMITED TO WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT. ALL SUCH WARRANTIES ARE EXPRESSLY DISCLAIMED. TO THE MAXIMUM
* EXTENT PERMITTED NOT PROHIBITED BY LAW, NEITHER RENESAS ELECTRONICS CORPORATION NOR ANY OF ITS AFFILIATED COMPANIES
* SHALL BE LIABLE FOR ANY DIRECT, INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES FOR ANY REASON RELATED TO THIS
* SOFTWARE, EVEN IF RENESAS OR ITS AFFILIATES HAVE BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
* Renesas reserves the right, without notice, to make changes to this software and to discontinue the availability of
* this software. By using this software, you agree to the additional terms and conditions found by accessing the
* following link:
* http://www.renesas.com/disclaimer
*
* Copyright (C) 2014(2015) Renesas Electronics Corporation. All rights reserved.
***********************************************************************************************************************/
/***********************************************************************************************************************
* File Name    : r_usb_PMSC_apl.c
* Description  : USB Peripheral Mass-Storage Sample Code
***********************************************************************************************************************/
/**********************************************************************************************************************
* History : DD.MM.YYYY Version Description
*         : 04.01.2014 1.00 First Release
*         : 30.01.2015 1.01    Added RX71M.
*         : 30.09.2015 1.11    Added RX63N.
***********************************************************************************************************************/

/******************************************************************************
Includes   <System Includes> , "Project Includes"
******************************************************************************/
#include "Global.h"
#include "LCD_demo.h"
#include "r_usb_pmsc_apl.h"
#include "r_flash_rx_if.h"

/******************************************************************************
Private global variables
******************************************************************************/
USB_UTR_t msc_utr;
xTaskHandle      USBPMSCHnd;     /* RTOS_USAGE */

static void R_USB_Scheduler(void *pvParameters);

/******************************************************************************
Renesas Peripheral Mass-Storage Sample Code functions
******************************************************************************/

/******************************************************************************
Function Name   : usb_main
Description     : Start task CSTD; "common" basic USB low level task.
Arguments       : USB_UTR_t *ptr      : 
Return value    : none
******************************************************************************/
void usb_main( void )
{
    usb_mcu_init();              /* USB pin function and port mode setting. */
    usb_board_init();            /* RSK I/O Initialisation process */
    usb_driver_init();           /* Start Peripheral USB driver */
    media_driver_init();         /* Initialisation Media driver */

    (void)xTaskCreate(R_USB_Scheduler,
                      "u:USBPMCS",
                      configMINIMAL_STACK_SIZE+256,
                      NULL,
                      mainDEMOTASKS_PRIORITY+2,
                      &USBPMSCHnd);    /* RTOS_USAGE */

}


/******************************************************************************
Function Name   : R_USB_Scheduler
Description     : Task created under FreeRTOS to repeatedly call the USB
                  scheduler
Argument        : none
Return          : none
******************************************************************************/
static void R_USB_Scheduler(void *pvParameters)
{
	for ( ; ; )
	{
		usb_msc_driver();
        vTaskDelay(1);    /* Quick sleep to allow other tasks in */
	}

}



/******************************************************************************
Function Name   : usb_msc_driver
Description     : USB Driver(Peripheral MSC)
Argument        : none
Return          : none
******************************************************************************/
void usb_msc_driver(void)
{
    R_usb_cstd_Scheduler();                         /* Scheduler */

    if( USB_FLGSET == R_usb_cstd_CheckSchedule() )
    {
        R_usb_pstd_PcdTask((USB_VP_INT_t)0);        /* USB Peripheral control driver Task */
        R_usb_pmsc_atapi_task((USB_VP_INT_t)0);     /* Execute ATAPI commands Process */
        R_usb_pmsc_task((USB_VP_INT_t)0);           /* Peripheral Mass Storage Class Task */
    }
}   /* eof usb_msc_driver() */


/******************************************************************************
Function Name   : msc_configured
Description     : Peripheral Mass Strage Class open function
Arguments       : USB_UTR_t *ptr            : IP Information(Mode,IP No.,Reg Address)
                : uint16_t data1            : Not use
                : uint16_t data2            : Not use
Return value    : none
******************************************************************************/
void msc_configured(USB_UTR_t *ptr, uint16_t data1, uint16_t data2)
{
    R_usb_pmsc_Open( ptr, data1, data2 );
}   /* eof cdc_configured() */

/******************************************************************************
Function Name   : msc_detach
Description     : Peripheral Mass Strage Class close function
Arguments       : USB_UTR_t *ptr            : IP Information(Mode,IP No.,Reg Address)
                : uint16_t data1            : Not use
                : uint16_t data2            : Not use
Return value    : none
******************************************************************************/
void msc_detach(USB_UTR_t *ptr, uint16_t data1, uint16_t data2)
{
    R_usb_pmsc_Close( ptr, data1, data2 );
}

/******************************************************************************
Function Name   : msc_suspended
Description     : Suspended state process
Arguments       : USB_UTR_t *ptr            : IP Information(Mode,IP No.,Reg Address)
                : uint16_t data1            : Not use
                : uint16_t data2            : Not use
Return value    : none
******************************************************************************/
void msc_suspended(USB_UTR_t *ptr, uint16_t data1, uint16_t data2)
{
	/* No action */
}



#if( USB_SPEED_MODE_PP == USB_HS_PP )
/******************************************************************************
Function Name   : msc_default
Description     : Descriptor change
Arguments       : uint16_t mode             : Speed mode
                : uint16_t data2            : Not use
Return value    : none
******************************************************************************/
void msc_default(USB_UTR_t *ptr, uint16_t mode, uint16_t data2)
{
    if( mode == USB_HSCONNECT )
    {
        /* Hi-Speed Mode */
        usb_gpmsc_HsConfigration[1]     = USB_DT_CONFIGURATION;
        usb_gpmsc_FsConfigration[1]       = USB_DT_OTHER_SPEED_CONF;
        /* HIGH */
        usb_gpmsc_ConPtr[0]             = usb_gpmsc_HsConfigration;
        /* FULL */
        usb_gpmsc_ConPtrOther[0]        = usb_gpmsc_FsConfigration;
        usb_gpmsc_EpTbl1[3]             = 512;
        usb_gpmsc_EpTbl1[USB_EPL + 3u]  = 512;
    }
    else
    {
        /* Full-Speed Mode */
        usb_gpmsc_FsConfigration[1]       = USB_DT_CONFIGURATION;
        usb_gpmsc_HsConfigration[1]     = USB_DT_OTHER_SPEED_CONF;
        /* FULL */
        usb_gpmsc_ConPtr[0]             = usb_gpmsc_FsConfigration;
        /* HIGH */
        usb_gpmsc_ConPtrOther[0]        = usb_gpmsc_HsConfigration;
        usb_gpmsc_EpTbl1[3]             = 64;
        usb_gpmsc_EpTbl1[USB_EPL + 3u]  = 64;
    }
}   /* eof msc_default() */
#endif  /* USB_SPEED_MODE_PP == USB_HS_PP */

/******************************************************************************
Function Name   : usb_mcu_init
Description     : USB port mode and Switch mode Initialize
Arguments       : none
Return value    : none
******************************************************************************/
void usb_mcu_init(void)
{
    R_BSP_RegisterProtectDisable(BSP_REG_PROTECT_LPC_CGC_SWR);
    R_BSP_RegisterProtectDisable(BSP_REG_PROTECT_MPC);

#if (USB_FUNCSEL_USBIP0_PP != USB_NOUSE_PP)
    PORT1.PMR.BIT.B6    = 1u;
    MPC.P16PFS.BYTE = 0x11; /* USB0_VBUS */
#endif /*  (USB_FUNCSEL_USBIP0_PP != USB_NOUSE_PP)  */

#if (USB_FUNCSEL_USBIP1_PP != USB_NOUSE_PP)
    PORT1.PMR.BIT.B1    = 1u;
    MPC.P11PFS.BYTE = 0x14; /* USBHS_VBUS */
#endif /*  (USB_FUNCSEL_USBIP1_PP != USB_NOUSE_PP) */

    R_BSP_RegisterProtectEnable(BSP_REG_PROTECT_MPC);
    R_BSP_RegisterProtectEnable(BSP_REG_PROTECT_LPC_CGC_SWR);
}   /* eof usb_mcu_init() */


/******************************************************************************
Function Name   : usb_board_init
Description     : RSK I/O Initialization process
Arguments       : none
Return value    : none
******************************************************************************/
void usb_board_init( void )
{
#ifdef USB_DTC_ENABLE
    dtc_err_t err;
#endif /* USB_DTC_ENABLE */


#ifdef USB_DTC_ENABLE
    err = R_DTC_Open();               /* Initializes the DTC module */
    if( DTC_SUCCESS != err )
    {
        USB_PRINTF0("DTC Err");
    }
#endif /* USB_DTC_ENABLE */

    usb_cpu_target_init();
}   /* eof usb_board_init() */


/******************************************************************************
Function Name   : usb_driver_init
Description     : Set peripheral application task start conditions. 
Arguments       : none
Return value    : none
******************************************************************************/
void usb_driver_init( void )
{
    USB_ER_t    err;

    msc_utr.ip = USB_PERI_USBIP_NUM;           /* IP number(0or1) */
    msc_utr.ipp = R_usb_cstd_GetUsbIpAdr( msc_utr.ip );

    usb_cstd_ScheInit();                        /* Scheduler init */

    pmsc_registration( &msc_utr );              /* Peripheral Application Registration */
    R_usb_pmsc_driver_start( &msc_utr );        /* Peripheral Class Driver Task Start Setting */
    R_usb_pstd_usbdriver_start( &msc_utr );     /* Peripheral USB Driver Start Setting */

#if USB_FUNCSEL_USBIP0_PP != USB_NOUSE_PP
    err = R_USB_Open( USB_IP0 );            /* Initializes the USB module */
#endif /* USB_FUNCSEL_USBIP0_PP != USB_NOUSE_PP */

#if USB_FUNCSEL_USBIP1_PP != USB_NOUSE_PP
    err = R_USB_Open( USB_IP1 );            /* Initializes the USB module */
#endif /*  USB_FUNCSEL_USBIP1_PP != USB_NOUSE_PP */
    if( USB_SUCCESS == err)
    {
        R_usb_cstd_UsbIpInit( &msc_utr, USB_PERI );     /* Initialize USB IP */
    }
}   /* eof usb_driver_init() */


/******************************************************************************
Function Name   : pmsc_registration
Description     : Registration of peripheral Mass Strage Class Driver
Arguments       : none
Return value    : none
******************************************************************************/
void pmsc_registration(USB_UTR_t *ptr)
{
    USB_PCDREG_t    driver;

    /* Driver registration */
    driver.pipetbl      = &usb_gpmsc_EpPtr[0];                      /* Pipe Define Table address */
    driver.devicetbl    = (uint8_t*)&usb_gpmsc_DeviceDescriptor;    /* Device descriptor Table address */
    driver.configtbl    = (uint8_t**)&usb_gpmsc_ConPtr;             /* Configuration descriptor Table address */
    driver.stringtbl    = (uint8_t**)&usb_gpmsc_StrPtr;             /* String descriptor Table address */
    driver.classinit    = &usb_cstd_DummyFunction;                  /* Driver init */
    driver.devconfig    = (USB_CB_t)&msc_configured;                /* Device configuered */
    driver.devdetach    = (USB_CB_t)&msc_detach;                    /* Device detach */
    driver.devsuspend   = (USB_CB_t)&msc_suspended;                 /* Device suspend */
    driver.devresume    = &usb_cstd_DummyFunction;                  /* Device resume */
    driver.interface    = &R_usb_pmsc_SetInterface;                 /* Interfaced change */
    driver.ctrltrans    = &R_usb_pmsc_UsrCtrlTransFunction;         /* Control Transfer */

#if( USB_SPEED_MODE_PP == USB_HS_PP )
    driver.qualitbl     = (uint8_t*)&usb_gpmsc_QualifierDescriptor; /* Qualifier descriptor Table address */
    driver.othertbl     = (uint8_t**)&usb_gpmsc_ConPtrOther;        /* Other configuration descriptor Table address */
    driver.devdefault   = &msc_default;                             /* Device default */
#else   /* USB_SPEED_MODE_PP == USB_HS_PP  */
    driver.qualitbl     = (uint8_t*)USB_NULL;                       /* Qualifier descriptor Table address */
    driver.othertbl     = (uint8_t**)USB_NULL;                      /* Other configuration descriptor Table address */
    driver.devdefault   = &usb_cstd_DummyFunction;                  /* Device default */
#endif  /* USB_SPEED_MODE_PP == USB_HS_PP */

    R_usb_pstd_DriverRegistration(ptr, &driver);
}   /* eof pmsc_registration() */

/******************************************************************************
Function Name   : media_driver_init
Description     : Initializeation Media driver
Arguments       : none
Return value    : none
******************************************************************************/
void media_driver_init( void )
{
    bool        ret_code;
    media_ret_t result;

    usb_pmsc_flashdisk_section_init();                    /* Initialize section */

    ret_code = R_MEDIA_Initialize(ATAPI_MEDIA_LUN, &g_dataflash_media_driver);    /* Register the media device driver. */
    if ( true != ret_code )
    {
        while(1);
    }

    result = R_MEDIA_Open(ATAPI_MEDIA_LUN);             /* Start up the  media device hardware. */
    if( MEDIA_RET_OK != result )
    {
        while(1);
    }
}   /* eof media_driver_init() */



/******************************************************************************
End  Of File
******************************************************************************/
