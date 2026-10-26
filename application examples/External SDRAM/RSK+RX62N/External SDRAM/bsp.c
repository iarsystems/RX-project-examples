/**
 * \file  bsp.c
 * \brief Implementation file for the vibcom board support package
 *
 * This is file implementes all of the board specific configurations
 * for the vibcom board
 *
 * Change Log
 * This log is for internal use only!
 * ------------------------------------------------------------
 *        By: APS
 *      Date: 2011-06-16
 *    Change: Created the file
 * ------------------------------------------------------------
 *        By:
 *      Date:
 *    Change:
 * ------------------------------------------------------------
 *
 * \internal Author, version, and date are provided by SVN.  Do not edit \endinternal
 * $Author$
 *          
 * $Date$
 *
 * (C)2011 INOVA Geophysical Equipment
 * All Rights Reserved
 *
 */

/******************************************************************************
 * Project Includes
 ******************************************************************************/
//! Contains delcarations for the functions defined in this file
#include "bsp.h"


void BSP_Board_init( void )
{
  //Configure operating mode
  SYSTEM.SYSCR0.WORD = ( ROME_ONCHIP_ROM_ENABLED |
                         EXBE_EXTBUS_ENABLED |
                         SYSCRO_KEY_CODE );
  BSP_Cfg_Clocks();
  BSP_Cfg_Port_Function();
  BSP_Cfg_SDRAM();
}


void BSP_Cfg_Clocks( void )
{
  SYSTEM.SCKCR.LONG = ( PCK_EXTAL_4 |  //Peripheral module clk = 48Mhz
                        BCK_EXTAL_4 |  //External bus clock    = 48Mhz
                        PSTOP0_SDCLK_OUTPUT_ENABLE  |
                        PSTOP1_BCLK_OUTPUT_ENABLE |
                        ICK_EXTAL_8 ); //System clk            = 96Mhz

  //SYSTEM.OSTDCR.WORD = ( MAIN_OSC_DETECT_STOP_DISABLED | OSTDCR_KEY_CODE );
  SYSTEM.BCKCR.BYTE = BCLKDIV_1; //Not necessary but will do anyhow
  //SYSTEM.SUBOSCCR.BYTE = SUBSTOP_SUBCLOCK_STOPPED;
}

void BSP_Cfg_SDRAM( void )
{
  BSC.SDIR.WORD = 0x00F0;         /* SDRAM initialization...AutoRefresh interval=3 cycles, Precharge cycles count=3cycles, 15 times */
  BSC.SDICR.BIT.INIRQ = 1;        /* start SDRAM initialization */
  while(BSC.SDSR.BIT.INIST == 1)  /* wait for initialization complete */
    ;
  BSC.SDCMOD.BIT.EMODE = 0;       /* set endian to little */

  BSC.SDCCR.BIT.BSIZE = 0;       /* set SDRAM to 16bit access */

  BSC.SDMOD.WORD = 0x0220;        /* Set write single location, CL=2 */
  BSC.SDRFCR.BIT.REFW = 5-1;      /* set refresh cycle count to 5 */
  BSC.SDRFCR.BIT.RFC = 720-1;     /* set refresh period to 720 clock cycles (15uS at 48Mhz) */
  BSC.SDTR.LONG = 0x000002;       /* set RAS=1, RCD=1, RP=1, WR=1, CL=2, cycles */
  BSC.SDADR.BIT.MXC = 1;          /* set shift to 9 bits */
  BSC.SDRFEN.BIT.RFEN = 1;        /* enable auto refresh */
  BSC.SDCCR.BIT.EXENB = 1;        /* enable SDRAM operation...this should be last operation */
}

void BSP_Cfg_Port_Function( void )
{
  //Configure the External bus address lines3
  // Don't need address lines A23:16
  // PF3BUS
  
  //Enable address lines A15:0
  IOPORT.PF4BUS.BYTE = ( ADRLE_ADDRESS_A9_TO_A0_ENABLED |
                         A10E_ADDRESS_A10_ENABLED |
                         A11E_ADDRESS_A11_ENABLED |
                         A12E_ADDRESS_A12_ENABLED |
                         A13E_ADDRESS_A13_ENABLED |
                         A14E_ADDRESS_A14_ENABLED |
                         A15E_ADDRESS_A15_ENABLED );
  
  // Use A19:16 'A' pin configuration, disable D15:8, Don't use WR1#/BC1# (Default)
  IOPORT.PF5BUS.BYTE = ( ADRHMS_A19_TO_A16_A |
                         DHE_D15_TO_D8_ENABLE |
                         WR1BC1E_P51_IO );

  //Disable SDRAM and SDRAM clock output (Default)
  IOPORT.PF6BUS.BYTE = ( WAITS_P55_WAIT_B |
                         MDSDE_SDRAM_ENABLE |
                         DQM1E_SDRAM_ENABLE_ALL |
                         SDCLKE_SDCLK_OUT_ENABLED );
  
  { 
    int delay = 100*100; 
    while (--delay > 0); 
  } /* delay at least 100uS after SDCLK active */
}


/**
 * This function is used to retreive the peripheral clock frequency from the processor
 * \arg none
 * \return Peripheral clock frequency in cycles
 */
uint32_t  BSP_CPU_PerClkFreq (void)
{
    uint32_t  sys_clk;
    uint32_t  per_clk;
    uint32_t  per_div;

    sys_clk =  SYSTEM.SCKCR.LONG;
    per_div = (sys_clk >> 8u) & 0xFu;
    if (per_div > 3u) {
        return (0u);
    }

    per_clk =  BSP_CPU_EXT_CLK_FREQ << (3u - per_div);
    return (per_clk);
}