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
*******************************************************************************/
/* Copyright (C) 2011 Renesas Electronics Corporation. All rights reserved.   */
/*******************************************************************************
* File Name		: intprg.c
* Version 		: 1.00
* Device 		: R5F52108DFP
* Tool-Chain 	: RX Family C Compiler
* H/W Platform	: RPBRX210
* Description   : Sets up interrupt handlers
*******************************************************************************/
/*******************************************************************************
* History 		: 13.08.2011  Ver. 1.00 First Release
*******************************************************************************/

/*******************************************************************************
User Includes (Project Level Includes)
*******************************************************************************/
/* Include machine.h to use the wait() function */
#include <machine.h>
/* Vector table */
#include "vect.h"

//#pragma section IntPRG

/* Exception(Supervisor Instruction) */
void Excep_SuperVisorInst(void){/* brk(){  } */}

/* Exception(Undefined Instruction) */
void Excep_UndefinedInst(void){/* brk(){  } */}

/* Exception(Floating Point) */
void Excep_FloatingPoint(void){/* brk(){  } */}

/* NMI */
void NonMaskableInterrupt(void){/* brk(){  } */}

/* Dummy */
void Dummy(void){/* brk(){  } */}

/* BRK */
void Excep_BRK(void){ wait(); }

/* BUSERR */
void Excep_BUSERR(void){ }

/* FCU_FCUERR */
void Excep_FCU_FCUERR(void){ }

/* FCU_FRDYI */
void Excep_FCU_FRDYI(void){ }

/* ICU_SWINT */
void Excep_ICU_SWINT(void){  }

/* CMTU0_CMT0 */
void Excep_CMTU0_CMT0(void){  }

/* CMTU0_CMT1 */
void Excep_CMTU0_CMT1(void){  }

/* CMTU1_CMT2 */
void Excep_CMTU1_CMT2(void){  }

/* CMTU1_CMT3 */
void Excep_CMTU1_CMT3(void){  }

/* CAC_FERRF */
void Excep_CAC_FERRF(void){  }

/* CAC_MENDF */
void Excep_CAC_MENDF(void){  }

/* CAC_OVFF */
void Excep_CAC_OVFF(void){  }

/* RSPI0_SPEI0 */
void Excep_RSPI0_SPEI0(void){  }

/* RSPI0_SPRI0 */
void Excep_RSPI0_SPRI0(void){  }

/* RSPI0_SPTI0 */
void Excep_RSPI0_SPTI0(void){  }

/* RSPI0_SPII0 */
void Excep_RSPI0_SPII0(void){  }

/* DOC_DOPCF */
void Excep_DOC_DOPCF(void){  }

/* COMPB1 */
void Excep_COMPB1(void){  }

/* COMPB2 */
void Excep_COMPB2(void){  }

/* RTC_COUNTUP */
void Excep_RTC_COUNTUP(void){  }

/* IRQ0 */
void Excep_IRQ0(void){  }

/* IRQ1 */
void Excep_IRQ1(void){  }

/* IRQ2 */
void Excep_IRQ2(void){  }

/* IRQ3 */
void Excep_IRQ3(void){  }

/* IRQ4 */
void Excep_IRQ4(void){  }

/* IRQ5 */
void Excep_IRQ5(void){  }

/* IRQ6 */
void Excep_IRQ6(void){  }

/* IRQ7 */
void Excep_IRQ7(void){  }

/* LVD1 */
void Excep_LVD1(void){  }

/* LVD2 */
void Excep_LVD2(void){  }

/* RTC_ALARM */
void Excep_RTC_ALARM(void){  }

/* RTC_PRD */
void Excep_RTC_PRD(void){  }

/* S12AD0_S12ADI0 */
void Excep_S12AD0_S12ADI0(void){  }

/* S12AD0_GBADI */
void Excep_S12AD0_GBADI(void){  }

/* ELC_ELSR18I */
void Excep_ELC_ELSR18I(void){  }

/* MTU20_TGIA0 */
void Excep_MTU20_TGIA0(void){  }

/* MTU20_TGIB0 */
void Excep_MTU20_TGIB0(void){  }

/* MTU20_TGIC0 */
void Excep_MTU20_TGIC0(void){  }

/* MTU20_TGID0 */
void Excep_MTU20_TGID0(void){  }

/* MTU20_TGIV0 */
void Excep_MTU20_TGIV0(void){  }

/* MTU20_TGIE0 */
void Excep_MTU20_TGIE0(void){  }

/* MTU20_TGIF0 */
void Excep_MTU20_TGIF0(void){  }

/* MTU21_TGIA1 */
void Excep_MTU21_TGIA1(void){  }

/* MTU21_TGIB1 */
void Excep_MTU21_TGIB1(void){  }

/* MTU21_TGIV1 */
void Excep_MTU21_TGIV1(void){  }

/* MTU21_TGIU1 */
void Excep_MTU21_TGIU1(void){  }

/* MTU22_TGIA2 */
void Excep_MTU22_TGIA2(void){  }

/* MTU22_TGIB2 */
void Excep_MTU22_TGIB2(void){  }

/* MTU22_TGIV2 */
void Excep_MTU22_TGIV2(void){  }

/* MTU22_TGIU2 */
void Excep_MTU22_TGIU2(void){  }

/* MTU23_TGIA3 */
void Excep_MTU23_TGIA3(void){  }

/* MTU23_TGIB3 */
void Excep_MTU23_TGIB3(void){  }

/* MTU23_TGIC3 */
void Excep_MTU23_TGIC3(void){  }

/* MTU23_TGID3 */
void Excep_MTU23_TGID3(void){  }

/* MTU23_TGIV3 */
void Excep_MTU23_TGIV3(void){  }

/* MTU24_TGIA4 */
void Excep_MTU24_TGIA4(void){  }

/* MTU24_TGIB4 */
void Excep_MTU24_TGIB4(void){  }

/* MTU24_TGIC4 */
void Excep_MTU24_TGIC4(void){  }

/* MTU24_TGID4 */
void Excep_MTU24_TGID4(void){  }

/* MTU24_TGIV4 */
void Excep_MTU24_TGIV4(void){  }

/* MTU25_TGIU5 */
void Excep_MTU25_TGIU5(void){  }

/* MTU25_TGIV5 */
void Excep_MTU25_TGIV5(void){  }

/* MTU25_TGIW5 */
void Excep_MTU25_TGIW5(void){  }

/* POE2_OEI1 */
void Excep_POE2_OEI1(void){  }

/* POE2_OEI2 */
void Excep_POE2_OEI2(void){  }

/* TMR0_CMIA0 */
void Excep_TMR0_CMIA0(void){  }

/* TMR0_CMIB0 */
void Excep_TMR0_CMIB0(void){  }

/* TMR0_OVI0 */
void Excep_TMR0_OVI0(void){  }

/* TMR1_CMIA1 */
void Excep_TMR1_CMIA1(void){  }

/* TMR1_CMIB1 */
void Excep_TMR1_CMIB1(void){  }

/* TMR1_OVI1 */
void Excep_TMR1_OVI1(void){  }

/* TMR2_CMIA2 */
void Excep_TMR2_CMIA2(void){  }

/* TMR2_CMIB2 */
void Excep_TMR2_CMIB2(void){  }

/* TMR2_OVI2 */
void Excep_TMR2_OVI2(void){  }

/* TMR3_CMIA3 */
void Excep_TMR3_CMIA3(void){  }

/* TMR3_CMIB3 */
void Excep_TMR3_CMIB3(void){  }

/* TMR3_OVI3 */
void Excep_TMR3_OVI3(void){  }

/* DMAC_DMAC0I */
void Excep_DMAC_DMAC0I(void){  }

/* DMAC_DMAC1I */
void Excep_DMAC_DMAC1I(void){  }

/* DMAC_DMAC2I */
void Excep_DMAC_DMAC2I(void){  }

/* DMAC_DMAC3I */
void Excep_DMAC_DMAC3I(void){  }

/* SCI0_ERI0 */
void Excep_SCI0_ERI0(void){  }

/* SCI0_RXI0 */
void Excep_SCI0_RXI0(void){  }

/* SCI0_TXI0 */
void Excep_SCI0_TXI0(void){  }

/* SCI0_TEI0 */
void Excep_SCI0_TEI0(void){  }

/* SCI1_ERI1 */
void Excep_SCI1_ERI1(void){  }

/* SCI1_RXI1 */
void Excep_SCI1_RXI1(void){  }

/* SCI1_TXI1 */
void Excep_SCI1_TXI1(void){  }

/* SCI1_TEI1 */
void Excep_SCI1_TEI1(void){  }

/* SCI5_ERI5 */
void Excep_SCI5_ERI5(void){  }

/* SCI5_RXI5 */
void Excep_SCI5_RXI5(void){  }

/* SCI5_TXI5 */
void Excep_SCI25_TXI5(void){  }

/* SCI5_TEI5 */
void Excep_SCI5_TEI5(void){  }

/* SCI6_ERI6 */
void Excep_SCI6_ERI6(void){  }

/* SCI6_RXI6 */
void Excep_SCI6_RXI6(void){  }

/* SCI6_TXI6 */
void Excep_SCI6_TXI6(void){  }

/* SCI6_TEI63 */
void Excep_SCI6_TEI6(void){  }

/* SCI8_ERI8 */
void Excep_SCI8_ERI8(void){  }

/* SCI8_RXI8 */
void Excep_SCI8_RXI8(void){  }

/* SCI8_TXI8 */
void Excep_SCI8_TXI8(void){  }

/* SCI8_TEI8 */
void Excep_SCI8_TEI8(void){  }

/* SCI9_ERI9 */
void Excep_SCI9_ERI9(void){  }

/* SCI9_RXI9 */
void Excep_SCI9_RXI9(void){  }

/* SCI9_TXI9 */
void Excep_SCI9_TXI9(void){  }

/* SCI9_TEI9 */
void Excep_SCI9_TEI9(void){  }

/* SCI12_ERI12 */
void Excep_SCI12_ERI12(void){  }

/* SCI12_RXI12 */
void Excep_SCI12_RXI12(void){  }

/* SCI12_TXI12 */
void Excep_SCI12_TXI12(void){  }

/* SCI12_TEI12 */
void Excep_SCI12_TEI12(void){  }

/* SCI12_SCIX0 */
void Excep_SCI12_SCIX0(void){  }

/* SCI12_SCIX1 */
void Excep_SCI12_SCIX1(void){  }

/* SCI12_SCIX2 */
void Excep_SCI12_SCIX2(void){  }

/* SCI12_SCIX3 */
void Excep_SCI12_SCIX3(void){  }

/* RIIC0_EEI0 */
void Excep_RIIC0_EEI0(void){  }

/* RIIC0_RXI0 */
void Excep_RIIC0_RXI0(void){  }

/* RIIC0_TXI0 */
void Excep_RIIC0_TXI0(void){  }

/* RIIC0_TEI0 */
void Excep_RIIC0_TEI0(void){  }

/* End of file */