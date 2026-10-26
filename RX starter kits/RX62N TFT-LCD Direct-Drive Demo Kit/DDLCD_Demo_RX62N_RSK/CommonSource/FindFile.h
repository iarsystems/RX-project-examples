/******************************************************************************
* DISCLAIMER
* Please refer to http://www.renesas.com/disclaimer
******************************************************************************/
/***********************************************************************/
/*                                                                     */
/*  FILE        FindImage.h                                            */
/*  DATE        :Jan 12, 2008                                          */
/*  DESCRIPTION :Helper utility to find files in external flash        */
/*                                                                     */
/***********************************************************************/
/************************************************************************
Revision History
DD.MM.YYYY OSO-UID Description
01.22.2008 RTA-JMB Driver Rewrite to Parameterization
************************************************************************/
#ifndef FINDIMAGE_H
#define FINDIMAGE_H

typedef struct
{
  char name[24];
  uint8_t address[4];
  uint8_t size[4];
} File_Table_type;


void const * FileFind( File_Table_type const *pFiles, char const *pName);

#endif
