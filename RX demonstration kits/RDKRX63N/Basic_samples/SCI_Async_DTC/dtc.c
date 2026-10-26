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
* http://www.renesas.com/disclaimer
*******************************************************************************/


/*
 * dtc.c
 *
 *  Created on: 01 Oct 2011
 *      Author: RJW
 *              Reneses Electronics Europe Ltd
 */

/******************************************************************************
    System Includes                                                           
******************************************************************************/
#include <iorx63n.h>


/******************************************************************************
    User Includes                                                             
******************************************************************************/
#include "dtc.h"
#include "sci.h"

/******************************************************************************
    DTC structure and RAM declaration
******************************************************************************/
#define DTC_VECTOR_BASE_ADR 0xFFFF0000

/* DTC transfer data for SCI Rx                                              */
#pragma location=0x1000
__no_init struct st_dtc_xfer_full __evenaccess sciRXI_dtc_table;

/* DTC transfer data for SCI Tx                                              */
#pragma location=0x1010
__no_init struct st_dtc_xfer_full __evenaccess sciTXI_dtc_table;

#pragma location=0xFFFF0370
__root const unsigned long sciRXI_dtc_vector = (unsigned long)&sciRXI_dtc_table;

#pragma location=0xFFFF0374
__root const unsigned long sciTXI_dtc_vector = (unsigned long)&sciTXI_dtc_table;


/******************************************************************************
    Variables
******************************************************************************/
extern struct _Rx Rx;


/******************************************************************************
   Function     : InitDTC
   Description   : Initialises the DTC and DTC RAM so that the DTC takes the 
                  S12AD Interrupt
                  The DTC is configured for Normal Mode, 32 transfers 
   Argument    : none
   Return value : none
******************************************************************************/
void InitDTC(void)
{
  /* Take DTC out of Module Stop Mode.                                       */
  /* Should be enabled by default, but just to make sure                     */
  MSTP(DTC) = 0;
  
  /* DTC base address                                                        */
  DTC.DTCVBR = (void *)DTC_VECTOR_BASE_ADR;      
  
  /* Full address mode                                                       */     
  DTC.DTCADMOD.BIT.SHORT = 0;          
  
  /* DTC Start                                                               */
  DTC.DTCST.BIT.DTCST = 1;
  
/*****************************************************************************/
/* Set up DTC RAM table for the RXI0 ISR                                     */
/*****************************************************************************/
  sciRXI_dtc_table.SAR = (void *)&SCI2.RDR;
  /* Source Address                                                          */
  sciRXI_dtc_table.DAR = Rx.Data;
  /* Destination Address                                                     */
  sciRXI_dtc_table.MR.LONG = 0;
  
  sciRXI_dtc_table.MR.BIT.MRA_SM = 0;    /* source address is fixed          */
  sciRXI_dtc_table.MR.BIT.MRA_SZ = 0;    /* byte transfer size               */
  sciRXI_dtc_table.MR.BIT.MRA_MD = 1;    /* repeat transfer                  */
  
  sciRXI_dtc_table.MR.BIT.MRB_DM = 2;    /* increment destination address    */
  sciRXI_dtc_table.MR.BIT.MRB_DTS = 0;   /* Transfer destination is repeat   */
  sciRXI_dtc_table.MR.BIT.MRB_DISEL = 0;
  sciRXI_dtc_table.MR.BIT.MRB_CHNE = 0;
  
  sciRXI_dtc_table.CR.CRA = 0x0000;      /* 256 bytes                        */
  
  sciRXI_dtc_table.CR.CRB = 0;           /* NA: repeat mode being used       */
  
  
/*****************************************************************************/
/* Set up DTC RAM table for the TXI0 ISR                                     */
/*****************************************************************************/
  sciTXI_dtc_table.SAR = &Rx.Data[0];        
  
  sciTXI_dtc_table.DAR = (void *)&SCI2.TDR;
  
  sciTXI_dtc_table.MR.LONG = 0;
  
  sciTXI_dtc_table.MR.BIT.MRA_SM = 2;    /* source address is incremented    */
  sciTXI_dtc_table.MR.BIT.MRA_SZ = 0;    /* byte transfer size               */
  sciTXI_dtc_table.MR.BIT.MRA_MD = 0;    /* normal transfer                  */
  
  sciTXI_dtc_table.MR.BIT.MRB_DM = 0;    /* destination address is fixed     */
  sciTXI_dtc_table.MR.BIT.MRB_DTS = 1;   /* N/A:                             */
  sciTXI_dtc_table.MR.BIT.MRB_DISEL = 0;
  sciTXI_dtc_table.MR.BIT.MRB_CHNE = 0;
  
  sciTXI_dtc_table.CR.CRA = 0x0100;      /* 256 bytes                        */
  
  sciTXI_dtc_table.CR.CRB = 0;           /* NA: normal mode being used       */
}

/******************************************************************************
   Function     : ResetTXI2_ISR_DTC
   Description   : Re-Initialises the DTC so that the DTC takes the 
                  TXI2 Interrupt by reseting the Source Address,
                  the transfer count and resetting the DTC Enable bit                  
   Argument    : none
   Return value : none
******************************************************************************/
void ResetTXI2_ISR_DTC(void)
{
  DTCE(SCI2,TXI2) = 1;
  sciTXI_dtc_table.SAR = &Rx.Data[0];        
  sciTXI_dtc_table.CR.CRA = 0x0100;      /* 256 bytes                        */
}

