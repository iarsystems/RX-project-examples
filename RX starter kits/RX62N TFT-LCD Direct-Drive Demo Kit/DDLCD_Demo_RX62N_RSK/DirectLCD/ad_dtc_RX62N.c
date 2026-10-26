/* Adapted for use with IAR Embedded Workbench */
/******************************************************************************
* DISCLAIMER

* This software is supplied by Renesas Electronics Corp. and is only 
* intended for use with Renesas products. No other uses are authorized.

* This software is owned by Renesas Electronics Corp. and is protected under 
* all applicable laws, including copyright laws.

* THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES 
* REGARDING THIS SOFTWARE, WHETHER EXPRESS, IMPLIED OR STATUTORY, 
* INCLUDING BUT NOT LIMITED TO WARRANTIES OF MERCHANTABILITY, FITNESS FOR A 
* PARTICULAR PURPOSE AND NON-INFRINGEMENT.  ALL SUCH WARRANTIES ARE EXPRESSLY 
* DISCLAIMED.

* TO THE MAXIMUM EXTENT PERMITTED NOT PROHIBITED BY LAW, NEITHER RENESAS 
* ELECTRONICS CORP. NOR ANY OF ITS AFFILIATED COMPANIES SHALL BE LIABLE 
* FOR ANY DIRECT, INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES 
* FOR ANY REASON RELATED TO THE THIS SOFTWARE, EVEN IF RENESAS OR ITS 
* AFFILIATES HAVE BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.

* Renesas reserves the right, without notice, to make changes to this 
* software and to discontinue the availability of this software.  
* By using this software, you agree to the additional terms and 
* conditions found by accessing the following link:
* http://www.renesas.com/disclaimer
******************************************************************************/
/************************************************************************
Revision History
DD.MM.YYYY OSO-UID Description
02.28.2007 RTA-PMM First Release 
************************************************************************/
#include "Global.h"

/* allocate DTC vector and table */
#include "DTC_Vect_RX.h"

__root struct st_dtc_xfer_full __evenaccess __sfr ad_dtc_table@0x4500;

#pragma location=".CDTC_VECT_S12AD"
__root const struct st_dtc_xfer_full __evenaccess __sfr *const ad_dtc_vector = &ad_dtc_table;

/* return to normal section definition */
uint16_t AD_Buffer[8][8];         // allocate buffer for 8 samples on each A/D channel

void ad_init(void)
{
  /* configure A/D Converter */
  MSTP(S12AD) = 0;     /* enable A/D */
  S12AD.ADCSR.BIT.ADIE = 1; /* enable A/D to feed ICU */
  S12AD.ADCSR.BIT.ADCS = 1; /* continous scan */
  S12AD.ADCSR.BIT.CKS = 3; /* clock at pclk/1 */
  S12AD.ADANS.WORD = 0xFF;  /* enable sampling of all 8 channel */
  S12AD.ADADS.WORD = 0xFF;  /* enable multi-sample of all 8 channel */
  S12AD.ADADC.BYTE = 3;     /* configure for 4x addition */
  S12AD.ADCSR.BIT.ADST = 1; /* start conversions */

  //ad_dtc_table.SAR = (void *)&S12AD.ADDR0;
  ad_dtc_table.SAR = (void *)&S12AD.ADDR0;
  ad_dtc_table.DAR = AD_Buffer;
  ad_dtc_table.MR.LONG = 0;
  ad_dtc_table.MR.BIT.MRA_MD = 2;  // block transfer
  ad_dtc_table.MR.BIT.MRA_SZ = 1;  // long word transfer size (less bus cycles)
  ad_dtc_table.MR.BIT.MRA_SM = 2;  // increment source address
  ad_dtc_table.MR.BIT.MRB_DM = 2;  // increment destination address
  ad_dtc_table.MR.BIT.MRB_DTS = 1; // source side is block source
  ad_dtc_table.CR.CRA = 0x0808;            // 8 words (16 bytes) per transfer
  ad_dtc_table.CR.CRB = size_ra(AD_Buffer);  // number of block transfers

  S12AD.ADCSR.BIT.ADIE = 1;
  IEN(S12AD,ADI) = 1;
}

void ad_acquire(void)
{
  ad_dtc_table.DAR = AD_Buffer;
  ad_dtc_table.CR.CRB = size_ra(AD_Buffer);  // number of block transfers
  DTCE(S12AD,ADI) = 1;
  nop();
  IR(S12AD,ADI) = 0;
}

uint16_t ad_value(int channel)
{
  int index;
  uint32_t value;
  
  if (channel >= size_ra(AD_Buffer[1]))
    return(0);
  
  /* sum values */  
  for (index=0, value=0; index < size_ra(AD_Buffer); index++)
    value += AD_Buffer[index][channel];
  
  /* shift data */
  value /= 64;
  return ((uint16_t)value);
}
