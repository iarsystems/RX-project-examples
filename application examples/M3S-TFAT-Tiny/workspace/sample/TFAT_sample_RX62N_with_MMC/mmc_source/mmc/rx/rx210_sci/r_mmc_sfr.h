/*""FILE COMMENT""*******************************************************************
* File Name		: r_mmc_sfr.h
* Version      	: 1.23B
* Device(s)    	: RX210
* Tool-Chain 	: RX Standard Toolchain (V.1.1.0.0)
* H/W Platform 	: -
* Description	: MMC driver SFR RX210-SCI common definitions
*************************************************************************************
* History	: DD.MM.YYYY Version Description
*			: 27.06.2006 Ver1.13
*			: 21.02.2007 Ver1.14
*			: 21.06.2007 Ver1.15
*			: 10.12.2007 Ver1.20
*			: 17.01.2008 Ver1.21
*			: 29.01.2008 Ver1.22
*			: 26.03.2008 Ver1.23
*			: 25.06.2010 Ver1.23A
*			: 22.08.2011 Ver1.23B
*""FILE COMMENT END""***************************************************************/
#ifndef __MMC_SFR_H__
#define __MMC_SFR_H__
#include "r_mmc_user_config.h"

/************************************************************************************************/
/*	Definitions																					*/
/************************************************************************************************/
/*<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/
/*                              Do not change the following data.                               */
/*<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

#define CAT_2(a,b) a##b
#define CAT(a,b) CAT_2(a,b)

#define SCIn	CAT(SCI, MMC_SCI_CHANNEL)
#define _SCIn	CAT(_SCI, MMC_SCI_CHANNEL)
#define MMC_RX	CAT(_SCIn, CAT(_RXI , MMC_SCI_CHANNEL))
#define MMC_TX	CAT(_SCIn, CAT(_TXI , MMC_SCI_CHANNEL))

#define	__IR_BYTE( x )		ICU.IR[ IR ## x ].BYTE
#define	 _IR_BYTE( x )		__IR_BYTE( x )
#define	__DTCE_BYTE( x )	ICU.DTCER[ DTCE ## x ].BYTE
#define	 _DTCE_BYTE( x )	__DTCE_BYTE( x )
#define	__PDR( x,y )		PORT ## x.PDR.BIT.B ## y
#define	 _PDR( x,y )		__PDR( x , y )
#define	__PIDR( x,y )		PORT ## x.PIDR.BIT.B ## y
#define	 _PIDR( x,y )		__PIDR( x , y )
#define	__PODR( x,y )		PORT ## x.PODR.BIT.B ## y
#define	 _PODR( x,y )		__PODR( x , y )
#define	__PMR( x,y )		PORT ## x.PMR.BIT.B ## y
#define	 _PMR( x,y )		__PMR( x , y )
#define	__DSCR( x,y )		PORT ## x.DSCR.BIT.B ## y
#define	 _DSCR( x,y )		__DSCR( x , y )
#define	__PFS( x,y )		MPC.P ## x ## y ## PFS.BYTE
#define	 _PFS( x,y )		__PFS( x , y )

/*----------------------------------------------------------------------------------------------*/
/*	 Define the control port.			 														*/
/*----------------------------------------------------------------------------------------------*/
#define MMC_PFSWE			MPC.PWPR.BIT.PFSWE
#define MMC_B0WI			MPC.PWPR.BIT.B0WI

#if (MMC_SCI_CHANNEL == 0)
#define MMC_DATAI_PORTNO	2		/* MMC DataIn Port No. */
#define MMC_DATAI_BITNO		1		/* MMC DataIn Bit No.  */
#define	MMC_PFS_DATAI_INIT	(uint8_t)0x0A
#define MMC_CLK_PORTNO		2		/* MMC CLK Port No.  */
#define MMC_CLK_BITNO		2		/* MMC CLK Bit  No.  */
#define	MMC_PFS_CLK_INIT	(uint8_t)0x0A
#define MMC_DATAO_PORTNO	2		/* MMC DataOut Port No. */
#define MMC_DATAO_BITNO		0		/* MMC DataOut Bit No.  */
#define	MMC_PFS_DATAO_INIT	(uint8_t)0x0A


#elif (MMC_SCI_CHANNEL == 1)
#if (MMC_SCI_RXD_PIN == 'A')
#define MMC_DATAI_PORTNO	1		/* MMC DataIn Port No. */
#define MMC_DATAI_BITNO		5		/* MMC DataIn Bit No.  */
#define	MMC_PFS_DATAI_INIT	(uint8_t)0x0A
#elif (MMC_SCI_RXD_PIN == 'B')
#define MMC_DATAI_PORTNO	3		/* MMC DataIn Port No. */
#define MMC_DATAI_BITNO		0		/* MMC DataIn Bit No.  */
#define	MMC_PFS_DATAI_INIT	(uint8_t)0x0A
#endif	/* MMC_SCI_RXD_PIN */

#if (MMC_SCI_SCK_PIN == 'A')
#define MMC_CLK_PORTNO		1		/* MMC CLK Port No.  */
#define MMC_CLK_BITNO		6		/* MMC CLK Bit  No.  */
#define	MMC_PFS_CLK_INIT	(uint8_t)0x0A
#elif (MMC_SCI_SCK_PIN == 'B')
#define MMC_CLK_PORTNO		2		/* MMC CLK Port No.  */
#define MMC_CLK_BITNO		7		/* MMC CLK Bit  No.  */
#define	MMC_PFS_CLK_INIT	(uint8_t)0x0A
#endif	/* MMC_SCI_SCK_PIN */

#if (MMC_SCI_TXD_PIN == 'A')
#define MMC_DATAO_PORTNO	1		/* MMC DataOut Port No. */
#define MMC_DATAO_BITNO		7		/* MMC DataOut Bit No.  */
#define	MMC_PFS_DATAO_INIT	(uint8_t)0x0A
#elif (MMC_SCI_TXD_PIN == 'B')
#define MMC_DATAO_PORTNO	2		/* MMC DataOut Port No. */
#define MMC_DATAO_BITNO		6		/* MMC DataOut Bit No.  */
#define	MMC_PFS_DATAO_INIT	(uint8_t)0x0A
#endif	/* MMC_SCI_TXD_PIN */


#elif (MMC_SCI_CHANNEL == 5)
#if (MMC_SCI_RXD_PIN == 'A')
#define MMC_DATAI_PORTNO	A		/* MMC DataIn Port No. */
#define MMC_DATAI_BITNO		2		/* MMC DataIn Bit No.  */
#define	MMC_PFS_DATAI_INIT	(uint8_t)0x0A
#elif (MMC_SCI_RXD_PIN == 'B')
#define MMC_DATAI_PORTNO	A		/* MMC DataIn Port No. */
#define MMC_DATAI_BITNO		3		/* MMC DataIn Bit No.  */
#define	MMC_PFS_DATAI_INIT	(uint8_t)0x0A
#elif (MMC_SCI_RXD_PIN == 'C')
#define MMC_DATAI_PORTNO	C		/* MMC DataIn Port No. */
#define MMC_DATAI_BITNO		2		/* MMC DataIn Bit No.  */
#define	MMC_PFS_DATAI_INIT	(uint8_t)0x0A
#endif	/* MMC_SCI_RXD_PIN */

#if (MMC_SCI_SCK_PIN == 'A')
#define MMC_CLK_PORTNO		A		/* MMC CLK Port No.  */
#define MMC_CLK_BITNO		1		/* MMC CLK Bit  No.  */
#define	MMC_PFS_CLK_INIT	(uint8_t)0x0A
#elif (MMC_SCI_SCK_PIN == 'B')
#define MMC_CLK_PORTNO		C		/* MMC CLK Port No.  */
#define MMC_CLK_BITNO		1		/* MMC CLK Bit  No.  */
#define	MMC_PFS_CLK_INIT	(uint8_t)0x0A
#elif (MMC_SCI_SCK_PIN == 'C')
#define MMC_CLK_PORTNO		C		/* MMC CLK Port No.  */
#define MMC_CLK_BITNO		4		/* MMC CLK Bit  No.  */
#define	MMC_PFS_CLK_INIT	(uint8_t)0x0A
#endif	/* MMC_SCI_SCK_PIN */

#if (MMC_SCI_TXD_PIN == 'A')
#define MMC_DATAO_PORTNO	A		/* MMC DataOut Port No. */
#define MMC_DATAO_BITNO		4		/* MMC DataOut Bit No.  */
#define	MMC_PFS_DATAO_INIT	(uint8_t)0x0A
#elif (MMC_SCI_TXD_PIN == 'B')
#define MMC_DATAO_PORTNO	C		/* MMC DataOut Port No. */
#define MMC_DATAO_BITNO		3		/* MMC DataOut Bit No.  */
#define	MMC_PFS_DATAO_INIT	(uint8_t)0x0A
#endif	/* MMC_SCI_TXD_PIN */

#elif (MMC_SCI_CHANNEL == 6)
#if (MMC_SCI_RXD_PIN == 'A')
#define MMC_DATAI_PORTNO	3		/* MMC DataIn Port No. */
#define MMC_DATAI_BITNO		3		/* MMC DataIn Bit No.  */
#define	MMC_PFS_DATAI_INIT	(uint8_t)0x0B
#elif (MMC_SCI_RXD_PIN == 'B')
#define MMC_DATAI_PORTNO	B		/* MMC DataIn Port No. */
#define MMC_DATAI_BITNO		0		/* MMC DataIn Bit No.  */
#define	MMC_PFS_DATAI_INIT	(uint8_t)0x0B
#endif	/* MMC_SCI_RXD_PIN */

#if (MMC_SCI_SCK_PIN == 'A')
#define MMC_CLK_PORTNO		3		/* MMC CLK Port No.  */
#define MMC_CLK_BITNO		4		/* MMC CLK Bit  No.  */
#define	MMC_PFS_CLK_INIT	(uint8_t)0x0B
#elif (MMC_SCI_SCK_PIN == 'B')
#define MMC_CLK_PORTNO		B		/* MMC CLK Port No.  */
#define MMC_CLK_BITNO		3		/* MMC CLK Bit  No.  */
#define	MMC_PFS_CLK_INIT	(uint8_t)0x0B
#endif	/* MMC_SCI_SCK_PIN */

#if (MMC_SCI_TXD_PIN == 'A')
#define MMC_DATAO_PORTNO	3		/* MMC DataOut Port No. */
#define MMC_DATAO_BITNO		2		/* MMC DataOut Bit No.  */
#define	MMC_PFS_DATAO_INIT	(uint8_t)0x0B
#elif (MMC_SCI_TXD_PIN == 'B')
#define MMC_DATAO_PORTNO	B		/* MMC DataOut Port No. */
#define MMC_DATAO_BITNO		1		/* MMC DataOut Bit No.  */
#define	MMC_PFS_DATAO_INIT	(uint8_t)0x0B
#endif	/* MMC_SCI_TXD_PIN */


#elif (MMC_SCI_CHANNEL == 8)
#define MMC_DATAI_PORTNO	C		/* MMC DataIn Port No. */
#define MMC_DATAI_BITNO		6		/* MMC DataIn Bit No.  */
#define	MMC_PFS_DATAI_INIT	(uint8_t)0x0A
#define MMC_CLK_PORTNO		C		/* MMC CLK Port No.  */
#define MMC_CLK_BITNO		5		/* MMC CLK Bit  No.  */
#define	MMC_PFS_CLK_INIT	(uint8_t)0x0A
#define MMC_DATAO_PORTNO	C		/* MMC DataOut Port No. */
#define MMC_DATAO_BITNO		7		/* MMC DataOut Bit No.  */
#define	MMC_PFS_DATAO_INIT	(uint8_t)0x0A


#elif (MMC_SCI_CHANNEL == 9)
#define MMC_DATAI_PORTNO	B		/* MMC DataIn Port No. */
#define MMC_DATAI_BITNO		6		/* MMC DataIn Bit No.  */
#define	MMC_PFS_DATAI_INIT	(uint8_t)0x0A
#define MMC_CLK_PORTNO		B		/* MMC CLK Port No.  */
#define MMC_CLK_BITNO		5		/* MMC CLK Bit  No.  */
#define	MMC_PFS_CLK_INIT	(uint8_t)0x0A
#define MMC_DATAO_PORTNO	B		/* MMC DataOut Port No. */
#define MMC_DATAO_BITNO		7		/* MMC DataOut Bit No.  */
#define	MMC_PFS_DATAO_INIT	(uint8_t)0x0A


#elif (MMC_SCI_CHANNEL == 12)
#define MMC_DATAI_PORTNO	E		/* MMC DataIn Port No. */
#define MMC_DATAI_BITNO		2		/* MMC DataIn Bit No.  */
#define	MMC_PFS_DATAI_INIT	(uint8_t)0x0C
#define MMC_CLK_PORTNO		E		/* MMC CLK Port No.  */
#define MMC_CLK_BITNO		0		/* MMC CLK Bit  No.  */
#define	MMC_PFS_CLK_INIT	(uint8_t)0x0C
#define MMC_DATAO_PORTNO	E		/* MMC DataOut Port No. */
#define MMC_DATAO_BITNO		1		/* MMC DataOut Bit No.  */
#define	MMC_PFS_DATAO_INIT	(uint8_t)0x0C
#endif	/* MMC_SCI_CHANNEL */

#define MMC_PDR_DATAI		 _PDR( MMC_DATAI_PORTNO , MMC_DATAI_BITNO )		/* MMC DataIn	*/
#define MMC_PIDR_DATAI		_PIDR( MMC_DATAI_PORTNO , MMC_DATAI_BITNO )		/* MMC DataIn	*/
#define MMC_PMR_DATAI		 _PMR( MMC_DATAI_PORTNO , MMC_DATAI_BITNO )		/* MMC DataIn	*/
#define MMC_PFS_DATAI		 _PFS( MMC_DATAI_PORTNO , MMC_DATAI_BITNO )		/* MMC DataIn	*/
#define MMC_PDR_CLK			 _PDR( MMC_CLK_PORTNO , MMC_CLK_BITNO )			/* MMC CLK		*/
#define MMC_PODR_CLK		_PODR( MMC_CLK_PORTNO , MMC_CLK_BITNO )			/* MMC CLK		*/
#define MMC_PMR_CLK			 _PMR( MMC_CLK_PORTNO , MMC_CLK_BITNO )			/* MMC CLK		*/
#define MMC_DSCR_CLK		_DSCR( MMC_CLK_PORTNO , MMC_CLK_BITNO )			/* MMC CLK		*/
#define MMC_PFS_CLK			 _PFS( MMC_CLK_PORTNO , MMC_CLK_BITNO )			/* MMC CLK		*/
#define MMC_PDR_DATAO		 _PDR( MMC_DATAO_PORTNO , MMC_DATAO_BITNO )		/* MMC DataOut	*/
#define MMC_PODR_DATAO		_PODR( MMC_DATAO_PORTNO , MMC_DATAO_BITNO )		/* MMC DataOut	*/
#define MMC_PMR_DATAO		 _PMR( MMC_DATAO_PORTNO , MMC_DATAO_BITNO )		/* MMC DataOut	*/
#define MMC_DSCR_DATAO		_DSCR( MMC_DATAO_PORTNO , MMC_DATAO_BITNO )		/* MMC DataOut	*/
#define MMC_PFS_DATAO		 _PFS( MMC_DATAO_PORTNO , MMC_DATAO_BITNO )		/* MMC DataOut	*/

#define MMC_PDR_CS0			 _PDR(MMC_CS0_PORTNO , MMC_CS0_BITNO )			/* MMC CS0		(Negative-true logic)	*/
#define MMC_PODR_CS0		_PODR(MMC_CS0_PORTNO , MMC_CS0_BITNO )			/* MMC CS0		(Negative-true logic)	*/
#define MMC_PDR_DETECT0		 _PDR(MMC_DETECT0_PORTNO , MMC_DETECT0_BITNO )	/* MMC DETECT0	(Negative-true logic)	*/
#define MMC_PIDR_DETECT0	_PIDR(MMC_DETECT0_PORTNO , MMC_DETECT0_BITNO )	/* MMC DETECT0	(Negative-true logic)	*/

#if (MMC_SLOT_NUM > 1)
#define MMC_PDR_CS1			 _PDR(MMC_CS1_PORTNO , MMC_CS1_BITNO )			/* MMC CS1		(Negative-true logic)	*/
#define MMC_PODR_CS1		_PODR(MMC_CS1_PORTNO , MMC_CS1_BITNO )			/* MMC CS1		(Negative-true logic)	*/
#define MMC_PDR_DETECT1		 _PDR(MMC_DETECT1_PORTNO , MMC_DETECT1_BITNO )	/* MMC DETECT1	(Negative-true logic)	*/
#define MMC_PIDR_DETECT1	_PIDR(MMC_DETECT1_PORTNO , MMC_DETECT1_BITNO )	/* MMC DETECT1	(Negative-true logic)	*/
#endif	/* #if (MMC_SLOT_NUM > 1) */

/*----------- Definitions of resource used -----------*/
#define MMC_SCI_USED										/* SCI used								*/
#define MMC_SYNC_LSBF										/* SCI LSB first						*/

#ifdef MMC_OPTION_1
#define MMC_CRC_SOFT										/* Encode by soft CRC (ROM table used)	*/
#endif	/* #ifdef MMC_OPTION_1 */

#ifdef MMC_OPTION_2
#define MMC_CRC_HARD										/* Encode by hard CRC					*/
#define MMC_CRC_MSBF										/* CRC MSB first						*/
#endif	/* #ifdef MMC_OPTION_2 */

#ifdef MMC_OPTION_3
#define MMC_CRC_SOFT										/* Encode by soft CRC (ROM table used)	*/
#define MMC_DTC_USED										/* DTC Used (Read Only) 				*/
#endif	/* #ifdef MMC_OPTION_3 */

#ifdef MMC_OPTION_4
#define MMC_CRC_HARD										/* Encode by hard CRC					*/
#define MMC_CRC_MSBF										/* CRC MSB first						*/
#define MMC_DTC_USED										/* DTC Used (Read Only) 				*/
#endif	/* #ifdef MMC_OPTION_4 */


/*----------------- PRCR definitions -----------------*/
#define MMC_PRCR		SYSTEM.PRCR.WORD
#define MMC_PRCR_EI1	(uint16_t)0xA500
#define MMC_PRCR_EI2	(uint16_t)0xA502
#define MMC_PRCR_INIT	(uint16_t)0x0000

#define MMC_PRCR_EI() do{ 			\
	MMC_PRCR = MMC_PRCR_EI1;		\
	MMC_PRCR = MMC_PRCR_EI2;		\
}while(0)

#define MMC_PRCR_DI() do{ 			\
	MMC_PRCR = MMC_PRCR_INIT;		\
}while(0)

/*----------------- SCI definitions -----------------*/
#define MMC_MSTPCR_SCI			_MSTP(_SCIn)				/* SCI Module stop setting 				*/

#define MMC_SCI_SMR				SCIn.SMR.BYTE				/* Serial mode register					*/
#define MMC_SCI_SCR				SCIn.SCR.BYTE				/* Serial control register				*/
#define MMC_SCI_SSR				SCIn.SSR.BYTE				/* Serial status register				*/
#define MMC_SCI_SCMR			SCIn.SCMR.BYTE				/* Smart card mode register				*/
#define MMC_SCI_BRR				SCIn.BRR					/* Bit rate register					*/
#define MMC_SCI_SEMR			SCIn.SEMR.BYTE				/* Serial extend mode register			*/
#define MMC_SCI_TXBUF			SCIn.TDR					/* SCI Transmit FIFO data register		*/
#define MMC_SCI_RXBUF			SCIn.RDR					/* SCI Receive FIFO data register		*/

#define MMC_SCI_ORER			SCIn.SSR.BIT.ORER			/* SCI Overrun error flag				*/
#define MMC_SCI_PER				SCIn.SSR.BIT.PER			/* SCI Parity error flag				*/
#define MMC_SCI_FER				SCIn.SSR.BIT.FER			/* SCI Framing error flag				*/
#define MMC_SCI_TXEND			SCIn.SSR.BIT.TEND			/* SCI Transmit end flag				*/
#define MMC_SCI_TXNEXT			_IR_BYTE(MMC_TX)			/* SCI Transmit data empty				*/
#define MMC_SCI_RXNEXT			_IR_BYTE(MMC_RX)			/* SCI Receive data full				*/
#define MMC_VECT_TXNEXT			_VECT(MMC_TX)				/* SCI Transmit data empty vector number*/
#define MMC_VECT_RXNEXT			_VECT(MMC_RX)				/* SCI Receive data full vector number	*/
#define MMC_SCI_TXNEXT_ISEL		_DTCE_BYTE(MMC_TX)			/* SCI Transmit data empty interrupt select		*/
#define MMC_SCI_RXNEXT_ISEL		_DTCE_BYTE(MMC_RX)			/* SCI Receive data full interrupt select		*/
#define MMC_SCI_TXNEXT_IER		_IEN(MMC_TX)				/* SCI Transmit data interrupt enable flag		*/
#define MMC_SCI_RXNEXT_IER		_IEN(MMC_RX)				/* SCI Receive data full interrupt enable flag	*/

/* SCI setting */
#define MMC_USMR_NOUSE			(uint8_t)0x80
/*	10000000B	*/	/* SMR SCI unused setting	(Set after TE=RE=0.)	*/
/*	||||||++---------- Clock select			: Pclk clock				*/
/*	|||||+------------ Reserved				: Writes 0.					*/
/*	|++++------------- Handle both asynchronous mode only				*/
/*	+----------------- Communication mode	: Clocked synchronous mode	*/
#define MMC_USCR_NOUSE			(uint8_t)0x00
/*	00000000B	*/	/* SCR SCI unused setting	(Set after TE=RE=0.)	*/
/*	||||||++---------- Clock enable			: Internal clock			*/
/*	|||||+------------ Transimit end interrupt enable					*/
/*	||||+------------- Reserved				: Writes 0.					*/
/*	|||+-------------- Receive enable		: Disable					*/
/*	||+--------------- Transmit enable		: Disable					*/
/*	|+---------------- RXI/ERI enable		: DI						*/
/*	+----------------- TXI enable			: DI						*/
#define MMC_USMR_INIT			(uint8_t)0x80
/*	10000000B	*/	/* SMR SCI unused setting	(Set after TE=RE=0.)	*/
/*	||||||++---------- Clock select			: Pclk clock				*/
/*	|||||+------------ Reserved				: Writes 0.					*/
/*	|++++------------- Handle both asynchronous mode only				*/
/*	+----------------- Communication mode	: Clocked synchronous mode	*/
#define MMC_USCR_INIT			(uint8_t)0x00
/*	00000000B	*/	/* SCR SCI unused setting (Set after TE=RE=0.)		*/
/*	||||||++---------- Clock enable			: Internal clock			*/
/*	|||||+------------ Transimit end interrupt enable					*/
/*	||||+------------- Reserved				: Writes 0.					*/
/*	|||+-------------- Receive enable		: Disable					*/
/*	||+--------------- Transmit enable		: Disable					*/
/*	|+---------------- RXI/ERI enable		: DI						*/
/*	+----------------- TXI enable			: DI						*/
#define MMC_USSR_INIT			(uint8_t)0xC0
/*	11000000B	*/	/* SSR initial setting								*/
/*	||||||++---------- Reserved				: Writes 0.					*/
/*	|||||+------------ Transmit end			: Clear						*/
/*	||||+------------- Parity error			: Clear						*/
/*	|||+-------------- Framing error		: Clear						*/
/*	||+--------------- Overrun error		: Clear						*/
/*	++---------------- Reserved				: Writes 1.					*/
#define MMC_USCMR_INIT			(uint8_t)0x72
/*	01110010B	*/	/* SCMR initial setting		(Set after TE=RE=0.)	*/
/*	|||||||+---------- Serial communication interface mode				*/
/*	||||||+----------- Reserved				: Writes 1.					*/
/*	|||||+------------ Handle both smartcard interface mode only		*/
/*	||||+------------- LSB first										*/
/*	|+++-------------- Reserved				: Writes 1.					*/
/*	+----------------- Handle both smartcard interface mode only		*/
#define MMC_USEMR_INIT			(uint8_t)0x00
/*	00000000B	*/	/* SEMR initial setting		(Set after TE=RE=0.)	*/
/*	|||||||+---------- Handle both asynchronous mode only				*/
/*	||||+++----------- Reserved				: Writes 0.					*/
/*	|||+-------------- Handle both asynchronous mode only				*/
/*	+++--------------- Reserved				: Writes 0.					*/

#define MMC_USCR_TX				(uint8_t)0xA0
/*	10100000B	*/	/* SCR transmit setting								*/
/*	||||||++---------- Clock enable			: Internal clock			*/
/*	|||||+------------ Transimit end interrupt enable					*/
/*	||||+------------- Reserved				: Writes 0.					*/
/*	|||+-------------- Receive enable		: Disable					*/
/*	||+--------------- Transmit enable		: Enable					*/
/*	|+---------------- RXI/ERI enable		: DI						*/
/*	+----------------- TXI enable			: EI						*/

#define MMC_USCR_TRX			(uint8_t)0xF0
/*	11110000B	*/	/* SCR transmit&receive setting						*/
/*	||||||++---------- Clock enable			: Internal clock			*/
/*	|||||+------------ Transimit end interrupt enable					*/
/*	||||+------------- Reserved				: Writes 0.					*/
/*	|||+-------------- Receive enable		: Enable					*/
/*	||+--------------- Transmit enable		: Enable					*/
/*	|+---------------- RXI/ERI enable		: EI						*/
/*	+----------------- TXI enable			: EI						*/

#define MMC_SCI_TXNEXT_MASK		(uint8_t)0x01
/*	00000001B	*/	/* Interrupt Request Flag Check						*/
/*	|||||||+---------- Interrupt Status Flag : Check					*/
/*	+++++++----------- Reserved				 : No Check.				*/

#define MMC_SCI_TXNEXT_OFF		(uint8_t)0x00
/*	00000000B	*/	/* Interrupt Request Flag Check						*/
/*	|||||||+---------- Interrupt Status Flag : Clear					*/
/*	+++++++----------- Reserved				 : Writes 0.				*/

#define MMC_SCI_RXNEXT_MASK		(uint8_t)0x01
/*	00000001B	*/	/* Interrupt Request Flag Check						*/
/*	|||||||+---------- Interrupt Status Flag : Check					*/
/*	+++++++----------- Reserved				 : No Check.				*/

#define MMC_SCI_RXNEXT_OFF		(uint8_t)0x00
/*	00000000B	*/	/* Interrupt Request Flag Check						*/
/*	|||||||+---------- Interrupt Status Flag : Clear					*/
/*	+++++++----------- Reserved				 : Writes 0.				*/

#define MMC_DUMMY_DATA			(uint8_t)0xFF				/* SCI dummy data for receiving data	*/

/*----------------- CRC definitions ------------------*/
#ifdef MMC_CRC_HARD
#define MMC_CRCC				CRC.CRCCR.BYTE				/* CRC control register					*/
#define MMC_CRCIN				CRC.CRCDIR					/* CRC input register					*/
#define MMC_CRCD				CRC.CRCDOR					/* CRC data register					*/
#define MMC_MSTPCR_CRC			SYSTEM.MSTPCRB.BIT.MSTPB23	/* CRC Module stop setting 				*/

#endif	/* #ifdef MMC_CRC_HARD */

#ifdef MMC_DTC_USED
#define	MMC_DTCCR				DTC.DTCCR.BYTE
#define	MMC_DTCVBR				DTC.DTCVBR
#define	MMC_DTCADMOD			DTC.DTCADMOD.BYTE
#define	MMC_DTCST				DTC.DTCST.BYTE
#define MMC_MSTPCR_DTC			SYSTEM.MSTPCRA.BIT.MSTPA28	/* DTC Module stop setting 				*/

#define MMC_DTCCR_INIT1			(uint8_t)0x00
/*	00000000B	*/	/* DTCCR initial setting						*/
/*	|||||||+---------- Transfer Stop Flag	: Read only 			*/
/*	|||||++----------- Reserved				: Writes 0.				*/
/*	||||+------------- Chain after repeat	: Disable				*/
/*	|||+-------------- Read Skip			: Disable				*/
/*	+++--------------- Reserved				: Writes 0.				*/

#define MMC_DTCCR_INIT2			(uint8_t)0x10
/*	00010000B	*/	/* DTCCR initial setting (Set after DTC table.)	*/
/*	|||||||+---------- Transfer Stop Flag	: Read only 			*/
/*	|||||++----------- Reserved				: Writes 0.				*/
/*	||||+------------- Chain after repeat	: Disable				*/
/*	|||+-------------- Read skip			: Enable				*/
/*	+++--------------- Reserved				: Writes 0.				*/

#define MMC_DTCADOMOD_INIT		(uint8_t)0x00
/*	00000000B	*/	/* DTCADOMOD initial setting					*/
/*	|||||||+---------- Short-Address Mode 	: Full-address mode		*/
/*	+++++++----------- Reserved				: Writes 0.				*/

#define MMC_DTCST_INIT1			(uint8_t)0x00
/*	00000000B	*/	/* DTCST initial setting (Set after DTC table.) */
/*	|||||||+---------- DTC Module Start 	: DTC module stop		*/
/*	+++++++----------- Reserved				: Writes 0.				*/

#define MMC_DTCST_INIT2			(uint8_t)0x01
/*	00000001B	*/	/* DTCST initial setting						*/
/*	|||||||+---------- DTC Module Start 	: DTC module start		*/
/*	+++++++----------- Reserved				: Writes 0.				*/


#define	MMC_MRA_TX_EI 			(uint8_t)0x00
/*	00000000B	*/	/* DTC Mode Register A for TX						*/
/*	||||||++---------- Reserved 				: Writes 0.				*/
/*	||||++------------ Source Addressing Mode	: Fixed					*/
/*	||++-------------- Data Transfer Size		: Byte transfer			*/
/*	++---------------- DTC Mode					: Normal transfer mode	*/

#define	MMC_MRB_TX_EI 			(uint8_t)0x00
/*	00000000B	*/	/* DTC Mode Register B for TX						*/
/*	||||||++---------- Reserved 			: Writes 0.					*/
/*	||||++------------ Destination Addressing Mode	: Fixed				*/
/*	|||+-------------- Transfer Mode Select	: Don't Care (Normal mode)	*/
/*	||+--------------- Interrupt Select		: When specified data transfer is completed	*/
/*	|+---------------- Chain Transfer Select: Don't Care (Chain disable)*/
/*	+----------------- Chain Transfer Enable: Disable					*/

#define	MMC_MRA_RX_EI			(uint8_t)0x00
/*	00000000B	*/	/* DTC Mode Register A for RX						*/
/*	||||||++---------- Reserved 				: Writes 0.				*/
/*	||||++------------ Source Addressing Mode	: Fixed					*/
/*	||++-------------- Data Transfer Size		: Byte transfer			*/
/*	++---------------- DTC Mode					: Normal transfer mode	*/

#define	MMC_MRB_RX_EI 			(uint8_t)0x08
/*	00001000B	*/	/* DTC Mode Register B for RX						*/
/*	||||||++---------- Reserved 			: Writes 0.					*/
/*	||||++------------ Destination Addressing Mode	: Increment			*/
/*	|||+-------------- Transfer Mode Select	: Don't Care (Normal mode)	*/
/*	||+--------------- Interrupt Select		: When specified data transfer is completed	*/
/*	|+---------------- Chain Transfer Select: Don't Care (Chain disable)*/
/*	+----------------- Chain Transfer Enable: Disable					*/

#define	MMC_ISEL_DTC  (uint8_t)0x01
/*	00000001B	*/	/* DTC Activation Setting							*/
/*	|||||||+---------- DTC Activation Enable : Enable					*/
/*	+++++++----------- Reserved				 : Writes 0.				*/


#endif	/* #ifdef MMC_DTC_USED */

/************************************************************************************************/
/*	Unions																						*/
/************************************************************************************************/

/************************************************************************************************/
/*	Structs																						*/
/************************************************************************************************/

/************************************************************************************************/
/*	Externs																						*/
/************************************************************************************************/

/************************************************************************************************/
/*	Macros																						*/
/************************************************************************************************/
/*------------- Driver use port control --------------*/
#if (MMC_SLOT_NUM == 1)
/* !!!! The other Ports are set to input mode. !!!! */
#define MMC_IO_INIT() do {								/* Driver use terminal initial setting	*/	\
	MMC_B0WI = MMC_FALSE;								/* PFSWE bit write enable				*/	\
	MMC_PFSWE = MMC_TRUE;								/* PmnPFS Register write enable			*/	\
	MMC_PDR_DATAI   = MMC_IN;							/* DataIn			Input				*/	\
	MMC_PMR_DATAI   = MMC_LOW;							/* DataIn			Peripheral disable	*/	\
	MMC_PFS_DATAI	= MMC_PFS_DATAI_INIT;				/* DataIn 			Pin Function Select	*/	\
	MMC_PDR_DATAO   = MMC_IN;							/* DataOut			Input				*/	\
	MMC_PMR_DATAO   = MMC_LOW;							/* DataOut			Peripheral disable	*/	\
	MMC_PFS_DATAO	= MMC_PFS_DATAO_INIT;				/* DataOut 			Pin Function Select	*/	\
	MMC_PDR_CLK   	= MMC_IN;							/* CLK				Input				*/	\
	MMC_PMR_CLK 	= MMC_LOW;							/* CLK				Peripheral disable	*/	\
	MMC_PFS_CLK		= MMC_PFS_CLK_INIT;					/* CLK 				Pin Function Select	*/	\
	MMC_PFSWE 		= MMC_FALSE;						/* PmnPFS Register write disable		*/	\
	MMC_B0WI 		= MMC_TRUE;							/* PFSWE bit write disable				*/	\
	MMC_PMR_DATAI 	= MMC_HI;							/* DataIn			Peripheral enable	*/	\
	MMC_PMR_DATAO	= MMC_HI;							/* DataOut			Peripheral enable	*/	\
	MMC_PMR_CLK 	= MMC_HI;							/* CLK				Peripheral enable	*/	\
	MMC_DSCR_DATAO 	= MMC_HI;							/* DataOut			High-drive output	*/	\
	MMC_DSCR_CLK 	= MMC_HI;							/* CLK				High-drive output	*/	\
	MMC_PODR_CS0    = MMC_HI;							/* CS0				"H"					*/	\
	MMC_PDR_CS0     = MMC_OUT;							/* CS0				Output				*/	\
	MMC_PODR_CS0    = MMC_HI;							/* CS0				"H"					*/	\
	MMC_PDR_DETECT0 = MMC_IN;							/* DETECT0			Input				*/	\
} while (0)
#endif	/* #if (MMC_SLOT_NUM == 1) */

#if (MMC_SLOT_NUM > 1)
/* !!!! The other Ports are set to input mode. !!!! */
#define MMC_IO_INIT() do {								/* Driver use terminal initial setting	*/	\
	MMC_B0WI = MMC_FALSE;\
	MMC_PFSWE = MMC_TRUE;\
	MMC_PDR_DATAI   = MMC_IN;							/* DataIn			Input				*/	\
	MMC_PMR_DATAI   = MMC_LOW;							/* DataIn			Peripheral disable	*/	\
	MMC_PFS_DATAI	= MMC_PFS_DATAI_INIT;				/* DataIn 			Pin Function Select	*/	\
	MMC_PDR_DATAO   = MMC_IN;							/* DataOut			Input				*/	\
	MMC_PMR_DATAO   = MMC_LOW;							/* DataOut			Peripheral disable	*/	\
	MMC_PFS_DATAO	= MMC_PFS_DATAO_INIT;				/* DataOut 			Pin Function Select	*/	\
	MMC_PDR_CLK   	= MMC_IN;							/* CLK				Input				*/	\
	MMC_PMR_CLK 	= MMC_LOW;							/* CLK				Peripheral disable	*/	\
	MMC_PFS_CLK		= MMC_PFS_CLK_INIT;					/* CLK 				Pin Function Select	*/	\
	MMC_PFSWE 		= MMC_FALSE;						/* PmnPFS Register write disable		*/	\
	MMC_B0WI 		= MMC_TRUE;							/* PFSWE bit write disable				*/	\
	MMC_PMR_DATAI 	= MMC_HI;							/* DataIn			Peripheral enable	*/	\
	MMC_PMR_DATAO 	= MMC_HI;							/* DataOut			Peripheral enable	*/	\
	MMC_PMR_CLK 	= MMC_HI;							/* CLK				Peripheral enable	*/	\
	MMC_DSCR_DATAO 	= MMC_HI;							/* DataOut			High-drive output	*/	\
	MMC_DSCR_CLK 	= MMC_HI;							/* CLK				High-drive output	*/	\
	MMC_PODR_CS0    = MMC_HI;							/* CS0				"H"					*/	\
	MMC_PDR_CS0     = MMC_OUT;							/* CS0				Output				*/	\
	MMC_PODR_CS0    = MMC_HI;							/* CS0				"H"					*/	\
	MMC_PDR_DETECT0 = MMC_IN;							/* DETECT0			Input				*/	\
	MMC_PODR_CS1    = MMC_HI;							/* CS1				"H"					*/	\
	MMC_PDR_CS1     = MMC_OUT;							/* CS1				Output				*/	\
	MMC_PODR_CS1    = MMC_HI;							/* CS1				"H"					*/	\
	MMC_PDR_DETECT1 = MMC_IN;							/* DETECT1			Input				*/	\
} while (0)
#endif	/* #if (MMC_SLOT_NUM > 1) */

/*------------------ DataIn control ------------------*/
#define MMC_DATAI_INIT() do {							/* DataIn initial setting				*/	\
} while (0)

/*----------------- DataOut control ------------------*/
#define MMC_DATAO_INIT() do {							/* DataOut initial setting				*/	\
} while (0)

#define MMC_DATAO_OPEN() do {							/* DataOut open setting					*/	\
} while (0)

/*------------------- CLK control --------------------*/
#define MMC_CLK_INIT() do {								/* CLK initial setting					*/	\
} while (0)

#define MMC_CLK_OPEN() do {								/* CLK open setting						*/	\
} while (0)

/*------------------ DETECT control ------------------*/
#if (MMC_SLOT_NUM == 1)
#define MMC_DETECT_INIT(SlotNo) do {					/* DETECT initial setting				*/	\
	MMC_PDR_DETECT0     = MMC_IN;						/* DETECT			Input				*/	\
} while (0)

#define MMC_GET_DETECT(SlotNo, Lv) do {					/* DETECT input control					*/	\
	Lv = MMC_PIDR_DETECT0;																			\
} while (0)
#endif	/* #if (MMC_SLOT_NUM == 1) */

#if (MMC_SLOT_NUM > 1)
#define MMC_DETECT_INIT(SlotNo) do {					/* DETECT initial setting				*/	\
	switch (SlotNo) {																				\
	case MMC_SLOT0:		MMC_PDR_DETECT0 = MMC_IN;	break;											\
	case MMC_SLOT1:		MMC_PDR_DETECT1 = MMC_IN;	break;											\
	default:										break;											\
	}																								\
} while (0)

#define MMC_GET_DETECT(SlotNo, Lv) do {					/* DETECT input control					*/	\
	Lv = MMC_HI;										/* Initialize by non-detection			*/	\
	switch (SlotNo) {																				\
	case MMC_SLOT0:		Lv = MMC_PIDR_DETECT0;		break;											\
	case MMC_SLOT1:		Lv = MMC_PIDR_DETECT1;		break;											\
	default:										break;											\
	}																								\
} while (0)
#endif	/* #if (MMC_SLOT_NUM > 1) */

/*-------------------- CS control --------------------*/
#if (MMC_SLOT_NUM == 1)
#define MMC_CS_INIT(SlotNo) do {						/* CS initial setting					*/	\
	MMC_PODR_CS0     = MMC_HI;							/* CS0				"H"					*/	\
	MMC_PDR_CS0		 = MMC_OUT;							/* CS0				Output				*/	\
	MMC_PODR_CS0     = MMC_HI;							/* CS0				"H"					*/	\
} while (0)

#define MMC_CS_OPEN(SlotNo) do {						/* CS open setting						*/	\
	MMC_PDR_CS0     = MMC_IN;							/* CS0				Input				*/	\
} while (0)

#define MMC_SET_CS(SlotNo, Lv) do {						/* CS output control					*/	\
	MMC_PODR_CS0       = Lv;																		\
} while (0)
#endif	/* #if (MMC_SLOT_NUM == 1) */

#if (MMC_SLOT_NUM > 1)
#define MMC_CS_INIT(SlotNo) do {						/* CS initial setting					*/	\
	switch (SlotNo) {																				\
	case MMC_SLOT0:																					\
		MMC_PODR_CS0   = MMC_HI;						/* CS0				"H"					*/	\
		MMC_PDR_CS0  = MMC_OUT;							/* CS0				Output				*/	\
		MMC_PODR_CS0   = MMC_HI;						/* CS0				"H"					*/	\
		break;																						\
	case MMC_SLOT1:																					\
		MMC_PODR_CS1   = MMC_HI;						/* CS1				"H"					*/	\
		MMC_PDR_CS1  = MMC_OUT;							/* CS1				Output				*/	\
		MMC_PODR_CS1   = MMC_HI;						/* CS1				"H"					*/	\
		break;																						\
	default:		break;																			\
	}																								\
} while (0)

#define MMC_CS_OPEN(SlotNo) do {						/* CS open setting						*/	\
	switch (SlotNo) {																				\
	case MMC_SLOT0:		MMC_PDR_CS0 = MMC_IN;	break;	/* CS0			Input					*/	\
	case MMC_SLOT1:		MMC_PDR_CS1 = MMC_IN;	break;	/* CS1			Input					*/	\
	default:									break;												\
	}																								\
} while (0)

#define MMC_SET_CS(SlotNo, Lv) do {						/* CS output control					*/	\
	switch (SlotNo) {																				\
	case MMC_SLOT0:		MMC_PODR_CS0 = Lv;		break;												\
	case MMC_SLOT1:		MMC_PODR_CS1 = Lv;		break;												\
	default:									break;												\
	}																								\
} while (0)

#endif	/* #if (MMC_SLOT_NUM > 1) */

/*------------------- SCI control -------------------*/

#define MMC_SCI_DI() do {								/* SCI disable setting					*/	\
	MMC_SCI_SCR  = MMC_USCR_NOUSE;	/* Sets TE=RE=0.											*/	\
	MMC_SCI_SMR  = MMC_USMR_NOUSE;	/* Sets CM,SCK after TE=RE=0.								*/	\
	MMC_SCI_SSR_CLEAR();								/* OER/FER/PER clear					*/	\
	MMC_PRCR_EI();																					\
	MMC_MSTPCR_SCI = 1;									/* SCIn Module stop enable setting		*/	\
	MMC_PRCR_DI();																					\
} while (0)

#define MMC_SCI_EI() do {								/* MMC enable setting					*/	\
	MMC_PRCR_EI();																					\
	MMC_MSTPCR_SCI = 0;									/* SCI Module stop disable setting		*/	\
	MMC_PRCR_DI();																					\
	MMC_SCI_SCR  = MMC_USCR_INIT;	/* Sets TE=RE=0.*/	/* Transmit&receive disable				*/	\
	MMC_SCI_SMR  = MMC_USMR_INIT;	/* Sets CM,SCK.	*/	/* Clocked synchronous mode setting		*/	\
	MMC_SCI_SCMR = MMC_USCMR_INIT;	/* Sets SMIF after TE=RE=0. Serial interface mode setting	*/	\
	MMC_SCI_SSR_CLEAR();								/* OER/FER/PER clear					*/	\
	MMC_SCI_SEMR = MMC_USEMR_INIT;						/* Serial extend mode initial			*/	\
} while (0)

#define MMC_SCI_SSR_CLEAR() do {																	\
	if(MMC_SCI_ORER){																				\
		MMC_SCI_ORER = 0;																			\
		while(MMC_SCI_ORER == 1){}																	\
	}																								\
	if(MMC_SCI_FER){																				\
		MMC_SCI_FER = 0;																			\
		while(MMC_SCI_FER == 1){}																	\
	}																								\
	if(MMC_SCI_PER){																				\
		MMC_SCI_PER = 0;																			\
		while(MMC_SCI_PER == 1){}																	\
	}																								\
} while (0)

#define MMC_SCI_BRG_SET(UBRG_DATA) do {					/* SCI bit rate setting					*/	\
	MMC_SCI_BRR  = UBRG_DATA;																		\
} while (0)

#define MMC_SCI_TX_EI() do {							/* MMC transmit enable setting			*/	\
	MMC_SCI_SCR  = MMC_USCR_TX;							/* Transmit enable						*/	\
} while (0)

#define MMC_SCI_TRX_EI() do {							/* MMC transmit&receive enable setting	*/	\
	MMC_SCI_SCR  = MMC_USCR_TRX;						/* Transmit&receive enable				*/	\
} while (0)

#define MMC_SCI_TX_DI() do {							/* MMC transmit disable setting			*/	\
	MMC_SCI_SCR  = MMC_USCR_INIT;						/* Transmit disable						*/	\
} while (0)

#define MMC_SCI_TRX_DI() do {							/* MMC transmit&receive disable setting	*/	\
	MMC_SCI_SCR  = MMC_USCR_INIT;						/* Transmit&receive disable				*/	\
} while (0)

#define MMC_TXNEXT_CHECK()	((MMC_SCI_TXNEXT & MMC_SCI_TXNEXT_MASK) == 0x00)
#define MMC_TXNEXT_CLR()	(MMC_SCI_TXNEXT = MMC_SCI_TXNEXT_OFF)
#define MMC_RXNEXT_CHECK()	((MMC_SCI_RXNEXT & MMC_SCI_RXNEXT_MASK) == 0x00)
#define MMC_RXNEXT_CLR()	(MMC_SCI_RXNEXT = MMC_SCI_RXNEXT_OFF)

/*----------------- CRC calculation ------------------*/
#ifdef MMC_CRC_HARD
#define MMC_CRC_CLR()		MMC_PRCR_EI();	MMC_MSTPCR_CRC = 0;	MMC_PRCR_DI();	MMC_CRCC  = 0x87
#define MMC_CRC_SET(Data)	MMC_CRCIN = Data
#define MMC_CRC_GET(Data)	Data = MMC_CRCD
#else	/* #ifdef MMC_CRC_HARD */
#define MMC_CRC_CLR()			gMmc_CrcData.us = 0x0000
#ifdef MTL_MCU_LITTLE
#define MMC_CRC_SET(Data) do {																		\
	uint8_t	ucTmp;																					\
	ucTmp              = gMmc_CrcData.uc[0];														\
	gMmc_CrcData.us    = T_Crc_Ccitt[gMmc_CrcData.uc[1] ^ Data];									\
	gMmc_CrcData.uc[1] = gMmc_CrcData.uc[1] ^ ucTmp;												\
} while (0)
#else	/* #ifdef MTL_MCU_LITTLE */
#define MMC_CRC_SET(Data) do {																		\
	uint8_t	ucTmp;																					\
	ucTmp              = gMmc_CrcData.uc[1];														\
	gMmc_CrcData.us    = T_Crc_Ccitt[gMmc_CrcData.uc[0] ^ Data];									\
	gMmc_CrcData.uc[0] = gMmc_CrcData.uc[0] ^ ucTmp;												\
} while (0)
#endif	/* #ifdef MTL_MCU_LITTLE */
#define MMC_CRC_GET(Data)		Data = gMmc_CrcData.us
#endif	/* #ifdef MMC_CRC_HARD */

/*----------------- DTC control------------------*/
#ifdef MMC_DTC_USED

#define MMC_DTC_EI() do{																			\
	MMC_PRCR_EI();																					\
	MMC_MSTPCR_DTC 	= 0;																			\
	MMC_PRCR_DI();																					\
	MMC_DTCCR 		= MMC_DTCCR_INIT1;																\
	MMC_DTCVBR 		= (uint32_t)&gDtcVctTbl;														\
	MMC_DTCADMOD	= MMC_DTCADOMOD_INIT;															\
	MMC_DTCST		= MMC_DTCST_INIT1;																\
}while(0)

#define MMC_DTC_DI() do{																			\
	MMC_DTCST		= MMC_DTCST_INIT1;																\
	MMC_DTCCR 		= MMC_DTCCR_INIT1;																\
	MMC_PRCR_EI();																					\
	MMC_MSTPCR_DTC 	= 1;																			\
	MMC_PRCR_DI();																					\
}while(0)

#define MMC_DTC_TRX_EI(pAdr, cnt) do{																\
	MMC_DTCCR					= MMC_DTCCR_INIT1;													\
	gDtcAlloc[0].reserved		= 0x0000;															\
	gDtcAlloc[0].MRA			= MMC_MRA_TX_EI;													\
	gDtcAlloc[0].MRB			= MMC_MRB_TX_EI;													\
	gDtcAlloc[0].SAR			= (uint32_t)gMmcDummyData;											\
	gDtcAlloc[0].DAR			= (uint32_t)&MMC_SCI_TXBUF;											\
	gDtcAlloc[0].CRA			= (uint16_t)cnt;													\
	gDtcAlloc[1].reserved		= 0x0000;															\
	gDtcAlloc[1].MRA			= MMC_MRA_RX_EI;													\
	gDtcAlloc[1].MRB			= MMC_MRB_RX_EI;													\
	gDtcAlloc[1].SAR			= (uint32_t)&MMC_SCI_RXBUF;											\
	gDtcAlloc[1].DAR			= (uint32_t)pAdr;													\
	gDtcAlloc[1].CRA			= (uint16_t)cnt;													\
	gDtcVctTbl[MMC_VECT_TXNEXT]	= (uint32_t)&gDtcAlloc[0];											\
	gDtcVctTbl[MMC_VECT_RXNEXT]	= (uint32_t)&gDtcAlloc[1];											\
	MMC_DTCCR 					= MMC_DTCCR_INIT2;													\
	MMC_SCI_TXNEXT_ISEL			= MMC_ISEL_DTC;														\
	MMC_SCI_RXNEXT_ISEL			= MMC_ISEL_DTC;														\
	MMC_SCI_TXNEXT_IER			= MMC_TRUE;															\
	MMC_SCI_RXNEXT_IER			= MMC_TRUE;															\
	MMC_TXNEXT_CLR();																				\
	MMC_RXNEXT_CLR();																				\
	MMC_DTCST					= MMC_DTCST_INIT2;													\
}while(0)

#define MMC_DTC_TRX_DI() do{																		\
	MMC_SCI_RXNEXT				= MMC_FALSE;														\
	MMC_SCI_TXNEXT_IER			= MMC_FALSE;														\
	MMC_SCI_RXNEXT_IER			= MMC_FALSE;														\
}while(0)
#else	/* #ifdef MMC_DTC_USED */
#define MMC_DTC_EI()
#define MMC_DTC_DI()
#define MMC_DTC_TRX_EI(pAdr, cnt)
#define MMC_DTC_TRX_DI()
#endif	/* #ifdef MMC_DTC_USED */

/************************************************************************************************/
/*	Prototypes																					*/
/************************************************************************************************/
typedef	struct	MMC_DTCALLOC_FULL_tag
{
#if defined(MTL_MCU_LITTLE)
	uint16_t	reserved;
	uint8_t		MRB;
	uint8_t		MRA;
	volatile uint32_t	SAR;
	volatile uint32_t	DAR;
	volatile uint16_t	CRB;
	volatile uint16_t	CRA;
#else
	uint8_t		MRA;
	uint8_t		MRB;
	uint16_t	reserved;
	volatile uint32_t	SAR;
	volatile uint32_t	DAR;
	volatile uint16_t	CRA;
	volatile uint16_t	CRB;
#endif
}MMC_DTCALLOC_FULL_t;

#endif /* __MMC_SFR_H__ */
