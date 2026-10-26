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


/******************************************************************************
    DTC structure and RAM declaration
******************************************************************************/
#define DTC_VECTOR_BASE_ADR 0xFFFF0000

/* DTC transfer data for S12AD                                               */
#pragma location=0x1000
__no_init struct st_dtc_xfer_full __evenaccess s12ad_dtc_table;

#pragma location=0xFFFF0198
__root const unsigned long s12ad_dtc_vector = (unsigned long)&s12ad_dtc_table;


/******************************************************************************
    Variables
******************************************************************************/
extern unsigned short S12AD_Buffer[];


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
  /* Take DTC out of Module Stop Mode.                                     */
  /* Should be enabled by default, but just to make sure                   */
  MSTP(DTC) = 0;
  
  /* DTC base address                                                      */
  DTC.DTCVBR = (void *)DTC_VECTOR_BASE_ADR;      
  
  /* Full address mode                                                     */     
  DTC.DTCADMOD.BIT.SHORT = 0;          
  
  /* DTC Start                                                             */
  DTC.DTCST.BIT.DTCST = 1;          
  
  /* DTC RAM registers for the S12ADC / DTC transfer                       */  
  s12ad_dtc_table.SAR = (void *)&S12AD.ADDR2;
  /* Source Address                                                        */             
  s12ad_dtc_table.DAR = S12AD_Buffer;                                                    
  /* Destination Address                                                   */             
  
  s12ad_dtc_table.MR.LONG = 0;          /* Zero out all contents of MR     */
  
  s12ad_dtc_table.MR.BIT.MRA_SM = 0;    /* Source address is fixed         */
  s12ad_dtc_table.MR.BIT.MRA_SZ = 1;    /* Word transfer size              */
  s12ad_dtc_table.MR.BIT.MRA_MD = 0;    /* Normal transfer                 */
  
  s12ad_dtc_table.MR.BIT.MRB_DM = 2;    /* Increment destination address   */
  s12ad_dtc_table.MR.BIT.MRB_DTS = 0;   /* NA: normal mode being used      */
  s12ad_dtc_table.MR.BIT.MRB_DISEL = 0; /* Interrupt passed to the CPU     */
  s12ad_dtc_table.MR.BIT.MRB_CHNE = 0;  /* No Chain                        */
  
  s12ad_dtc_table.CR.CRA = 0x0020;      /* 32 transfers specified          */
  
  s12ad_dtc_table.CR.CRB = 0;           /* NA: normal mode being used      */
}

/******************************************************************************
   Function     : ReInitDTC
   Description   : Re-Initialises the DTC so that the DTC takes the 
                  S12AD Interrupt by reseting the Destination Address,
                  the transfer count and resetting the DTC Enable bit                  
   Argument    : none
   Return value : none
******************************************************************************/
void ReInitDTC(void)
{
  DTCE(S12AD,S12ADI0) = 1;
  
  /* Reset the Destination Address Register                                */
  s12ad_dtc_table.DAR = S12AD_Buffer;
  
  /* Reset the Transfer Count Value                                        */
  s12ad_dtc_table.CR.CRA = 0x0020;       
}    
