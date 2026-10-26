/* Adapted for use with IAR Embedded Workbench */
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
* Copyright (C) 2011 Renesas Electronics Corporation. All rights reserved.    
*******************************************************************************/
/*******************************************************************************
* File Name     : dtc.c
* Version       : 1.0
* Description   : Defines DTC transfer functions used in this sample.
* Limitations   : As a sample, this implementation only supports a limited number 
*               : of the DTC available features. This version supports only  
*               : fixed source address, and incrementing destination address. 
*******************************************************************************/
/*******************************************************************************
* History : DD.MM.YYYY   Version    Description
*         : 20.09.2011   1.00       First release
*           23.01.2012              CS revisions
*******************************************************************************/

/*******************************************************************************
Includes <System Includes>, "Project Includes"
*******************************************************************************/
#include <stdint.h>
#include <iorx63n.h>

#include "dtc.h"


/******************************************************************************
Local function declarations and local global variables
******************************************************************************/
/* Reserve an area for the DTC vector table */
#pragma location = 0x00015000
__no_init uint32_t dtc_vector_table[256];

/* little endian form of DTC transfer data settings*/
volatile static struct dtc_tdata_full_addr
{
  uint16_t RESERVED0;  
  uint8_t  MRB;
  uint8_t  MRA;
  uint32_t SAR;
  uint32_t DAR;
  uint16_t CRB;
  uint16_t CRA; 
} dtc_tdata;


/*****************************************************************************
* Function Name : DtcInit()
* Description   : initializes the DTC
*                 Note: Several of the DTC registers are not directly CPU accessible.
*                       These are automatically loaded from the DTC transfer data 
*                       structure that is mapped to the particular interrupt 
*                       vector in the DTC vector table. The values are read from the 
*                       DTC transfer data when the associated interrupt is triggered.
* Arguments     : dtc_vector -
*                       Vector that indexes to transfer control data structure.
*                 dtc_source - 
*                       32-bit pointer to be written to the DTC's source address register (SAR). 
*                 dtc_dest -
*                       32-bit pointer to be written to the DTC's destination address register (DAR). 
*                 num_transfers -
*                       The number of DTC transfers to be performed.
* Return Value  : None
*
* Limitations   : This implementation only supports fixed source address, 
*                 and incrementing destination address, normal mode.
*****************************************************************************/
void DtcInit(uint8_t  dtc_vector, 
             uint32_t dtc_source, 
             uint32_t dtc_dest, 
             uint16_t num_transfers)
{
  /* Set vector to point to transfer data(MRA, MRB, SAR, DAR, CRA, and CRB) */
  dtc_vector_table[dtc_vector] = (uint32_t)&dtc_tdata;
  
  SYSTEM.PRCR.WORD = 0xA50B; /* Protect off */
  
  /* Cancel DTC module stop state. */
  MSTP(DTC) = 0;  /*  Expands to: SYSTEM.MSTPCRA.BIT.MSTPA28 = 0; */
  
  SYSTEM.PRCR.WORD = 0xA500; /* Protect on  */
  
  /* DTC Vector Base Register (DTCVBR) */
  DTC.DTCVBR = dtc_vector_table; /* Set vector base register */
  
  /* DTC Address Mode Register (DTCADMOD) */
  DTC.DTCADMOD.BIT.SHORT = 0;      // 0: Full 32-bit address mode. 
  
  /* DTC Mode Register A (MRA)      */       
  /*  b7  b6  b5  b4  b3   b2   b1  b0  */
  /*  MD[1:0] SZ[1:0] SM[1:0]   —   —   */
  /*    |   |   |   |   |   |   |   | 
        |   |   |   |   |   |   |   +-- Reserved. Always write 0.   
        |   |   |   |   |   |   +------ Reserved. Always write 0.
        |   |   |   |   |   +---------- Transfer Source Address Addressing Mode:   
        |   |   |   |   +--------------     00b: SAR value is address fixed
        |   |   |   +------------------ DTC Data Transfer Size:   
        |   |   +----------------------     01b: word transfer.
        |   +-------------------------- DTC Transfer Mode Select.   
        +------------------------------     00b: Normal transfer mode. */ 
  dtc_tdata.MRA = 0x10;  // Normal mode, word transfer, fixed SAR. 
  
  /* DTC Mode Register B (MRB)      */       
  /*  b7   b6   b5    b4  b3 b2    b1  b0  */
  /* CHNE CHNS DISEL DTS DM[1:0]   —   —   */
  /*    |   |    |    |   |   |    |   |
        |   |    |    |   |   |    |   +-- Reserved. Always write 0.   
        |   |    |    |   |   |    +------ Reserved. Always write 0.
        |   |    |    |   |   +----------- Transfer Destination Address Addressing Mode:   
        |   |    |    |   +---------------     10b = DAR value is incremented after data transfer.
        |   |    |    +------------------- DTC Transfer Mode Select. 0: Destination is repeat area or block area.   
        |   |    +------------------------ DTC Interrupt Select. 0: IRQ generated when data transfer is completed.
        |    +----------------------------- DTC Chain Transfer Select.   
        +--------------------------------- DTC Chain Transfer Enable. 0: Chain transfer is disabled */     
  dtc_tdata.MRB = 0x08;               // Chain disabled, IRQ when complete, DAR increments.   
  
  dtc_tdata.RESERVED0 = 0x0000;       // reserved. always 0
  
  /* DTC Transfer Source Address register (SAR) */     
  dtc_tdata.SAR = dtc_source;          // Set transfer source source address.
  
  /* DTC Transfer Destination Address Register (DAR) */
  dtc_tdata.DAR = dtc_dest;          // Destination start address.
  
  /* DTC Transfer Count Register A (CRA) */
  dtc_tdata.CRA = num_transfers;      // Transfer count. 
  
  /* DTC Transfer Count Register B (CRB) */
  /* CRB is used to set the block transfer count for block transfer mode. */
  dtc_tdata.CRB = 0;                  // Not using block transfer mode.
  
  /* DTC Control Register (DTCCR) */    
  DTC.DTCCR.BIT.RRS = 0;        // DTC Transfer Data Read not skipped 
} /* End of function DtcInit() */


/*****************************************************************************
* Function Name : DtcStart()
* Description   : Starts the DTC transfer process.
* Argument      : dtc_vector -
*                       The vector number mapped to the DTC operation.
* Return Value  : None
*****************************************************************************/
void DtcStart(uint8_t dtc_vector)
{
    /* DTC Module Start Register (DTCST) */
    DTC.DTCST.BYTE = 0x01;          // 0: DTC module start.
    
    /* Set the enable bit for a startup source interrupt */    
    ICU.DTCER[dtc_vector].BIT.DTCE = 1; // Route this interrupt to DTC.    
} /* End of function DtcStart() */


/*****************************************************************************
* Function Name : dtc_stop()
* Description   : Stops the DTC transfer process.
* Argument      : dtc_vector -
*                       The vector number mapped to the DTC operation.
* Return Value  : None
*****************************************************************************/
void DtcStop(uint8_t dtc_vector)
{
    /* DTC Module Start Register (DTCST) */  
    DTC.DTCST.BYTE = 0x00;          // 0: DTC module stop.  
    ICU.DTCER[dtc_vector].BIT.DTCE = 0; // Stop routing this interrupt to DTC.
    
    /* DTC Status Register (DTCSTS) */
    /* b15: ACT DTC Active Flag     0: DTC transfer operation is not in progress.
                                    1: DTC transfer operation is     in progress. */  
    while (DTC.DTCSTS.BIT.ACT)         // Wait for active transfer to complete.
    {
    }  
} /* End of function DtcStop() */

/*******************************************************************************
End of file: dtc.c
*******************************************************************************/
