/******************************************************************************
 * DISCLAIMER

 * This software is supplied by Renesas Electronics Corp. and is only
 * intended for use with Renesas products. No other uses are authorized.

 * This software is owned by Renesas Electronics Corp. and is protected under
 * all applicable laws, including copyright laws.

 * THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES
 * REGARDING THIS SOFTWARE, WHETHER EXPRESS, IMPLIED OR STATUTORY,
 * INCLUDING BUT NOT LIMITED TO WARRANTIES OF MERCHANTABILITY, FITNESS FOR A
 * PARTICULAR PURPOSE AND NON-INFRINGEMENT.  ALL SUCH WARRANTIES ARE EXPRESSLY
 * DISCLAIMED.

 * TO THE MAXIMUM EXTENT PERMITTED NOT PROHIBITED BY LAW, NEITHER RENESAS
 * ELECTRONICS CORP. NOR ANY OF ITS AFFILIATED COMPANIES SHALL BE LIABLE
 * FOR ANY DIRECT, INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES
 * FOR ANY REASON RELATED TO THE THIS SOFTWARE, EVEN IF RENESAS OR ITS
 * AFFILIATES HAVE BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.

 * Renesas reserves the right, without notice, to make changes to this
 * software and to discontinue the availability of this software.
 * By using this software, you agree to the additional terms and
 * conditions found by accessing the following link:
 * http://www.renesas.com/disclaimer
 ******************************************************************************/
/**********************************************************************************
 FILE NAME    : r_ddlcd.c

 DESCRIPTION  : LCD Direct Drive RX LCD Direct Driver - for internal memory.
 This is a Parallel RGB LCD driver that uses MTU channels and
 DMA to drive the LCD panel directly from internal SRAM.

 This version of DDCD only supports internal SRAM memory
 (needs no external memory).

 Please note there are other variants of Renesas Direct Drive
 that supports external memory of various types.

 Copyright   : 2016 Renesas Electronics America
 Copyright   : 2016 Renesas Electronics Corporation.
 All Rights Reserved
 *********************************************************************************/
#include "FreeRTOS.h"
#include "config_r_gapi.h"
#include "r_ddlcd_public.h" 
#include "r_ddlcd_private.h"
#include "task.h"

#define FRAME_BSC2(cs, reg) MERGE2(MERGE2(BSC.CS, cs), reg)
#define FRAME_BSC(reg) FRAME_BSC2(DD_FRAME_CS, reg)

/************************************************* */
/* INTERNAL Memory Allocation */
typedef struct
{
    uint32_t wcr1; /* wait control register 1 */
    uint32_t wcr2; /* wait control register 2 */
} cs_reg_t;

static int16_t Hcount;       // line counter
static int16_t VLinesPeriod; // number or lines in the frame...changes dynamically to set desired frame rate.
static int16_t FrameRate = DD_DESIRED_FRAME_RATE;    // Frame rate corresponding to the VLinesPeriod

typedef struct
{
    Raster_t *pLine;           // pointer to starting line of region
    int16_t  LineNext;          // value to change pLine by to get to start of next line
    int16_t  Height;            // number of lines in this region
} line_list_type;

static line_list_type LineList[DD_FRAME_REGIONS_MAX + 1]; // information about raster data to display (allocate spare block to ensure proper list termination)
static line_list_type *pLineList;                   // pointer to current LineList record
static line_list_type ActiveLineList;        // Copy of LineList record that Driver modifies and uses for DMA transfer

static uint16_t *TFTptr;

uint16_t volatile DDLCD_Vcount;          // frame counter
uint16_t volatile DDLCD_BusActive;       // flag indicating if we're running DMA on this line

/******************************************************************************
 Function Prototypes
 ******************************************************************************/
static void dmac_setup (void);
static void reset_lcd (void);
__interrupt __nested static void R_DDLCD_DD_DMACTRIG_isr (void);

static const uint8_t TMR_mask[] =
{ 0x01, 0x02, 0x04, 0x40, 0x80, 0x00 };

#if defined(BSP_MCU_RX64_ALL) || defined(BSP_MCU_RX71_ALL)
    #if (BSP_PCLKA_HZ < BSP_PCLKB_HZ)
        #error PCLKA must be greater than or equal to PCLKB
    #else
        #define PCLKA_MULTIPLIER 1
    #endif
#endif

/*****************************************************************************
 Function Name: R_DDLCD_Init
 Description:   Initialises the Direct Drive LCD system.
 Parameters:    None
 Return value:  0 - Setup Successful
 *****************************************************************************/
DDLCD_Resp_t R_DDLCD_Init (void)
{
    /* Unlock MPC registers to enable writing to them. */
    R_BSP_RegisterProtectDisable(BSP_REG_PROTECT_MPC);

#ifdef DD_PANEL_POWER_PIN
    PINo(PANEL_POWER) = 1;      // prepare to turn on power to display (when output enabled below)
    DDRo(PANEL_POWER);// turn on power
#endif

    /* Stop all LCD_DD channels */
    DD_TMR_UNIT.TSTRA.BYTE &= (~((((((TMR_mask[DD_HPER_CHANNEL]) | (TMR_mask[DD_HSYNC_CHANNEL]))
            | (TMR_mask[DD_VSYNC_CHANNEL])) | (TMR_mask[DD_HDEN_CHANNEL])) | (1u << DD_PDEN_CHANNEL))
            | (TMR_mask[DD_DOTCLK_CHANNEL])));
    reset_lcd();

    dmac_setup();                               // Configure DMA

    /* Configure Vsynch */
    PFCRs(VSYNC, 0x1);

    TMR_DD(VSYNC).TMDR1.BYTE = 0x02;              // use PWM mode 1
    TMR_DD(VSYNC).TCR.BYTE = 0x60;                // Configure for HPER Synch clear (may be overwritten by HPER)
    TMR_DD_IO2(VSYNC) = 0x6;                      // Vsynch init high, high on match.
    TMR_DD_TGR(VSYNC) = DOT_TO_PCLK(H_DOT_PERIOD_BLANK) - 1; // period timeout...Vsynch pulse is multiples of Hperiod

    TMR_DD(VPER).TMDR1.BYTE = 0x02;              // use PWM mode 1
    TMR_DD(VPER).TCR.BYTE = 0x60;                // Configure for HPER Synch clear (may be overwritten by HPER)
    TMR_DD(VPER).TIOR.BIT.IOB = 0x06;
    TMR_DD_TGR(VPER) = 10;                       // Vsynch pulse is multiples of Hperiod

#if defined DD_HSYNC_TIOC
    /* Configure Horizontal Synch */
    #if (defined(__RX63NIODEFINE_HEADER__) || defined(__RX64MIODEFINE_HEADER__) || defined(__RX71MIODEFINE_HEADER__))
    PFCRs(HSYNC, 0x1);
    #endif

    TMR_DD(HSYNC).TMDR1.BYTE = 0x02;               // use PWM mode 1
    TMR_DD(HSYNC).TCR.BYTE = 0x60;                 // Configure for HPER Synch clear (may be overwritten by HPER)
    TMR_DD_IO(HSYNC) = 0x2;                        // Hsynch init low, high on match.
    TMR_DD_TGR(HSYNC) = DOT_TO_PCLK(H_DOT_PULSE) - 1; // Hynch Pulse Width
    TMR_DD_TGR(HSYNC) *= PCLKA_MULTIPLIER;
#endif

    /* Configure Data Enable */
    PFCRs(HDEN, 0x1);

    TMR_DD(HDEN).TMDR1.BYTE = 0x02;                // PWM Mode 1 operation
    TMR_DD(HDEN).TCR.BYTE = 0x60;                  // Clear from the HPER Synchronous Input
    TMR_DD_IO(HDEN) = 0x6;                         // Active Data Mask init high, High on Match

    TMR_DD_TGR(HDEN) = ((DOT_TO_PCLK(H_DOT_PULSE+H_DOT_BACK_PORCH + 0.5) - DMA_REQ_TO_READ) - (DOT_BUS_CYCLES - 1)) - 1;

    TMR_DD_IO(HDEN2) = 0x6;                       // Active Data Mask init high, High on Match
    TMR_DD_TGR(HDEN2) = DOT_TO_PCLK(H_DOT_PULSE+H_DOT_BACK_PORCH+H_DOT_DISPLAY) - 1;

    /* because we're using PWM 1 mode for DOTCLOCK and HSYNC, we need to phase HDEN 1 cycle earlier...as state change is at END of DOTCLOCK and HSYNC (not 0 cycle) */
    TMR_DD_TGR(HDEN) -= 1;
    TMR_DD_TGR(HDEN2) -= 1;

    TMR_DD_TGR(HDEN) *= PCLKA_MULTIPLIER;
    TMR_DD_TGR(HDEN2) *= PCLKA_MULTIPLIER;

    /* Configure Horizontal Period */
    TMR_DD(HPER).TMDR1.BYTE = 0x02;                // use PWM mode 1
    TMR_DD(HPER).TCR.BYTE = TMR_DD_TCR(HPER);     // Set TCR to clear TCNT on HPER
    TMR_DD_IO(HPER) = 0x5;                        // Init high, 0 on match (for PWM mode 1)
    TMR_DD_TGR(HPER) = DOT_TO_PCLK(H_DOT_PERIOD_BLANK) - 1; // period timeout
    TMR_DD_TGR(HPER) *= PCLKA_MULTIPLIER;

    /* Configure Dot Clock */
    PFCRs(DOTCLK, 0x1);

    TMR_DD(DOTCLK).TMDR1.BYTE = 0x02;              // use PWM mode 1

    TMR_DD(DOTCLK).TCR.BYTE = TMR_DD_TCR(DOTPER); // Set TCR to clear TCNT on DOTPER

    TMR_DD_IO(DOTCLK) = 0x5;                      // Dot Clock init hi, Low on match
    TMR_DD_IO(DOTPER) = 0x6;                      // Used for timeout

    TMR_DD_TGR(DOTCLK) = DOT_TO_PCLK(0.5) - 1;      // set falling edge time
    TMR_DD_TGR(DOTPER) = DOT_TO_PCLK(1) - 1;        // Dot clock period

    /* Configure PDEN - used as an interrupt to trigger the DMA channel */
    TMR_DD(PDEN).TMDR1.BYTE = 0x02;                // PWM Mode 1 operation
    TMR_DD(PDEN).TCR.BYTE = 0x40;                  // Clear from the MTU1.TGRB
    TMR_DD(PDEN).TIOR.BIT.IOA = 0x07;              // Active Data Mask init high, TOGGLE ON MATCH

    TMR_DD_TGR(PDEN) = ((DOT_TO_PCLK(H_DOT_PULSE+H_DOT_BACK_PORCH + 0.5) - DMA_REQ_TO_READ) - (DOT_BUS_CYCLES - 1)) - 1;

    /* because we're using PWM 1 mode for DOTCLOCK and HSYNC, we need to phase PDEN 1 cycle earlier...as state change is at END of DOTCLOCK and HSYNC (not 0 cycle) */
    TMR_DD_TGR(PDEN) -= 1;
    TMR_DD_TGR(PDEN2) = DOT_TO_PCLK(H_DOT_PERIOD_BLANK) - 1; // period timeout

    TMR_DD_TGR(PDEN) *= PCLKA_MULTIPLIER;
    TMR_DD_TGR(PDEN2) *= PCLKA_MULTIPLIER;

    /* Allow interrupt access */
    TMR_DD_TGIE(PDEN) = 1;
    DD_TMR_ICU(IPR,PDEN)= DD_INTERRUPT_PRIORITY;
    DD_TMR_ICU(IEN,PDEN)= 1;

    /* Allow timers to access ICU */
    TMR_DD_TGIE(HDEN) = 1;
    TMR_DD_TGIE(HDEN2) = 1;
    TMR_DD_TGIE(DOTCLK) = 1;
    DD_TMR_ICU(IPR,HDEN)= DD_INTERRUPT_PRIORITY;
    DD_TMR_ICU(IPR,HDEN2)= DD_INTERRUPT_PRIORITY;

    /* Run ISR at end of Active Data Mask */
    DD_TMR_ICU(IEN,HDEN)= 1;

    /* Run Task Control ISR at start of Vertical Active period (via DOTCLK IRQ) */
    DD_TMR_ICU(IPR,DOTCLK)= DD_NOTIFY_INTERRUPT_PRIORITY;

    /* Synch horizontal channels */
    DD_TMR_UNIT.TSYRA.BYTE = ((((((TMR_mask[DD_HPER_CHANNEL]) | (TMR_mask[DD_HSYNC_CHANNEL]))
            | (TMR_mask[DD_VSYNC_CHANNEL])) | (TMR_mask[DD_HDEN_CHANNEL])) | (1u << DD_PDEN_CHANNEL)));

    /* Start all LCD_DD channels */
    DD_TMR_UNIT.TSTRA.BYTE |= (((((((TMR_mask[DD_HPER_CHANNEL]) | (TMR_mask[DD_HSYNC_CHANNEL]))
            | (TMR_mask[DD_VSYNC_CHANNEL])) | (TMR_mask[DD_HDEN_CHANNEL])) | (1u << DD_PDEN_CHANNEL))
            | (TMR_mask[DD_DOTCLK_CHANNEL])));

    Hcount = 0;              // ensure first IRQ starts new frame
    VLinesPeriod = V_LINES_PERIOD;

    /* Lock MPC registers. */
    R_BSP_RegisterProtectEnable(BSP_REG_PROTECT_MPC);

    return (0);
}
/*******************************************************************************
 End of function R_DDLCD_Init
 *******************************************************************************/

/*******************************************************************************
 Name: R_DDLCD_SetLineSource()
 Arguments:
 Region: Region of display (horizontal strip). Ranging from 0 (bottom) to DD_FRAME_REGIONS_MAX (user defined MACRO top).
 LineCount: Number of lines for this region.
 pSource: Address of first pixel of first line of region.
 LineStep: Distance (in pixels/int16_t) from first pixel of first line to first pixel of second line (source regions can be wider than the panel).
 Note: Rotate field of DDLCD_Descriptor_t is handled here by the DMA DD_LCD module however it is not supported
 in this internal memory DMA implementation. If rotated flag is set an error is returned.
 Returns:  0 = ok, -1 = NG.
 Description: setActiveFrame defines which frame buffer to display
 *****************************************************************************/
int16_t R_DDLCD_SetLineSource (int16_t Region, int16_t LineCount, void *pSource, int16_t LineStep)
{
    int32_t PixelStart;
    Raster_t *pS = (Raster_t *) pSource;

    if (Region >= DD_FRAME_REGIONS_MAX)
    {
        return (-1);
    }

    /* set line step and determine pixel start offset */
    if (0 == pDDLCD_Descriptor->Panel.flags.Rotate)
    {
        /* only allow region to be up to panel size */
        if (LineCount > V_LINES_PANEL)
        {
            return (-1);
        }

        if (0 == pDDLCD_Descriptor->Panel.flags.Flip_Columns)
        {
            PixelStart = 0;                       // move to start of line
            DD_DMAC.DMAMD.BIT.SM = 0x2;    // increment source address after each xfer
        }
        else
        {
            PixelStart = H_DOT_DISPLAY - 1;           // move to end of line
            DD_DMAC.DMAMD.BIT.SM = 0x3;    // decrement source address after each xfer
        }
        if (0 == pDDLCD_Descriptor->Panel.flags.Flip_Rows)
        {
            LineList[Region].LineNext = LineStep;   // Increment through lines
        }
        else
        {
            LineList[Region].LineNext = -LineStep;  // Decrement through lines
            PixelStart += ((LineCount - 1) * LineStep);  // move to end of frame
        }
    }
    else
    {
        /*   Note: Rotate field of DDLCD_Descriptor_t is handled here by the exDMA DD_LCD module however it is not supported
         in this internal memory DMA implementation. If rotated flag is set an error is returned.   */
        return (-1);
    }

    LineList[Region].pLine = pS + PixelStart;
    LineList[Region].Height = LineCount;
    return (0);
}
/*******************************************************************************
 End of function R_DDLCD_SetLineSource
 *******************************************************************************/

/*****************************************************************************
 Name: R_DDLCD_GetFrameRate()
 Arguments: none:
 Returns:  Current frame Rate in Hz.
 Description: Returns current frame rate.
 *****************************************************************************/
int16_t R_DDLCD_GetFrameRate (void)
{
    return (FrameRate);
}
/*******************************************************************************
 End of function R_DDLCD_GetFrameRate
 *******************************************************************************/

/*****************************************************************************
 Name: R_DDLCD_SetFrameRate()
 Arguments: rate: Desired Frame Rate in Hz.
 Returns:  Positive % MCU access if we can achieve frame rate, Negative if cannot achieve frame rate.
 Description: Controls the display frame rate by controlling the vertical blanking time.
 *****************************************************************************/
int16_t R_DDLCD_SetFrameRate (int16_t rate)
{
    /* Check frame rate is supported by the panel */
    if (rate >= DD_MIN_FRAME_RATE)
    {
        /* first calculate how many DOTCLK cycles remaining per second at desired rate */
        int32_t VLinesExtend = DD_DOT_CLOCK_FREQUENCY_BLANK - (H_DOT_PERIOD_PANEL * (int32_t) rate);

        /* now divide remaining DOTCLK into "rate" number of lines */
        VLinesExtend /= (H_DOT_PERIOD_BLANK * (int32_t) rate);

        /* if positive, set the value for use */
        if ((VLinesExtend > 0) && (rate > 0))
        {
            int32_t HDotsBlank;
            VLinesPeriod = V_LINES_PANEL + (int16_t) VLinesExtend;
            HDotsBlank = (VLinesPeriod - V_LINES_DISPLAY) * (H_DOT_PERIOD_BLANK);
            FrameRate = rate;
            return ((int16_t) ((HDotsBlank * 100L) / (HDotsBlank + H_DOTS_DATA)));
        }
        else
        {
            return ((int16_t) VLinesExtend);
        }
    }
    else /* Rate < DD_MIN_FRAME_RATE */
    {
        return (-1); /* Return an error */
    }
}
/*******************************************************************************
 End of function R_DDLCD_SetFrameRate
 *******************************************************************************/

/*****************************************************************************
 Name: R_DDLCD_Off()
 Arguments: none
 Returns:  nothing
 Description: Shuts down the LCD - powers down LCD module
 *****************************************************************************/
void R_DDLCD_Off (void)
{
#ifdef DD_PANEL_POWER_PIN
    PINo(PANEL_POWER) = 0;            // turn off power to LCD module
#endif
}
/*******************************************************************************
 End of function R_DDLCD_Off
 *******************************************************************************/

/*****************************************************************************
 Name: R_DDLCD_Backlight()
 Arguments: int state
 Returns:  nothing
 Description: nonzero value turns backlight on...
 *****************************************************************************/
void R_DDLCD_Backlight (int state)
{
    if (state)
    {
        DDRo(BACKLIGHT);           // enable backlight by setting direction register
        PINo(BACKLIGHT) = 1;       // turn on power to backlight
    }
    else
    {
        PINo(BACKLIGHT) = 0;       // turn off power to backlight
        DDRi(BACKLIGHT);           // disable backlight by setting direction register
    }
}
/*******************************************************************************
 End of function R_DDLCD_Backlight
 *******************************************************************************/

/******************************************************************************
 Private Functions
 ******************************************************************************/

/*****************************************************************************
 Function Name: dmac_setup
 Description:   Setup the DMAC ready to transfer data from internal SRAM to
 external address space upon which the LCD is connected
 Parameters:    None
 Return value:  None
 *****************************************************************************/
static void dmac_setup (void)
{
    /* Unlock MPC registers to enable writing to them. */
    R_BSP_RegisterProtectDisable(BSP_REG_PROTECT_MPC);

    /* Protect off for MSTP access */
    R_BSP_RegisterProtectDisable(BSP_REG_PROTECT_LPC_CGC_SWR);

    DMAC.DMAST.BIT.DMST = 0; /* Disable any DMA transfers in progress */

    /* Configure external bus for DMAC transfer of data to LCD */

    /* Turn on external data bus - used for Display */
    SYSTEM.SYSCR0.WORD = 0x5A03; /* enable the external bus...must also set key and internal ROME bit */

    BSC.CS3CR.BIT.EXENB = 1; /* Enable CS3 space, used for LCD data bus access */
    BSC.CS3CR.BIT.EMODE = 1; /* Endian is not the same as the operating mode, used for LCD data bus access */

#ifdef LCD_16BIT
    MPC.PFBCR0.BYTE = 0x10; /* Enable PE[0..7] as D8-15 */
    BSC.CS3CR.BIT.BSIZE = 0; /* 16-bit external access */
#else
    BSC.CS3CR.BIT.BSIZE = 2; /* 8-bit external access */
#endif //LCD_16BIT

    /* initial chip select configuration */
    /* Calculate the wait states required for accessing the LCD */
    FRAME_BSC(WCR1.BIT.CSRWAIT) = DOT_BUS_CYCLES - 1;
    FRAME_BSC(WCR1.BIT.CSWWAIT) = DOT_BUS_CYCLES - 1;
    FRAME_BSC(WCR2.BIT.RDON) = 0;

    TFTptr = (uint16_t*) 0x05000000; /* CS3 area */

    /* Take DMA out of Module Stop Mode.                                */
    /* Should be enabled by default, but just to make sure              */
    MSTP(DMAC) = 0;

    DD_DMAC.DMDAR = (void*) TFTptr; /* Destination address              */

    DD_DMAC.DMTMD.BIT.MD = 2; /* Block Transfer Mode                    */
    DD_DMAC.DMTMD.BIT.DTS = 2; /* No repeat area                        */

#ifdef LCD_16BIT
    DD_DMAC.DMTMD.BIT.SZ = 1; /* WORD transfer                          */
#else
    DD_DMAC.DMTMD.BIT.SZ = 0; /* BYTE transfer                          */
#endif //LCD_16BIT

    DD_DMAC.DMTMD.BIT.DCTG = 1; /* Interrupt Trigger                    */

    /* Vector Number of interrupt to trigger DMA */
    /* Refer to section 15.3.1 of the hardware   */
    /* user manual and the table 15.4 for the    */
    /* activation source vector number           */
    ICU.DMRSR0 = DD_TMR_ICU(VECT, PDEN);

    DD_DMAC.DMINT.BIT.DTIE = 0; /* No interrupt                            */
    DD_DMAC.DMINT.BIT.ESIE = 0; /* No interrupt                            */
    DD_DMAC.DMINT.BIT.RPTIE = 0; /* No interrupt                            */
    DD_DMAC.DMINT.BIT.SARIE = 0; /* No interrupt                            */
    DD_DMAC.DMINT.BIT.DARIE = 0; /* No interrupt                            */

    DD_DMAC.DMAMD.BIT.SARA = 0;
    DD_DMAC.DMAMD.BIT.DM = 0; /* Destination is fixed                    */
    DD_DMAC.DMAMD.BIT.DARA = 0;
    DD_DMAC.DMOFR = 0;

    DD_DMAC.DMREQ.BIT.CLRS = 0;
    DD_DMAC.DMREQ.BIT.SWREQ = 0;

    DD_DMAC.DMCSL.BIT.DISEL = 1; /* No CPU interrupt issued                 */
    DMAC.DMAST.BIT.DMST = 1;

    /* Lock MPC registers. */
    R_BSP_RegisterProtectEnable(BSP_REG_PROTECT_MPC);

    /* Protect on for MSTP access */
    R_BSP_RegisterProtectEnable(BSP_REG_PROTECT_LPC_CGC_SWR);
}
/*****************************************************************************
 End of function  dmac_setup
 ******************************************************************************/

/*****************************************************************************
 Function Name: reset_lcd
 Description:   Asserts the LCD reset to the LCD panel for the required time
 Parameters:    None
 Return value:  None
 *****************************************************************************/
static void reset_lcd (void)
{
#ifdef DD_LCDRESET_PIN
    PINo(LCDRESET) = 0;    // Reset Asserted (when output enabled below)
    DDRo(LCDRESET);        // Enable output
    vTaskDelay(LCD_RESET_TIME); // Wait for some time
    PINo(LCDRESET) = 1;    // Deassert reset
#endif //DD_LCDRESET_PIN
}
/*****************************************************************************
 End of function  reset_lcd
 ******************************************************************************/

/*****************************************************************************
 Function Name: R_DDLCD_DD_DMACTRIG_isr
 Description:    Description: Direct Drive interrupt service routine during the vertical blanking period.
 During the blanking period, we interrupt at the BEGINNING of the data enable period.
 This allows us maximum time to set up the TMR units prior to the start of the next line..
 VECTOR 209 - INTERRUPT 8 - MTU1.A
 Parameters:    None
 Return value:  None
 *****************************************************************************/
#pragma vector=DD_TMR_ICU(VECT,PDEN)
__interrupt __nested static void R_DDLCD_DD_DMACTRIG_isr (void)
{
    /* This is the ISR for MTU1A, this channel is used for PDEN */
    /* This ISR does nothing, however the interrupt triggers the DMA */
    /* starting the data transfer to the panel */
}
/*****************************************************************************
 End of function  R_DDLCD_DD_DMACTRIG_isr
 ******************************************************************************/

/*****************************************************************************
 Function Name: R_DDLCD_DD_BLANK_isr
 Description:   Direct Drive interrupt service routine during the vertical blanking period.
 During the blanking period, we interrupt at the BEGINNING of the data enable period.
 This allows us maximum time to set up the TMR units prior to the start of the next line..
 HDEN - Int 223 - TGR 3A
 Parameters:    None
 Return value:  None
 *****************************************************************************/
#pragma vector=DD_TMR_ICU(VECT,HDEN)
__interrupt __nested static void R_DDLCD_DD_BLANK_isr (void)
{
    /* Configure behaviour for the NEXT line */
    if (Hcount >= VLinesPeriod)
    {
        MTU2.TIOR.BIT.IOA = 0x1;                     // Vsynch init low, Low on match
        MTU2.TIOR.BIT.IOB = 0x1;                     // Vsynch init low, Low on match
        Hcount = 0;
    }
    else
    {
        switch (Hcount)
        {
            case (V_LINES_PULSE) :
            {
                MTU2.TIOR.BIT.IOA = 0x6;                     // Vsynch init High, High on match
                MTU2.TIOR.BIT.IOB = 0x6;                     // Vsynch init High, High on match
                break;
            }

            case (V_LINES_PULSE + V_LINES_BACK_PORCH) :
            {
                TMR_DD_IO(HDEN) = 0x5;                      // Enable Active Data Mask init high, Low on Match
                DDLCD_BusActive = 1;

                pLineList = LineList;                       // reset line list
                ActiveLineList = *pLineList++;            // copy initial line list for the driver and increment pointer
                ActiveLineList.Height += ((V_LINES_PULSE + V_LINES_BACK_PORCH) - 1); // offset Height so I can compare against "Hcount"

                DD_TMR_ICU(IR,HDEN2)= 0;                   // clear pending interrupt request
                DD_TMR_ICU(IEN,HDEN2)= 1;                  // start using HDEN2
                DD_TMR_ICU(IEN,DOTCLK)= 1;                 // Signal scheduler to suspend RAM access tasks
                break;
            }

            case ((V_LINES_PULSE + V_LINES_BACK_PORCH) + 1) :
            {
                DD_TMR_ICU(IEN,HDEN)= 0;                   // stop using HDEN (re-enabeld by HDEN2 ISR)
                TMR_DD_TGR(HPER) = DOT_TO_PCLK(H_DOT_PERIOD_DATA)-1;// period timeout during data transfer

                TMR_DD_TGR(HPER) *= PCLKA_MULTIPLIER;

                TMR_DD_TGR(HDEN2) = DOT_TO_PCLK(H_DOT_PULSE+H_DOT_BACK_PORCH+H_DOT_DISPLAY_DATA)-1;// end of enable for data transfer

                TMR_DD_TGR(HDEN2) *= PCLKA_MULTIPLIER;

                break;
            }

            case (((V_LINES_PULSE + V_LINES_BACK_PORCH) + V_LINES_DISPLAY) + 1):
            {
                TMR_DD_TGR(HPER) = DOT_TO_PCLK(H_DOT_PERIOD_BLANK)-1; // period timeout during blanking
                TMR_DD_TGR(HPER) *= PCLKA_MULTIPLIER;
                TMR_DD_TGR(HDEN2) = DOT_TO_PCLK(H_DOT_PULSE+H_DOT_BACK_PORCH+H_DOT_DISPLAY)-1;// end of enable during blanking (generates interrupts, so must be accurate even during blanking)
                TMR_DD_TGR(HDEN2) *= PCLKA_MULTIPLIER;

                break;
            }

            default:
            {
                /* Do nothing */
            }
        }
    }

    if ((Hcount < (V_LINES_PULSE+V_LINES_BACK_PORCH)) || (Hcount > ((V_LINES_PULSE+V_LINES_BACK_PORCH)+V_LINES_DISPLAY)))
    {
        Hcount++;                                       // increment the horizontal line counter
    }
}
/*******************************************************************************
 End of function R_DDLCD_DD_BLANK_isr
 *******************************************************************************/

/*****************************************************************************
 Function Name: R_DDLCD_DD_DATA_isr
 Description:   Direct Drive interrupt service routine during the active data piece of the frame.
 During the data portion, we interrupt at the END of the data enable period.
 We use this configure the DMA for the next line of the transfer.
 HDEN2 - Int 224 - TGR 3B
 Parameters:    None
 Return value:  None
 *****************************************************************************/
#pragma vector=DD_TMR_ICU(VECT,HDEN2)
__interrupt __nested static void R_DDLCD_DD_DATA_isr (void)
{
    if (Hcount < ((V_LINES_PULSE + V_LINES_BACK_PORCH) + V_LINES_DISPLAY))
    {
        DD_DMAC.DMSAR = ActiveLineList.pLine;    // start of next line transfer
        DD_DMAC.DMCRB = 1;                           // output one line (block) of pixels

        DD_DMAC.DMCRA = (H_DOT_DISPLAY << 16) + H_DOT_DISPLAY;  // transfer one panel line

        DD_DMAC.DMCNT.BIT.DTE = 1;                          // re-start DMA transfer
        ActiveLineList.pLine += ActiveLineList.LineNext;  // Active portion, need to update line pointer

        if (Hcount >= (((V_LINES_PULSE + V_LINES_BACK_PORCH) + V_LINES_DISPLAY) - 1))
        {
            /* this is last line to display...turn on HDEN */
            DD_TMR_ICU(IR,HDEN)= 0;                  // clear pending interrupt request
            DD_TMR_ICU(IEN,HDEN) = 1;// start using HDEN
        }
    }
    else
    {
        TMR_DD_IO(HDEN) = 0x6;       // Disable Active Data Mask init high, High on Match
        DDLCD_BusActive = 0;
        DD_TMR_ICU(IEN,HDEN2) = 0;// stop using HDEN2
        DD_TMR_ICU(IEN,DOTCLK) = 1;// Signal scheduler to resume RAM access tasks
        DDLCD_Vcount++;//  increment v count here to give maximum time while bus idle
    }

    Hcount++;        // increment the horizontal line counter
}
/*******************************************************************************
 End of function R_DDLCD_DD_DATA_isr
 *******************************************************************************/
