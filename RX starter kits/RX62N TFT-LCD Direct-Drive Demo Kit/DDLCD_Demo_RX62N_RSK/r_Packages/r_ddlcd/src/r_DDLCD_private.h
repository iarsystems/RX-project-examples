/******************************************************************************
* DISCLAIMER
* Please refer to http://www.renesas.com/disclaimer
******************************************************************************/
#ifndef R_DDLCD_INTERNAL_H
#define R_DDLCD_INTERNAL_H

#include <iorx62n.h>
#include "config_r_Packages.h"
#include "config_r_ddlcd_platform.h"
#include "config_r_ddlcd_panel.h"
#include "config_r_ddlcd.h"

//************************************************
// INTERNAL MACROS and CHECKING
// The following were renamed for user, but don't want to for libary source readibility
#define V_LINES_PULSE       DD_V_LINES_PULSE
#define V_LINES_BACK_PORCH  DD_V_LINES_BACK_PORCH
#define V_LINES_DISPLAY     DD_V_LINES_DISPLAY
#define V_LINES_FRONT_PORCH DD_V_LINES_FRONT_PORCH
#define H_DOT_PULSE         DD_H_DOT_PULSE
#define H_DOT_BACK_PORCH    DD_H_DOT_BACK_PORCH
#define H_DOT_DISPLAY       DD_H_DOT_DISPLAY
#define H_DOT_FRONT_PORCH   DD_H_DOT_FRONT_PORCH

/* Define Pin Macros to ease implementation (secondary macros _1, _2 used for expansion ONLY */
#define CAT(a,b) a ## b
#define MERGE2(a,b) CAT(a,b)

/* Set signal DDR to output */
#define DDRo2(port,pin) MERGE2(PORT,port) MERGE2(.DDR.BIT.B,pin) = 1;
#define DDRo(signal) DDRo2(DD_##signal##_PORT, DD_##signal##_PIN)

/* Set signal DDR to input */
#define DDRi2(port,pin) MERGE2(PORT,port) MERGE2(.DDR.BIT.B,pin) = 0;
#define DDRi(signal) DDRi2(DD_##signal##_PORT, DD_##signal##_PIN)

/* Write signal level (for output) */
#define PINo2(port,pin) MERGE2(PORT,port) MERGE2(.DR.BIT.B,pin)
#define PINo(signal) PINo2(DD_##signal##_PORT, DD_##signal##_PIN)

/* Read signal level (for input) */
#define PINi2(port,pin) MERGE2(PORT,port) MERGE2(.PORT.BIT.B,pin)
#define PINi(signal) PINi2(DD_##signal##_PORT, DD_##signal##_PIN)

/* Signals buffer control (for inputs) */
#define ICRi2(port,pin) MERGE2(PORT,port) MERGE2(.ICR.BIT.B,pin)
#define ICRi(signal) ICRi2(DD_##signal##_PORT, DD_##signal##_PIN)

#define size_ra(ra) (sizeof(ra)/sizeof(ra[0]))
/* macros to allow easy access to new style ICU bits and IODEFINE macros */
#define _ICU2_(func,unit_channel) _##func(unit_channel)
#define _ICU_(func,irq) _ICU2_(func, MERGE2(irq ## _UNIT, MERGE2(_,irq ## _CHANNEL)))


#ifdef DD_TMR_MTU
#define DD_TMR_UNIT MTUA
#define DD_TMR MTU
#define DD_TMR_IRQ _MTU
#endif
#ifdef DD_TMR_TPU
#define DD_TMR_UNIT TPU
#define DD_TMR TPU
#define DD_TMR_IRQ _TPU
#endif

// Ensure that the resolution of the panel matches the resolution of the linked resources.
// To change the linked resources, copy the appropriate resolution sub-directory to 
// the "resources" directory.
// For example... $(Workspace)\Resources\QVGA -> $(Workspace)\Resources
#ifndef DD_PANEL_ROTATE
  #if ((V_LINES_DISPLAY > DD_FRAME_HEIGHT) || (H_DOT_DISPLAY > DD_FRAME_WIDTH))
  #error Resource Resolution does not match Panel Resolution.
  #endif
#else  
  #if ((V_LINES_DISPLAY > DD_FRAME_WIDTH) || (H_DOT_DISPLAY > DD_FRAME_HEIGHT))
  #error Resource Resolution does not match Panel Resolution.
  #endif
#endif

// number of bus cycles in the dot clock
#define DOT_BUS_CYCLES (BCLK_FREQUENCY/DD_DOT_CLOCK_FREQUENCY_DATA)
#if (DOT_BUS_CYCLES*DD_DOT_CLOCK_FREQUENCY_DATA < (BCLK_FREQUENCY-2)) || (DOT_BUS_CYCLES*DD_DOT_CLOCK_FREQUENCY_DATA > (BCLK_FREQUENCY+2))
  #error BCLK_FREQUENCY must be integer multiple of DD_DOT_CLOCK_FREQUENCY_DATA
#endif

#if defined DD_MODE_SRAM_NOMUX || defined DD_MODE_SRAM
  #if (DOT_BUS_CYCLES < DD_FRAME_BUS_CYCLES)
    #error ATTEMPTING TO RUN DOT CLOCK FASTER THAN RAM ALLOWS (DD_DOT_CLOCK_FREQUENCY_DATA)
  #endif
#elif defined DD_MODE_SDRAM
  #if (DD_DOT_CLOCK_FREQUENCY_DATA != (BCLK_FREQUENCY/(DD_FRAME_SDRAM_CAS_LATENCY+1)))
    #error DD_DOT_CLOCK_FREQUENCY_DATA MUST BE BCLK_FREQUENCY/(DD_FRAME_SDRAM_CAS_LATENCY+1) WITH DD_MODE_SDRAM MODE
  #endif
#elif defined DD_MODE_SDRAM_CLUSTER
  #if (DD_DOT_CLOCK_FREQUENCY_DATA != BCLK_FREQUENCY)
    #error DD_DOT_CLOCK_FREQUENCY_DATA MUST BE BCLK_FREQUENCY WITH DD_MODE_SDRAM_CLUSTER MODE
  #endif
#endif

// number of TMR cycles in the dot clock
#define DOT_TMR_CYCLES (PCLK_FREQUENCY/DD_DOT_CLOCK_FREQUENCY_BLANK)
#if (DOT_TMR_CYCLES*DD_DOT_CLOCK_FREQUENCY_BLANK < (PCLK_FREQUENCY-2)) || (DOT_TMR_CYCLES*DD_DOT_CLOCK_FREQUENCY_BLANK > (PCLK_FREQUENCY+2))
  #error PCLK_FREQUENCY must be integer multiple of DD_DOT_CLOCK_FREQUENCY_BLANK
#endif

#if ((PCLK_FREQUENCY/DD_DOT_CLOCK_FREQUENCY_BLANK) < 2)
  #error NEED AT LEAST 2 PCLK STATES PER DOT CLOCK (MODIFY DD_DOT_CLOCK_FREQUENCY_BLANK)
#endif

/* Define ratio of bus clock to peripheral clock 1x=1, 2x=2, 4x=4 */
// #define BCLK_PCLK_RATIO (BCLK_MUL/PCLK_MUL)
#define DOT_TO_PCLK(DOT_CLOCKS) ((DOT_CLOCKS) * (PCLK_FREQUENCY/DD_DOT_CLOCK_FREQUENCY_BLANK))
#define BCLK_TO_DOTB(BCLK_CLOCKS) ((BCLK_CLOCKS) / (BCLK_FREQUENCY/DD_DOT_CLOCK_FREQUENCY_BLANK))
#define DOTD_TO_DOTB(DOTD_CLOCKS) ((DOTD_CLOCKS) * (DD_DOT_CLOCK_FREQUENCY_BLANK/1000L) / (DD_DOT_CLOCK_FREQUENCY_DATA/1000L)) // Div by 1000 required to keep from exceeding 32 bit size

#if defined DD_MODE_SRAM_NOMUX
  #if ( DD_DOT_CLOCK_FREQUENCY_DATA != DD_DOT_CLOCK_FREQUENCY_BLANK)
    #error DD_DOT_CLOCK_FREQUENCY_DATA MUST MATCH DD_DOT_CLOCK_FREQUENCY_BLANK WITH DD_MODE_SRAM_NOMUX
  #endif
#endif

#ifndef H_DOT_FRONT_PORCH_BLANK
#define H_DOT_FRONT_PORCH_BLANK 0
#endif

// ExDMA Constants and BUS cycle additions
#define DRAM_REFRESH_BCLK (6)       // BClock cycles to refresh DRAM (includes RAS)
#define DRAM_RAS_BCLK (2)           // Additional BClock cyles for RAS (on page change)
#ifndef EXDMA_REQ_TO_READ           // allow this to be overridden if necessary (hidden feature)
  #define EXDMA_REQ_TO_READ (4)     // BClock Cycles from request to data read start if nothing else on bus 
#endif
#define EXDMA_NEGOTIATE (3)         // BClock Cycles to renogitiate for the bus
#if (BCLK_FREQUENCY == ICLK_FREQUENCY)
#define EXDMA_CLUSTER_NEGOTIATE (5) // BClock cycles added to each cluster (more synchronization when equal?)
#else
#define EXDMA_CLUSTER_NEGOTIATE (4) // BClock cycles added to each cluster
#endif
#define EXDMA_SEGMENTS (4)          // Number of segments to break up transfer (H8S 2456 transfer/request block limit)

#if defined DD_MODE_SRAM
  // Number of extra dot clocks required during block transfer
  #define ExDMA_BLK_EXTRA (EXDMA_REQ_TO_READ)  // Number of extra dot clocks required during block transfer
  // Factor in all extras and bus rates to determine number of dot clocks in ExDMA transfer (round up to integer dot clock value).
  #define H_DOT_DISPLAY_DATA (DOTD_TO_DOTB(H_DOT_DISPLAY) + BCLK_TO_DOTB(ExDMA_BLK_EXTRA) + 1)
#elif defined DD_MODE_SRAM_NOMUX
  // Number of extra bus clocks required during block transfer
  #define ExDMA_BLK_EXTRA 0
  // Number of Dot Clocks during data portion of line
  #define H_DOT_DISPLAY_DATA (H_DOT_DISPLAY)
#elif defined DD_MODE_SDRAM_CLUSTER
  // Number of extra bus clocks required during block transfer
  #define ExDMA_BLK_EXTRA (EXDMA_REQ_TO_READ + \
                          (DRAM_REFRESH_BCLK *(1+(H_DOT_DISPLAY)/(BCLK_FREQUENCY / 64000L))) +  \
                          ((EXDMA_CLUSTER_NEGOTIATE) *(1+((H_DOT_DISPLAY-1)/16))) +  \
                          (DRAM_RAS_BCLK *(2+(H_DOT_DISPLAY/DD_FRAME_SDRAM_PAGE_SIZE))))
                          // time from ExDMA request to start of transfer
                          // number of BCLK cycles consumed by refresh cycles (assumes refresh every 15.625us...64000L = 1000000L/15.625
                          // number of BCLK cycles consumed by cluster negotiation
                          // number of BCLK cycles consumed by Row Address Strobes (minimum 2...one at start ExDMA and one or more during transfer)
  
  // Factor in all extras and bus rates to determine number of dot clocks in ExDMA transfer (round up to integer dot clock value).
  #define H_DOT_DISPLAY_DATA (BCLK_TO_DOTB(H_DOT_DISPLAY + ExDMA_BLK_EXTRA) + 1)
#elif defined DD_MODE_SDRAM
  // Number of extra bus clocks required during block transfer
  #define ExDMA_BLK_EXTRA (EXDMA_REQ_TO_READ + \
                          (DRAM_REFRESH_BCLK *(1+(H_DOT_DISPLAY * (DD_FRAME_SDRAM_CAS_LATENCY+1))/(BCLK_FREQUENCY / 64000L))) +  \
                          (DRAM_RAS_BCLK *(2+(H_DOT_DISPLAY/DD_FRAME_SDRAM_PAGE_SIZE))))
                          // time from ExDMA request to start of transfer
                          // number of BCLK cycles consumed by refresh cycles (assumes refresh every 15.625us...64000L = 1000000L/15.625
                          // number of BCLK cycles consumed by Row Address Strobes (minimum 2...one at start ExDMA and one or more during transfer)
  
  // Factor in all extras and bus rates to determine number of dot clocks in ExDMA transfer (round up to integer dot clock value).
  #define H_DOT_DISPLAY_DATA (BCLK_TO_DOTB((H_DOT_DISPLAY * (DD_FRAME_SDRAM_CAS_LATENCY+1)) + ExDMA_BLK_EXTRA) + 1)
#else
  #error NO DD MEMORY TYPE SPECIFIED
#endif

/***************************************************************************************************/
/* Ensure we can meet desired behavior */
#define V_LINES_PANEL (V_LINES_PULSE+V_LINES_BACK_PORCH+V_LINES_DISPLAY+V_LINES_FRONT_PORCH)
#define H_DOT_PERIOD_DATA (H_DOT_PULSE+H_DOT_BACK_PORCH+H_DOT_DISPLAY_DATA+H_DOT_FRONT_PORCH)
#define H_DOT_PERIOD_BLANK (H_DOT_PULSE+H_DOT_BACK_PORCH+H_DOT_DISPLAY+H_DOT_FRONT_PORCH+H_DOT_FRONT_PORCH_BLANK)

#define H_DOT_PERIOD_PANEL ((V_LINES_DISPLAY*H_DOT_PERIOD_DATA)+((V_LINES_PANEL-V_LINES_DISPLAY)*H_DOT_PERIOD_BLANK))
#define VLINES_EXTEND ((DD_DOT_CLOCK_FREQUENCY_BLANK-(H_DOT_PERIOD_PANEL*DD_DESIRED_FRAME_RATE))/(H_DOT_PERIOD_BLANK*DD_DESIRED_FRAME_RATE))
#if (VLINES_EXTEND < 0)
  #error Cannot achieve DD_DESIRED_FRAME_RATE
#endif

#define V_LINES_PERIOD (V_LINES_PANEL+VLINES_EXTEND)

#define H_DOTS_BLANK ((V_LINES_PERIOD - V_LINES_DISPLAY) * (H_DOT_PERIOD_BLANK))
#define H_DOTS_DATA (V_LINES_DISPLAY * H_DOT_PERIOD_DATA)
#define MCU_ACCESS_PCT ((H_DOTS_BLANK * 100)/(H_DOTS_BLANK+H_DOTS_DATA))
#if (DD_MINIMUM_MCU_ACCESS_PCT > MCU_ACCESS_PCT)
  #error Cannot achieve MINIMUM_MCU_ACCESS
#endif
/***************************************************************************************************/

#define TMR_CHANNEL(channel) MERGE2(DD_TMR,channel)
#define TMR_DD(signal) TMR_CHANNEL(DD_##signal##_CHANNEL)

#define TMR_IO_A .TIORH
#define TMR_IO_B .TIORH
#define TMR_IO_C .TIORL
#define TMR_IO_D .TIORL

#define TMR_IO(channel,pin) TMR_CHANNEL(channel)  MERGE2(TMR_IO_,pin) MERGE2(.BIT.IO,pin)
#define TMR_DD_IO(signal) TMR_IO(DD_##signal##_CHANNEL,DD_##signal##_TIOC)

#define TMR_TGR(channel,pin) TMR_CHANNEL(channel)  MERGE2(.TGR,pin)
#define TMR_DD_TGR(signal) TMR_TGR(DD_##signal##_CHANNEL,DD_##signal##_TIOC)

#define TMR_TGF(channel,pin)TMR_CHANNEL(channel)  MERGE2(.TSR.BIT.TGF,pin)
#define TMR_DD_TGF(signal) TMR_TGF(DD_##signal##_CHANNEL,DD_##signal##_TIOC)

#define TMR_TGIE(channel,pin) TMR_CHANNEL(channel)  MERGE2(.TIER.BIT.TGIE,pin)
#define TMR_DD_TGIE(signal) TMR_TGIE(DD_##signal##_CHANNEL,DD_##signal##_TIOC)

#define TMR_TCRA 0x20 /* clear on channel A */
#define TMR_TCRB 0x40 /* clear on channel B */
#define TMR_TCRC 0xA0 /* clear on channel C */
#define TMR_TCRD 0xC0 /* clear on channel D */
#define TMR_TCR(pin) MERGE2(TMR_TCR,pin)
#define TMR_DD_TCR(signal) TMR_TCR(DD_##signal##_TIOC)

#define DD_TMR_ICU2(func,unit,channeln,pinn) _ICU2_(func, MERGE2(MERGE2(unit,channeln), MERGE2(MERGE2(_TGI,pinn),channeln)))
#define  DD_TMR_ICU(func,irq) DD_TMR_ICU2(func, DD_TMR_IRQ, DD_##irq ## _CHANNEL, DD_##irq ## _TIOC)

#endif
