

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
 * dtc.h
 *
 *  Created on: 01 Oct 2011
 *      Author: RJW
 *              Reneses Electronics Europe Ltd
 */
#ifndef DTC_H_
#define DTC_H_

/******************************************************************************
    Function Prototypes
******************************************************************************/
void InitDTC(void);
void ReInitDTC(void);


/******************************************************************************
    Structure definitions for setting up the DTC RAM
******************************************************************************/
#define __evenaccess
#pragma bitfields=reversed
struct st_dtc_xfer_full
{
  union
  {
    struct
    {
      unsigned long MRA_MD    :2;
      unsigned long MRA_SZ    :2;
      unsigned long MRA_SM    :2;
      unsigned long           :2;
      unsigned long MRB_CHNE  :1;
      unsigned long MRB_CHNS  :1;
      unsigned long MRB_DISEL :1;
      unsigned long MRB_DTS   :1;
      unsigned long MRB_DM    :2;
      unsigned long           :2;
      unsigned long           :16;
    }BIT;
    unsigned long LONG;
  }MR;
  void * SAR;
  void * DAR;
  struct
  {
    unsigned long CRA:16;
    unsigned long CRB:16;
  }CR;
};

struct st_dtc_xfer_short
{
  union
  {
    struct
    {
      unsigned long MRA_MD  :2;
      unsigned long MRA_SZ  :2;
      unsigned long MRA_SM  :2;
      unsigned long         :2;
      unsigned long SAR     :24;
    }BIT;
    unsigned long LONG;
  }SAR_MRA;
  union
  {
    struct
    {
      unsigned long MRB_CHNE  :1;
      unsigned long MRB_CHNS  :1;
      unsigned long MRB_DISEL :1;
      unsigned long MRB_DTS   :1;
      unsigned long MRB_DM    :2;
      unsigned long           :2;
      unsigned long DAR       :24;
    }BIT;
    unsigned long LONG;
  }DAR_MRB;
  struct
  {
    unsigned long CRA:16;
    unsigned long CRB:16;
  }CR;
};

#pragma bitfields=default
#endif
