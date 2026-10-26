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
FILE NAME    : r_DDLCD.c

DESCRIPTION  : LCD Direct Drive RX LCD Direct Driver.

Copyright   : 2010 Renesas Electronics America
Copyright   : 2010 Renesas Electronics Corporation.
          All Rights Reserved

**********************************************************************************/
#include "r_ddlcd_public.h" 
#include "r_DDLCD_private.h"

#define FRAME_BSC2(cs, reg) MERGE2(MERGE2(BSC.CS, cs), reg)
#define FRAME_BSC(reg) FRAME_BSC2(DD_FRAME_CS, reg)

//************************************************
// INTERNAL Memory Allocation
typedef struct
{
  uint32_t wcr1; /* wait control register 1 */
  uint32_t wcr2; /* wait control register 2 */
} cs_reg_t;

#if defined DD_MODE_SRAM || defined DD_MODE_SRAM_NOMUX
  static cs_reg_t cs_data[2]; /* [0]= original user, [1]=during data transfer */
#endif

static int16_t Hcount;       // line counter
static int16_t VLinesPeriod; // number or lines in the frame...changes dynamically to set desired frame rate.
static int16_t FrameRate=DD_DESIRED_FRAME_RATE;    // Frame rate corresponding to the VLinesPeriod

typedef struct
{
  Raster_t *pLine;     // pointer to starting line of region
  int16_t LineNext;          // value to change pLine by to get to start of next line
  int16_t Height;            // number of lines in this region
} line_list_type;

static line_list_type LineList[DD_FRAME_REGIONS_MAX+1];  // information about raster data to display (allocate spare block to ensure proper list termination)
static line_list_type *pLineList;                   // pointer to current LineList record
static line_list_type ActiveLineList;               // Copy of LineList record that Driver modifies and uses for ExDMA transfer

uint16_t DDLCD_Vcount;          // frame counter
uint16_t DDLCD_BusActive;       // flag indicating if we're running ExDMA on this line

/*****************************************************************************
Name: R_DDLCD_init()    
Arguments: none                     
Returns:  Error Code as necesary     
Description:sequences the power to the LCD display, initalizes the exDMA,
and timer output TOICA0 to generate the exDMA request signal
*****************************************************************************/
#ifdef DD_TMR_MTU
static const uint8_t TMR_mask[]={0x01,0x02,0x04,0x40,0x80,0x00};
#else
static const uint8_t TMR_mask[]={0x01,0x02,0x04,0x08,0x10,0x20};
#endif

#pragma section=".BDD_RASTERS"

DDLCD_Resp_t R_DDLCD_Init(void)
{
  #ifdef DD_PANEL_POWER_PIN
    PINo(PANEL_POWER) = 1;    // prepare to turn on power to display (when output enabled below)
    DDRo(PANEL_POWER);        // turn on power
  #endif

  PINo(HSYNC) = 1;              // Hsync active low , so set high.
  DDRo(HSYNC);                  // set Hsync to output
  PINo(VSYNC) = 1;              // Vsync active low , so set high
  DDRo(VSYNC);                  // set Vsync to output

  PINo(DOTCLK) = 1;  // Set to idle state of DOTCLK
  DDRo(DOTCLK);      // Set EDACK2 to output DOTCLK when we're bit banging

  // configure ExDMA
  ICRi(EXDMAC_REQ) = 1;          // configure ExDMA pin to connect to peripheral
  DD_EXDMAC.EDMCNT.BIT.DTE = 0;     // stops DMA transfer
  DD_EXDMAC.EDMTMD.BIT.DCTG = 2;    // transfer source: 0=software, 2=external, 3=peripheral

  if (sizeof(uint16_t) == sizeof(Raster_t))
  {
    DD_EXDMAC.EDMTMD.BIT.SZ = 1;    // dma size:  0=8bit, 1=16 bit, 2=32bit
  }
  else
  {
    DD_EXDMAC.EDMTMD.BIT.SZ = 2;    // dma size:  0=8bit, 1=16 bit, 2=32bit
  }

  DD_EXDMAC.EDMRMD.BIT.DREQS = 2;   // 0=rising edge, 1=falling edge, 2=low level

#ifdef DD_MODE_SDRAM_CLUSTER
  DD_EXDMAC.EDMTMD.BIT.MD = 3;      // transfer mode: 0= normal, 1= repeat, 2= block, 3=cluster
#else
  DD_EXDMAC.EDMTMD.BIT.MD = 2;      // transfer mode: 0= normal, 1= repeat, 2= block, 3=cluster
#endif

#if defined DD_MODE_SDRAM || defined DD_MODE_SRAM
  DD_EXDMAC.EDMOMD.BIT.DACKE = 1;   // Allow EDACK to output
  #ifdef DD_INVERT_DOT
    DD_EXDMAC.EDMOMD.BIT.DACKS = 1; // Invert EDACK as latching on falling edge
  #else
    DD_EXDMAC.EDMOMD.BIT.DACKS = 0; // No inversion as latching on rising edge
  #endif
#else
  DD_EXDMAC.EDMOMD.BIT.DACKE = 0;   // Don't output EDACK
#endif
                                    
  DD_EXDMAC.EDMAMD.LONG = 0;        // sets register to reset state
  DD_EXDMAC.EDMAMD.BIT.AMS = 1;     // single address mode selected 
  DD_EXDMAC.EDMAMD.BIT.DIR = 0;     // single address direction read from EDMSAR
  DD_EXDMAC.EDMTMD.BIT.DTS = 2;     // 0=block repeat source, 1=block repeat destination, 2=no block repeat
  EXDMAC.EDMAST.BIT.DMST = 1;       // start EXDMAC unit

#ifndef DD_PANEL_ROTATE
  #ifndef DD_INVERT_H_DOT
    DD_EXDMAC.EDMAMD.BIT.SM = 0x2;    // increment source address after each xfer
  #else                               
    DD_EXDMAC.EDMAMD.BIT.SM = 0x3;    // decrement source address after each xfer
  #endif
#else
  DD_EXDMAC.EDMAMD.BIT.SM = 0x1;      // use offset register to control DMA transfers
  #ifndef DD_INVERT_V_LINES
    DD_EXDMAC.EDMOFR = -V_LINES_RESOURCE*2;  // Two bytes per transfer (decrement through "Rows")
  #else
    DD_EXDMAC.EDMOFR = V_LINES_RESOURCE*2;  // Two bytes per transfer (increment through "Rows")
  #endif
#endif

  {
    /* initialize line list with default values */
    int16_t region;
    for (region=0; region < size_ra(LineList); region++)
      R_DDLCD_SetLineSource(region, DD_FRAME_HEIGHT, (Raster_t *)__section_begin(".BDD_RASTERS"), DD_FRAME_WIDTH);
  }

#if defined DD_MODE_SRAM || defined DD_MODE_SRAM_NOMUX
  /* initial chip select configuration */
  /* copy user settings */
  cs_data[0].wcr1 = FRAME_BSC(WCR1.LONG);
  cs_data[0].wcr2 = FRAME_BSC(WCR2.LONG);

  /* create lcd dd settings */
  FRAME_BSC(WCR1.BIT.CSRWAIT) = DOT_BUS_CYCLES-1;
  FRAME_BSC(WCR2.BIT.RDON) = 0;

  /* copy for runtime use */
  cs_data[1].wcr1 = FRAME_BSC(WCR1.LONG);
  cs_data[1].wcr2 = FRAME_BSC(WCR2.LONG);

  DD_EXDMAC.EDMOMD.BIT.DACKW = 1;   // set EDACK to more square duty cycle (only SRAM use affected)

  /* restore user settings */
  FRAME_BSC(WCR1.LONG)= cs_data[0].wcr1 ;
  FRAME_BSC(WCR2.LONG)= cs_data[0].wcr2 ;
#endif
    
  // Stop all LCD_DD channels
  DD_TMR_UNIT.TSTR.BYTE &= ~((TMR_mask[DD_HPER_CHANNEL])|(TMR_mask[DD_HSYNC_CHANNEL])|(TMR_mask[DD_VSYNC_CHANNEL])|(TMR_mask[DD_HDEN_CHANNEL]) | (TMR_mask[DD_DOTCLK_CHANNEL]));

  // Configure Vsynch
  TMR_DD(VSYNC).TMDR.BYTE = 0x02;               // use PWM mode 1
  TMR_DD(VSYNC).TCR.BYTE = 0x60;                // Configure for HPER Synch clear (may be overwritten by HPER)
  TMR_DD_IO(VSYNC) = 0x6;                       // Vsynch init high, high on match.
  TMR_DD_TGR(VSYNC) = DOT_TO_PCLK(H_DOT_PERIOD_BLANK)-1; // period timeout...Vsynch pulse is multiples of Hperiod

  TMR_DD(VPER).TMDR.BYTE = 0x02;               // use PWM mode 1
  TMR_DD(VPER).TCR.BYTE = 0x60;                // Configure for HPER Synch clear (may be overwritten by HPER)
  TMR_DD_IO(VPER) = 0x6;                       // Vsynch init high, high on match.
  TMR_DD_TGR(VPER) = 10;                        // Vsynch pulse is multiples of Hperiod

  // Configure Horizontal Synch
  TMR_DD(HSYNC).TMDR.BYTE = 0x02;               // use PWM mode 1
  TMR_DD(HSYNC).TCR.BYTE = 0x60;                // Configure for HPER Synch clear (may be overwritten by HPER)
  TMR_DD_IO(HSYNC) = 0x2;                       // Hsynch init low, high on match.
  TMR_DD_TGR(HSYNC) = DOT_TO_PCLK(H_DOT_PULSE)-1; // Hynch Pulse Width

  // Configure Data Enable (ExDMA Request)
  TMR_DD(HDEN).TMDR.BYTE = 0x02;                // PWM Mode 1 operation
  TMR_DD(HDEN).TCR.BYTE = 0x60;                 // Clear from the HPER Synchronous Input
  TMR_DD_IO(HDEN) = 0x6;                        // Active Data Mask init high, High on Match
                                                // 
#if defined DD_MODE_SRAM_NOMUX
  TMR_DD_TGR(HDEN) = DOT_TO_PCLK(H_DOT_PULSE+H_DOT_BACK_PORCH + 0.5) - EXDMA_REQ_TO_READ - (DOT_BUS_CYCLES-1) -1;
#elif defined DD_MODE_SDRAM_CLUSTER  
  TMR_DD_TGR(HDEN) = DOT_TO_PCLK(H_DOT_PULSE+H_DOT_BACK_PORCH)-1; 
#else
  TMR_DD_TGR(HDEN) = DOT_TO_PCLK(H_DOT_PULSE+H_DOT_BACK_PORCH)-1-1; // extra -1 because in controlling the external mux we don't want to switch CLK as it is changing to avoid potential "glitch". Output of MUX should be same before/after.
#endif

  TMR_DD_IO(HDEN2) = 0x6;                       // Active Data Mask init high, High on Match
  TMR_DD_TGR(HDEN2) = DOT_TO_PCLK(H_DOT_PULSE+H_DOT_BACK_PORCH+H_DOT_DISPLAY)-1;

  /* because we're useing PWM 1 mode for DOTCLOCK and HSYNC, we need to phase HDEN 1 cycle earlier...as state change is at END of DOTCLOCK and HSYNC (not 0 cycle) */
  TMR_DD_TGR(HDEN) -= 1;
  TMR_DD_TGR(HDEN2) -= 1;
  
  // Configure Horizontal Period
//  TMR_DD(HPER).TMDR.BYTE = 0xC3;                // use PWM mode 2
  TMR_DD(HPER).TMDR.BYTE = 0x02;                // use PWM mode 1
  TMR_DD(HPER).TCR.BYTE = TMR_DD_TCR(HPER);     // Set TCR to clear TCNT on HPER
  TMR_DD_IO(HPER) = 0x5;                        // Init high, 0 on match (for PWM mode 1)
  TMR_DD_TGR(HPER) = DOT_TO_PCLK(H_DOT_PERIOD_BLANK)-1; // period timeout

  // Configure Dot Clock & Dot Period (must be same channel)
  TMR_DD(DOTCLK).TMDR.BYTE = 0x02;              // use PWM mode 1
  TMR_DD(DOTCLK).TCR.BYTE = TMR_DD_TCR(DOTPER); // Set TCR to clear TCNT on DOTPER
#ifdef DD_INVERT_DOT
  TMR_DD_IO(DOTCLK) = 0x5;                      // Dot Clock init hi, Low on match
  TMR_DD_IO(DOTPER) = 0x6;                      // Used for timeout
#else
  TMR_DD_IO(DOTCLK) = 0x2;                      // Dot Clock init low, High on match
  TMR_DD_IO(DOTPER) = 0x1;                      // Used for timeout
#endif
  TMR_DD_TGR(DOTCLK) = DOT_TO_PCLK(0.5)-1;      // set falling edge time
  TMR_DD_TGR(DOTPER) = DOT_TO_PCLK(1)-1;        // Dot clock period

  // Allow timers to access ICU
  TMR_DD_TGIE(HDEN) = 1;
  TMR_DD_TGIE(HDEN2) = 1;
  TMR_DD_TGIE(DOTCLK) = 1;
  DD_TMR_ICU(IPR,HDEN) = DD_INTERRUPT_PRIORITY;
  
  // Run ISR at end of Active Data Mask
  DD_TMR_ICU(IEN,HDEN) = 1;
  
  // Run Task Control ISR at start of Vertical Active period (via DOTCLK IRQ)
  DD_TMR_ICU(IPR,DOTCLK) = DD_NOTIFY_INTERRUPT_PRIORITY;

  // Synch horizontal channels
  DD_TMR_UNIT.TSYR.BYTE = ((TMR_mask[DD_HPER_CHANNEL])|(TMR_mask[DD_HSYNC_CHANNEL])|(TMR_mask[DD_VSYNC_CHANNEL])|(TMR_mask[DD_HDEN_CHANNEL]));
  // Start all LCD_DD channels
  DD_TMR_UNIT.TSTR.BYTE |= ((TMR_mask[DD_HPER_CHANNEL])|(TMR_mask[DD_HSYNC_CHANNEL])|(TMR_mask[DD_VSYNC_CHANNEL])|(TMR_mask[DD_HDEN_CHANNEL]) | (TMR_mask[DD_DOTCLK_CHANNEL]));

  Hcount = 0;              // ensure first IRQ starts new frame
  VLinesPeriod = V_LINES_PERIOD;

  return(0);
}
 
/*****************************************************************************
Name:  R_DDLCD__DD_BLANK_isr   
Arguments: none                     
Returns:  nothing     
Description: Direct Drive interrupt service routine during the vertical blanking period.
  During the blanking period, we interrupt at the BEGINNING of the data enable period.
  This allows us maximum time to set up the TMR units prior to the start of the next line..
*****************************************************************************/
#pragma vector=DD_TMR_ICU(VECT,HDEN)
__interrupt void R_DDLCD_DD_BLANK_isr(void)
{
  // Configure behavior for the NEXT line
  if (Hcount >= VLinesPeriod)
  {
    TMR_DD_IO(VSYNC) = 0x1;                     // Vsynch init low, Low on match
    TMR_DD_IO(VPER) = 0x1;                     // Vsynch init low, Low on match
    Hcount = 0;
  }
  else
  switch (Hcount)
  {
    case (V_LINES_PULSE):
    {
      TMR_DD_IO(VSYNC) = 0x6;                     // Vsynch init High, High on match
      TMR_DD_IO(VPER) = 0x6;                     // Vsynch init High, High on match
      break;
    }

    case (V_LINES_PULSE+V_LINES_BACK_PORCH):
    {
      TMR_DD_IO(HDEN) = 0x5;                      // Enable Active Data Mask init high, Low on Match
      DDLCD_BusActive = 1;

      /* Configure RAM for ExDMA access timing */
#if defined DD_MODE_SRAM || defined DD_MODE_SRAM_NOMUX
      /* configure ExDMAC settings */
      FRAME_BSC(WCR1.LONG)= cs_data[1].wcr1 ;
      FRAME_BSC(WCR2.LONG)= cs_data[1].wcr2 ;
#endif
      pLineList = LineList;                       // reset line list
      ActiveLineList = *pLineList++;              // copy initial line list for the driver and increment pointer
      ActiveLineList.Height += V_LINES_PULSE+V_LINES_BACK_PORCH-1; // offset Height so I can compare against "Hcount"
      
      DD_TMR_ICU(IR,HDEN2) = 0;                   // clear pending interrupt request
      DD_TMR_ICU(IEN,HDEN2) = 1;                  // start using HDEN2
      DD_TMR_ICU(IEN,DOTCLK) = 1;                 // Signal scheduler to suspend RAM access tasks
      break;
    }
    
    case (V_LINES_PULSE+V_LINES_BACK_PORCH+1):
    {
      DD_TMR_ICU(IEN,HDEN) = 0;                   // stop using HDEN (re-enabeld by HDEN2 ISR)
      TMR_DD_TGR(HPER) = DOT_TO_PCLK(H_DOT_PERIOD_DATA)-1; // period timeout during data transfer
      TMR_DD_TGR(HDEN2) = DOT_TO_PCLK(H_DOT_PULSE+H_DOT_BACK_PORCH+H_DOT_DISPLAY_DATA)-1;  // end of enable for data transfer
      break;
    }
      
    case (V_LINES_PULSE+V_LINES_BACK_PORCH+V_LINES_DISPLAY+1):
    {
      TMR_DD_TGR(HPER) = DOT_TO_PCLK(H_DOT_PERIOD_BLANK)-1; // period timeout during blanking
      TMR_DD_TGR(HDEN2) = DOT_TO_PCLK(H_DOT_PULSE+H_DOT_BACK_PORCH+H_DOT_DISPLAY)-1; // end of enable during blanking (generates interrupts, so must be accurate even during blanking)
      break;
    }
  }

  if ((Hcount < (V_LINES_PULSE+V_LINES_BACK_PORCH)) || (Hcount > (V_LINES_PULSE+V_LINES_BACK_PORCH+V_LINES_DISPLAY)))
    Hcount++;                                       // increment the horizontal line counter
}

/*****************************************************************************
Name:  R_DDLCD__DD_DATA_isr   
Arguments: none                     
Returns:  nothing     
Description: Direct Drive interrupt service routine during the active data piece of the frame.
  During the data portion, we interrupt at the END of the data enable period.
  We use this configure the ExDMA for the next line of the transfer.
*****************************************************************************/
#pragma vector=DD_TMR_ICU(VECT,HDEN2)
__interrupt void R_DDLCD_DD_DATA_isr(void)
{
  if (Hcount < (V_LINES_PULSE+V_LINES_BACK_PORCH+V_LINES_DISPLAY))
  {
    DD_EXDMAC.EDMSAR = ActiveLineList.pLine;    // start of next line transfer
    DD_EXDMAC.EDMCRB = 1;                       // output one line (block) of pixels 

#ifdef DD_MODE_SDRAM_CLUSTER
    DD_EXDMAC.EDMCRA = (16L << 8)+8L;              // transfer one 16 word cluster at a time.
#else
    DD_EXDMAC.EDMCRA = (H_DOT_DISPLAY << 16)+H_DOT_DISPLAY;  // transfer one panel line
#endif

    DD_EXDMAC.EDMCNT.BIT.DTE = 1;                     // re-start DMA transfer  
    ActiveLineList.pLine += ActiveLineList.LineNext;  // Active portion, need to update line pointer 
    
    #if (DD_FRAME_REGIONS_MAX > 1)
      /* check if ready for next region...conditionally compiled based on need */
      if (Hcount >= ActiveLineList.Height)
      {
        ActiveLineList = *pLineList++;
        ActiveLineList.Height += Hcount;
      }
    #endif

    if (Hcount >= (V_LINES_PULSE+V_LINES_BACK_PORCH+V_LINES_DISPLAY-1))
    {
      /* this is last line to display...turn on HDEN */
      DD_TMR_ICU(IR,HDEN) = 0;                  // clear pending interrupt request
      DD_TMR_ICU(IEN,HDEN) = 1;                 // start using HDEN
    }
  }
  else
  {
    TMR_DD_IO(HDEN) = 0x6;                        // Disable Active Data Mask init high, High on Match
    DDLCD_BusActive = 0;
#if defined DD_MODE_SRAM || defined DD_MODE_SRAM_NOMUX
    /* restore user settings */
    FRAME_BSC(WCR1.LONG)= cs_data[0].wcr1 ;
    FRAME_BSC(WCR2.LONG)= cs_data[0].wcr2 ;
#endif
    DD_TMR_ICU(IEN,HDEN2) = 0;                    // stop using HDEN2
    DD_TMR_ICU(IEN,DOTCLK) = 1;                   // Signal scheduler to resume RAM access tasks
    DDLCD_Vcount++;                               //  increment v count here to give maximum time while bus idle
  }

  Hcount++;                                       // increment the horizontal line counter
}

/*****************************************************************************
Name: R_DDLCD_SetLineSource()    
Arguments:
  Region: Region of display (horizontal strip). Ranging from 0 (bottom) to DD_FRAME_REGIONS_MAX (user defined MACRO top).
  LineCount: Number of lines for this region.
  pSource: Address of first pixel of first line of region.
  LineStep: Distance (in pixels/uint16_t�s) from first pixel of first line to first pixel of second line (source regions can be wider than the panel).
Returns:  0 = ok, -1 = NG.
Description: setActiveFrame defines which frame buffer to display
*****************************************************************************/
int16_t R_DDLCD_SetLineSource(int16_t Region, int16_t LineCount, void *pSource, int16_t LineStep)
{
  int32_t PixelStart;
  Raster_t *pS = (Raster_t *)pSource;

  if (Region >= DD_FRAME_REGIONS_MAX)
    return(-1);

  /* pSource must be within DD_RASTERS section */
  if ((pS < (Raster_t *)__section_begin(".BDD_RASTERS")) ||
      (pS > (Raster_t *)__section_end(".BDD_RASTERS")))
    return(-1);

  if ((&pS[(int32_t)LineCount*(int32_t)LineStep] < (Raster_t *)__section_begin(".BDD_RASTERS")) ||
      (&pS[(int32_t)LineCount*(int32_t)LineStep] > (Raster_t *)__section_end(".BDD_RASTERS")))
    return(-1);

  /* only allow region to be up to panel size */
  if (LineCount > V_LINES_PANEL)
    return(-1);

  /* set line step and determine pixel start offset */
  #ifndef DD_PANEL_ROTATE
    #ifndef DD_INVERT_H_DOT
      PixelStart = 0;                       // move to start of line
    #else                               
      PixelStart = H_DOT_DISPLAY-1;           // move to end of line
    #endif
    #ifndef DD_INVERT_V_LINES
      LineList[Region].LineNext = LineStep;   // Increment through lines
    #else
      LineList[Region].LineNext = -LineStep;  // Decrement through lines
      PixelStart += ((LineCount-1) * LineStep);  // move to end of frame
    #endif
  #else
    #ifndef DD_INVERT_H_DOT
      PixelStart = 0;                       // move to start of line
      LineList[Region].LineNext = 1;          // Increment through "Columns"
    #else
      PixelStart = H_DOT_DISPLAY-1;           // move to end of line
      LineList[Region].LineNext = -1;         // Decrement through "Columns"
    #endif
    #ifndef DD_INVERT_V_LINES
      PixelStart += ((int32_t)LineCount * ((int32_t)LineStep-1));  // move to end of frame
    #else
    #endif
  #endif

  LineList[Region].pLine = pS + PixelStart;
  LineList[Region].Height = LineCount;

  return(0);
}

/*****************************************************************************
Name: R_DDLCD_GetFrameRate()    
Arguments: none:
Returns:  Current frame Rate in Hz.
Description: Returns current frame rate.
*****************************************************************************/
int16_t R_DDLCD_GetFrameRate(void)
{
  return(FrameRate);
}

/*****************************************************************************
Name: R_DDLCD_SetFrameRate()    
Arguments: rate: Desired Frame Rate in Hz.
Returns:  Positive % MCU access if we can achieve frame rate, Negative if cannot achieve frame rate.
Description: Controls the display frame rate by controlling the vertical blanking time.
*****************************************************************************/
int16_t R_DDLCD_SetFrameRate(int16_t rate)
{
  /* first calculate how many DOTCLK cycles remaining per second at desired rate */
  int32_t VLinesExtend = DD_DOT_CLOCK_FREQUENCY_BLANK-(H_DOT_PERIOD_PANEL* (int32_t)rate);
  /* now divide remaining DOTCLK into "rate" number of lines */
  VLinesExtend /= (H_DOT_PERIOD_BLANK* (int32_t)rate);
  /* if postive, set the value for use */
  if ((VLinesExtend > 0) && (rate > 0))
  {
    int32_t HDotsBlank;
    VLinesPeriod = V_LINES_PANEL+(int16_t)VLinesExtend;
    HDotsBlank = (VLinesPeriod - V_LINES_DISPLAY) * (H_DOT_PERIOD_BLANK);
    FrameRate = rate;
    return ((int16_t)((HDotsBlank * 100L)/(HDotsBlank + H_DOTS_DATA)));
  }
  else
  {
    return((int16_t)VLinesExtend);
  }
}

/*****************************************************************************
Name: R_DDLCD_Off()    
Arguments: none                     
Returns:  nothing     
Description: Shuts down the LCD - powers down LCD module
*****************************************************************************/
void R_DDLCD_Off(void)
{
  #ifdef DD_PANEL_POWER_PIN
    PINo(PANEL_POWER) = 0;            // turn off power to LCD module
  #endif
}

/*****************************************************************************
Name: R_DDLCD_Backlight()    
Arguments: int state                     
Returns:  nothing     
Description: nonzero value turns backlight on...
*****************************************************************************/
void R_DDLCD_Backlight(int state)
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
