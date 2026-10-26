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
/**************************************************************************************************
 *                                                                                                  
 *  FILE        :FindFile.c
 *  DATE        :Sep 12, 2015
 *  DESCRIPTION :
 *             
 *
 *             
 *               The files are stored in the flash with a table at the beginning
 *               which tells the  file's name, location, and size. 24 bytes
 *               are used for the name, which is in ascii hex with a 0x00
 *               designating the end.  After these 24 bytes, there are 4 bytes for
 *               file's location (in hex) and 4 bytes for the file's size (in hex).
 *               An example table entry is shown below:
 *             
 *               67 72 65 65 6E 00 FF FF FF FF FF FF FF FF FF FF   ('green' 0x00 then all 0xFF's)
 *               FF FF FF FF FF FF FF FF 00 00 04 00 00 00 0B DE   (location = 0x400, size = 0xBDE)
 *            
 *               The end of the table is detected when a 0xFF is encountered for
 *               the first lettern in the name.                      
 *                                                                     
 *  Copyright:   2015 Renesas Electronics Corporation.
 *               All Rights Reserved
 *                                                                     
 *************************************************************************************************/
#include <ctype.h>
#include "Global.h"
#include "FindFile.h"

/************* Function Prototypes ***********************************/
/*****************************************************************************
Name:        NameFind
Arguments:  pFiles  -   pointer to file list 
             pName   -   name of file to search for in flash
             
Returns:     pointer to file record (file was found)
             0 = File Not Found
Description: Searches the file table for the designated file.
             If it is found then it returns pointer to file record.
*****************************************************************************/
static File_Table_type const * NameFind( File_Table_type const *pFiles, char const *pName)
{
  unsigned char i = 0;
  
  /* check file pointer */
  if(NULL == pFiles)
  {
    return(0);
  }

  /*Each file header is 32 bytes.  The first time that a non-print character is found
    in the 'name' field, the search will stop if the name has not already
    been found. */
  for (; isprint(pFiles->name[0]); pFiles++ )
  {
    /*i is used for the current position inside the current header */
    i = 0;
  
    while(tolower(pFiles->name[i]) == tolower(pName[i]))  //keep looping while characters are the same
    {   
      if(0 == pFiles->name[i])        //there was a complete name match
      { 
        return (pFiles);
      }
      i++;
    }
  }
  return 0;
}
/*******************************************************************************
End of function NameFind
*******************************************************************************/

/*****************************************************************************
Name:        FileFind
Arguments:  pFiles  -   pointer to file list 
             pName   -   name of file to search for in flash
             
Returns:     address of the file (file was found)
             0 = File Not Found
Description: Searches the file table for the designated file.
             If it is found then it returns location of the file.
*****************************************************************************/
void const * FileFind( File_Table_type const *pFiles, char const *pName)
{
  /* address will be relative to file table */
  uint32_t address=(uint32_t)pFiles;
  
  pFiles = NameFind(pFiles, pName);

  /* check file pointer */
  if(NULL == pFiles)
  {
	  return (0);
  }
    
  /*fill in information for location of file */
  address += (((uint32_t)(pFiles->address[0]) << 0)  | 
              ((uint32_t)(pFiles->address[1]) << 8)  |
              ((uint32_t)(pFiles->address[2]) <<16)  |
              ((uint32_t)(pFiles->address[3]) <<24));

  return ((void *)address);
}
/*******************************************************************************
End of function FileFind
*******************************************************************************/

/*****************************************************************************
Name:        FileSizeFind
Arguments:  pFiles  -   pointer to file list 
             pName   -   name of file to search for in flash
             
Returns:     size of the file (file was found)
             0 = File Not Found
Description: Searches the file table for the designated file.
             If it is found then it returns size of the file.
*****************************************************************************/
uint32_t FileSizeFind( File_Table_type const *pFiles, char const *pName)
{
  pFiles = NameFind(pFiles, pName);

  /* check file pointer */
  if(NULL == pFiles)
  {
    return(0);
  }
    
  /*fill in information for size of file */
  return (((uint32_t)(pFiles->size[0]) << 0)  | 
          ((uint32_t)(pFiles->size[1]) << 8)  |
          ((uint32_t)(pFiles->size[2]) <<16)  |
          ((uint32_t)(pFiles->size[3]) <<24));
}
/*******************************************************************************
End of function FileSizeFind
*******************************************************************************/
