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
*
* Copyright (C) 2011(2012) Renesas Electronics Corporation. All rights reserved.
*******************************************************************************/

/*""FILE COMMENT""*******************************************************************
* File Name     : r_tfat_drv_if.c
* Version       : 2.00
* Device(s)     : V850E2/ML4
* Tool-Chain    : CX V1.20
* H/W Platform  : -
* Description   : TFAT FS memory driver interface file
*************************************************************************************
* History       : DD.MM.YYYY Version Description
*""FILE COMMENT END""****************************************************************/

/******************************************************************************
Includes <System Includes> , "Project Includes"
******************************************************************************/
#include "r_tfat_lib.h"
#include "r_mmc.h"

/*******************************************************************************
Macro definitions
*******************************************************************************/
/* Start user code for macro. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */

/*******************************************************************************
Macro definitions
*******************************************************************************/
#define MMC_INS_CHAT     20 /* counter value of chattering detection */
#define DISABLE_IRQ()
#define ENABLE_IRQ()

/******************************************************************************
Exported global variables and functions (to be accessed by other files)
******************************************************************************/
/*	Real Time Clock - Time set to 23 FEB 2009 15:02:20	*/
uint8_t g_rtcYear = 109, g_rtcMon = 2, g_rtcMday = 23;
uint8_t g_rtcHour = 15, g_rtcMin = 2, g_rtcSec = 20;

uint16_t gDetChatCnt[MMC_SLOT_NUM]; /* Counter of chattering detection */
uint16_t gDetSts_Old[MMC_SLOT_NUM]; /* Previous status of detection    */
MMC_INFO gMmcInfo[MMC_SLOT_NUM];

int16_t R_card_insertion_chk(uint8_t slot_num);
void R_init_card_detect_chat(uint8_t);
void clock_countup(void);


/*""FUNC COMMENT""*******************************************************************
* Function Name : R_tfat_disk_initialize
* Include       : r_tfat_drv_if.h
* Declaration   : DSTATUS R_tfat_disk_initialize ()
* Description   : This function initializes the memory medium for file operations
* Arguments     : none
* Return value  : Status of the memory medium
*""FUNC COMMENT END""****************************************************************/
DSTATUS R_tfat_disk_initialize(void)
{
	/*	Please put the code for disk_initalize driver interface function over here	*/
	/*	Please refer the application note for details								*/
	/*	Please put the code for memory driver initialization, if any, over here		*/
	int mmc_ret;
	DSTATUS ret_value  = TFAT_STA_NODISK | TFAT_STA_NOINIT;

	mmc_ret = R_card_insertion_chk(MMC_SLOT0);
	if (mmc_ret < MMC_OK)
	{
		R_init_card_detect_chat(MMC_SLOT0);
		return ret_value;
	}
	if (mmc_ret == MMC_FALSE)
	{
		return ret_value;
	}

	ret_value &= ~TFAT_STA_NODISK;
	mmc_ret = R_mmc_Init_Slot(MMC_SLOT0);
	if (mmc_ret < MMC_OK)
	{
		R_init_card_detect_chat(MMC_SLOT0);
		return ret_value;
	}

	mmc_ret = R_mmc_Get_MmcInfo(MMC_SLOT0, &gMmcInfo[MMC_SLOT0]);
	if (mmc_ret < MMC_OK)
	{
		R_init_card_detect_chat(MMC_SLOT0);
		return ret_value;
	}

	ret_value &= ~TFAT_STA_NOINIT;
	return 	ret_value;
}

/*""FUNC COMMENT""*******************************************************************
* Function Name : R_tfat_disk_read
* Include       : r_tfat_drv_if.h
* Declaration   : DRESULT R_tfat_disk_read (uint8_t Drive, uint8_t* Buffer, uint32_t SectorNumber, uint8_t SectorCount)
* Description   : This function reads data from the specified location of the memory medium
* Arguments     : uint8_t  Drive        : Physical drive number
*               : uint8_t* Buffer       : Pointer to the read data buffer
*               : uint32_t SectorNumber : uint32_t SectorNumber
*               : uint8_t SectorCount   : Number of sectors to read
* Return value  : Result of function execution
*""FUNC COMMENT END""****************************************************************/
DRESULT R_tfat_disk_read(
    uint8_t Drive,         /* Physical drive number 			*/
    uint8_t* Buffer,       /* Pointer to the read data buffer 	*/
    uint32_t SectorNumber, /* Start sector number 				*/
    uint8_t SectorCount    /* Number of sectors to read 		*/
)
{
	/*	Please put the code for R_tfat_disk_read driver interface function over here	*/
	/*	Please refer the application note for details									*/
	DRESULT func_ret;
	int16_t mmc_ret;
	mmc_ret = R_mmc_Read_Data(MMC_SLOT0, SectorNumber, SectorCount, Buffer, MMC_MODE_NORMAL);
	switch (mmc_ret)
	{
		case MMC_OK:
			func_ret = TFAT_RES_OK;
			break;
		case MMC_ERR_PARAM:
			func_ret = TFAT_RES_PARERR;
			break;
		case MMC_ERR_OTHER:
		default:
			func_ret = TFAT_RES_ERROR;
			break;
	}
	return func_ret;
}

/*""FUNC COMMENT""*******************************************************************
* Function Name : R_tfat_disk_write
* Include       : r_tfat_drv_if.h
* Declaration   : DRESULT R_tfat_disk_write (uint8_t Drive, const uint8_t* Buffer, uint32_t SectorNumber, uint8_t SectorCount)
* Description   : This function writes data to a specified location of the memory medium
* Arguments     : uint8_t Drive : Physical drive number
*               : const uint8_t* Buffer       : Pointer to the write data
*               : uint32_t       SectorNumber : Sector number to write
*               : uint8_t        SectorCount  : Number of sectors to write
* Return value  : Result of function execution
*""FUNC COMMENT END""****************************************************************/
DRESULT R_tfat_disk_write(
    uint8_t Drive,          /* Physical drive number 			*/
    const uint8_t* Buffer,  /* Pointer to the write data 		*/
    uint32_t SectorNumber,  /* Sector number to write 			*/
    uint8_t SectorCount     /* Number of sectors to write 		*/
)
{
	/*	Please put the code for R_tfat_disk_write driver interface function over here	*/
	/*	Please refer the application note for details									*/
	DRESULT func_ret;
	int16_t mmc_ret;

	if (gMmcInfo[MMC_SLOT0].Card != MMC_CARD_MMC)
	{
		return TFAT_RES_NOTRDY;
	}
	if (gMmcInfo[MMC_SLOT0].WProtect != MMC_FALSE)
	{
		return TFAT_RES_WRPRT;
	}

	mmc_ret = R_mmc_Write_Data(MMC_SLOT0, SectorNumber, SectorCount, (uint8_t *)Buffer, MMC_MODE_NORMAL);
	switch (mmc_ret)
	{
		case MMC_OK:
			func_ret = TFAT_RES_OK;
			break;
		case MMC_ERR_PARAM:
			func_ret = TFAT_RES_PARERR;
			break;
		case MMC_ERR_WP:
			func_ret = TFAT_RES_WRPRT;
			break;
		case MMC_ERR_OTHER:
		default:
			func_ret = TFAT_RES_ERROR;
			break;
	}
	return func_ret;
}

/*""FUNC COMMENT""*******************************************************************
* Function Name : R_tfat_disk_ioctl
* Include       : r_tfat_drv_if.h
* Declaration   : DRESULT R_tfat_disk_ioctl (uint8_t Drive, uint8_t Command, void* Buffer)
* Description   : This function is used to execute memory operations other than read\write
* Arguments     : uint8_t Drive   : Drive number
*               : uint8_t Command : Control command code
*               : void*   Buffer  : Data transfer buffer
* Return value  : Result of function execution
*""FUNC COMMENT END""****************************************************************/
DRESULT R_tfat_disk_ioctl(
    uint8_t Drive,          /* Drive number 			*/
    uint8_t Command,        /* Control command code 	*/
    void* Buffer            /* Data transfer buffer 	*/
)
{
	/*	Please put the code for R_tfat_disk_ioctl driver interface function over here	*/
	/*	Please refer the application note for details									*/
	return TFAT_RES_OK;
}

/*""FUNC COMMENT""*******************************************************************
* Function Name : R_tfat_disk_status
* Include       : r_tfat_drv_if.h
* Declaration   : DSTATUS R_tfat_disk_status (uint8_t Drive)
* Description   : This function is used to retrieve the current status of the disk
* Arguments     : uint8_t Drive : Physical drive number
* Return value  : Status of the disk
*""FUNC COMMENT END""****************************************************************/
DSTATUS R_tfat_disk_status(
    uint8_t Drive           /* Physical drive number 	*/
)
{
	/*	Please put the code for R_tfat_disk_status driver interface function over here	*/
	/*	Please refer the application note for details									*/
	uint8_t cdetect;

	if (gDetSts_Old[MMC_SLOT0] == MMC_FALSE)
	{
		return TFAT_STA_NODISK | TFAT_STA_NOINIT;
	}
	if (MMC_OK != R_mmc_Chk_Detect(MMC_SLOT0, &cdetect))
	{
		return TFAT_STA_NODISK | TFAT_STA_NOINIT;
	}
	if (cdetect == MMC_FALSE)
	{
		return TFAT_STA_NODISK | TFAT_STA_NOINIT;
	}
	if (gMmcInfo[MMC_SLOT0].Card != MMC_CARD_MMC)
	{
		return TFAT_STA_NOINIT;
	}
	if (gMmcInfo[MMC_SLOT0].WProtect != MMC_FALSE)
	{
		return TFAT_STA_PROTECT;
	}
	return 0;

}

/*""FUNC COMMENT""*******************************************************************
* Function Name : R_tfat_get_fattime
* Include       : r_tfat_sample_software.h
* Declaration   : void R_tfat_get_fattime(void)
* Description   : This function returns the current date and time
* Arguments     : none
* Return value  : uint32_t
* NOTE          : Please DO NOT modify this function. This function is used by the FAT
                : library to get the current date and time during file manipulations.
*""FUNC COMMENT END""****************************************************************/
uint32_t R_tfat_get_fattime(void)
{
	uint32_t tmr;

	/* Disable interrupts */
	DISABLE_IRQ();

	/* Convert the time to store in FAT entry 	*/
	tmr = (((uint32_t)g_rtcYear - 80) << 25);
	tmr |= ((uint32_t)g_rtcMon << 21);
	tmr |= ((uint32_t)g_rtcMday << 16);
	tmr |= ((uint32_t)g_rtcHour << 11);
	tmr |= ((uint32_t)g_rtcMin << 5);
	tmr |= ((uint32_t)g_rtcSec >> 1);

	/* Enable interrupts	*/
	ENABLE_IRQ();

	return tmr;
}

/*-----------------------------------------------------------------------*/
/* Sub module function                                                   */
/*-----------------------------------------------------------------------*/

/*******************************************************************************
* Declaration  : int16_t R_card_insertion_chk(slot_num)
* Function Name: R_card_insertion_chk
* Description  : Check whether a card is inserted.
               : After insertion confirmation, sample program initialize it for MMC
* Argument     : uint8_t slot_num: Slot No.
* Return Value : <0: Error, =0:Removal, >0 : Insertion
*******************************************************************************/
int16_t R_card_insertion_chk(uint8_t slot_num)
{
	uint8_t  DetSts;  /* Detection status */
	int16_t  api_ret;

	/* Check MMC insertion. */
	api_ret = R_mmc_Chk_Detect(slot_num, &DetSts); /* Get status of MMC insertion. */
	if (api_ret < MMC_OK)
	{
		return api_ret;
	}
	if (DetSts != gDetSts_Old[slot_num]) /* Status Changed! */
	{
		if (DetSts == MMC_TRUE) /* Removal -> Insertion */
		{
			gDetChatCnt[slot_num]--;        /* Chattering counter decrement */
			if (gDetChatCnt[slot_num] == 0) /* counter =0                   */
			{
				/* Initialize MMC slot. */
				gDetChatCnt[slot_num] = MMC_INS_CHAT;
				gDetSts_Old[slot_num] = DetSts;
			}
		}
		else
		{
			/* Insertion -> Removal    */
			/* Do not care chattering. */
			gDetChatCnt[slot_num] = MMC_INS_CHAT;
			gDetSts_Old[slot_num] = DetSts;
		}
	}
	else
	{
		/* No change */
		gDetChatCnt[slot_num] = MMC_INS_CHAT;
	}
	return gDetSts_Old[slot_num];
}

/******************************************************************************
* Declaration  : void R_init_card_detect_chat(uint8_t)
* Function Name: R_init_card_detect_chat
* Description  : clear of the chattering counter
* Argument     : = MMC_SLOT_NUM :Initialize all card detect information
*              : < MMC_SLOT_NUM :Initialize specified card detect information
* Return Value : None
******************************************************************************/
void R_init_card_detect_chat(uint8_t drv) /* Initial process */
{
	uint8_t slot_num; /* Slot number */

	if (drv >= MMC_SLOT_NUM)
	{
		for (slot_num = MMC_SLOT0; slot_num < MMC_SLOT_NUM; slot_num++)
		{
			gDetChatCnt[slot_num] = MMC_INS_CHAT; /* Reset the counter of chattering detection.             */
			gDetSts_Old[slot_num] = MMC_FALSE;    /* Set the previous status of detection	to "removal".   */
		}
	}
	else
	{
		gDetChatCnt[drv] = MMC_INS_CHAT; /* Reset the counter of chattering detection.             */
		gDetSts_Old[drv] = MMC_FALSE;    /* Set the previous status of detection	to "removal".   */
	}
}


void _10ms_timer_interrupt(void)
{
	static int count_10ms = 0;

	count_10ms++;
	if (count_10ms > 100)
	{
		count_10ms = 0;
		clock_countup();
	}
}

void clock_countup(void)
{
	static const uint8_t stTime[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
	uint8_t n;

	if (++g_rtcSec >= 60)
	{
		g_rtcSec = 0;
		if (++g_rtcMin >= 60)
		{
			g_rtcMin = 0;
			if (++g_rtcHour >= 24)
			{
				g_rtcHour = 0;
				n = stTime[g_rtcMon - 1];
				if ((n == 28) && !(g_rtcYear & 3))
				{
					n++;
				}
				if (++g_rtcMday > n)
				{
					g_rtcMday = 1;
					if (++g_rtcMon > 12)
					{
						g_rtcMon = 1;
						g_rtcYear++;
					}
				}
			}
		}
	}
}
