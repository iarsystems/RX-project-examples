/*******************************************************************************
* DISCLAIMER
* This software is supplied by Renesas Electronics Corporation and is only
* intended for use with Renesas products. No other uses are authorized. This
* software is owned by Renesas Electronics Corporation and is protected under
* all applicable laws, including copyright laws.
* THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES REGARDING
* THIS SOFTWARE, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING BUT NOT
* LIMITED TO WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE
* AND NON-INFRINGEMENT.  ALL SUCH WARRANTIES ARE EXPRESSLY DISCLAIMED.
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

* Copyright (C) 2011 Renesas Electronics Corporation All rights reserved.
*******************************************************************************/

/*""FILE COMMENT""**************************************************************
* File Name     : r_sample_tfat.c
* Version       : 1.00
* Device(s)     : V850E2/ML4
* Tool-Chain    : CX V1.20
* H/W Platform  : -
* Description   : TFAT sample software source file
********************************************************************************
* History       : DD.MM.YYYY Version Description

*""FILE COMMENT END""**********************************************************/

/*******************************************************************************
Pragma directive
*******************************************************************************/
/* Start user code for pragma. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */

/*******************************************************************************
Include files
*******************************************************************************/
#include <iorx62n.h>
#include "r_stdint.h"
#include "r_tfat_lib.h"
#include "r_data_file.h"
#include "r_board.h"
#include "r_mtl_com.h"
#include "r_mmc.h"
#include <intrinsics.h>


/*******************************************************************************
Imported global variables and functions (from other files)
*******************************************************************************/
extern void R_init_card_detect_chat(uint8_t);

/*******************************************************************************
Exported global variables and functions (to be accessed by other files)
*******************************************************************************/

/*******************************************************************************
Private global variables and functions
*******************************************************************************/
static DSTATUS	g_Stat = TFAT_STA_NOINIT;
static uint8_t	g_Buff[512];			/* Working buffer                        */
static FATFS	g_fatfs;				/* Structure to hold the FAT information */
static FIL		g_file;					/* Structure to FILE information		 */

static void R_error(uint8_t err_code);

/*******************************************************************************
* Declaration  : void tfat_sample(void);
* Function Name: tfat_sample
* Description  : TFAT sample program
* Arguments    : none
* Return Value : none
*******************************************************************************/
void main(void)
{
	int32_t		byte_count;
	uint16_t	data_count;
	uint16_t	file_rw_cnt;
	FRESULT 	result;

  __enable_interrupt();

	g_Stat = TFAT_STA_NOINIT;

	/* Initialize MMC driver. */
	R_mmc_Init_Driver();

	/* Initialize card detect chattering. */
	R_init_card_detect_chat(MMC_SLOT_NUM);

	/* Check if the Card has been inserted	*/
	while ((g_Stat & (TFAT_STA_NODISK | TFAT_STA_NOINIT)) != 0)
	{
		/* If the card is not inserted then stop here. */
		g_Stat = R_tfat_disk_initialize();
		LED3_OFF();
		LED2_ON();
	}

	/* Turn on LED0 and turn off LED1 to indicate program running */
	LED3_ON();
	LED2_OFF();

	/* Mount the file system */
	if (TFAT_FR_OK == R_tfat_f_mount(0, &g_fatfs))
	{
		/* Create the file to be written */
		if (TFAT_FR_OK != R_tfat_f_open(&g_file, (const uint8_t *)"renesas.txt", TFAT_FA_CREATE_ALWAYS | TFAT_FA_WRITE))
		{
			R_error(1);
		}
		/* file open complete file name */
		/* Copy the contents to the newly created file	*/
		result = R_tfat_f_write(&g_file, (void*)filebuffer, sizeof(filebuffer), &file_rw_cnt);

		if (result != TFAT_FR_OK || file_rw_cnt < sizeof(filebuffer))
		{
			R_error(2);
		}
		/* file write complete. */

		/* Close the file */
		R_tfat_f_close(&g_file);

		/* Open the file to be read */
		if (TFAT_FR_OK != R_tfat_f_open(&g_file, (const uint8_t *)"renesas.txt", TFAT_FA_OPEN_ALWAYS | TFAT_FA_READ))
		{
			R_error(3);
		}

		/* Clear the counter before counting the no. of bytes read */
		byte_count = 0;

		/* Ensure that the file pointer points at beginning of the file */
		R_tfat_f_lseek(&g_file, 0);

		/* Repeat the read process until required no. of bytes are read */
		while (byte_count < (sizeof(filebuffer) - 1))
		{

			/* Read the file. 512 bytes at a time. */
			result = R_tfat_f_read(&g_file, (void*)g_Buff, sizeof(g_Buff), &file_rw_cnt);

			/* Check if the read operation was successful & the no. of bytes read
			   by the function matches with the size of the R/W buffer */
			if (result != TFAT_FR_OK || file_rw_cnt < sizeof(g_Buff))
			{
				R_error(4);
			}
			else
			{
				/* Compare the contents of the file with the ROM data */
				for (data_count = 0 ; data_count < sizeof(g_Buff) ; data_count++)
				{
					if (g_Buff[data_count] != filebuffer[byte_count + data_count])
					{
						/* error indication */
						/* explicit error data, address */
						R_error(5);
					}
				}
				/* verify complete */
				/* ================== */
			}
			/* Update the counter with the total no. of bytes read from the file */
			byte_count += file_rw_cnt;
		}

		/* Close the file	*/
		R_tfat_f_close(&g_file);
	}
	else
	{
		R_error(6);
	}

	while (1)
	{
		/* Indicates that the User Sample software has finished the write/read operation successfully */
		LED3_ON();
		LED2_ON();
	}
}

/*******************************************************************************
* Declaration  : static void R_error(uint8_t err_code)
* Function Name: R_error
* Description  : Error handling function
* Arguments    : Error code
* Return Value : none
*******************************************************************************/
static void R_error(uint8_t err_code)
{
	uint8_t	i = 1;
	LED3_OFF();
	LED2_ON();

	switch (err_code)
	{
		case 1:
			/* Error while opening the file for writing */
			while (i);
			break;
		case 2:
			/* Write function was not successful or all the bytes were not written to the file */
			while (i);
			break;
		case 3:
			/* Error while opening the file for reading */
			while (i);
			break;
		case 4:
			/* Error during the file read operation	*/
			while (i);
			break;
		case 5:
			/* File written on the CARD does not match filebuffer[] contents */
			while (i);
			break;
		case 6:
			/* R_tfat_f_mount function failed */
			while (i);
			break;
	}
}

