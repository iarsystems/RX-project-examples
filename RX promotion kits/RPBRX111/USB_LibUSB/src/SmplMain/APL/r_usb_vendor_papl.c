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
* http://www.renesas.com/disclaimer
*
* © 2012-2013 Renesas Electronics Corporation All rights reserved.
*******************************************************************************/
/******************************************
* File Name    : r_usb_vendor_papl.c
* Version      : 2.01
* Description  : USB Peripheral vendor sample application code.
*******************************************************************************/
/*******************************************************************************
* History   : DD.MM.YYYY Version Description
*           : 31.03.2012 1.80    Basic FW mini Ver2.00(ES Version)
*           : 30.11.2012 2.00    Basic FW mini Ver2.00 Release
*           : 31.01.2013 2.01    Corresponded to RL78/L1C
*           : 08.05.2013         For LibUSB.
*******************************************************************************/

/******************************************************************************
Section definitions
******************************************************************************/
#ifdef  R8CUSB
#pragma section program P_smpl
#pragma section bss B_smpl
#pragma section data R_smpl
#pragma section rom C_smpl
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
#include "r_usb_usrconfig.h"            /* System definition, USB-FW Library Header */
#include "r_usb_usbip.h"                /* Definition of the USB register & bit define */
#include "r_usb_api.h"                  /* USB API public function header */
#include "r_usb_vendor_api.h"
#include "r_usb_vendor_apl.h"
#include "hw_resource.h"

/* LibUSB */
#include "string.h"
//#if DEBUG_CONSOLE
    #include <stdio.h>
//#endif
#include "r_s12ad_rx_if.h"

/******************************************************************************
Macro definitions
******************************************************************************/
/* LibUSB */
#define CLEAR_PIPES_BETWEEN_TRANSFERS    0       /* 0 to not clear pipes between transfers. Faster code. */
#define USB_LIBUSB_CMD_LED               0x01    /* Toggle LED */
#define USB_LIBUSB_CMD_ADC               0x02    /* Read ADC command */
#define USB_LIBUSB_CMD_LCD               0x04    /* Set LCD */
#define USB_LIBUSB_CMD_LAB_BULK          0x09    /* $REA LAB. 0x08 or 0x09 does not work w Python. */
/* ADC. UNCALIBRATED values for conversion of ADC counts to voltage. */
#define MAX_COUNTS  (float)4095.0
#define VREFH       (float)3.3
#define VREFL       (float)0.0

/******************************************************************************
Typedef definitions
******************************************************************************/
/* There is no typedef definition. */

/******************************************************************************
Private global variables and functions
******************************************************************************/
/* variables */
USB_STATIC usb_tskinfo_t    g_usb_psmpl_Message;                        /* Pcd Task recive message */

/* functions */
USB_STATIC void     usb_psmpl_keysequence(usb_tskinfo_t *mess);
USB_STATIC void     usb_psmpl_application(usb_utr_t *mess);
USB_STATIC void     usb_psmpl_driver_registration(void);
USB_STATIC void     usb_psmpl_device_state(uint16_t data, uint16_t state);
USB_STATIC void     usb_psmpl_control_transfer(usb_request_t *data1, uint16_t data2);
USB_STATIC void     usb_psmpl_transfer_start(usb_pipe_t Pipe);
USB_STATIC void     usb_psmpl_transfer_result(usb_utr_t *mess);
USB_STATIC void     usb_psmpl_tranfer_all(void);
USB_STATIC void     usb_psmpl_transfer_end_all(void);
/* LibUSB */
USB_STATIC void     libusb_cmd_exec(const char* host_command_string, int32_t data_len);

/******************************************************************************
Exported global variables and functions (to be accessed by other files)
******************************************************************************/
/* variables */
uint32_t    totalcount1;
uint32_t    totalcount2;

/* Functions */
void    usb_psmpl_main_init(void);
void    usb_psmpl_apl_task(void);
void    usb_psmpl_keyprocess(uint8_t keydata);

/* LibUSB */
void    display_ad_result(uint16_t adc_cnt);


/******************************************************************************
Imported global variables and functions (from other files)
******************************************************************************/
/* Variables */
extern uint8_t      g_usb_psmpl_DeviceDescriptor[];
extern uint8_t      g_usb_psmpl_ConfigurationF_1[];
extern uint8_t      *g_usb_psmpl_StringPtr[];
extern uint16_t     g_usb_psmpl_EpTbl1[];
extern usb_utr_t    g_usb_SmplTrnMsg[];
extern usb_leng_t   g_usb_SmplTrnSize[];
extern uint16_t     g_usb_SmplTrnCnt[];
extern uint8_t      *g_usb_SmplTrnPtr[];

/* Functions */
extern void         usb_smpl_data_init(void);
extern void         usb_smpl_counter_init(void);
extern void         usb_smpl_lcd_pipe_count(usb_pipe_t pipe);
extern void         usb_smpl_lcd_numeric(uint8_t pos, uint8_t data);
extern void         usb_smpl_lcd_string(uint8_t pos, uint8_t *str);
extern void         usb_smpl_set_suspend_flag(uint8_t data);
extern void         usb_cpu_deep_sleep_mode(void);
extern void         usb_cpu_soft_stanby_mode(void);
/* LibUSB */
extern void         usb_smpl_led_position(uint8_t ledpos, uint8_t data);

/******************************************************************************
Renesas Peripheral Sample Code functions
******************************************************************************/

/******************************************************************************
Function Name   : usb_psmpl_main_init
Description     : Initialize process
Arguments       : none
Return value    : none
******************************************************************************/
void usb_psmpl_main_init(void)
{
    /* Target board initialize */
    usb_cpu_target_init();

    /* USB-IP initialized */
    R_usb_pstd_PcdChangeDeviceState(USB_DO_INITHWFUNCTION);

    /* PCD driver open & registratuion */
    R_usb_pstd_PcdOpen();                    /* PCD task open */
    usb_psmpl_driver_registration();         /* Sample driver registration */

    /* Scheduler initialized */
    R_usb_pstd_PcdChangeDeviceState(USB_DO_SETHWFUNCTION);

}/* eof usb_psmpl_main_init() */


/******************************************************************************
Function Name   : usb_psmpl_apl_task
Description     : Main Task
Arguments       : none
Return value    : none
******************************************************************************/
void usb_psmpl_apl_task(void)
{
    usb_tskinfo_t   *mess;

    /* Receive message */
    if (R_USB_RCV_MSG(USB_PSMP_MBX, (usb_msg_t**)&mess) != USB_E_OK)
    {
        return;
    }

    switch( mess->msginfo )
    {
        case USB_SMPL_TASK:
            usb_psmpl_application((usb_utr_t *)mess);
        break;
        case USB_SMPL_KEYPROCESS:
            /* Only SW1 interrupt for original LibUSB Demo. */
            #if USB_KEY_ENABLE
            usb_psmpl_keysequence((usb_tskinfo_t*)mess);
            #endif
        break;
        default:
        break;
    }
}/* eof usb_psmpl_apl_task() */


/******************************************************************************
Function Name   : usb_psmpl_keyprocess
Description     : Host Sample Key data processing
Arguments       : none
Return value    : none
******************************************************************************/
void usb_psmpl_keyprocess(uint8_t keydata)
{
    g_usb_psmpl_Message.msginfo = USB_SMPL_KEYPROCESS;
    g_usb_psmpl_Message.keyword = (usb_strct_t)keydata;
    if (R_USB_SND_MSG(USB_PSMP_MBX, (usb_msg_t*)&g_usb_psmpl_Message) != USB_E_OK)
    {
        USB_DEBUG_HOOK(USB_DEBUG_HOOK_APL | USB_DEBUG_HOOK_CODE9);
    }
}/* eof usb_psmpl_keyprocess() */


/******************************************************************************
Function Name   : usb_psmpl_keysequence
Description     : Peri Sample Key data processing
Arguments       : none
Return value    : none
******************************************************************************/
USB_STATIC void usb_psmpl_keysequence(usb_tskinfo_t *mess)
{
    uint16_t table[4];

    /* Manager Mode Change */
    switch(mess->keyword)
    {
        case 0:
        break;
        case 1:     /* Remote wakeup */
            R_usb_pstd_DeviceInformation(table);
            if (table[3] == USB_YES)
            {
                R_usb_pstd_PcdChangeDeviceState(USB_DO_REMOTEWAKEUP);
                usb_smpl_lcd_string(LCD_POS_U0, (uint8_t *)"WAKEUP  ");
            }
        break;
        case 2:     /* Port disable */
            R_usb_pstd_PcdChangeDeviceState(USB_DO_PORT_DISABLE);
            usb_smpl_lcd_string(LCD_POS_U0, (uint8_t *)"PORT OFF");
        break;
        case 3:     /* Port enable */
            R_usb_pstd_PcdChangeDeviceState(USB_DO_PORT_ENABLE);
            usb_smpl_lcd_string(LCD_POS_U0, (uint8_t *)"PORT ON ");
        break;
        case 4:     /* Forced ending */
            usb_psmpl_transfer_end_all();
            //usb_smpl_lcd_string(LCD_POS_U0, (uint8_t *)"TransEnd");
        break;

        default:
            USB_DEBUG_HOOK(USB_DEBUG_HOOK_APL | USB_DEBUG_HOOK_CODE10);
        break;
    }
}/* eof usb_psmpl_keysequence() */

/******************************************************************************
Function Name   : usb_psmpl_application
Description     : Sample Main Task Initialize
Arguments       : usb_utr_t *mess           : Utr Pointer
Return value    : none
******************************************************************************/
USB_STATIC void usb_psmpl_application(usb_utr_t *mess)      /* nonOS */
{
    /* LibUSB */
    uint8_t*    host_command_string;
    usb_leng_t  data_len;

    #if CLEAR_PIPES_BETWEEN_TRANSFERS
    uint8_t     j;
    #endif

    switch( mess->status )
    {
        case USB_DATA_OK:               /* IN (data receive), OUT (data transmission) */
        case USB_DATA_SHT:              /* IN (data receive) */
        case USB_DATA_OVR:              /* IN (data receive) */
            if (g_usb_SmplTrnCnt[mess->pipenum]++ == 0xFFFF)
            {
                g_usb_SmplTrnCnt[mess->pipenum] = USB_0;
            }

            /****************
             * Added LibUSB *
             ***************/
            /* Bulk Pipe 4 <=> EP1 (OUT),
             * Bulk Pipe 5 <=> EP2 (IN),
             * Interrupt Pipe 6 <=> EP3 (OUT),
             * Interrupt Pipe 7 <=> EP4 (IN). */

            /* How long is the message? mess->tranadr is the remaining unused bytes in buffer,
             * meaning data length is: USB_SMPL_TRNSIZE - mess->tranlen. */
            data_len = g_usb_SmplTrnSize[mess->pipenum] - mess->tranlen;

            if (mess->pipenum == USB_PIPE4)
            {
                /* Read the LibUSB demo command string from host. */
                host_command_string = mess->tranadr;
                libusb_cmd_exec((const char*) host_command_string, data_len);

                /* Set pipe ready to receive next incoming command. */
                usb_psmpl_transfer_start( mess->pipenum );
            }

            else if (mess->pipenum == USB_PIPE6)
            {
                /* Interrupt IN data was sent from RSK111 to the host, because the
                 * user pressed SW1 on the board. Reenable interrupts, so the user
                 * can do it again. Interrupts are enabled and disabled to electrically
                 * debounce the switch. */
                usb_cpu_irq4_interrupt_enable();
            }

            #if CLEAR_PIPES_BETWEEN_TRANSFERS
                /* Clear pipe for next round. */
                for( j = 0; j < g_usb_SmplTrnSize[mess->pipenum]; j++ )
                {
                    g_usb_SmplTrnPtr[mess->pipenum][j] = 0u;
                }
            #endif

        break;
        case USB_DATA_STALL:        /* STALL */
            /* Clear STALL is necessary. */
        break;
        case USB_DATA_ERR:          /* transfer error */
        break;
        case USB_DATA_DTCH:         /* detach */
        break;
        case USB_DATA_STOP:
            /* data transfer end */
        break;
        default:
            USB_DEBUG_HOOK(USB_DEBUG_HOOK_APL | USB_DEBUG_HOOK_CODE11);
        break;
    }
}/* eof usb_psmpl_application() */

/******************************************************************************
Function Name   : usb_psmpl_driver_registration
Description     : Peripheral Sample Driver Registration
Arguments       : none
Return value    : none
******************************************************************************/
/* eof usb_psmpl_driver_registration() */

/******************************************************************************
Function Name   : usb_psmpl_device_state
Description     : Device State CallBack Check
Arguments       : uint16_t data
                : uint16_t state            : Device state
Return value    : none
******************************************************************************/
USB_STATIC void usb_psmpl_driver_registration(void)
{
    usb_pcdreg_t    driver;

    /* Driver registration */
    driver.pipetbl      = g_usb_psmpl_EpTbl1;
    driver.devicetbl    = g_usb_psmpl_DeviceDescriptor;
    driver.configtbl    = g_usb_psmpl_ConfigurationF_1;
    driver.stringtbl    = g_usb_psmpl_StringPtr;
    driver.statediagram = &usb_psmpl_device_state;
    driver.ctrltrans    = &usb_psmpl_control_transfer;
    R_usb_pstd_DriverRegistration(&driver);
    R_usb_pvndr_DriverStart();
}
USB_STATIC void usb_psmpl_device_state(uint16_t data, uint16_t state)
{
//    uint16_t        info;
//    uint16_t        information[4];

    R_usb_pvndr_StateCallback(data, state);
    switch( state )
    {
        case USB_STS_DETACH:
            usb_psmpl_transfer_end_all();
            usb_smpl_set_suspend_flag(USB_NO);
            usb_smpl_lcd_string(LCD_POS_U0, (uint8_t *)"DETACH P");
#ifdef USB_STOP_MODE_ENABLE
  #ifdef __RX
            usb_cpu_soft_stanby_mode();
  #endif  /* __RX */
#endif /* USB_STOP_MODE_ENABLE */
        break;
        case USB_STS_DEFAULT:
            usb_psmpl_transfer_end_all();
            usb_smpl_set_suspend_flag(USB_NO);
            usb_smpl_lcd_string(LCD_POS_U0, (uint8_t *)"DEFAULT ");
            usb_smpl_lcd_string(LCD_POS_D0, (uint8_t *)"        ");
        break;
        case USB_STS_ADDRESS:
        break;
        case USB_STS_CONFIGURED:
            totalcount1 = 0;
            totalcount2 = 0;
            usb_smpl_data_init();
            //usb_psmpl_tranfer_all();    //LibUSB: Don't send ADC (IN data) data to host until asked.
            usb_psmpl_transfer_start(USB_PIPE4);    //After configured, set up to receive LibUSB host commands.

            usb_smpl_set_suspend_flag(USB_NO);
            usb_smpl_lcd_string(LCD_POS_U0, (uint8_t *)"CONFIG. ");
            usb_smpl_lcd_string(LCD_POS_D0, (uint8_t *)"LibUSB! ");
        break;
        case USB_STS_SUSPEND:
            usb_smpl_set_suspend_flag(USB_YES);
            usb_smpl_lcd_string(LCD_POS_U0, (uint8_t *)"SUSPEND ");
        break;
        case USB_STS_RESUME:
            usb_smpl_lcd_string(LCD_POS_U0, (uint8_t *)"RESUME  ");
            usb_smpl_set_suspend_flag(USB_NO);
        break;
        default:
        break;
    }
}/* eof usb_psmpl_device_state() */

/******************************************************************************
Function Name   : usb_psmpl_control_transfer
Description     :
Arguments       : usb_request_t *data1
                : uint16_t      data2       : Not Use
Return value    : none
******************************************************************************/
USB_STATIC void usb_psmpl_control_transfer(usb_request_t *request, uint16_t data)
{
    R_usb_pvndr_ControlCallback(request, data);
}/* eof usb_psmpl_control_transfer() */

/******************************************************************************
Function Name   : usb_psmpl_transfer_start
Description     : Data Transfer
Arguments       : uint16_t pipe             : Pipe No
Return value    : none
******************************************************************************/
USB_STATIC void usb_psmpl_transfer_start(usb_pipe_t pipe)
{
    /* PIPE Transfer set */
    g_usb_SmplTrnMsg[pipe].pipenum  = pipe;
    g_usb_SmplTrnMsg[pipe].tranadr  = g_usb_SmplTrnPtr[pipe];
    g_usb_SmplTrnMsg[pipe].tranlen  = g_usb_SmplTrnSize[pipe];
    g_usb_SmplTrnMsg[pipe].setup    = (uint16_t*)USB_NULL;
    g_usb_SmplTrnMsg[pipe].complete = (usb_cb_t)&usb_psmpl_transfer_result;
    R_usb_pvndr_TransferStart((usb_utr_t*)&g_usb_SmplTrnMsg[pipe]);
    usb_smpl_lcd_pipe_count(pipe);
}/* eof usb_psmpl_transfer_start() */

/******************************************************************************
Function Name   : usb_psmpl_transfer_result
Description     : Data Transfer Callback Function
Arguments       : usb_utr_t *mess           : Utr Pointer
Return value    : none
******************************************************************************/
USB_STATIC void usb_psmpl_transfer_result(usb_utr_t *mess)
{
    usb_er_t    err;

    mess->msginfo = USB_SMPL_TASK;

    err = R_USB_SND_MSG(USB_PSMP_MBX, (usb_msg_t*)mess);
    if (err != USB_E_OK)
    {
        USB_DEBUG_HOOK(USB_DEBUG_HOOK_APL | USB_DEBUG_HOOK_CODE12);
    }
}/* eof usb_psmpl_transfer_result() */

/******************************************************************************
Function Name   : usb_psmpl_tranfer_all
Description     : User Data Trasfer Start
Arguments       : none
Return value    : none
******************************************************************************/
USB_STATIC void usb_psmpl_tranfer_all(void)
{
#ifndef  USB_LSPERI_PP
    usb_psmpl_transfer_start(USB_PIPE4);
    usb_psmpl_transfer_start(USB_PIPE5);
#endif  /* USB_LSPERI_PP */
    usb_psmpl_transfer_start(USB_PIPE6);
    usb_psmpl_transfer_start(USB_PIPE7);
}/* eof usb_psmpl_tranfer_all() */


/******************************************************************************
Function Name   : usb_psmpl_transfer_end_all
Description     : ALL pipe transfer end
Arguments       : none
Return value    : none
******************************************************************************/
USB_STATIC void usb_psmpl_transfer_end_all(void)
{
#ifndef  USB_LSPERI_PP
    R_usb_pvndr_TransferStop(USB_PIPE4);
    R_usb_pvndr_TransferStop(USB_PIPE5);
#endif  /* USB_LSPERI_PP */
    R_usb_pvndr_TransferStop(USB_PIPE6);
    R_usb_pvndr_TransferStop(USB_PIPE7);
}/* eof usb_psmpl_transfer_end_all() */

/******************************************************************************


      ***  L i b U S B  ***


******************************************************************************/
/******************************************************************************
Function:       libusb_cmd_exec
Description:    LibUSB command check and execute.
Arguments:      host_command_string   : Pointer to demo command data from host.
                Nr bytes in command string total from host.
Return          -
******************************************************************************/
USB_STATIC void libusb_cmd_exec(const char* host_command_string, int32_t data_len)
{
    static uint8_t  j;
    uint8_t         lcd_string[LCD_MAXCHARA + 1];   //Add place for NULL for security
    adc_err_t       adc_retval;
    uint16_t        adc_data = 0;
    //char*           adc_string = NULL;
    uint8_t*        usb_write_buf_addr;

    #if DEBUG_CONSOLE
        /* _INIT_IOLIB() must be uncommented in resetprg.c for printf() to work with the Debug Console. */
        printf("host_command_string: ");
        for (i = 0; i < data_len; i++)
            printf("0x%X ", host_command_string[i]);
        printf("\n");
    #endif

    switch (host_command_string[0])    //Set BP here to catch commands from host.
    {
        case USB_LIBUSB_CMD_LED:
            /* Toggle the LEDs. */
            if (++j%2)
            {
                usb_smpl_led_position(LED_POS0, LED_ON);
                usb_smpl_led_position(LED_POS1, LED_ON);
                usb_smpl_led_position(LED_POS2, LED_ON);
                usb_smpl_led_position(LED_POS3, LED_ON);
            }
            else
            {
                usb_smpl_led_position(LED_POS0, LED_OFF);
                usb_smpl_led_position(LED_POS1, LED_OFF);
                usb_smpl_led_position(LED_POS2, LED_OFF);
                usb_smpl_led_position(LED_POS3, LED_OFF);
            }
            break;

        case USB_LIBUSB_CMD_LCD:
            /* Write to LCD. */
            /****************************************************************************************
            * Command string  =  command byte  +  lcd line nr  +  DISPLAY STRING (0-8 bytes)  +  NULL
            *****************************************************************************************/
            /* If line number (byte #1) too high, clear the display. */
            if (host_command_string[1] > 3)
            {
                usb_smpl_lcd_string(LCD_POS_U0, (uint8_t *)"        ");
                usb_smpl_lcd_string(LCD_POS_D0, (uint8_t *)"        ");
            }
            else
            {
                /* Copy only string length to new string, then send to display.
                 * String length is host_command_string - 3 bytes, see top comment.
                 * Make sure to end with null. */
                if ((data_len - 2) < LCD_MAXCHARA)
                {
                    strncpy((char*)lcd_string, &host_command_string[2], data_len - 2);

                    /* End with a null. */
                    lcd_string[data_len - 3] = 0;
                }
                else
                {
                    strncpy((char*)lcd_string, &host_command_string[2], LCD_MAXCHARA);

                    /* End with a null. */
                    lcd_string[LCD_MAXCHARA] = 0;
                }

                /* Write LCD. Skip first byte since it is the command nr. */
                usb_smpl_lcd_string(host_command_string[1]*8, (uint8_t *)lcd_string);
            }
            break;

        case USB_LIBUSB_CMD_ADC:
            /* Read A/D converter. */
            #if USB_ADC_ENABLE
            /* Read conversion result from a single channel, sensor, or the double trigger register. */
            /* Trigger a READ. */
            adc_retval = R_ADC_Control(ADC_CMD_SOFTWARE_TRIG_START_SCAN, NULL);
            if (ADC_SUCCESS != adc_retval)
                usb_smpl_lcd_string(LCD_POS_U0, (uint8_t *) "ADC err.");

            adc_retval = ADC_ERR_SCAN_NOT_DONE;
            while (adc_retval == ADC_ERR_SCAN_NOT_DONE)
            {
                adc_retval = R_ADC_Control(ADC_CMD_CHECK_SCAN_DONE, NULL);
            }

            adc_retval = R_ADC_Read(ADC_CH0_REG, &adc_data);

            if (ADC_SUCCESS == adc_retval)
            {
                usb_write_buf_addr = g_usb_SmplTrnPtr[USB_PIPE5];

                /* Acc. to appnote R01an0492, The first byte means: 0 = OK, 1 = NOT OK. */
                *usb_write_buf_addr = 0;

                /* Return AD data as 2-4 bytes starting at byte 1. */
                usb_write_buf_addr++;
                *usb_write_buf_addr = adc_data;
                //(uint16_t) *(++usb_write_buf_addr) = adc_data;

                /* Set the transmit size. */
                //g_usb_SmplTrnSize[USB_PIPE5];
            }

            /* Show ADC result. */
            /* Via debugger console. */
            printf("ADC = 0x%3X\n", adc_data);

            /* LCD. */
            #if 1   /* Fancy display using float on LCD. */
             display_ad_result(adc_data);
            #else   /* Short version. */
             sprintf(adc_string, "AD:0x%3X", adc_data);
             usb_smpl_lcd_string(LCD_POS_U0, (uint8_t *) adc_string);
            #endif


             /* Start transfer */
            usb_psmpl_transfer_start(USB_PIPE5);
            #endif
        break;

        default:
        break;
    }
}/* end libusb_cmd_exec() */

/******************************************************************************
Function:       adc_rsk111_init
Description:    Initialize ADC of RSK111 for reading via LibUSB host.
                ADPOT is on Port 4_0
                See Table 1.5 List of Pins and Pin Functions (64-Pin LQFP):

Arguments:      -
Return          -
******************************************************************************/
void adc_rsk111_init(void)
{
    adc_cfg_t         ad_config;
    adc_ch_cfg_t     ch_cfg;
    adc_err_t        adc_retval;

    /* INIT ADC HW */
    MPC.PWPR.BYTE = 0x00;       // unlock this reg
    MPC.PWPR.BYTE = 0x40;       // unlock PFS regs

    /* Init ADC channel AN000 */
    MPC.P40PFS.BYTE = 0x80;     // Pin Func Select P40 AN000
    PORT4.PDR.BIT.B0 = 0;       // set AN pin direction to input (dflt)
    PORT4.PMR.BIT.B0 = 1;       // set AN pin mode to peripheral

    MPC.PWPR.BYTE = 0;          // lock PFS regs
    MPC.PWPR.BYTE = 0x80;       // lock this reg


    /* ADC OPEN
     * Apply power to the A/D peripheral, set operational mode, set trigger source,
     * interrupt priority, and configurations common to all channels and sensors.
     * Optionally takes a callback function pointer for notifying the user at inter-
     * rupt level whenever a scan has completed. */

    /* Software to trigger reads. */
    ad_config.trigger = ADC_TRIG_SOFTWARE;

    /* Interrupt level 0-15. 0 denotes polling! */
    ad_config.priority = 0;

    /* The number of samples added. Addition functionality disabled with ADC_ADD_OFF. */
    ad_config.add_cnt = ADC_ADD_OFF;

    /* Right-align the .alignment .clearing and .conversion fields. */
    ad_config.alignment = ADC_ALIGN_RIGHT;

    ad_config.clearing = ADC_CLEAR_AFTER_READ;

    ad_config.conv_speed = ADC_CONVERT_NORM_SPEED;

    adc_retval = R_ADC_Open(ADC_MODE_SS_ONE_CH, &ad_config, NULL);
    if (ADC_SUCCESS != adc_retval)
        usb_smpl_lcd_string(LCD_POS_U0, (uint8_t *) "ADC err.");


    /* ADC Control
     * Provides commands for enabling channels and sensors, and for runtime operations.
     * These include enabling/disabling trigger sources and interrupts, initiating a
     * software trigger, and checking for scan completion. */
    ch_cfg.chan_mask = ADC_CH0_MASK;
    ch_cfg.add_mask = ADC_CH0_MASK;

    adc_retval = R_ADC_Control(ADC_CMD_ENABLE_CHANS, &ch_cfg);
    if (ADC_SUCCESS != adc_retval)
        usb_smpl_lcd_string(LCD_POS_U0, (uint8_t *) "ADC err.");

}/* end adc_rsk111_init() */

/*******************************************************************************
* Function name: display_ad_result
* Description  : Converts ADC counts to volts value and shows both on the LCD
* Arguments    : uint16_t adc_cnt: current reading from ADC
* Return value : none
*******************************************************************************/
void display_ad_result(uint16_t adc_cnt)
{
    /* For converting the reading to volts */
    float adc_volts;

    /* String to display */
    uint8_t result_string[9];

    /* Convert to a string */
    sprintf((char*)result_string, (const char*)"ADC=%4d", (const char*)adc_cnt);

    /* Make sure all strings end with NULL. Display string. */
    result_string[8] = 0;

    usb_smpl_lcd_string(LCD_POS_U0, result_string);

    /* Convert S12ADC counts to volts and display */
    adc_volts = ((VREFH - VREFL)*(float)adc_cnt)/MAX_COUNTS;

    /* Convert to a string */
    sprintf((char*)result_string, (const char*)"V=%4.2f  ", adc_volts);

    /* Make sure all strings end with NULL. Display string. */
    result_string[8] = 0;
    usb_smpl_lcd_string(LCD_POS_D0, result_string);
} /* end display_ad_result() */

/******************************************************************************
Function Name:  usb_vendor_int_in_handler
Description::   For interrupt part of USB lab. SW1/IRQ8 is used together with
                interrupt IN EP3 (Pipe 6).
Return:         -
******************************************************************************/
void usb_vendor_int_in_handler(void)
{
    static uint8_t  switch_1_pushes = 0;
    static uint32_t i = 0;
    uint8_t*        usb_write_buf_addr;

    /* To debounce the switch, we use callback logic to disable/enable interrupts.
    USB transmit callback USB_PSTD_INT_SEND_END will reenable the SW1 interrupt. */
    usb_cpu_irq4_interrupt_disable();

    /* Send demo data; incrementing numbers filling the pipe. */
    usb_write_buf_addr = g_usb_SmplTrnPtr[USB_PIPE6];
    for (i = 0;  i < g_usb_SmplTrnSize[USB_PIPE6];  i++)
    {
       /* Incrementing integer Lab data */
        *usb_write_buf_addr++ = (switch_1_pushes*g_usb_SmplTrnSize[USB_PIPE6] + i);
    }

    /* Set the transmit size. */
    //g_usb_SmplTrnSize[USB_PIPE6] = ;        //By default set to USB_SMPL_TRNSIZE.

    /* Start transfer */
    usb_psmpl_transfer_start(USB_PIPE6);

    /* Keep track of nr of user presses. */
    switch_1_pushes++;

}/* end usb_vendor_int_in_handler() */
/******************************************************************************
End Of File
******************************************************************************/
