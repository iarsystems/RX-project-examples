/*""FILE COMMENT""*******************************************************************
* File Name		: r_mmc_user_config.h
* Version      	: 1.23B
* Device(s)    	: RX62N
* Tool-Chain 	: RX Standard Toolchain (V.1.1.0.0)
* H/W Platform 	: R5F562N8
* Description	: MMC driver SFR for RX62N-RSPI user configuration definitions
*************************************************************************************
* History	: DD.MM.YYYY Version Description
*			: 22.08.2011 Ver1.23B
*""FILE COMMENT END""***************************************************************/
#ifndef __R_MMC_USER_CONFIG_H__
#define __R_MMC_USER_CONFIG_H__

/*<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/
/*                         Please define resources of each user system.                         */
/*<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

/* RSPI Channel Select (0 or 1)*/
#define MMC_RSPI_CHANNEL	1									/** SET **/

/* RSPI PIN select  ( 'A' or 'B' ) */
/*
                        ||    Select Port                 |
------------+-----------++--------------------------------+
  MMC_SCI_  | MMC_RSPI_ ||    RxD   |   SCK    |   TxD    |
  CHANNEL   | PIN       ||  Select  |  Select  |  Select  |
   Value    |  Value    ||   port   |   port   |   port   |
============+===========++==========+==========+==========+
     0      | 'A'       || PC7      | PC5      |  PC6     |
            |-----------++----------+----------+----------+
(=Use RSPI0)| 'B'       || PA7      | PA5      |  PA6     |
------------+-----------++----------+----------+----------+
     1      | 'A'       || P30      | P27      |  P26     |
            |-----------++----------+----------+----------+
(=Use RSPI1)| 'B'       || PE7      | PE5      |  PE6     |
------------+-----------++----------+----------+----------+*/
#define MMC_RSPI_PIN		'B'									/** SET **/

/*----------------------------------------------------------------------------------------------*/
/*	 Define the control port.			 														*/
/*----------------------------------------------------------------------------------------------*/
#define MMC_CS0_PORTNO			9		/* CS0 Port No. */		/** SET **/
#define MMC_CS0_BITNO			0		/* CS0 Bit  No.	*/		/** SET **/

#define MMC_DETECT0_PORTNO		9		/* DETECT0 Port No. */	/** SET **/
#define MMC_DETECT0_BITNO		1		/* DETECT0 Bit  No.	*/	/** SET **/

#if (MMC_SLOT_NUM > 1)
#define MMC_CS1_PORTNO					/* CS1 Port No. */		/** SET **/
#define MMC_CS1_BITNO					/* CS1 Bit  No.	*/		/** SET **/

#define MMC_DETECT1_PORTNO				/* DETECT1 Port No. */	/** SET **/
#define MMC_DETECT1_BITNO				/* DETECT1 Bit  No.	*/	/** SET **/
#endif	/* #if (MMC_SLOT_NUM > 1) */


/*----------------------------------------------------------------------------------------------*/
/* Macro "MMC_NOCHK_TIMEOUT" omits detecting timeout during communication.						*/
/* If user omits detecting timeout, please define this macro.									*/
/* If this macro is defined, processing speed would be increased.								*/
/*----------------------------------------------------------------------------------------------*/
#define MMC_NOCHK_TIMEOUT								/* No Check Communication Timeout		*/	/** SET **/

/*----------------------------------------------------------------------------------------------*/
/* If MMC_NOCHK_TIMEOUT would be not defined, please set timeout time.
/* MMC_T_RSPI_WAIT is unit of measuring timeout.
/* Please select value from "r_mtl_tim.h"
/* Please set value of (timeout time/unit) to MMC_RSPI_TX_WAIT(transmitting)
/* and MMC_RSPI_RX_WAIT(receiving).
/*----------------------------------------------------------------------------------------------*/
#define MMC_T_RSPI_WAIT		(natural_uint_t)MTL_T_250NS		/* RSPI transmit&receive completion waiting polling time			*/	/** SET **/

#define MMC_RSPI_TX_WAIT	(natural_uint_t)200000			/* RSPI transmission completion waiting time 200000 * 250ns = 50ms	*/	/** SET **/
#define MMC_RSPI_RX_WAIT	(natural_uint_t)200000			/* RSPI receive completion waiting time		 200000 * 250ns = 50ms	*/	/** SET **/


/*----------------------------------------------------------------------------------------------*/
/*	 Define the combination of the MCU's resources.												*/
/*----------------------------------------------------------------------------------------------*/
#define MMC_OPTION_1			/* Low speed	*/	/* RSPI									*/	/** SET **/
//#define MMC_OPTION_2			/* Low speed	*/	/* RSPI + CRC calculation circuit		*/	/** SET **/
//#define MMC_OPTION_3			/* High speed	*/	/* RSPI + DTC							*/	/** SET **/
//#define MMC_OPTION_4			/* High speed	*/	/* RSPI + CRC + DTC						*/	/** SET **/

/*----------------------------------------------------------------------------------------------*/
/*	 Define the value of the bit rate register according to a communication baud rate.			*/
/*	 Set the frequency of CLK to 6MHz or less.													*/
/*	 The possible maximum transfer frequency of CLK is depends on hardware circuit 				*/
/*	 and MCU conditions.																		*/
/*   Refer to MCU hardware manual/memory card specifications and specify the buad rate.			*/
/*	 When operating card with SPI mode, 														*/
/*	 specify the following two definitions of Identification mode and Data Transfer mode.		*/
/*	 Specify the definition to meet tODLY of both Identification mode and Data Transfer mode.	*/
/*	 In addition, meet tOD (100KHz <= tOD <= 400KHz) at Identification mode						*/
/*	 and tPP (0.1MHz <= tPP <= 20MHz ) at Data Transfer mode.									*/
/*	 The maximum frequency depends on MCU type.													*/
/*		 																						*/
/*  bitrate =   -----------------------------------												*/
/*              2 * (n{1) * (2 to the power of N)												*/
/*	PCLK: Operating frequency [MHz]																*/
/*	n   : Bit rate [bit/s]	 																	*/
/*	N   : Determined by the SMR settings shown in the following table.							*/
/*----------------------------------------------------------------------------------------------*/
/* PCLK = 48MHz, n=0 */
#define MMC_UBRG_IDENTIFICATION	(uint8_t)0x3B				/* BRR identification mode setting	*/	/** SET **/
/*	                                       ++---------------- 400KHz							*/	/** SET **/
#define MMC_UBRG_D_TRANSFER		(uint8_t)0x01				/* BRR data Transfer mode setting	*/	/** SET **/
/*	                                       ++---------------- 12.0MHz							*/	/** SET **/
#define MMC_CLK_D_TRANSFER		(uint32_t)12000000		/* Data Transfer mode clock frequency	*/	/** SET **/

#endif	/* #ifndef __R_MMC_USER_CONFIG_H__ */