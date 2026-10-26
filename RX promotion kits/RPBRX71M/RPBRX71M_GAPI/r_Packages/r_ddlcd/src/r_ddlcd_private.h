/******************************************************************************
 * DISCLAIMER
 * Please refer to http://www.renesas.com/disclaimer
 ******************************************************************************/
#ifndef R_DDLCD_INTERNAL_H
    #define R_DDLCD_INTERNAL_H

    #include "platform.h"
    #include "config_r_packages.h"
    #include "config_r_ddlcd_platform.h"
    #include "config_r_ddlcd_panel.h"
    #include "config_r_ddlcd.h"

/************************************************* */
/* INTERNAL MACROS and CHECKING */
/* The following were renamed for user, but don't want to for library source readability */
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

/* backend macros */
    #define DDRo2(port,pin) port MERGE2(.PDR.BIT.B,pin) = 1;
    #define DDRi2(port,pin) port MERGE2(.PDR.BIT.B,pin) = 0;
    #define PINo2(port,pin) port MERGE2(.PODR.BIT.B,pin)
    #define PINi2(port,pin) port MERGE2(.PIDR.BIT.B,pin)

/* Set signal DDR to output */
    #define DDRo(signal) DDRo2(DD_##signal##_PORT, DD_##signal##_PIN)

/* Set signal DDR to input */
    #define DDRi(signal) DDRi2(DD_##signal##_PORT, DD_##signal##_PIN)

/* Write signal level (for output) */
    #define PINo(signal) PINo2(DD_##signal##_PORT, DD_##signal##_PIN)

/* Read signal level (for input) */
    #define PINi(signal) PINi2(DD_##signal##_PORT, DD_##signal##_PIN)

/* Signals buffer control (for inputs) */
    #define ICRi2(port,pin) port MERGE2(.ICR.BIT.B,pin)
    #define ICRi(signal) ICRi2(DD_##signal##_PORT, DD_##signal##_PIN)
/* Signals port function control */
    #define PFCRs2(mpc, port,pin,value) MERGE2(MERGE2(MPC., mpc),MERGE2(pin,PFS.BIT.PSEL)=value;) port MERGE2(.PMR.BIT.B,pin) = 1;
    #define PFCRs(signal,value) PFCRs2(DD_##signal##_MPC, DD_##signal##_PORT, DD_##signal##_PIN, value)

    #define size_ra(ra) (sizeof(ra)/sizeof(ra[0]))
/* macros to allow easy access to new style ICU bits and IODEFINE macros */
    #define _ICU2_(func,unit_channel) _##func(unit_channel)
    #define _ICU_(func,irq) _ICU2_(func, MERGE2(irq ## _UNIT, MERGE2(_,irq ## _CHANNEL)))

    #ifdef DD_TMR_MTU
        #define DD_TMR_UNIT MTU
        #define TMR_CHANNEL(channel) MTU##channel
        #define DD_TMR_IRQ _MTU
    #endif

/* define DD_PDEN_CHANNEL always to make synch/start/stop of channels simpler */
    #ifndef DD_PDEN_CHANNEL
        #define DD_PDEN_CHANNEL DD_HDEN_CHANNEL
    #endif

/* define DD_HSYNC_CHANNEL always to make synch/start/stop of channels simpler */
    #ifndef DD_HSYNC_CHANNEL
        #define DD_HSYNC_CHANNEL DD_HDEN_CHANNEL
    #endif

/* define DD_VSYNC_CHANNEL always to make synch/start/stop of channels simpler */
    #ifndef DD_VSYNC_CHANNEL
        #define DD_VSYNC_CHANNEL DD_HDEN_CHANNEL
    #endif

/* Ensure that the resolution of the panel matches the resolution of the linked resources.
 To change the linked resources, copy the appropriate resolution sub-directory to
 the "resources" directory.
 For example... $(Workspace)\Resources */
    #if (V_LINES_DISPLAY * H_DOT_DISPLAY) > DD_FRAME_SIZE
        #error Frame Buffer not large enought to hold panel image
    #endif

/* number of bus cycles in the dot clock */
    #define DOT_BUS_CYCLES (BSP_BCLK_HZ/DD_DOT_CLOCK_FREQUENCY_DATA)
    #if ((DOT_BUS_CYCLES*DD_DOT_CLOCK_FREQUENCY_DATA < (BSP_BCLK_HZ-2)) || (DOT_BUS_CYCLES*DD_DOT_CLOCK_FREQUENCY_DATA > (BSP_BCLK_HZ+2)))
        #error BSP_BCLK_HZ must be integer multiple of DD_DOT_CLOCK_FREQUENCY_DATA
    #endif

    #if (DOT_BUS_CYCLES < DD_FRAME_BUS_CYCLES)
        #error ATTEMPTING TO RUN DOT CLOCK FASTER THAN RAM ALLOWS (DD_DOT_CLOCK_FREQUENCY_DATA)
    #endif

/* number of TMR cycles in the dot clock */
    #define DOT_TMR_CYCLES (BSP_PCLKA_HZ/DD_DOT_CLOCK_FREQUENCY_BLANK)
    #if (DOT_TMR_CYCLES*DD_DOT_CLOCK_FREQUENCY_BLANK < (BSP_PCLKA_HZ-2)) || (DOT_TMR_CYCLES*DD_DOT_CLOCK_FREQUENCY_BLANK > (BSP_PCLKA_HZ+2))
        #error BSP_PCLKB_HZ must be integer multiple of DD_DOT_CLOCK_FREQUENCY_BLANK
    #endif

    #if ((BSP_PCLKB_HZ/DD_DOT_CLOCK_FREQUENCY_BLANK) < 2)
        #error NEED AT LEAST 2 PCLK STATES PER DOT CLOCK (MODIFY DD_DOT_CLOCK_FREQUENCY_BLANK)
    #endif

/* Define ratio of bus clock to peripheral clock 1x=1, 2x=2, 4x=4 */
    #define DOT_TO_PCLK(DOT_CLOCKS) ((DOT_CLOCKS) * (BSP_PCLKA_HZ/DD_DOT_CLOCK_FREQUENCY_BLANK))

    #define BCLK_TO_DOTB(BCLK_CLOCKS) ((BCLK_CLOCKS) / (BSP_BCLK_HZ/DD_DOT_CLOCK_FREQUENCY_BLANK))
    #define DOTD_TO_DOTB(DOTD_CLOCKS) ((DOTD_CLOCKS) * (DD_DOT_CLOCK_FREQUENCY_BLANK/1000L) / (DD_DOT_CLOCK_FREQUENCY_DATA/1000L)) // Div by 1000 required to keep from exceeding 32 bit size

    #if defined DD_MODE_SRAM_NOMUX
        #if ( DD_DOT_CLOCK_FREQUENCY_DATA != DD_DOT_CLOCK_FREQUENCY_BLANK)
            #error DD_DOT_CLOCK_FREQUENCY_DATA MUST MATCH DD_DOT_CLOCK_FREQUENCY_BLANK WITH DD_MODE_SRAM_NOMUX
        #endif
    #endif

    #ifndef H_DOT_FRONT_PORCH_BLANK
        #define H_DOT_FRONT_PORCH_BLANK 0
    #endif

/* DMA Constants and BUS cycle additions */
    #ifndef DMA_REQ_TO_READ           /* allow this to be overridden if necessary (hidden feature) */
        #define DMA_REQ_TO_READ (30)     /* Bus Clock Cycles from request to data read start if nothing else is on bus  */
/* This value has been derrived empirically, it takes in to account interrupt latency,
 * DMA trigger latency, bus arbitration and initial access. This is needed to align
 * the DMA data output with the panel timing signals */
    #endif

    #if defined DD_MODE_SRAM_NOMUX
/* Number of Dot Clocks during data portion of line */
        #define H_DOT_DISPLAY_DATA (H_DOT_DISPLAY)
    #else
        #error NO DD MEMORY TYPE SPECIFIED
    #endif

/***************************************************************************************************/
/* Ensure we can meet desired behaviour */
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

    #define TMR_DD2(channel) TMR_CHANNEL(channel)
    #define TMR_DD(signal) TMR_DD2(DD_##signal##_CHANNEL)

    #define TMR_IO_A .TIORH
    #define TMR_IO_B .TIORH
    #define TMR_IO_C .TIORL
    #define TMR_IO_D .TIORL

    #define TMR_IO(channel,pin) TMR_CHANNEL(channel)  MERGE2(TMR_IO_,pin) MERGE2(.BIT.IO,pin)
    #define TMR_DD_IO(signal) TMR_IO(DD_##signal##_CHANNEL,DD_##signal##_TIOC)

    #define TMR_IO2(channel,pin) TMR_CHANNEL(channel)  .TIOR MERGE2(.BIT.IO,pin)
    #define TMR_DD_IO2(signal) TMR_IO2(DD_##signal##_CHANNEL,DD_##signal##_TIOC)

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
