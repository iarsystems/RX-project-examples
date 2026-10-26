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
* File Name    : r_sci_async_1ch_rx_config.c
* Description  : Configures this code
************************************************************************************************************************
* History : DD.MM.YYYY Version Description           
*         : 24.02.2012 1.00    First Release            
*         : 10.05.2012 1.10    Updated to be compliant with FIT Module Spec v0.7
*         : 13.02.2013 1.20    Renamed to r_sci_async_1ch_rx to not conflict with multi-channel r_sci_async_rx module.
*                              Updated to FIT v1.02 spec.
***********************************************************************************************************************/
#ifndef SCI_ASYNC_RX_CONFIG_HEADER_FILE
#define SCI_ASYNC_RX_CONFIG_HEADER_FILE

/***********************************************************************************************************************
Includes   <System Includes> , "Project Includes"
***********************************************************************************************************************/
/* Used for determining board being used. */
#include "platform.h"

/***********************************************************************************************************************
Configuration Options
***********************************************************************************************************************/
/* Which SCI Channel to use. If you are using a RSK or RDK then the default SCI channel that is connected to the 
   RS232 transceiver is chosen by default. */
#if   defined(BSP_BOARD_RSKRX62N) || defined(BSP_BOARD_RDKRX62N) || defined(BSP_BOARD_RDKRX63N)
#define SCIA_CFG_CHANNEL            2
#elif defined(BSP_BOARD_RSKRX210) || defined(BSP_BOARD_RSKRX63N)
#define SCIA_CFG_CHANNEL            0
#else
#error "Please choose a SCI channel in r_sci_async_1ch_rx_config.h!"
#endif

/* SCI receive interrupts are used. Choose the interrupt priority level to be assigned to these interrupts. */
#define SCIA_CFG_INT_PRIORITY       (6)

/* Function to call when data is received. The prototype should be void *Function_Name* (uint8_t data); because this
   code will pass the user the data that was received. */
#define SCIA_CFG_CALLBACK            my_sci_callback  

/* The bit rate use for communications. */
#define SCIA_CFG_BIT_RATE           (115200)

#endif /* SCI_ASYNC_RX_CONFIG_HEADER_FILE */


