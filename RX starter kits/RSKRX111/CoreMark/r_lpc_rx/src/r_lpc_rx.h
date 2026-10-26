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
* Copyright (C) 2012 Renesas Electronics Corporation. All rights reserved.    
***********************************************************************************************************************/
/***********************************************************************************************************************
* File Name	   : r_lpc_rx.h
* Description  :
***********************************************************************************************************************/
/***********************************************************************************************************************
* History : DD.MM.YYYY Version Description           
*         : 10.03.2012 1.00    First Release
***********************************************************************************************************************/

#ifndef LPC_PRIVATE_HEADER_FILE
#define LPC_PRIVATE_HEADER_FILE

/***********************************************************************************************************************
Macro definitions
***********************************************************************************************************************/
/* Tests to make sure the user configured the code correctly. */
#define TRANSITION_ONGOING 1
#define TRANSITION_COMPLETE 0

#define OSC_STOP_ENABLED 1

#define MAX_MIDDLE_SPEED_FREQ  8000000
#define MAX_LOW_SPEED_FREQ     32768

#define SUB_CLOCK 0x0300

#define CLOCK_SWITCH_ENABLE 0x80
#define CLOCK_SWITCH_DISABLE 0x00

#define CLOCK_SWITCH_MAIN_OSC_MAX_FREQ 8000000

#define LPC_CLOCK_INACTIVE    0x01
#define LPC_CLOCK_ACTIVE      0x00
/***********************************************************************************************************************
Typedef definitions
***********************************************************************************************************************/

/***********************************************************************************************************************
Exported global variables
***********************************************************************************************************************/

/***********************************************************************************************************************
Exported global functions (to be accessed by other files)
***********************************************************************************************************************/

#endif/* LPC_PRIVATE_HEADER_FILE */
