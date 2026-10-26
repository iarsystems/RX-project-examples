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
* Copyright (C) 2011 Renesas Electronics Corporation. All rights reserved.
*******************************************************************************/
/*******************************************************************************
* File Name     : r_mmc_sci.c
* Version       : 1.23B
* Device(s)     : RX Family
* Tool-Chain    : RX Standard Toolchain (V.1.1.0.0)
* H/W Platform  : -
* Description	: MMC driver SPI mode SCI communication module
******************************************************************************/
/******************************************************************************
* History       : DD.MM.YYYY Version Description
*               : 22.08.2011 1.23B
******************************************************************************/

#include "r_mtl_com.h"									/* Renesas tool common definitions				*/
#include "r_mmc.h"										/* MMC driver common definitions				*/
#include "r_mmc_sub.h"									/* MMC driver sub module definitions			*/
#include "r_mmc_sfr.h"									/* MMC driver SFR definitions					*/
#include "r_mmc_io.h"									/* MMC driver I/O module definitions			*/

#if defined(MMC_SCI_USED)
/************************************************************************************************/
/*	Definitions																					*/
/************************************************************************************************/
#define MMC_FID							(8UL << 6 | 5UL)

#if defined(MMC_NOCHK_TIMEOUT)
#define MMC_SCI_TX_TIMEOUT_CHK(wait , line)
#define MMC_SCI_TRX_TIMEOUT_CHK(wait , line)
#define MMC_SCI_TRX_DTC_TIMEOUT_CHK(wait , line)
#else	/* #if defined(MMC_NOCHK_TIMEOUT) */
#define MMC_SCI_TX_TIMEOUT_CHK(wait , line)	do											\
{																						\
	wait--;																				\
	if (wait == 0) 								/* Transmission waiting time over	*/	\
	{																					\
		MMC_SCI_TX_DI();						/* SCI transmit disable				*/	\
		R_mtl_Log(MMC_LOG_ERR, 0, MMC_FID, line);										\
		return MMC_ERR_HARD;															\
	}																					\
	R_mtl_wait_lp(MMC_T_SCI_WAIT);														\
}while(0)

#define MMC_SCI_TRX_TIMEOUT_CHK(wait , line)	do										\
{																						\
	wait--;																				\
	if (wait == 0)								/* Transmission waiting time over	*/	\
	{																					\
		MMC_SCI_TRX_DI();						/* SCI transmit disable				*/	\
		R_mtl_Log(MMC_LOG_ERR, 0, MMC_FID, line);										\
		return MMC_ERR_HARD;															\
	}																					\
	R_mtl_wait_lp(MMC_T_SCI_WAIT);														\
}while(0)

#define MMC_SCI_TRX_DTC_TIMEOUT_CHK(wait , line)	do									\
{																						\
	wait--;																				\
	if (wait == 0) 								/* Receive waiting time over		*/	\
	{																					\
		MMC_DTC_TRX_DI();																\
		MMC_DTC_DI();																	\
		MMC_SCI_TRX_DI();					/* SCI transmit&receive disable			*/	\
		R_mtl_Log(MMC_LOG_ERR, 0, MMC_FID, line);									\
		return MMC_ERR_HARD;															\
	}																					\
	R_mtl_wait_lp(MMC_T_SCI_WAIT);														\
}while(0)
#endif	/* #if defined(MMC_NOCHK_TIMEOUT) */

#if defined(MMC_SYNC_ORERR)
#define MMC_SCI_ORERR_CHK(line)	do														\
{																						\
	if (MMC_SCI_ORER == MMC_TRUE) 				/* Overrun error ?					*/	\
	{																					\
		MMC_SCI_TRX_DI();						/* SCI transmit&receive disable		*/	\
		R_mtl_Log(MMC_LOG_ERR, 0, MMC_FID, line);										\
		return MMC_ERR_HARD;															\
	}																					\
}while(0)
#else	/* #if defined(MMC_SYNC_ORERR) */
#define MMC_SCI_ORERR_CHK(line)
#endif	/* #if defined(MMC_SYNC_ORERR) */

#ifdef MMC_DTC_USED
#pragma section DTCTBL
uint32_t 				gDtcVctTbl[256];
MMC_DTCALLOC_FULL_t 	gDtcAlloc[2];
#pragma section
#endif	/* #ifdef MMC_DTC_USED */

/******************************************************************************
* Declaration  : void R_mmc_Init_Sfr(void)
* Function Name: R_mmc_Init_Sfr
* Description  : Initialize SFR to use.
* Argument     : None
* Return Value : None
******************************************************************************/
void R_mmc_Init_Sfr(void)
{
	MMC_SCI_DI();										/* SCI disable setting							*/
}

/******************************************************************************
* Declaration  : vvoid R_mmc_Sync_Enable(uint8_t flag, uint16_t bitrate)
* Function Name: R_mmc_Sync_Enable
* Description  : Enable SIO Module (option) and Set boudrate.
* Argument     : uint8_t  flag : 0=bitrate set ,1=SIO enable + bitrate set
*              : uint16_t bitrate : bitrate
* Return Value : None
******************************************************************************/
void R_mmc_Sync_Enable(uint8_t flag, uint16_t bitrate)
{
	if (flag != 0)
	{
		MMC_SCI_EI();										/* SCI enable setting							*/
	}
	MMC_SCI_BRG_SET(bitrate);			/* It starts in Identification mode				*/
	R_mtl_wait_lp(MMC_T_REGSET_WAIT);
}

/******************************************************************************
* Declaration  : void R_mmc_Sync_Disable(void)
* Function Name: R_mmc_Sync_Disable
* Description  : Disable SIO Module.
* Argument     : None
* Return Value : None
******************************************************************************/
void R_mmc_Sync_Disable(void)
{
	MMC_SCI_DI();
}

/******************************************************************************
* Declaration  : int16_t R_mmc_Sync_MultTx(natural_uint_t TxCnt, uint8_t *pData)
* Function Name: R_mmc_Sync_MultTx
* Description  : Transmit the data for the designated number of bytes consecutively.
* Argument     : natural_uint_t	TxCnt	; Number of send bytes
*              : uint8_t 	*pData	; Send data buffer pointer
* Return Value : MMC_OK				; Success
*              : MMC_ERR_HARD		; Hardware Error
******************************************************************************/
int16_t R_mmc_Sync_MultTx(natural_uint_t TxCnt, uint8_t *pData)
{
#if !defined(MMC_NOCHK_TIMEOUT)
	natural_uint_t		TxWait = MMC_SCI_TX_WAIT;		/* Transmission waiting counter					*/
#endif	/* #if !defined(MMC_NOCHK_TIMEOUT) */
	uint8_t				TxData;

	MMC_SCI_TX_EI();									/* SCI transmit enable							*/

	do 													/* Loop for transmission byte					*/
	{
		TxData = *pData++;
		MMC_BIT_EXCHG(TxData);
		/* Transmission preparation completion check */
		while (MMC_TXNEXT_CHECK())
		{
			MMC_SCI_TX_TIMEOUT_CHK(TxWait , __LINE__);
		}
		/* Transmission data -> Transmission buffer register */
		MMC_TXNEXT_CLR();
		MMC_SCI_TXBUF  = TxData;						/* Transmission data setting					*/
		TxCnt--;
	}
	while (TxCnt != 0);

	/* Final data transmission completion check */
	while (MMC_SCI_TXEND == MMC_FALSE) 				/* Loop for transmission completion				*/
	{
		MMC_SCI_TX_TIMEOUT_CHK(TxWait , __LINE__);
	}

	/* Clear IR bit of IR register */
	MMC_TXNEXT_CLR();								/* Transmission empty flag clear			*/
	MMC_SCI_TX_DI();									/* SCI transmit disable						*/

	return MMC_OK;
}

/******************************************************************************
* Declaration  : int16_t R_mmc_Sync_Rx(natural_uint_t RxCnt, uint8_t *pData)
* Function Name: R_mmc_Sync_Rx
* Description  : Receive the data for the designated number of bytes.
* Argument     : natural_uint_t	RxCnt	; Number of recieve bytes
*              : uint8_t 	*pData	; Recieve data buffer pointer
* Return Value : MMC_OK				; Success
*              : MMC_ERR_HARD		; Hardware Error
******************************************************************************/
int16_t R_mmc_Sync_Rx(natural_uint_t RxCnt, uint8_t *pData)
{
#if !defined(MMC_NOCHK_TIMEOUT)
	natural_uint_t		RxWait = MMC_SCI_RX_WAIT;		/* Receive waiting counter						*/
#endif	/* #if !defined(MMC_NOCHK_TIMEOUT) */
	uint8_t				RxData;

	MMC_SCI_TRX_EI();									/* SCI transmit&receive enable					*/

	if (RxCnt > 0)
	{
		/* Transmission preparation completion check */
		while (MMC_TXNEXT_CHECK()) 				/* Loop for transmission empty					*/
		{
			MMC_SCI_TRX_TIMEOUT_CHK(RxWait , __LINE__);
		}
		/* Receive dummy data -> Transmission buffer register */
		MMC_TXNEXT_CLR();
		MMC_SCI_TXBUF  = MMC_DUMMY_DATA;
		RxCnt--;

		while (RxCnt != 0)
		{
			/* Receive completion check */
			while (MMC_RXNEXT_CHECK())				 		/* Loop for receive completion					*/
			{
				MMC_SCI_ORERR_CHK(__LINE__);
				MMC_SCI_TRX_TIMEOUT_CHK(RxWait , __LINE__);
			}
			/* Receive data -> Receive data buffer */
			MMC_RXNEXT_CLR();								/* Receive completion flag clear				*/
			RxData = MMC_SCI_RXBUF;						/* Receive data taking							*/

			while (MMC_TXNEXT_CHECK())  				/* Loop for transmission empty					*/
			{
				MMC_SCI_TRX_TIMEOUT_CHK(RxWait , __LINE__);
			}
			/* Receive dummy data -> Transmission buffer register */
			MMC_TXNEXT_CLR();
			MMC_SCI_TXBUF  = MMC_DUMMY_DATA;
			MMC_BIT_EXCHG(RxData);
			*pData = RxData;
			pData++;
			RxCnt--;
		}

		while (MMC_RXNEXT_CHECK())  				/* Loop for transmission empty					*/
		{
			MMC_SCI_ORERR_CHK(__LINE__);
			MMC_SCI_TRX_TIMEOUT_CHK(RxWait , __LINE__);
		}
		/* Receive data -> Receive data buffer */
		MMC_RXNEXT_CLR();								/* Receive completion flag clear				*/
		RxData = MMC_SCI_RXBUF;						/* Receive data taking							*/
		MMC_BIT_EXCHG(RxData);
		*pData = RxData;
	}

	/* Clear IR bit of IR register */
	MMC_TXNEXT_CLR();								/* Transmission empty flag clear				*/
	MMC_RXNEXT_CLR();								/* Receive completion flag clear				*/
	MMC_SCI_TRX_DI();									/* SCI transmit&receive disable				*/

	return MMC_OK;
}

/******************************************************************************
* Declaration  : int16_t R_mmc_Sync_DataOut(uint8_t *pData)
* Function Name: R_mmc_Sync_DataOut
* Description  : Transmit data and CRC-CCITT for 1 block.
* Argument     : uint8_t *pData		; Send data buffer pointer
* Return Value : MMC_OK				; Success
*              : MMC_ERR_HARD		; Hardware Error
******************************************************************************/
int16_t R_mmc_Sync_DataOut(uint8_t *pData)
{
#if !defined(MMC_NOCHK_TIMEOUT)
	natural_uint_t			TxWait = MMC_SCI_TX_WAIT;	/* Transmission waiting counter					*/
#endif	/* #if !defined(MMC_NOCHK_TIMEOUT) */
	natural_uint_t			TxCnt;						/* Transmission byte counter					*/
	uint8_t					*pCrcBuf;					/* Transmission CRC data buffer pointer			*/
	uint16_t				CrcBuf;						/* Transmission CRC data buffer					*/
	int16_t					Ret;
	uint8_t					TxData;
	uint8_t					work[MMC_CRC_SIZE];			/* Transmission CRC data buffer					*/

	TxCnt = MMC_BLK_SIZE;

	MMC_CRC_CLR();

	MMC_SCI_TX_EI();									/* SCI transmit enable							*/

	do
	{
		TxData = *pData++;
		MMC_CRC_SET(TxData);							/* CRC-CCITT calculation						*/
		MMC_BIT_EXCHG(TxData);
		/* Transmission preparation completion check */
		while (MMC_TXNEXT_CHECK())
		{
			MMC_SCI_TX_TIMEOUT_CHK(TxWait , __LINE__);
		}
		/* Transmission data -> Transmission buffer register */
		MMC_TXNEXT_CLR();						/* Transmission empty flag clear				*/
		MMC_SCI_TXBUF  = TxData;						/* Transmission data setting					*/
		TxCnt--;
	}
	while (TxCnt != 0);									/* Loop for 1block								*/

	/* Final data transmission completion check */
	while (MMC_SCI_TXEND == MMC_FALSE) 				/* Loop for transmission completion				*/
	{
		MMC_SCI_TX_TIMEOUT_CHK(TxWait , __LINE__);
	}
	/* Clear IR bit of IR register */
	MMC_TXNEXT_CLR();								/* Transmission empty flag clear			*/
	MMC_SCI_TX_DI();									/* SCI transmit disable						*/

	/* Calculation CRC-CCITT data acquisition */
	CrcBuf = R_ExchgCrc();

	/* CRC-CCITT data transmission */
	pCrcBuf = (uint8_t*) & CrcBuf;

	work[0] = *(pCrcBuf + 1);
	work[1] = *(pCrcBuf + 0);
	Ret = R_mmc_Sync_MultTx(MMC_CRC_SIZE, work);
	if (Ret < MMC_OK)
	{
		R_mtl_Log(MMC_LOG_ERR, 0, MMC_FID, __LINE__);
		return Ret;
	}

	return MMC_OK;
}

/******************************************************************************
* Declaration  : int16_t R_mmc_Sync_DataIn(natural_uint_t RxCnt, uint8_t *pData)
* Function Name: R_mmc_Sync_DataIn
* Description  : Receive data for the specified byte and CRC-CCITT, and check CRC-CCITT.
* Argument     : natural_uint_t RxCnt			; Recieve data size
*              : uint8_t 		*pData			; Recieve data storage buffer pointer
* Return Value : MMC_OK							; Success
*              : MMC_ERR_HARD					; Hardware Error
*              : MMC_ERR_CRC					; CRC check error
******************************************************************************/
int16_t R_mmc_Sync_DataIn(natural_uint_t RxCnt, uint8_t *pData)
{
#if !defined(MMC_NOCHK_TIMEOUT)
	natural_uint_t			RxWait = MMC_SCI_RX_WAIT;		/* Receive waiting counter						*/
#endif	/* #if !defined(MMC_NOCHK_TIMEOUT) */
	MMC_EXCHG_SHORT		CrcBuf;							/* Receive CRC-CCITT data buffer				*/
	uint16_t			CrcData;						/* Calculation CRC-CCITT data storage buffer	*/
	int16_t				Ret;
#if !defined(MMC_DTC_USED)
	uint8_t				RxData;
#endif /* #if !defined(MMC_DTC_USED) */
	uint8_t				work[MMC_CRC_SIZE];

	MMC_CRC_CLR();

#if defined(MMC_DTC_USED)

	MMC_DTC_EI();
	MMC_DTC_TRX_EI(pData  , RxCnt);
	MMC_SCI_TRX_EI();									/* SCI transmit&receive enable					*/

	while (gDtcAlloc[1].CRA != 0) 						/* Loop for receive completion					*/
	{
		if ((uint32_t)pData < gDtcAlloc[1].DAR)
		{
			MMC_BIT_EXCHG(*pData);
			MMC_CRC_SET(*pData);						/* CRC-CCITT calculation						*/
			pData++;
		}
		MMC_SCI_TRX_DTC_TIMEOUT_CHK(RxWait , __LINE__);
	}
	while ((uint32_t)pData < gDtcAlloc[1].DAR)
	{
		MMC_BIT_EXCHG(*pData);
		MMC_CRC_SET(*pData);							/* CRC-CCITT calculation						*/
		pData++;
	}

	MMC_SCI_TRX_DI();									/* SCI transmit&receive enable					*/
	MMC_DTC_TRX_DI();
	MMC_DTC_DI();

#else /* #if defined(MMC_DTC_USED) */
	MMC_SCI_TRX_EI();									/* SCI transmit&receive enable					*/

	if (RxCnt > 0)
	{
		while (MMC_TXNEXT_CHECK())
		{
			MMC_SCI_TRX_TIMEOUT_CHK(RxWait , __LINE__);
		}
		/* Receive dummy data -> Transmission buffer register */
		MMC_TXNEXT_CLR();
		MMC_SCI_TXBUF  = MMC_DUMMY_DATA;
		RxCnt--;

		while (RxCnt != 0)
		{
			while (MMC_RXNEXT_CHECK())
			{
				MMC_SCI_ORERR_CHK(__LINE__);
				MMC_SCI_TRX_TIMEOUT_CHK(RxWait , __LINE__);
			}
			/* Receive data -> Receive data buffer */
			MMC_RXNEXT_CLR();
			RxData = MMC_SCI_RXBUF;

			while (MMC_TXNEXT_CHECK())
			{
				MMC_SCI_TRX_TIMEOUT_CHK(RxWait , __LINE__);
			}
			/* Receive dummy data -> Transmission buffer register */
			MMC_TXNEXT_CLR();
			MMC_SCI_TXBUF  = MMC_DUMMY_DATA;

			MMC_BIT_EXCHG(RxData);
			*pData++ = RxData;						/* Receive data taking	*/
			MMC_CRC_SET(RxData);							/* CRC-CCITT calculation						*/
			RxCnt--;
		}

		while (MMC_RXNEXT_CHECK())
		{
			MMC_SCI_ORERR_CHK(__LINE__);
			MMC_SCI_TRX_TIMEOUT_CHK(RxWait , __LINE__);
		}
		/* Receive data -> Receive data buffer */
		MMC_RXNEXT_CLR();
		RxData = MMC_SCI_RXBUF;							/* Receive data taking							*/
		MMC_BIT_EXCHG(RxData);
		*pData = RxData;
		MMC_CRC_SET(RxData);							/* CRC-CCITT calculation						*/
	}
	/* Clear IR bit of IR register */
	MMC_TXNEXT_CLR();									/* Transmission empty flag clear				*/
	MMC_RXNEXT_CLR();									/* Receive completion flag clear				*/
	MMC_SCI_TRX_DI();									/* SCI transmit&receive disable				*/

#endif /* #if defined(MMC_DTC_USED) */

	/* Calculation CRC-CCITT data acquisition */
	CrcData = R_ExchgCrc();

	Ret = R_mmc_Sync_Rx(MMC_CRC_SIZE, work);
	if (Ret < MMC_OK)
	{
		R_mtl_Log(MMC_LOG_ERR, 0, MMC_FID, __LINE__);
		return Ret;
	}

	CrcBuf.uc[1] = work[0];
	CrcBuf.uc[0] = work[1];

	/* CRC check */
	if (CrcData != CrcBuf.us)
	{
		R_mtl_Log(MMC_LOG_ERR, 0, MMC_FID, __LINE__);
		return MMC_ERR_CRC;
	}

	return MMC_OK;
}

#endif	/* #if defined(MMC_SCI_USED) */