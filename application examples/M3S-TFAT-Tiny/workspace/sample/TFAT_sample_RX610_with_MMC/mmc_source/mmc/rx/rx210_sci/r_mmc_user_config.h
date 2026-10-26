/*""FILE COMMENT""*******************************************************************
* File Name		: r_mmc_user_config.h
* Version      	: 1.23B
* Device(s)    	: RX210
* Tool-Chain 	: RX Standard Toolchain (V.1.1.0.0)
* H/W Platform 	: R5F52108
* Description	: MMC driver SFR for RX210-SCI user configuration definitions
*************************************************************************************
* History	: DD.MM.YYYY Version Description
*			: 22.08.2011 Ver1.23B
*""FILE COMMENT END""***************************************************************/
#ifndef __R_MMC_USER_CONFIG_H__
#define __R_MMC_USER_CONFIG_H__

/*<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/
/*                         Please define resources of each user system.                         */
/*<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

/* SCI Channel Select (0 or 1 or 5 or 6 or 8 or 9 or 12) */
#define MMC_SCI_CHANNEL		9													/** SET **/

/* When MMC_SCI_CHANNEL defines 1 or 5 or 6 ,
    MMC_SCI_SCK_PIN and MMC_SCI_RXD_PIN and MMC_SCI_TXD_PIN must define. */
/*
                       ||    Select Port                 |
------------+----------++--------------------------------+
  MMC_SCI_  | MMC_SCI_ ||    RxD   |   SCK    |   TxD    |
  CHANNEL   | XXX_PIN  ||  Select  |  Select  |  Select  |
   Value    |  Value   ||   port   |   port   |   port   |
============+==========++==========+==========+==========+
     0      |(invalid) || P21      | P22      |  P20     |
(=Use SCI0) |          ||          |          |          |
------------+----------++----------+----------+----------+
     1      | 'A'      || P15      | P16      |  P17     |
            |----------++----------+----------+----------+
(=Use SCI1) | 'B'      || P30      | P27      |  P26     |
------------+----------++----------+----------+----------+
     5      | 'A'      || PA2      | PA1      |  PA4     |
            |----------++----------+----------+----------+
(=Use SCI5) | 'B'      || PA3      | PC1      |  PC3     |
            |----------++----------+----------+----------+
            | 'C'      || PC2      | PC4      |  -       |
------------+----------++----------+----------+----------+
     6      | 'A'      || P33      | P34      |  P32     |
            |----------++----------+----------+----------+
(=Use SCI6) | 'B'      || PB0      | PB3      |  PB1     |
------------+----------++----------+----------+----------+
     8      |(invalid) || PC6      | PC5      |  PC7     |
(=Use SCI8) |          ||          |          |          |
------------+----------++----------+----------+----------+
     9      |(invalid) || PB6      | PB5      |  PB7     |
(=Use SCI9) |          ||          |          |          |
------------+----------++----------+----------+----------+
    12      |(invalid) || PE2      | PE0      |  PE1     |
(=Use SCI12)|          ||          |          |          |
------------+----------++----------+----------+----------+*/
#define MMC_SCI_RXD_PIN			'A'												/** SET **/
#define MMC_SCI_SCK_PIN			'A'												/** SET **/
#define MMC_SCI_TXD_PIN			'A'												/** SET **/

/*----------------------------------------------------------------------------------------------*/
/*	 Define the control port.			 														*/
/*----------------------------------------------------------------------------------------------*/
#define MMC_CS0_PORTNO			5		/* CS0 Port No. */		/** SET **/
#define MMC_CS0_BITNO			0		/* CS0 Bit  No.	*/		/** SET **/

#define MMC_DETECT0_PORTNO		5		/* DETECT0 Port No. */	/** SET **/
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
/* MMC_T_SCI_WAIT is unit of measuring timeout.
/* Please select value from "r_mtl_tim.h"
/* Please set value of (timeout time/unit) to MMC_SCI_TX_WAIT(transmitting)
/* and MMC_SCI_RX_WAIT(receiving).
/*----------------------------------------------------------------------------------------------*/
#define MMC_T_SCI_WAIT		(natural_uint_t)MTL_T_250NS		/* SCI transmit&receive completion waiting polling time				*/	/** SET **/

#define MMC_SCI_TX_WAIT		(natural_uint_t)200000			/* SCI transmission completion waiting time 200000 * 250ns = 50ms	*/	/** SET **/
#define MMC_SCI_RX_WAIT		(natural_uint_t)200000			/* SCI receive completion waiting time		200000 * 250ns = 50ms	*/	/** SET **/


/*----------------------------------------------------------------------------------------------*/
/*	 Define the combination of the MCU's resources.												*/
/*----------------------------------------------------------------------------------------------*/
#define MMC_OPTION_1			/* Low speed	*/	/* SCI									*/	/** SET **/
//#define MMC_OPTION_2			/* Low speed	*/	/* SCI + CRC calculation circuit		*/	/** SET **/
//#define MMC_OPTION_3			/* High speed	*/	/* SCI + DTC							*/	/** SET **/
//#define MMC_OPTION_4			/* High speed	*/	/* SCI + CRC + DTC						*/	/** SET **/

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
/*	BRR = (PCLK / (8 * 2 ^ (2n - 1) * B)) - 1													*/
/*	PCLK: Operating frequency [MHz]																*/
/*	B   : Bit rate [bit/s]	 																	*/
/*	n   : Determined by the SMR settings shown in the following table.							*/
/*																								*/
/*		 CKS1 | CKS0 |  n																		*/
/*		------+------+------																	*/
/*		  0   |  0   |  0																		*/
/*		  0   |  1   |  1																		*/
/*		  1   |  0   |  2																		*/
/*		  1   |  1   |  3																		*/
/*----------------------------------------------------------------------------------------------*/
/* PCLK = 25MHz, n=0 */
#define MMC_UBRG_IDENTIFICATION	(uint8_t)0x0f				/* BRR identification mode setting	*/	/** SET **/
/*	                                       ++---------------- 391KHz							*/	/** SET **/
#define MMC_UBRG_D_TRANSFER		(uint8_t)0x00				/* BRR data Transfer mode setting	*/	/** SET **/
/*	                                       ++---------------- 6.25MHz							*/	/** SET **/
#define MMC_CLK_D_TRANSFER		(uint32_t)6250000		/* Data Transfer mode clock frequency	*/	/** SET **/


#endif	/* #ifndef __R_MMC_USER_CONFIG_H__ */