/***********************************************************************************************************************
* DISCLAIMER
* This software is supplied by Renesas Electronics Corporation and is only intended for use with Renesas products.
* No other uses are authorized. This software is owned by Renesas Electronics Corporation and is protected under all
* applicable laws, including copyright laws. 
* THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES REGARDING THIS SOFTWARE, WHETHER EXPRESS, IMPLIED
* OR STATUTORY, INCLUDING BUT NOT LIMITED TO WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
* NON-INFRINGEMENT.  ALL SUCH WARRANTIES ARE EXPRESSLY DISCLAIMED.TO THE MAXIMUM EXTENT PERMITTED NOT PROHIBITED BY
* LAW, NEITHER RENESAS ELECTRONICS CORPORATION NOR ANY OF ITS AFFILIATED COMPANIES SHALL BE LIABLE FOR ANY DIRECT,
* INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES FOR ANY REASON RELATED TO THIS SOFTWARE, EVEN IF RENESAS OR
* ITS AFFILIATES HAVE BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
* Renesas reserves the right, without notice, to make changes to this software and to discontinue the availability 
* of this software. By using this software, you agree to the additional terms and conditions found by accessing the 
* following link:
* http://www.renesas.com/disclaimer
*
* Copyright (C) 2022 Renesas Electronics Corporation. All rights reserved.
***********************************************************************************************************************/

/***********************************************************************************************************************
* File Name        : Config_PORT.c
* Component Version: 2.4.1
* Device(s)        : R5F56609HxFB
* Description      : This file implements device driver for Config_PORT.
***********************************************************************************************************************/

/***********************************************************************************************************************
Pragma directive
***********************************************************************************************************************/
/* Start user code for pragma. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */

/***********************************************************************************************************************
Includes
***********************************************************************************************************************/
#include "r_cg_macrodriver.h"
#include "Config_PORT.h"
/* Start user code for include. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */
#include "r_cg_userdefine.h"

/***********************************************************************************************************************
Global variables and functions
***********************************************************************************************************************/
/* Start user code for global. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */

/***********************************************************************************************************************
* Function Name: R_Config_PORT_Create
* Description  : This function initializes the PORT
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/

void R_Config_PORT_Create(void)
{
    /* Set PORT0 registers */
    PORT0.PODR.BYTE = _10_Pm4_OUTPUT_1 | _40_Pm6_OUTPUT_1;
    PORT0.ODR0.BYTE = _00_Pm0_CMOS_OUTPUT | _00_Pm1_CMOS_OUTPUT | _00_Pm2_CMOS_OUTPUT | _00_Pm3_CMOS_OUTPUT;
    PORT0.ODR1.BYTE = _00_Pm4_CMOS_OUTPUT | _00_Pm5_CMOS_OUTPUT | _00_Pm6_CMOS_OUTPUT | _00_Pm7_CMOS_OUTPUT;
    PORT0.DSCR.BYTE = _00_Pm0_HIDRV_OFF | _00_Pm1_HIDRV_OFF | _00_Pm2_HIDRV_OFF | _00_Pm4_HIDRV_OFF;
    PORT0.PMR.BYTE = _00_Pm4_PIN_GPIO | _00_Pm6_PIN_GPIO;
    PORT0.PDR.BYTE = _10_Pm4_MODE_OUTPUT | _40_Pm6_MODE_OUTPUT;

    /* Set PORT7 registers */
    PORT7.PODR.BYTE = _02_Pm1_OUTPUT_1 | _00_Pm2_OUTPUT_0;
    PORT7.ODR0.BYTE = _00_Pm0_CMOS_OUTPUT | _00_Pm1_CMOS_OUTPUT | _00_Pm2_CMOS_OUTPUT | _00_Pm3_CMOS_OUTPUT;
    PORT7.ODR1.BYTE = _00_Pm4_CMOS_OUTPUT | _00_Pm5_CMOS_OUTPUT | _00_Pm6_CMOS_OUTPUT | _00_Pm7_CMOS_OUTPUT;
    PORT7.DSCR.BYTE = _00_Pm0_HIDRV_OFF | _00_Pm1_HIDRV_OFF | _00_Pm2_HIDRV_OFF | _00_Pm3_HIDRV_OFF | 
                      _00_Pm4_HIDRV_OFF | _00_Pm5_HIDRV_OFF | _00_Pm6_HIDRV_OFF | _00_Pm7_HIDRV_OFF;
    PORT7.PMR.BYTE = _00_Pm1_PIN_GPIO | _00_Pm2_PIN_GPIO;
    PORT7.PDR.BYTE = _02_Pm1_MODE_OUTPUT | _04_Pm2_MODE_OUTPUT;

    /* Set PORTF registers */
    PORTF.PODR.BYTE = _20_Pm5_OUTPUT_1;
    PORTF.ODR1.BYTE = _00_Pm5_CMOS_OUTPUT | _00_Pm6_CMOS_OUTPUT | _00_Pm7_CMOS_OUTPUT;
    PORTF.DSCR.BYTE = _00_Pm5_HIDRV_OFF | _00_Pm6_HIDRV_OFF | _00_Pm7_HIDRV_OFF;
    PORTF.PMR.BYTE = _00_Pm5_PIN_GPIO;
    PORTF.PDR.BYTE = _20_Pm5_MODE_OUTPUT | _1F_PDRF_DEFAULT;

    /* Set PORTJ registers */
    PORTJ.PODR.BYTE = _08_Pm3_OUTPUT_1;
    PORTJ.ODR0.BYTE = _00_Pm1_CMOS_OUTPUT | _00_Pm3_CMOS_OUTPUT;
    PORTJ.ODR1.BYTE = _00_Pm4_CMOS_OUTPUT | _00_Pm5_CMOS_OUTPUT | _00_Pm6_CMOS_OUTPUT | _00_Pm7_CMOS_OUTPUT;
    PORTJ.DSCR.BYTE = _00_Pm1_HIDRV_OFF | _00_Pm3_HIDRV_OFF | _00_Pm4_HIDRV_OFF | _00_Pm5_HIDRV_OFF;
    PORTJ.PMR.BYTE = _00_Pm3_PIN_GPIO;
    PORTJ.PDR.BYTE = _08_Pm3_MODE_OUTPUT | _05_PDRJ_DEFAULT;

    /* Set PORTL registers */
    PORTL.PODR.BYTE = _01_Pm0_OUTPUT_1;
    PORTL.ODR0.BYTE = _00_Pm0_CMOS_OUTPUT | _00_Pm1_CMOS_OUTPUT;
    PORTL.DSCR.BYTE = _00_Pm0_HIDRV_OFF | _00_Pm1_HIDRV_OFF;
    PORTL.PMR.BYTE = _00_Pm0_PIN_GPIO;
    PORTL.PDR.BYTE = _01_Pm0_MODE_OUTPUT | _FC_PDRL_DEFAULT;

    R_Config_PORT_Create_UserInit();
}

/* Start user code for adding. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */
