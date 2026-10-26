/*""FILE COMMENT""*************************************************************
* System Name	: Interrupt program for RX210xx
* File Name		: Interrupt_not_RPDL.c
* Version		: 2.00
* Contents		: Interrupt handlers for peripherals outside the scope
*				: of RPDL
* Customer		:
* Model		 	:
* Order		 	:
* CPU			: RX
* Compiler		: RXC
* OS			: Nothing
* Programmer	:
* Note			:
*******************************************************************************
* Copyright, 2013. Renesas Electronics Corporation and Renesas Solutions Corp.
*******************************************************************************
* History		: 2013.05.14
*				: Ver 2.00
*				: CS Release
*""FILE COMMENT END""*********************************************************/

#include "r_pdl_common_defs_RX210xx.h"
#include "r_pdl_user_definitions.h"

/* Remove the // comment mark if an RPDL interrupt file is not included
   in the build but an interrupt vector table entry is still required. */
//#define RPDL_ADC_12_not_used
//#define RPDL_BSC_not_used
//#define RPDL_CAC_not_used
//#define RPDL_CMT_not_used
//#define RPDL_CPA_LVD_not_used
//#define RPDL_CPB_not_used
//#define RPDL_DMAC_not_used
//#define RPDL_DOC_not_used
//#define RPDL_ELC_not_used
//#define RPDL_IIC_not_used
//#define RPDL_MTU2_not_used
//#define RPDL_POE_not_used
//#define RPDL_RTC_not_used
//#define RPDL_SCI_not_used
//#define RPDL_SPI_not_used
//#define RPDL_TMR_not_used
//#define RPDL_TPU_not_used

#ifdef RPDL_ADC_12_not_used
#pragma vector=VECT_S12AD_S12ADI0
__interrupt  void Interrupt_ADC_12_0(void)
#pragma vector=VECT_S12AD_GBADI
__interrupt  void Interrupt_ADC_12_0_b(void)
#endif

#ifdef RPDL_BSC_not_used
#pragma vector=VECT_BSC_BUSERR
__interrupt  void Interrupt_BSC(void)
#endif

#ifdef RPDL_CAC_not_used
#pragma vector=VECT_CAC_FERRF
__interrupt  void Interrupt_CAC_frequency_error(void)
#pragma vector=VECT_CAC_MENDF
__interrupt  void Interrupt_CAC_measurement_complete(void)
#pragma vector=VECT_CAC_OVFF
__interrupt  void Interrupt_CAC_overflow(void)
#endif

#ifdef RPDL_CMT_not_used
#pragma vector=VECT_CMT0_CMI0
__interrupt  void Interrupt_CMT0(void)
#pragma vector=VECT_CMT1_CMI1
__interrupt  void Interrupt_CMT1(void)
#pragma vector=VECT_CMT2_CMI2
__interrupt  void Interrupt_CMT2(void)
#pragma vector=VECT_CMT3_CMI3
__interrupt  void Interrupt_CMT3(void)
#endif

#ifdef RPDL_CPA_LVD_not_used
#pragma vector=VECT_CMPA_CMPA1
__interrupt  void Interrupt_CPA1_LVD1(void)
#pragma vector=VECT_CMPA_CMPA2
__interrupt  void Interrupt_CPA2_LVD2(void)
#endif

#ifdef RPDL_CPB_not_used
#pragma vector=VECT_CMPB_CMPB0
__interrupt  void Interrupt_CPB0(void)
#pragma vector=VECT_CMPB_CMPB1
__interrupt  void Interrupt_CPB1(void)
#endif

#ifdef RPDL_DMAC_not_used
#pragma vector=VECT_DMAC_DMAC0I
__interrupt  void Interrupt_DMAC0(void)
#pragma vector=VECT_DMAC_DMAC1I
__interrupt  void Interrupt_DMAC1(void)
#pragma vector=VECT_DMAC_DMAC2I
__interrupt  void Interrupt_DMAC2(void)
#pragma vector=VECT_DMAC_DMAC3I
__interrupt  void Interrupt_DMAC3(void)
#endif

#ifdef RPDL_DOC_not_used
#pragma vector=VECT_DOC_DOPCF
__interrupt  void Interrupt_DOC_DOPCF(void)
#endif

#ifdef RPDL_ELC_not_used
#pragma vector=VECT_ELC_ELSR18I
__interrupt  void Interrupt_ELSR18I(void)
#pragma vector=VECT_ELC_ELSR19I
__interrupt  void Interrupt_ELSR19I(void)
#endif

#ifdef RPDL_IIC_not_used
#pragma vector=VECT_RIIC0_EEI0
__interrupt  void Interrupt_IIC_EEI0(void)
#pragma vector=VECT_RIIC0_RXI0
__interrupt  void Interrupt_IIC_RXI0(void)
#pragma vector=VECT_RIIC0_TXI0
__interrupt  void Interrupt_IIC_TXI0(void)
#pragma vector=VECT_RIIC0_TEI0
__interrupt  void Interrupt_IIC_TEI0(void)
#endif

#ifdef RPDL_MTU2_not_used
#pragma vector=VECT_MTU0_TGIA0
__interrupt  void Interrupt_MTU0_TGIA0(void)
#pragma vector=VECT_MTU1_TGIA1
__interrupt  void Interrupt_MTU1_TGIA1(void)
#pragma vector=VECT_MTU2_TGIA2
__interrupt  void Interrupt_MTU2_TGIA2(void)
#pragma vector=VECT_MTU3_TGIA3
__interrupt  void Interrupt_MTU3_TGIA3(void)
#pragma vector=VECT_MTU4_TGIA4
__interrupt  void Interrupt_MTU4_TGIA4(void)
#pragma vector=VECT_MTU0_TGIB0
__interrupt  void Interrupt_MTU0_TGIB0(void)
#pragma vector=VECT_MTU1_TGIB1
__interrupt  void Interrupt_MTU1_TGIB1(void)
#pragma vector=VECT_MTU2_TGIB2
__interrupt  void Interrupt_MTU2_TGIB2(void)
#pragma vector=VECT_MTU3_TGIB3
__interrupt  void Interrupt_MTU3_TGIB3(void)
#pragma vector=VECT_MTU4_TGIB4
__interrupt  void Interrupt_MTU4_TGIB4(void)
#pragma vector=VECT_MTU0_TGIC0
__interrupt  void Interrupt_MTU0_TGIC0(void)
#pragma vector=VECT_MTU3_TGIC3
__interrupt  void Interrupt_MTU3_TGIC3(void)
#pragma vector=VECT_MTU4_TGIC4
__interrupt  void Interrupt_MTU4_TGIC4(void)
#pragma vector=VECT_MTU0_TGID0
__interrupt  void Interrupt_MTU0_TGID0(void)
#pragma vector=VECT_MTU3_TGID3
__interrupt  void Interrupt_MTU3_TGID3(void)
#pragma vector=VECT_MTU4_TGID4
__interrupt  void Interrupt_MTU4_TGID4(void)
#pragma vector=VECT_MTU0_TGIE0
__interrupt  void Interrupt_MTU0_TGIE0(void)
#pragma vector=VECT_MTU0_TGIF0
__interrupt  void Interrupt_MTU0_TGIF0(void)
#pragma vector=VECT_MTU0_TCIV0
__interrupt  void Interrupt_MTU0_TCIV0(void)
#pragma vector=VECT_MTU1_TCIV1
__interrupt  void Interrupt_MTU1_TCIV1(void)
#pragma vector=VECT_MTU2_TCIV2
__interrupt  void Interrupt_MTU2_TCIV2(void)
#pragma vector=VECT_MTU3_TCIV3
__interrupt  void Interrupt_MTU3_TCIV3(void)
#pragma vector=VECT_MTU4_TCIV4
__interrupt  void Interrupt_MTU4_TCIV4(void)
#pragma vector=VECT_MTU1_TCIU1
__interrupt  void Interrupt_MTU1_TCIU1(void)
#pragma vector=VECT_MTU2_TCIU2
__interrupt  void Interrupt_MTU2_TCIU2(void)
#pragma vector=VECT_MTU5_TGIU5
__interrupt  void Interrupt_MTU5_TGIU5(void)
#pragma vector=VECT_MTU5_TGIV5
__interrupt  void Interrupt_MTU5_TGIV5(void)
#pragma vector=VECT_MTU5_TGIW5
__interrupt  void Interrupt_MTU5_TGIW5(void)
#endif

#ifdef RPDL_POE_not_used
#pragma vector=VECT_POE_OEI1
__interrupt  void Interrupt_OEI1(void)
#pragma vector=VECT_POE_OEI2
__interrupt  void Interrupt_OEI2(void)
#endif

#ifdef RPDL_RTC_not_used
#pragma vector=VECT_RTC_PRD
__interrupt  void Interrupt_RTC_PRD(void)
#pragma vector=VECT_RTC_ALM
__interrupt  void Interrupt_RTC_ALM(void)
#pragma vector=VECT_RTC_COUNTUP
__interrupt  void Interrupt_RTC_COUNTUP(void)
#endif

#ifdef RPDL_SCI_not_used
#pragma vector=VECT_SCI0_ERI0
__interrupt  void Interrupt_SCI0_ERI0(void)
#pragma vector=VECT_SCI1_ERI1
__interrupt  void Interrupt_SCI1_ERI1(void)
#pragma vector=VECT_SCI2_ERI2
__interrupt  void Interrupt_SCI2_ERI2(void)
#pragma vector=VECT_SCI3_ERI3
__interrupt  void Interrupt_SCI3_ERI3(void)
#pragma vector=VECT_SCI4_ERI4
__interrupt  void Interrupt_SCI4_ERI4(void)
#pragma vector=VECT_SCI5_ERI5
__interrupt  void Interrupt_SCI5_ERI5(void)
#pragma vector=VECT_SCI6_ERI6
__interrupt  void Interrupt_SCI6_ERI6(void)
#pragma vector=VECT_SCI7_ERI7
__interrupt  void Interrupt_SCI7_ERI7(void)
#pragma vector=VECT_SCI8_ERI8
__interrupt  void Interrupt_SCI8_ERI8(void)
#pragma vector=VECT_SCI9_ERI9
__interrupt  void Interrupt_SCI9_ERI9(void)
#pragma vector=VECT_SCI10_ERI10
__interrupt  void Interrupt_SCI10_ERI10(void)
#pragma vector=VECT_SCI11_ERI11
__interrupt  void Interrupt_SCI11_ERI11(void)
#pragma vector=VECT_SCI12_ERI12
__interrupt  void Interrupt_SCI12_ERI12(void)

#pragma vector=VECT_SCI0_RXI0
__interrupt  void Interrupt_SCI0_RXI0(void)
#pragma vector=VECT_SCI1_RXI1
__interrupt  void Interrupt_SCI1_RXI1(void)
#pragma vector=VECT_SCI2_RXI2
__interrupt  void Interrupt_SCI2_RXI2(void)
#pragma vector=VECT_SCI3_RXI3
__interrupt  void Interrupt_SCI3_RXI3(void)
#pragma vector=VECT_SCI4_RXI4
__interrupt  void Interrupt_SCI4_RXI4(void)
#pragma vector=VECT_SCI5_RXI5
__interrupt  void Interrupt_SCI5_RXI5(void)
#pragma vector=VECT_SCI6_RXI6
__interrupt  void Interrupt_SCI6_RXI6(void)
#pragma vector=VECT_SCI7_RXI7
__interrupt  void Interrupt_SCI7_RXI7(void)
#pragma vector=VECT_SCI8_RXI8
__interrupt  void Interrupt_SCI8_RXI8(void)
#pragma vector=VECT_SCI9_RXI9
__interrupt  void Interrupt_SCI9_RXI9(void)
#pragma vector=VECT_SCI10_RXI10
__interrupt  void Interrupt_SCI10_RXI10(void)
#pragma vector=VECT_SCI11_RXI11
__interrupt  void Interrupt_SCI11_RXI11(void)
#pragma vector=VECT_SCI12_RXI12
__interrupt  void Interrupt_SCI12_RXI12(void)

#pragma vector=VECT_SCI0_TEI0
__interrupt  void Interrupt_SCI0_TEI0(void)
#pragma vector=VECT_SCI1_TEI1
__interrupt  void Interrupt_SCI1_TEI1(void)
#pragma vector=VECT_SCI2_TEI2
__interrupt  void Interrupt_SCI2_TEI2(void)
#pragma vector=VECT_SCI3_TEI3
__interrupt  void Interrupt_SCI3_TEI3(void)
#pragma vector=VECT_SCI4_TEI4
__interrupt  void Interrupt_SCI4_TEI4(void)
#pragma vector=VECT_SCI5_TEI5
__interrupt  void Interrupt_SCI5_TEI5(void)
#pragma vector=VECT_SCI6_TEI6
__interrupt  void Interrupt_SCI6_TEI6(void)
#pragma vector=VECT_SCI7_TEI7
__interrupt  void Interrupt_SCI7_TEI7(void)
#pragma vector=VECT_SCI8_TEI8
__interrupt  void Interrupt_SCI8_TEI8(void)
#pragma vector=VECT_SCI9_TEI9
__interrupt  void Interrupt_SCI9_TEI9(void)
#pragma vector=VECT_SCI10_TEI10
__interrupt  void Interrupt_SCI10_TEI10(void)
#pragma vector=VECT_SCI11_TEI11
__interrupt  void Interrupt_SCI11_TEI11(void)
#pragma vector=VECT_SCI12_TEI12
__interrupt  void Interrupt_SCI12_TEI12(void)

#pragma vector=VECT_SCI0_TXI0
__interrupt  void Interrupt_SCI0_TXI0(void)
#pragma vector=VECT_SCI1_TXI1
__interrupt  void Interrupt_SCI1_TXI1(void)
#pragma vector=VECT_SCI2_TXI2
__interrupt  void Interrupt_SCI2_TXI2(void)
#pragma vector=VECT_SCI3_TXI3
__interrupt  void Interrupt_SCI3_TXI3(void)
#pragma vector=VECT_SCI4_TXI4
__interrupt  void Interrupt_SCI4_TXI4(void)
#pragma vector=VECT_SCI5_TXI5
__interrupt  void Interrupt_SCI5_TXI5(void)
#pragma vector=VECT_SCI6_TXI6
__interrupt  void Interrupt_SCI6_TXI6(void)
#pragma vector=VECT_SCI7_TXI7
__interrupt  void Interrupt_SCI7_TXI7(void)
#pragma vector=VECT_SCI8_TXI8
__interrupt  void Interrupt_SCI8_TXI8(void)
#pragma vector=VECT_SCI9_TXI9
__interrupt  void Interrupt_SCI9_TXI9(void)
#pragma vector=VECT_SCI10_TXI10
__interrupt  void Interrupt_SCI10_TXI10(void)
#pragma vector=VECT_SCI11_TXI11
__interrupt  void Interrupt_SCI11_TXI11(void)
#pragma vector=VECT_SCI12_TXI12
__interrupt  void Interrupt_SCI12_TXI12(void)
#endif

#ifdef RPDL_SPI_not_used
#pragma vector=VECT_RSPI0_SPEI0
__interrupt  void Interrupt_RSPI0_SPEI0(void)
#pragma vector=VECT_RSPI0_SPRI0
__interrupt  void Interrupt_RSPI0_SPRI0(void)
#pragma vector=VECT_RSPI0_SPTI0
__interrupt  void Interrupt_RSPI0_SPTI0(void)
#pragma vector=VECT_RSPI0_SPII0
__interrupt  void Interrupt_RSPI0_SPII0(void)
#endif

#ifdef RPDL_TMR_not_used
#pragma vector=VECT_TMR0_CMIA0
__interrupt  void Interrupt_TMR0_CMA(void)
#pragma vector=VECT_TMR0_CMIB0
__interrupt  void Interrupt_TMR0_CMB(void)
#pragma vector=VECT_TMR0_OVI0
__interrupt  void Interrupt_TMR0_OV(void)
#pragma vector=VECT_TMR1_CMIA1
__interrupt  void Interrupt_TMR1_CMA(void)
#pragma vector=VECT_TMR1_CMIB1
__interrupt  void Interrupt_TMR1_CMB(void)
#pragma vector=VECT_TMR1_OVI1
__interrupt  void Interrupt_TMR1_OV(void)
#pragma vector=VECT_TMR2_CMIA2
__interrupt  void Interrupt_TMR2_CMA(void)
#pragma vector=VECT_TMR2_CMIB2
__interrupt  void Interrupt_TMR2_CMB(void)
#pragma vector=VECT_TMR2_OVI2
__interrupt  void Interrupt_TMR2_OV(void)
#pragma vector=VECT_TMR3_CMIA3
__interrupt  void Interrupt_TMR3_CMA(void)
#pragma vector=VECT_TMR3_CMIB3
__interrupt  void Interrupt_TMR3_CMB(void)
#pragma vector=VECT_TMR3_OVI3
__interrupt  void Interrupt_TMR3_OV(void)
#endif

#ifdef RPDL_TPU_not_used
#pragma vector=VECT_TPU0_TGI0A
__interrupt  void Interrupt_TPU0_TGI0A(void)
#pragma vector=VECT_TPU0_TGI0B
__interrupt  void Interrupt_TPU0_TGI0B(void)
#pragma vector=VECT_TPU0_TGI0C
__interrupt  void Interrupt_TPU0_TGI0C(void)
#pragma vector=VECT_TPU0_TGI0D
__interrupt  void Interrupt_TPU0_TGI0D(void)
#pragma vector=VECT_TPU0_TCI0V
__interrupt  void Interrupt_TPU0_TCI0V(void)
#pragma vector=VECT_TPU1_TGI1A
__interrupt  void Interrupt_TPU1_TGI1A(void)
#pragma vector=VECT_TPU1_TGI1B
__interrupt  void Interrupt_TPU1_TGI1B(void)
#pragma vector=VECT_TPU1_TCI1V
__interrupt  void Interrupt_TPU1_TCI1V(void)
#pragma vector=VECT_TPU1_TCI1U
__interrupt  void Interrupt_TPU1_TCI1U(void)
#pragma vector=VECT_TPU2_TGI2A
__interrupt  void Interrupt_TPU2_TGI2A(void)
#pragma vector=VECT_TPU2_TGI2B
__interrupt  void Interrupt_TPU2_TGI2B(void)
#pragma vector=VECT_TPU2_TCI2V
__interrupt  void Interrupt_TPU2_TCI2V(void)
#pragma vector=VECT_TPU2_TCI2U
__interrupt  void Interrupt_TPU2_TCI2U(void)
#pragma vector=VECT_TPU3_TGI3A
__interrupt  void Interrupt_TPU3_TGI3A(void)
#pragma vector=VECT_TPU3_TGI3B
__interrupt  void Interrupt_TPU3_TGI3B(void)
#pragma vector=VECT_TPU3_TGI3C
__interrupt  void Interrupt_TPU3_TGI3C(void)
#pragma vector=VECT_TPU3_TGI3D
__interrupt  void Interrupt_TPU3_TGI3D(void)
#pragma vector=VECT_TPU3_TCI3V
__interrupt  void Interrupt_TPU3_TCI3V(void)
#pragma vector=VECT_TPU4_TGI4A
__interrupt  void Interrupt_TPU4_TGI4A(void)
#pragma vector=VECT_TPU4_TGI4B
__interrupt  void Interrupt_TPU4_TGI4B(void)
#pragma vector=VECT_TPU4_TCI4V
__interrupt  void Interrupt_TPU4_TCI4V(void)
#pragma vector=VECT_TPU4_TCI4U
__interrupt  void Interrupt_TPU4_TCI4U(void)
#pragma vector=VECT_TPU5_TGI5A
__interrupt  void Interrupt_TPU5_TGI5A(void)
#pragma vector=VECT_TPU5_TGI5B
__interrupt  void Interrupt_TPU5_TGI5B(void)
#pragma vector=VECT_TPU5_TCI5V
__interrupt  void Interrupt_TPU5_TCI5V(void)
#pragma vector=VECT_TPU5_TCI5U
__interrupt  void Interrupt_TPU5_TCI5U(void)
#endif

#if FAST_INTC_VECTOR == VECT_FCU_FIFERR
__fast_interrupt  void Interrupt_FIFERR(void)
#else
#pragma vector=VECT_FCU_FIFERR
__interrupt  void Interrupt_FIFERR(void)
#endif
{
	/* The user can put their own code here */
}

#if FAST_INTC_VECTOR == VECT_FCU_FRDYI
__fast_interrupt  void Interrupt_FRDYI(void)
#else
#pragma vector=VECT_FCU_FRDYI
__interrupt  void Interrupt_FRDYI(void)
#endif
{
	/* The user can put their own code here */
}
/* End of file */
