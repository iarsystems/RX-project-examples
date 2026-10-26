/* Adapted to IAR Embedded Workbench */
/******************************************************************************
* DISCLAIMER
* Please refer to http://www.renesas.com/disclaimer
******************************************************************************/
/***********************************************************************************
FILE NAME     dtc_vect.h
DESCRIPTION   
  
  Defines DTC Transfer Information structure and DTC vector table (short mode).

Copyright : 2008 Renesas Electronics Europe Ltd.
Copyright : 2008 Renesas Electronics Corporation.
All Rights Reserved
***********************************************************************************/

/***********************************************************************************
Revision History
DD.MM.YYYY OSO-UID Description
17.05.2009 RTA-JMB First Release
***********************************************************************************/
#ifndef DTC_VECT_H
#define DTC_VECT_H
/***********************************************************************************
DTC Vector Structure
***********************************************************************************/
#pragma bitfields=reversed
#pragma pack()
typedef struct st_dtc_xfer_full
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
} dtc_vect_table_t;

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
