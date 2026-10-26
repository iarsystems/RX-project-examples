/***********************************************************************************************************************
* DISCLAIMER
* This software is supplied by Renesas Electronics Corporation and is only intended for use with Renesas products.
* No other uses are authorized. This software is owned by Renesas Electronics Corporation and is protected under all
* applicable laws, including copyright laws.
* THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIESREGARDING THIS SOFTWARE, WHETHER EXPRESS, IMPLIED
* OR STATUTORY, INCLUDING BUT NOT LIMITED TO WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
* NON-INFRINGEMENT.  ALL SUCH WARRANTIES ARE EXPRESSLY DISCLAIMED.TO THE MAXIMUM EXTENT PERMITTED NOT PROHIBITED BY
* LAW, NEITHER RENESAS ELECTRONICS CORPORATION NOR ANY OF ITS AFFILIATED COMPANIES SHALL BE LIABLE FOR ANY DIRECT,
* INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES FOR ANY REASON RELATED TO THIS SOFTWARE, EVEN IF RENESAS OR
* ITS AFFILIATES HAVE BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
* Renesas reserves the right, without notice, to make changes to this software and to discontinue the availability
* of this software. By using this software, you agree to the additional terms and conditions found by accessing the
* following link:
* http://www.renesas.com/disclaimer
*
* Copyright (C) 2014 Renesas Electronics Corporation. All rights reserved.
***********************************************************************************************************************/

/***********************************************************************************************************************
* File Name    : r_lcd_appv2.h
* Version      : 1.00
* Device(s)    : R5F51138AxFP
* Tool-Chain   : IAR Systems ICCRX
* Description  : Header file for r_lcd_appv2.c - LCD handing routines
* Creation Date: 28/08/2014
***********************************************************************************************************************/

#ifndef R_LCD_APPV2
#define R_LCD_APPV2

/******************************************************************************
Includes <System Includes> , �Project Includes�E
******************************************************************************/
/* Following header file provides C99 type definitions. */
#define SECTC_TABLE		/* SECTC��\�����鎞�̏����n��TABLE�l���g�p����ꍇ�ݒ� */
						/* ���l�Őݒ肷��ꍇ�ɂ̓R�����g�����鎖				*/

/******************************************************************************
Macro Definitions
******************************************************************************/
/* Set to ensure base delay */
#define PANEL_DELAY_TIMING	(0x05)


/* Use the ROM area to indicate that a data pointer is not to be used */
#define PDL_NO_PTR 		((uint8_t *)(0xFFFFu))

/* Define Section Names */
#define EMPTY	 (0)
#define SECT1 	('1')
#define SECT2 	('2')
#define SECT3 	('3')
#define SECT4 	('4')
#define CLEAR	(0x23)
#define ISOTEST	(0x2A)

#define PANEL_LCD_LINE1	 ('3')
#define PANEL_LCD_LINE2	 ('1')
#define PANEL_LCD_LINE3	 ('2')
#define PANEL_LCD_SYMBOL ('4')

/* Define Symbol Names */
#define LCD_HEAT_ON			(0x3131u)
#define LCD_HEAT_OFF		(0x3130u)
#define LCD_FAN_ON			(0x3231u)
#define LCD_FAN_OFF			(0x3230u)
#define LCD_ZONE_ON			(0x3331u)
#define LCD_ZONE_OFF		(0x3330u)
#define LCD_MGML_ON			(0x3431u)
#define LCD_MGML_OFF		(0x3430u)
#define LCD_MMHG_ON			(0x3531u)
#define LCD_MMHG_OFF		(0x3530u)
#define LCD_VOLTS_ON		(0x3631u)
#define LCD_VOLTS_OFF		(0x3630u)
#define LCD_HEART_ON		(0x3731u)
#define LCD_HEART_OFF		(0x3730u)
#define LCD_DEGREESC_ON		(0x3831u)
#define LCD_DEGREESC_OFF	(0x3830u)
#define LCD_DEGREESF_ON		(0x3931u)
#define LCD_DEGREESF_OFF	(0x3930u)
#define LCD_ALARM_ON		(0x4131u)
#define LCD_ALARM_OFF		(0x4130u)
#define LCD_ALARM_ON_1		(0x6131u)
#define LCD_ALARM_OFF_1		(0x6130u)
#define LCD_AM_ON			(0x4231u)
#define LCD_AM_OFF			(0x4230u)
#define LCD_AM_ON_1			(0x6231u)
#define LCD_AM_OFF_1		(0x6230u)
#define LCD_PM_ON			(0x4331u)
#define LCD_PM_OFF			(0x4330u)
#define LCD_PM_ON_1			(0x6331u)
#define LCD_PM_OFF_1		(0x6330u)
#define LCD_R_LOGO_ON		(0x4431u)
#define LCD_R_LOGO_OFF		(0x4430u)
#define LCD_R_LOGO_ON_1		(0x6431u)
#define LCD_R_LOGO_OFF_1	(0x6430u)

/******************************************************************************
Type definitions
******************************************************************************/
/* Define Map Structure Used */
typedef union LCD_MAP{	int32_t WORD;
					    struct
						{
							int8_t ONE   : 4;
							int8_t TWO   : 4;
							int8_t THREE : 4;
							int8_t FOUR  : 4;
						} BYTE;
					  } lcdmap_t;

/******************************************************************************
Function Prototypes
******************************************************************************/
  static int16_t validate_input(uint8_t input, uint8_t * output);
  static void sect_a_glyph_map(uint8_t glyph, int16_t digit);
  static void sect_b_glyph_map (const uint8_t glyph, const uint16_t digit);
  static void sect_c_glyph_map(uint8_t glyph, int16_t digit);
  static void sect_d_glyph_map (const uint8_t level);
  static void sect_f_glyph_map (const uint8_t day);

  static void init_maps(void);
  int8_t R_LCD_DisplaySymbol (const uint16_t input);
  void R_LCD_DisplayPanelString(uint8_t position, uint8_t* string);
  void R_LCD_Create(void);
  void R_LCD_Start(void);
  void R_LCD_Stop(void);
  void R_LCD_ClearDisplay(void);

#endif
