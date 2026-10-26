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
* File Name	   : r_sci_async_1ch_rx.c
* Description  : This module controls the SCI unit on a RX MCU. It only operates in asynchronous mode.
***********************************************************************************************************************/
/**********************************************************************************************************************
* History : DD.MM.YYYY Version Description           
*         : 24.02.2012 1.00    First Release            
*         : 10.05.2012 1.10    Updated to be compliant with FIT Module Spec v0.7
*         : 13.02.2013 1.20    Renamed to r_sci_async_1ch_rx to not conflict with multi-channel r_sci_async_rx module.
*                              Updated to FIT v1.02 spec.
***********************************************************************************************************************/

/***********************************************************************************************************************
Includes   <System Includes> , "Project Includes"
***********************************************************************************************************************/
/* Fixed-size integer typedefs. */
#include <stdint.h>
/* bool support. */
#include <stdbool.h>
/* Has intrinsic support. Includes xchg() which is used in this code. */
#include <machine.h>
/* Includes board and MCU related header files. */
#include "platform.h"
/* Configuration for this package. */
#include "r_sci_async_1ch_rx_config.h"
/* Header file for this package. */
#include "r_sci_async_1ch_rx_if.h"
/* Private header file for this code. */
#include "r_sci_async_1ch_rx.h"

/***********************************************************************************************************************
Macro definitions
***********************************************************************************************************************/
/* These macros eliminates the need for duplicated code to choose SCI channels. */
#define X_SCI( x )  XX_SCI( x )
#define XX_SCI( x ) _SCI##x
#define Y_SCI( x ) YY_SCI( x )
#define YY_SCI( x ) SCI##x
#define Z_SCI( x , y ) ZZ_SCI( x , ##y )
#define ZZ_SCI( x , y ) _SCI##x##_##y##x 
#define LOCK_SCI( x ) XLOCK_SCI( x )
#define XLOCK_SCI( x ) BSP_LOCK_SCI##x

/***********************************************************************************************************************
Typedef definitions
***********************************************************************************************************************/
typedef enum
{
    SCI_STATE_READY = 0,
    SCI_STATE_INIT,    
    SCI_STATE_TRANSMITTING
} sci_states_t;
 
/***********************************************************************************************************************
Private global variables and functions
***********************************************************************************************************************/
/* State variable for this package. */
static sci_states_t g_sci_state = SCI_STATE_READY;

/* This variable determines whether the peripheral has already been initialized. */
static bool sci_initialized = false;

/* Internal functions. */
static bool sci_acquire_state(sci_states_t new_state);
static void sci_release_state(void);

/***********************************************************************************************************************
* Function Name: R_SCIA_Init
* Description  : Initialize SCI channel for asynchronous operations.
* Arguments    : none
* Return Value : true - 
*                    Channel initialized successfully.
*                false -
*                    There was an error initializing the channel.
***********************************************************************************************************************/
bool R_SCIA_Init (void)
{
    /* Grab state to make sure we do not interfere with another operation. */
    if (sci_acquire_state(SCI_STATE_INIT) != true)
    {
        /* Another operation is already in progress */
        return false;
    }

    /* Initialize the channel. */    

#if defined(BSP_MCU_RX21_ALL) || defined(BSP_MCU_RX63_ALL) || defined(BSP_MCU_RX11_ALL)
    /* Enable writing to MSTP registers. */
    SYSTEM.PRCR.WORD = 0xA502;
#endif

	/* Enable SCI Channel. */
    _MSTP(X_SCI(SCIA_CFG_CHANNEL)) = 0;


#if defined(BSP_MCU_RX21_ALL) || defined(BSP_MCU_RX63_ALL) || defined(BSP_MCU_RX11_ALL)
    /* Disable writing to MSTP registers. */
    SYSTEM.PRCR.WORD = 0xA500;
#endif
    


    /* Disable Tx/Rx */

    Y_SCI(SCIA_CFG_CHANNEL).SCR.BYTE = 0;

    

    /*  Set mode register

        -Asynchronous Mode

        -8 bits
        -no parity
        -1 stop bit

        -PCLK clock (n = 0) */

    Y_SCI(SCIA_CFG_CHANNEL).SMR.BYTE = 0x00;


    /* Enable RXI and TXI interrupts, even though we are not using the TX interrupts, we will be checking the IR bitsasdfsdafdasfdasfdas
    asdfdasfads
       as flags */
    Y_SCI(SCIA_CFG_CHANNEL).SCR.BIT.RIE = 1;

    Y_SCI(SCIA_CFG_CHANNEL).SCR.BIT.TIE = 1;

    

    /* Clear IR bits for TIE and RIE */;

    _IR( Z_SCI(SCIA_CFG_CHANNEL, RXI) ) = 0;

    _IR( Z_SCI(SCIA_CFG_CHANNEL, TXI) ) = 0;

    

    /* Enable RXI interrupts in ICU. */
    _IEN( Z_SCI(SCIA_CFG_CHANNEL, RXI)) = 1;

    _IPR( Z_SCI(SCIA_CFG_CHANNEL, RXI)) = SCIA_CFG_INT_PRIORITY;

    /* Disable TXI interrupts in ICU because we are polling */
    _IEN( Z_SCI(SCIA_CFG_CHANNEL, TXI) ) = 0;


    /* Set 8 base clock cycles per 1-bit period. This enables higher bit rates. */
    Y_SCI(SCIA_CFG_CHANNEL).SEMR.BIT.ABCS = 1;
    

    /*  Set baud rate to 115200. Example shown for 48MHz PCLK

        N = ((PCLK Frequency) / (64 * 2^(2*n - 1) * Bit Rate)) - 1

        N = ((48,000,000) / (64 * 2^(2*0 - 1) * 115200)) - 1

        N = 12 */

#if defined(BSP_MCU_RX61_ALL) || defined(BSP_MCU_RX62_ALL) 
    Y_SCI(SCIA_CFG_CHANNEL).BRR = ((BSP_PCLK_HZ) / (16 * SCIA_CFG_BIT_RATE)) - 1;            
#else
    Y_SCI(SCIA_CFG_CHANNEL).BRR = ((BSP_PCLKB_HZ) / (16 * SCIA_CFG_BIT_RATE)) - 1;            
#endif    



    /* Enable Tx and Rx */

    Y_SCI(SCIA_CFG_CHANNEL).SCR.BYTE |= 0x30;


    /* Mark channel as initialized. */
    sci_initialized = true;

    /* Release state so other operations can be performed. */
    sci_release_state();        

    return true;
} 

/***********************************************************************************************************************
* Function Name: R_SCIA_Send
* Description  : Send data using SCI. This is done through polling so this function will not exit until all data has
*                been transferred.
* Arguments    : data -
*                    Pointer to buffer with data to send.
*                num_bytes -
*                    Number of bytes to send.
* Return Value : true - 
*                    Data transmitted successfully.
*                false -
*                    There was an error transmitting the data.
***********************************************************************************************************************/
bool R_SCIA_Send (uint8_t * data, uint32_t num_bytes)
{
    /* Grab state to make sure we do not interfere with another operation. */
    if (sci_acquire_state(SCI_STATE_TRANSMITTING) != true)
    {
        /* Another operation is already in progress */
        return false;
    }

    /* Make sure channel is initialized. */    
    if (sci_initialized != true)
    {
        /* Channel must be initialized before sending data. */
        return false;
    }
    
    /* Send the data. */        
    while (num_bytes != 0)
    {    
    	while( _IR( Z_SCI(SCIA_CFG_CHANNEL, TXI) ) == 0)
        {
            /* Wait for transmit buffer to be empty */
        }

    	/* Clear TXI IR bit */
    	_IR( Z_SCI(SCIA_CFG_CHANNEL, TXI) ) = 0;

    	/* Write the character out */ 
    	Y_SCI(SCIA_CFG_CHANNEL).TDR = *data;    
        
        /* Move pointer. */
        data++;
        /* Decrement counter. */
        num_bytes--;
    }

    /* Release state so other operations can be performed. */
    sci_release_state();

    return true;
} 


/***********************************************************************************************************************
* Function Name: R_SCIA_SendStatus
* Description  : Returns whether the previous transmission has been sent out or not.
* Arguments    : none
* Return Value : true - 
*                    Last data has been sent out.
*                false -
*                    Data still being sent.
***********************************************************************************************************************/
bool R_SCIA_SendStatus (void)
{    
    /* Check transmit end flag. */
    if (Y_SCI(SCIA_CFG_CHANNEL).SSR.BIT.TEND == 0)
    {
        /* Data still being transmitted. */
        return false;
    }
    else
    {
        /* Transmission has finished. */
        return true;
    }
} 


/***********************************************************************************************************************
* Function Name: R_SCIA_GetStatus
* Description  : Returns current state of the driver.
* Arguments    : none
* Return Value : State of the driver
***********************************************************************************************************************/
uint8_t R_SCIA_GetStatus (void)
{
    return (uint8_t)g_sci_state;
}

/***********************************************************************************************************************
* Function Name: scia_rx_isr
* Description  : This interrupt is triggered whenever data is received over serial. The user's callback function will
*                be called.
* Arguments    : none
* Return Value : none
***********************************************************************************************************************/
#if defined(__IAR_SYSTEMS_ICC__)
#pragma vector = _VECT(Z_SCI(SCIA_CFG_CHANNEL, RXI))
__interrupt static void scia_rx_isr (void)
#else
#pragma interrupt scia_rx_isr(vect=_VECT(Z_SCI(SCIA_CFG_CHANNEL, RXI)))
static void scia_rx_isr (void)
#endif
{
    /* Call the user's callback function with received data. */
    SCIA_CFG_CALLBACK( Y_SCI(SCIA_CFG_CHANNEL).RDR );
}

/***********************************************************************************************************************
* Function Name: sci_acquire_state
* Description  : Attempt to acquire the state so that we right to perform an operation.
* Arguments    : new_state -
*                    Which state to attempt to transition to
* Return Value : true - 
*                    State was obtained
*                false - 
*                    State was not obtained because code is busy with another on-going operation.
***********************************************************************************************************************/
static bool sci_acquire_state (sci_states_t new_state)
{
    bool ret;

    /* Attempt to acquire lock. */
    ret = R_BSP_HardwareLock(LOCK_SCI(SCIA_CFG_CHANNEL));
   
    /* Check to see if lock was successfully taken */
    if (ret == true)
    {
        /* Semaphore obtained, we can change state */
        g_sci_state = new_state;
        
        /* Return success */
        ret = true;
    }
    else
    {
        /* Another operation is on-going */
    }

    return ret;
}

/***********************************************************************************************************************
* Function Name: sci_release_state
* Description  : Release state so that other operations can be performed.
* Arguments    : none
* Return Value : none
***********************************************************************************************************************/
static void sci_release_state (void)
{
    /* Done with operation. */
    g_sci_state = SCI_STATE_READY;

    /* Release lock. */
    R_BSP_HardwareUnlock(LOCK_SCI(SCIA_CFG_CHANNEL));    
}

/***********************************************************************************************************************
* Function Name: R_SCIA_GetVersion
* Description  : Returns the current version of this module. The version number is encoded where the top 2 bytes are the
*                major version number and the bottom 2 bytes are the minor version number. For example, Version 4.25 
*                would be returned as 0x00040019.
* Arguments    : none
* Return Value : Version of this module.
***********************************************************************************************************************/
#if defined(__IAR_SYSTEMS_ICC__)
#pragma inline
#else
#pragma inline(R_SCIA_GetVersion)
#endif
uint32_t R_SCIA_GetVersion (void)
{
    /* These version macros are defined in r_sci_async_1ch_rx_if.h. */
    return ((((uint32_t)SCIA_RX_VERSION_MAJOR) << 16) | (uint32_t)SCIA_RX_VERSION_MINOR);
} 

