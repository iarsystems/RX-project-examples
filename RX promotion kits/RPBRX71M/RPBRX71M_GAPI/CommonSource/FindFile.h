/******************************************************************************
* DISCLAIMER
* Please refer to http://www.renesas.com/disclaimer
******************************************************************************/
/***********************************************************************/
/*                                                                     */
/*  FILE        FindFile.h                                             */
/*  DATE        :Sep 12, 201                                           */
/*  DESCRIPTION :Helper utility to find files in external flash        */
/*                                                                     */
/***********************************************************************/
/************************************************************************
Revision History
DD.MM.YYYY OSO-UID Description
12.09.2015 REE - Ported from RX63N GAPI Demo
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
uint32_t FileSizeFind( File_Table_type const *pFiles, char const *pName);

#endif
