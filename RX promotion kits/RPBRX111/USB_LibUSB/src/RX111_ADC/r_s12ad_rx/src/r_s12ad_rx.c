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
* Copyright (C) 2013 Renesas Electronics Corporation. All rights reserved.
***********************************************************************************************************************/
/***********************************************************************************************************************
* File Name    : r_s12ad_rx.c
* Description  : Primary source code for 12-bit A/D Converter driver.
***********************************************************************************************************************/
/**********************************************************************************************************************
* History : DD.MM.YYYY Version Description
***********************************************************************************************************************/

/***********************************************************************************************************************
Includes   <System Includes> , "Project Includes"
***********************************************************************************************************************/
/* Includes board and MCU related header files. */
#include "platform.h"
/* Public interface header file for this package. */
#include "r_s12ad_rx_if.h"
/* Configuration for this package. */
#include "r_s12ad_rx_config.h"
/* Private header file for this package. */
#include "r_s12ad_rx_private.h"
/* LibUSB */
#include "hw_resource.h"

#include <intrinsics.h>

#if USB_ADC_ENABLE

/***********************************************************************************************************************
Macro definitions
***********************************************************************************************************************/
#define NULL    0

/***********************************************************************************************************************
Typedef definitions
***********************************************************************************************************************/

/***********************************************************************************************************************
Private global variables and functions
***********************************************************************************************************************/
adc_err_t adc_enable_chans(adc_ch_cfg_t *p_config);
void adc_enable_s12adi0(void);

/* In RAM */
static adc_ctrl_t g_dcb = { ADC_MODE_MAX, false, FIT_NO_FUNC};

/* In ROM */
// TODO: check this out.
//volatile __evenaccess uint16_t * const  dreg_ptrs[ADC_MAX_NUM_REGS] =
volatile uint16_t * const  dreg_ptrs[ADC_MAX_NUM_REGS] =
                      { (uint16_t  *)&S12AD.ADDR0,
                        (uint16_t  *)&S12AD.ADDR1,
                        (uint16_t  *)&S12AD.ADDR2,
                        (uint16_t  *)&S12AD.ADDR3,
                        (uint16_t  *)&S12AD.ADDR4,
                        NULL,
                        (uint16_t *)&S12AD.ADDR6,
                        NULL,
                        (uint16_t *)&S12AD.ADDR8,
                        (uint16_t *)&S12AD.ADDR9,
                        (uint16_t *)&S12AD.ADDR10,
                        (uint16_t *)&S12AD.ADDR11,
                        (uint16_t *)&S12AD.ADDR12,
                        (uint16_t *)&S12AD.ADDR13,
                        (uint16_t *)&S12AD.ADDR14,
                        (uint16_t *)&S12AD.ADDR15,
                        (uint16_t *)&S12AD.ADTSDR,      // temperature
                        (uint16_t *)&S12AD.ADOCDR,      // voltage
                        (uint16_t *)&S12AD.ADDBLDR      // double trigger
                      };
/* In ROM */
// TODO: checl this out.
//volatile __evenaccess uint8_t * const  sreg_ptrs[ADC_SST_NUM_REGS] =
volatile uint8_t * const  sreg_ptrs[ADC_SST_NUM_REGS] =
                      { (uint8_t *)&S12AD.ADSSTR0,
                        (uint8_t *)&S12AD.ADSSTR1,
                        (uint8_t *)&S12AD.ADSSTR2,
                        (uint8_t *)&S12AD.ADSSTR3,
                        (uint8_t *)&S12AD.ADSSTR4,
                        (uint8_t *)&S12AD.ADSSTR6,
                        (uint8_t *)&S12AD.ADSSTRL,     // 8-15
                        (uint8_t *)&S12AD.ADSSTRT,     // temperature
                        (uint8_t *)&S12AD.ADSSTRO      // voltage
                      };


/******************************************************************************
* Function Name: R_ADC_Open
* Description  : This function applies power to the A/D peripheral, sets the
*                operational mode, trigger sources, interrupt priority, and
*                configurations common to all channels and sensors. If interrupt
*                priority is non-zero, the function takes a callback function
*                pointer for notifying the user at interrupt level whenever a
*                scan has completed.
* Arguments    : mode-
*                    Operational mode (see enumeration below)
*                p_cfg-
*                    Pointer to configuration structure (see below)
*                p_callback-
*                    Optional pointer to function called from interrupt when
*                    a scan completes
* Return Value : ADC_SUCCESS-
*                    Successful
*                ADC_ERR_AD_LOCKED-
*                    Open() call is in progress elsewhere
*                ADC_ERR_AD_NOT_CLOSED-
*                    Peripheral is still running in another mode; Perform
*                    R_ADC_Close() first
*                ADC_ERR_INVALID_ARG-
*                    mode or element of p_cfg structure has invalid value.
*                ADC_ERR_ILLEGAL_ARG-
*                    an argument is illegal based upon mode
*                ADC_ERR_MISSING_PTR-
*                    p_cfg pointer is FIT_NO_PTR/NULL
*******************************************************************************/
adc_err_t R_ADC_Open(adc_mode_t const       mode,
                     adc_cfg_t * const      p_cfg,
                     void         (* const  p_callback)(void *p_args))
{

#if ADC_CFG_PARAM_CHECKING_ENABLE == 1
    if ((p_cfg == NULL) || (p_cfg == (adc_cfg_t *)FIT_NO_PTR))
    {
        return ADC_ERR_MISSING_PTR;
    }

    /* Check for valid argument values */
    if ((mode >= ADC_MODE_MAX)
     || ((p_cfg->trigger >= ADC_TRIG_HW_MAX) && (p_cfg->trigger != ADC_TRIG_SOFTWARE))
     || (p_cfg->priority > BSP_MCU_IPL_MAX)
     || (p_cfg->add_cnt >= ADC_ADD_MAX)
     || ((p_cfg->clearing != ADC_NO_CLEAR_AFTER_READ) && (p_cfg->clearing != ADC_CLEAR_AFTER_READ))
     || ((p_cfg->conv_speed != ADC_CONVERT_NORM_SPEED) && (p_cfg->conv_speed != ADC_CONVERT_HI_SPEED)))
    {
        return ADC_ERR_INVALID_ARG;
    }

    /* If interrupt driven, must have callback function */
    if ((p_cfg->priority != 0)
     && ((p_callback == NULL) || (p_callback == FIT_NO_FUNC)))
    {
        return ADC_ERR_ILLEGAL_ARG;
    }

    if (p_cfg->add_cnt == ADC_ADD_OFF)
    {
        /* Check alignment values only if addition is off */
        if ((p_cfg->alignment != ADC_ALIGN_LEFT) && (p_cfg->alignment != ADC_ALIGN_RIGHT))
        {
            return ADC_ERR_INVALID_ARG;
        }
    }
    else // addition on
    {
        /* Addition not allowed with temperature sensor */
        if (mode == ADC_MODE_SS_TEMPERATURE)
        {
            return ADC_ERR_ILLEGAL_ARG;
        }
    }

    /* In double trigger mode, SW and async triggers not allowed */
    if ((mode == ADC_MODE_SS_ONE_CH_DBLTRIG)
     && ((p_cfg->trigger == ADC_TRIG_SOFTWARE) || (p_cfg->trigger == ADC_TRIG_ASYNC_ADTRG0)))
    {
        return ADC_ERR_ILLEGAL_ARG;
    }

    /* Group checking; only synchronous triggers allowed; must be unique */
    if ((mode == ADC_MODE_SS_MULTI_CH_GROUPED) || (mode == ADC_MODE_SS_MULTI_CH_GROUPED_DBLTRIG_A))
    {
        if ((p_cfg->trigger == ADC_TRIG_ASYNC_ADTRG0)
         || (p_cfg->trigger == ADC_TRIG_SOFTWARE)
         || (p_cfg->trigger == p_cfg->groupb_trigger)
         || (p_cfg->groupb_trigger == ADC_TRIG_ASYNC_ADTRG0)
         || (p_cfg->groupb_trigger == ADC_TRIG_SOFTWARE))
        {
            return ADC_ERR_ILLEGAL_ARG;
        }

        if ((p_cfg->groupb_priority > BSP_MCU_IPL_MAX)
         || (p_cfg->trigger >= ADC_TRIG_HW_MAX)
         || (p_cfg->groupb_trigger >= ADC_TRIG_HW_MAX))
        {
            return ADC_ERR_INVALID_ARG;
        }

        if ((p_cfg->groupb_priority != 0)   // interrupt driven; must have callback func
         && ((p_callback == NULL) || (p_callback == FIT_NO_FUNC)))
        {
            return ADC_ERR_ILLEGAL_ARG;
        }
    }
#endif
    if (g_dcb.opened == true)
    {
        return ADC_ERR_AD_NOT_CLOSED;
    }
    if (R_BSP_HardwareLock(BSP_LOCK_S12AD) == false)
    {
        return ADC_ERR_AD_LOCKED;
    }


    /* APPLY POWER TO PERIPHERAL */

    SYSTEM.PRCR.WORD = 0xA502;                      // unlock MSTP regs
    MSTP(S12AD) = 0;
    SYSTEM.PRCR.WORD = 0xA500;                      // lock MSTP regs
    // ADCSR, ADEXICR, ADANSA, ADANSB, ADCER = 0 at power on


    /* SET MODE RELATED REGISTER FIELDS */

    g_dcb.mode = mode;
    if ((mode == ADC_MODE_SS_MULTI_CH_GROUPED)
     || (mode == ADC_MODE_SS_MULTI_CH_GROUPED_DBLTRIG_A))
    {
        S12AD.ADCSR.BIT.ADCS = ADC_ADCS_GROUP_SCAN;
    }
    else
    {
        if ((mode == ADC_MODE_CONT_ONE_CH) || (mode == ADC_MODE_CONT_MULTI_CH))
        {
            S12AD.ADCSR.BIT.ADCS = ADC_ADCS_CONT_SCAN;
        }
        // other modes have ADCS=0
    }

    if ((mode == ADC_MODE_SS_ONE_CH_DBLTRIG)
     || (mode == ADC_MODE_SS_MULTI_CH_GROUPED_DBLTRIG_A))
    {
        S12AD.ADCSR.BIT.DBLE = 1;                   // enable double trigger
    }


    /* SET TRIGGER AND INTERRUPT PRIORITY REGISTER FIELDS */

    if (p_cfg->trigger != ADC_TRIG_SOFTWARE)
    {
        S12AD.ADSTRGR.BIT.TRSA = p_cfg->trigger;
    }
    if (p_cfg->trigger == ADC_TRIG_ASYNC_ADTRG0)
    {
        S12AD.ADCSR.BIT.EXTRG = 1;
    }
    if (S12AD.ADCSR.BIT.ADCS == ADC_ADCS_GROUP_SCAN)
    {
        S12AD.ADSTRGR.BIT.TRSB = p_cfg->groupb_trigger;
        IPR(S12AD,GBADI) = p_cfg->groupb_priority;
    }
    IPR(S12AD,S12ADI0) = p_cfg->priority;


    /* SET REGISTER FIELDS FOR REMAINING PARAMETERS */

    S12AD.ADADC.BIT.ADC = p_cfg->add_cnt;
    S12AD.ADCER.WORD = p_cfg->alignment | (adc_align_t) p_cfg->clearing;
    S12AD.ADCSR.WORD |= p_cfg->conv_speed;


    /* SAVE CALLBACK FUNCTION POINTER */

    g_dcb.callback = p_callback;


    /* MARK DRIVER AS OPENED */

    g_dcb.opened = true;
    R_BSP_HardwareUnlock(BSP_LOCK_S12AD);
    return ADC_SUCCESS;
}


/******************************************************************************
* Function Name: R_ADC_Control
* Description  : This function provides commands for enabling channels and
*                sensors and for runtime operations. These include enabling/
*                disabling trigger sources and interrupts, initiating a
*                software trigger, and checking for scan completion.
*
* NOTE: Enabling a channel or a sensor, or setting the sample state count reg
*       cannot be done while the ADSCR.ADST bit is set (conversion in progress).
*       Because these commands should only be called once during initialization
*       before triggers are enabled, this should not be an issue. Registers
*       with this restriction include ADANSA, ADANSB, ADADS, ADADC, ADSSTR,
*       ADEXICR, and some bits in ADSCR.
*       No runtime operational sequence checking of any kind is performed.
*
* Arguments    : cmd-
*                    Command to run
*                p_cfg-
*                    Pointer to optional configuration structure
* Return Value : ADC_SUCCESS-
*                    Successful
*                ADC_ERR_MISSING_PTR-
*                    p_cfg pointer is FIT_NO_PTR/NULL when required as an argument
*                ADC_ERR_INVALID_ARG-
*                    cmd or element of p_cfg structure has invalid value.
*                ADC_ERR_ILLEGAL_CMD-
*                    cmd is illegal based upon mode
*                ADC_ERR_SCAN_NOT_DONE-
*                    The requested scan has not completed
*******************************************************************************/
adc_err_t R_ADC_Control(adc_cmd_t const     cmd,
                        void * const        p_cfg)
{
adc_err_t       err=ADC_SUCCESS;
adc_time_t      *p_sample;


    switch (cmd)
    {
    case ADC_CMD_ENABLE_CHANS:
        err = adc_enable_chans((adc_ch_cfg_t *) p_cfg);
        break;

    case ADC_CMD_ENABLE_TEMP_SENSOR:
#if ADC_CFG_PARAM_CHECKING_ENABLE == 1
        if (g_dcb.mode != ADC_MODE_SS_TEMPERATURE)
        {
            return ADC_ERR_ILLEGAL_ARG;
        }
#endif
        S12AD.ADEXICR.BIT.TSS = 1;                  // select temperature sensor
        adc_enable_s12adi0();
        break;


    case ADC_CMD_ENABLE_VOLT_SENSOR:
#if ADC_CFG_PARAM_CHECKING_ENABLE == 1
        if (g_dcb.mode != ADC_MODE_SS_INT_REF_VOLT)
        {
            return ADC_ERR_ILLEGAL_ARG;
        }
#endif
        S12AD.ADEXICR.BIT.OCS = 1;                  // select ref voltage sensor
        if (S12AD.ADADC.BIT.ADC != ADC_ADD_OFF)
        {
            S12AD.ADEXICR.BIT.OCSAD = 1;            // enable addition
        }
        adc_enable_s12adi0();
        break;


    case ADC_CMD_SET_SAMPLE_STATE_CNT:
        p_sample = (adc_time_t *) p_cfg;
#if ADC_CFG_PARAM_CHECKING_ENABLE == 1
        if ((p_sample->reg_id >= ADC_SST_NUM_REGS)
         || (p_sample->num_states < 7))
        {
            return ADC_ERR_INVALID_ARG;
        }
#endif
        *sreg_ptrs[p_sample->reg_id] = p_sample->num_states;
        break;


    case ADC_CMD_TRIG_ENABLE:
        S12AD.ADCSR.BIT.TRGE = 1;           // enable sync/async triggers
        break;

    case ADC_CMD_TRIG_DISABLE:
        S12AD.ADCSR.BIT.TRGE = 0;           // disable sync/async triggers
        break;

    case ADC_CMD_SOFTWARE_TRIG_START_SCAN:
        if (S12AD.ADCSR.BIT.ADST == 0)
        {
            S12AD.ADCSR.BIT.ADST = 1;
        }
        else
        {
            err = ADC_ERR_SCAN_NOT_DONE;
        }
        break;

    case ADC_CMD_INT_ENABLE:
        S12AD.ADCSR.BIT.ADIE = 1;           // enable S12ADI0 interrupt
        break;

    case ADC_CMD_INT_DISABLE:
        S12AD.ADCSR.BIT.ADIE = 0;           // disable S12ADI0 interrupt
        break;

    case ADC_CMD_INT_ENABLE_GROUPB:
        S12AD.ADCSR.BIT.GBADIE = 1;         // enable GBADI interrupt
        break;

    case ADC_CMD_INT_DISABLE_GROUPB:
        S12AD.ADCSR.BIT.GBADIE = 0;         // disable GBADI interrupt
        break;

    case ADC_CMD_CHECK_SCAN_DONE:           // default/Group A or Group B
        if (S12AD.ADCSR.BIT.ADST == 1)
        {
            err = ADC_ERR_SCAN_NOT_DONE;
        }
        break;

    case ADC_CMD_CHECK_SCAN_DONE_GROUPA:
        if (ICU.IR[IR_S12AD_S12ADI0].BIT.IR == 1)
        {
            ICU.IR[IR_S12AD_S12ADI0].BIT.IR = 0;
        }
        else
        {
            err = ADC_ERR_SCAN_NOT_DONE;
        }
        break;

    case ADC_CMD_CHECK_SCAN_DONE_GROUPB:
        if (ICU.IR[IR_S12AD_GBADI].BIT.IR == 1)
        {
            ICU.IR[IR_S12AD_GBADI].BIT.IR = 0;
        }
        else
        {
            err = ADC_ERR_SCAN_NOT_DONE;
        }
        break;
    }

    return err;
}


/******************************************************************************
* Function Name: adc_enable_chans
* Description  : This function does extensive checking on channel mask
*                settings based upon operational mode. Mask registers are
*                initialized and interrupts enabled in peripheral. Interrupts
*                are also enabled in ICU if corresponding priority is not 0.
* Arguments    : p_config
*                    Pointer to channel config structure containing masks
* Return Value : ADC_SUCCESS-
*                    Successful
*                ADC_ERR_INVALID_ARG-
*                    reg_id contains an invalid value.
*                ADC_ERR_ILLEGAL_ARG-
*                    an argument is illegal based upon mode
*******************************************************************************/
adc_err_t adc_enable_chans(adc_ch_cfg_t *p_config)
{
uint16_t    i=0,tmp=0;

#if ADC_CFG_PARAM_CHECKING_ENABLE == 1
    /* This command is illegal for sensor modes */
    if ((g_dcb.mode == ADC_MODE_SS_TEMPERATURE)
     || (g_dcb.mode == ADC_MODE_SS_INT_REF_VOLT))
    {
        return ADC_ERR_ILLEGAL_ARG;
    }

    /* Verify at least one bonded channel is selected */
    if ((p_config->chan_mask == 0)
     || ((p_config->chan_mask & ADC_INVALID_CH_MASK) != 0))
    {
        return ADC_ERR_INVALID_ARG;
    }

    /* Verify at least one unique bonded channel is selected for Group B */
    if (S12AD.ADCSR.BIT.ADCS == ADC_ADCS_GROUP_SCAN)
    {
        if ((p_config->groupb_mask == 0)
         || ((p_config->groupb_mask & ADC_INVALID_CH_MASK) != 0))
        {
            return ADC_ERR_INVALID_ARG;
        }
        else if ((p_config->chan_mask & p_config->groupb_mask) != 0)
        {
            return ADC_ERR_ILLEGAL_ARG;     // same chan in both groups
        }
        else
        {
            tmp = p_config->groupb_mask;
        }
    }
    else
    {
        p_config->groupb_mask = 0;          // for addition mask checking
    }

    /* Addition mask should not include bits from inactive channels */
    if (S12AD.ADADC.BIT.ADC != ADC_ADD_OFF)
    {
        tmp |= p_config->chan_mask;         // check Group A and B combined
        // Bit-AND with 1s-complement
        if ((p_config->add_mask & ~tmp) != 0)
        {
            return ADC_ERR_INVALID_ARG;
        }
    }

    /* Verify only 1 bit is set in default/Group A mask */
    if ((g_dcb.mode == ADC_MODE_SS_ONE_CH)
     || (g_dcb.mode == ADC_MODE_CONT_ONE_CH)
     || (S12AD.ADCSR.BIT.DBLE == 1))        // double trigger mode
    {
        tmp = p_config->chan_mask;
        // Bit-AND with 2s-complement
        if ((tmp & -tmp) != tmp)
        {
            return ADC_ERR_INVALID_ARG;
        }
    }
#endif

    /* SET MASKS FOR ALL CHANNELS */

    //S12AD.ADEXICR.WORD = 0;
    S12AD.ADANSA.WORD = p_config->chan_mask;
    if (S12AD.ADCSR.BIT.ADCS == ADC_ADCS_GROUP_SCAN)
    {
        S12AD.ADANSB.WORD = p_config->groupb_mask;
    }
    if (S12AD.ADADC.BIT.ADC != ADC_ADD_OFF)
    {
        S12AD.ADADS.WORD = p_config->add_mask;
    }


    /* SET DOUBLE TRIGGER CHANNEL */

    if (S12AD.ADCSR.BIT.DBLE == 1)
    {
        tmp = p_config->chan_mask;
        while (tmp >>= 1)                   // determine bit/ch number
        {
            i++;
        }
        S12AD.ADCSR.BIT.DBLANS = i;
    }


    /* ENABLE INTERRUPTS */

    adc_enable_s12adi0();
    if (S12AD.ADCSR.BIT.ADCS == ADC_ADCS_GROUP_SCAN)
    {
        IR(S12AD,GBADI) = 0;                // clear flag
        S12AD.ADCSR.BIT.GBADIE = 1;         // enable in peripheral
        if (ICU.IPR[IPR_S12AD_GBADI].BYTE != 0)
        {
            IEN(S12AD,GBADI) = 1;           // enable in ICU
        }
    }

    return ADC_SUCCESS;
}

/******************************************************************************
* Function Name: adc_enable_s12adi0
* Description  : This function clears the S12ADI0 interrupt flag and enables
*                interrupts in the peripheral (for IR flag usage). If priority
*                is not 0, interrupts are enabled in the ICU.
*                NOTE: This has nothing to do with enabling triggers.
* Arguments    : none
* Return Value : none
*******************************************************************************/
void adc_enable_s12adi0(void)
{

    IR(S12AD,S12ADI0) = 0;                  // clear flag
    S12AD.ADCSR.BIT.ADIE = 1;               // enable in peripheral
    if (ICU.IPR[IPR_S12AD_S12ADI0].BYTE != 0)
    {
        IEN(S12AD,S12ADI0) = 1;             // enable in ICU
    }
}

/*****************************************************************************
* Function Name: adc_s12adi0_isr
* Description  : Interrupt handler for normal/Group A/double trigger
*                scan complete.
* Arguments    : none
* Return Value : none
******************************************************************************/
#pragma vector=VECT_S12AD_S12ADI0
__interrupt static void adc_s12adi0_isr(void)
{
adc_cb_evt_t    event=ADC_EVT_SCAN_COMPLETE;

    // presence of callback function verified in Open()
    g_dcb.callback(&event);
}


/*****************************************************************************
* Function Name: adc_gbadi_isr
* Description  : Interrupt handler for Group B scan complete.
* Arguments    : none
* Return Value : none
******************************************************************************/
#pragma vector=VECT_S12AD_GBADI
__interrupt static void adc_gbadi_isr(void)
{
adc_cb_evt_t    event=ADC_EVT_GROUPB_SCAN_COMPLETE;

    // presence of callback function verified in Open()
    g_dcb.callback(&event);
}


/******************************************************************************
* Function Name: R_ADC_Read
* Description  : This function reads conversion results from a single channel,
*                sensor, or the double trigger register.
* Arguments    : reg_id-
*                    Id for the register to read (see enum below)
*                p_data-
*                    Pointer to variable to load value into.
* Return Value : ADC_SUCCESS-
*                    Successful
*                ADC_ERR_INVALID_ARG-
*                    reg_id contains an invalid value.
*                ADC_ERR_MISSING _PTR-
*                    p_data is FIT_NO_PTR/NULL
*******************************************************************************/
adc_err_t R_ADC_Read(adc_reg_t const    reg_id,
                     uint16_t * const   p_data)
{
#if ADC_CFG_PARAM_CHECKING_ENABLE == 1
    if ((p_data == NULL) || (p_data == (uint16_t *)FIT_NO_PTR))
    {
        return ADC_ERR_MISSING_PTR;
    }
    if ((reg_id >= ADC_MAX_NUM_REGS)
     || (dreg_ptrs[reg_id] == NULL))
    {
        return ADC_ERR_INVALID_ARG;
    }
    // Do not check if corresponding channel/sensor is enabled
#endif

    *p_data = *dreg_ptrs[reg_id];
    return ADC_SUCCESS;
}


/******************************************************************************
* Function Name: R_ADC_ReadaLL
* Description  : This function reads conversion results from all potential
*                sources, enabled or not.
* Arguments    : p_data-
*                    Pointer to structure to load register values into.
* Return Value : ADC_SUCCESS-
*                    Successful
*                ADC_ERR_MISSING _PTR-
*                    p_data is FIT_NO_PTR/NULL
*******************************************************************************/
adc_err_t R_ADC_ReadAll(adc_data_t * const  p_data)
{
#if ADC_CFG_PARAM_CHECKING_ENABLE == 1
    if ((p_data == NULL) || (p_data == (adc_data_t*)FIT_NO_PTR))
    {
        return ADC_ERR_MISSING_PTR;
    }
#endif

    p_data->chan[ADC_CH0_REG] = S12AD.ADDR0;
    p_data->chan[ADC_CH1_REG] = S12AD.ADDR1;
    p_data->chan[ADC_CH2_REG] = S12AD.ADDR2;
    p_data->chan[ADC_CH3_REG] = S12AD.ADDR3;
    p_data->chan[ADC_CH4_REG] = S12AD.ADDR4;
    p_data->chan[ADC_CH6_REG] = S12AD.ADDR6;
    p_data->chan[ADC_CH8_REG] = S12AD.ADDR8;
    p_data->chan[ADC_CH9_REG] = S12AD.ADDR9;
    p_data->chan[ADC_CH10_REG] = S12AD.ADDR10;
    p_data->chan[ADC_CH11_REG] = S12AD.ADDR11;
    p_data->chan[ADC_CH12_REG] = S12AD.ADDR12;
    p_data->chan[ADC_CH13_REG] = S12AD.ADDR13;
    p_data->chan[ADC_CH14_REG] = S12AD.ADDR14;
    p_data->chan[ADC_CH15_REG] = S12AD.ADDR15;
    p_data->dbltrig = S12AD.ADDBLDR;

    return ADC_SUCCESS;
}


/******************************************************************************
* Function Name: R_ADC_Close
* Description  : This function ends any scan in progress, disables interrupts,
*                and removes power to the A/D peripheral.
* Arguments    : none
* Return Value : none
*******************************************************************************/
void   R_ADC_Close(void)
{
#if 0
    S12AD.ADCSR.BIT.TRGE = 0;       // disable triggers
    S12AD.ADCSR.BIT.ADST = 0;       // stop conversion
    S12AD.ADCSR.BIT.ADIE = 0;       // disable S12ADI0 interrupt
    S12AD.ADCSR.BIT.GBADIE = 0;     // disable GBADI interrupt
#endif

    /* Stop triggers & conversions, and disable peripheral interrupts */
    S12AD.ADCSR.WORD = 0;
    IEN(S12AD,S12ADI0) = 0;         // disable interrupts in ICU
    IEN(S12AD,GBADI) = 0;           // disable interrupts in ICU

    /* Show driver as closed */
    g_dcb.opened = false;

    /* Power down peripheral */
    SYSTEM.PRCR.WORD = 0xA502;      // unlock MSTP regs
    MSTP(S12AD) = 1;
    SYSTEM.PRCR.WORD = 0xA500;      // lock MSTP regs

    return;
}


/*****************************************************************************
* Function Name: R_ADC_GetVersion
* Description  : Returns the version of this module. The version number is
*                encoded such that the top two bytes are the major version
*                number and the bottom two bytes are the minor version number.
* Arguments    : none
* Return Value : version number
******************************************************************************/
#pragma inline
uint32_t  R_ADC_GetVersion(void)
{
uint32_t const version = (ADC_VERSION_MAJOR << 16) | ADC_VERSION_MINOR;

    return version;
}
#endif
/* file end */
