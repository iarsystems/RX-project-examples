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
* Copyright (C) 2016, 2017 Renesas Electronics Corporation. All rights reserved.
***********************************************************************************************************************/

/***********************************************************************************************************************
* File Name    : Config_PORT.c
* Version      : 1.5.0
* Device(s)    : R5F566TEAxFP
* Description  : This file implements device driver for Config_PORT.
* Creation Date: 2018-11-20
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
    /* Set PORT6 registers */
    PORT6.PODR.BYTE = _02_Pm1_OUTPUT_1 | _04_Pm2_OUTPUT_1 | _00_Pm3_OUTPUT_0;
    PORT6.ODR0.BYTE = _00_Pm1_CMOS_OUTPUT | _00_Pm2_CMOS_OUTPUT | _00_Pm3_CMOS_OUTPUT;
    PORT6.PMR.BYTE = _00_Pm1_PIN_GPIO | _00_Pm2_PIN_GPIO | _00_Pm3_PIN_GPIO;
    PORT6.PDR.BYTE = _02_Pm1_MODE_OUTPUT | _04_Pm2_MODE_OUTPUT | _08_Pm3_MODE_OUTPUT;

    /* Set PORT9 registers */
    PORT9.PODR.BYTE = _08_Pm3_OUTPUT_1 | _10_Pm4_OUTPUT_1 | _20_Pm5_OUTPUT_1;
    PORT9.ODR0.BYTE = _00_Pm3_CMOS_OUTPUT;
    PORT9.ODR1.BYTE = _00_Pm4_CMOS_OUTPUT | _00_Pm5_CMOS_OUTPUT;
    PORT9.DSCR.BYTE = _00_Pm3_HIDRV_OFF | _00_Pm4_HIDRV_OFF | _00_Pm5_HIDRV_OFF;
    PORT9.DSCR2.BYTE = _00_Pm3_LARGECUR_OFF | _00_Pm4_LARGECUR_OFF | _00_Pm5_LARGECUR_OFF;
    PORT9.PMR.BYTE = _00_Pm3_PIN_GPIO | _00_Pm4_PIN_GPIO | _00_Pm5_PIN_GPIO;
    PORT9.PDR.BYTE = _08_Pm3_MODE_OUTPUT | _10_Pm4_MODE_OUTPUT | _20_Pm5_MODE_OUTPUT;

    /* Set PORTA registers */
    PORTA.PODR.BYTE = _04_Pm2_OUTPUT_1;
    PORTA.ODR0.BYTE = _00_Pm2_CMOS_OUTPUT;
    PORTA.DSCR.BYTE = _00_Pm2_HIDRV_OFF;
    PORTA.PMR.BYTE = _00_Pm2_PIN_GPIO;
    PORTA.PDR.BYTE = _04_Pm2_MODE_OUTPUT | _C0_PDRA_DEFAULT;

    R_Config_PORT_Create_UserInit();
}

/* Start user code for adding. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */
