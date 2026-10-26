/***********************************************************************************************************************
* DISCLAIMER
* This software is supplied by Renesas Electronics Corporation and is only intended for use with Renesas products. No
* other uses are authorized. This software is owned by Renesas Electronics Corporation and is protected under all
* applicable laws, including copyright laws.
* THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES REGARDING
* THIS SOFTWARE, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING BUT NOT LIMITED TO WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT. ALL SUCH WARRANTIES ARE EXPRESSLY DISCLAIMED. TO THE MAXIMUM
* EXTENT PERMITTED NOT PROHIBITED BY LAW, NEITHER RENESAS ELECTRONICS CORPORATION NOR ANY OF ITS AFFILIATED COMPANIES
* SHALL BE LIABLE FOR ANY DIRECT, INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES FOR ANY REASON RELATED TO THIS
* SOFTWARE, EVEN IF RENESAS OR ITS AFFILIATES HAVE BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
* Renesas reserves the right, without notice, to make changes to this software and to discontinue the availability of
* this software. By using this software, you agree to the additional terms and conditions found by accessing the
* following link:
* http://www.renesas.com/disclaimer
*
* Copyright (C) 2012 Renesas Electronics Corporation. All rights reserved.
***********************************************************************************************************************/

/***********************************************************************************************************************
Includes   <System Includes> , "Project Includes"
***********************************************************************************************************************/
#include    "platform.h"

#include    <stdint.h>

void clock_source_select (void);

/***********************************************************************************************************************
* Function name: operating_frequency_set
* Description  : Configures the clock settings for each of the device clocks
* Arguments    : none
* Return value : none
***********************************************************************************************************************/
void operating_frequency_set (void)
{
    /* Used for constructing value to write to SCKCR and CKOCR registers. */
    uint32_t temp_clock = 0;

    /*
    Default settings:
    Clock Description              Frequency
    ----------------------------------------
    Input Clock Frequency............  16 MHz
    PLL frequency (x3)...............  48 MHz
    Internal Clock Frequency.........  24 MHz
    Peripheral Clock Frequency.......  24 MHz
    Clock Out Frequency..............  1  MHz */

    /* Protect off. */
    SYSTEM.PRCR.WORD = 0xA50B;

    /* Select the clock based upon user's choice. */
    clock_source_select();

    /* Figure out setting for FCK bits. */
#if   BSP_CFG_FCK_DIV == 1
    /* Do nothing since FCK bits should be 0. */
#elif BSP_CFG_FCK_DIV == 2
    temp_clock |= 0x10000000;
#elif BSP_CFG_FCK_DIV == 4
    temp_clock |= 0x20000000;
#elif BSP_CFG_FCK_DIV == 8
    temp_clock |= 0x30000000;
#elif BSP_CFG_FCK_DIV == 16
    temp_clock |= 0x40000000;
#elif BSP_CFG_FCK_DIV == 32
    temp_clock |= 0x50000000;
#elif BSP_CFG_FCK_DIV == 64
    temp_clock |= 0x60000000;
#else
    #error "Error! Invalid setting for BSP_CFG_FCK_DIV in r_bsp_config.h"
#endif

    /* Figure out setting for ICK bits. */
#if   BSP_CFG_ICK_DIV == 1
    /* Do nothing since ICK bits should be 0. */
#elif BSP_CFG_ICK_DIV == 2
    temp_clock |= 0x01000000;
#elif BSP_CFG_ICK_DIV == 4
    temp_clock |= 0x02000000;
#elif BSP_CFG_ICK_DIV == 8
    temp_clock |= 0x03000000;
#elif BSP_CFG_ICK_DIV == 16
    temp_clock |= 0x04000000;
#elif BSP_CFG_ICK_DIV == 32
    temp_clock |= 0x05000000;
#elif BSP_CFG_ICK_DIV == 64
    temp_clock |= 0x06000000;
#else
    #error "Error! Invalid setting for BSP_CFG_ICK_DIV in r_bsp_config.h"
#endif

    /* Figure out setting for PCKB bits. */
#if   BSP_CFG_PCKB_DIV == 1
    /* Do nothing since PCKB bits should be 0. */
#elif BSP_CFG_PCKB_DIV == 2
    temp_clock |= 0x00000100;
#elif BSP_CFG_PCKB_DIV == 4
    temp_clock |= 0x00000200;
#elif BSP_CFG_PCKB_DIV == 8
    temp_clock |= 0x00000300;
#elif BSP_CFG_PCKB_DIV == 16
    temp_clock |= 0x00000400;
#elif BSP_CFG_PCKB_DIV == 32
    temp_clock |= 0x00000500;
#elif BSP_CFG_PCKB_DIV == 64
    temp_clock |= 0x00000600;
#else
    #error "Error! Invalid setting for BSP_CFG_PCKB_DIV in r_bsp_config.h"
#endif

    /* Figure out setting for PCKD bits. */
#if   BSP_CFG_PCKD_DIV == 1
    /* Do nothing since PCKD bits should be 0. */
#elif BSP_CFG_PCKD_DIV == 2
    temp_clock |= 0x00000001;
#elif BSP_CFG_PCKD_DIV == 4
    temp_clock |= 0x00000002;
#elif BSP_CFG_PCKD_DIV == 8
    temp_clock |= 0x00000003;
#elif BSP_CFG_PCKD_DIV == 16
    temp_clock |= 0x00000004;
#elif BSP_CFG_PCKD_DIV == 32
    temp_clock |= 0x00000005;
#elif BSP_CFG_PCKD_DIV == 64
    temp_clock |= 0x00000006;
#else
    #error "Error! Invalid setting for BSP_CFG_PCKD_DIV in r_bsp_config.h"
#endif

    /* Set SCKCR register. */
    SYSTEM.SCKCR.LONG = temp_clock;

    /* Choose clock source. Default for r_bsp_config.h is PLL. */
    SYSTEM.SCKCR3.WORD = ((uint16_t)BSP_CFG_CLOCK_SOURCE) << 8;

    /* Protect on. */
    SYSTEM.PRCR.WORD = 0xA500;
}

/***********************************************************************************************************************
* Function name: clock_source_select
* Description  : Enables and disables clocks as chosen by the user. This function also implements the software delays
*                needed for the clocks to stabilize.
* Arguments    : none
* Return value : none
***********************************************************************************************************************/
void clock_source_select (void)
{
    /* Declared volatile for software delay purposes. */
    volatile uint32_t i;

    /* NOTE: AS OF VERSION 0.50 OF THE RX111 HARDWARE MANUAL, ALL OF THE CLOCK STABILIZATION TIMES ARE TBD. FOR NOW,
       WHERE EVER A WAIT COUNT REGISTER IS AVAILABLE, THE DELAY IS SET TO THE MAX NUMBER OF CYCLES. IF A DELAY TIME
       IS NOT KNOWN, THEN THE DELAY OF THE RX63N IS USED FOR NOW. */

    /* The delay loops used below have been measured to take 11 cycles per iteration. This has been verified using the
       Renesas RX Toolchain with optimizations set to 2, size priority. The same result was obtained using 2, speed
       priority. At this time the MCU is still running on the 4MHz LOCO so it is approximately 2.75us per iteration. */

#if (BSP_CFG_CLOCK_SOURCE == 1)
    /* HOCO is chosen. Start it operating. */
    SYSTEM.HOCOCR.BYTE = 0x00;

    /* NOTE: This will be updated for correct time when value is known. See comment at top of this function for info.
             Currently using info from RX63N where needed.
       The delay period needed is to make sure that the HOCO has stabilized. According to Rev.X.XX of the RX111's HW
       manual the delay period is tHOCOWT2 which is Xms. A delay of 2.4ms has been used below to account for variations
       in the LOCO.
       2.4ms / 2.75us (per iteration) = 873 iterations */
    for(i = 0; i < 873; i++)
    {
        /* Wait 2.4ms. See comment above for why. */
        nop() ;
    }
#else
    /* HOCO is not chosen. Stop the HOCO. */
    SYSTEM.HOCOCR.BYTE = 0x01;
#endif

#if (BSP_CFG_CLOCK_SOURCE == 2)
    /* Main clock oscillator is chosen. Start it operating. */

    /* If the main oscillator is >10MHz then the main clock oscillator forced oscillation control register (MOFCR) must
       be changed. */
    if (BSP_CFG_XTAL_HZ > 10000000)
    {
        /* 10 - 20MHz. */
        SYSTEM.MOFCR.BIT.MODRV21 = 1;
    }
    else
    {
        /* 1 - 10MHz. */
        SYSTEM.MOFCR.BIT.MODRV21 = 0;
    }

    /* Wait 65,536 cycles * 16 MHz = 4.096 ms */
    SYSTEM.MOSCWTCR.BYTE = 0x07;

    /* Set the main clock to operating. */
    SYSTEM.MOSCCR.BYTE = 0x00;

    /* NOTE: This will be updated for correct time when value is known. See comment at top of this function for info.
             Currently using info from RX63N where needed.
       The delay period needed is to make sure that the main clock has stabilized. This delay period is tMAINOSCWT in
       the HW manual and according to the Renesas Technical Update document TN-RX*-A021A/E this is defined as:
       n = Wait time selected by MOSCWTCR.MSTS[] bits
       tMAINOSC = Main clock oscillator start-up time. From referring to various vendors, a start-up time of 4ms appears
                  to be a common maximum. To be safe we will use 5ms.
       tMAINOSCWT = tMAINOSC + ((n+16384)/fMAIN)
       tMAINOSCWT = 5ms + ((65536 + 16384)/16MHz)
       tMAINOSCWT = 10.12ms
       A delay of 11ms has been used below to account for variations in the LOCO.
       11ms / 2.75us (per iteration) = 4000 iterations */
    for(i = 0; i < 4000; i++)
    {
        /* Wait 11ms. See comment above for why. */
        nop() ;
    }
#else
    /* Set the main clock to stopped. */
    SYSTEM.MOSCCR.BYTE = 0x01;
#endif

#if (BSP_CFG_CLOCK_SOURCE == 3)
    /* Sub-clock oscillator is chosen. Start it operating. */
    /* In section 9.8.4, there is a reference to a SOSCWTCR register, but there is no
     * description for this register in the manual nor reference for it in iodefine.h. */

    /* Set the sub-clock to operating. */
    SYSTEM.SOSCCR.BYTE = 0x00;

    /* NOTE: This will be updated for correct time when value is known. See comment at top of this function for info.
             Currently using info from RX63N where needed.
       The delay period needed is to make sure that the sub-clock has stabilized. According to Rev.X.XX of the RX111's
       HW manual the delay period is tSUBOSCWT0 which is at minimum X.Xs and at maximum X.X seconds. We will use the
       maximum value to be safe.
       2.6s / 2.75us (per iteration) = 945455 iterations */
    for(i = 0; i< 945455; i++)
    {
        /* Wait 2.6s. See comment above for why. */
        nop() ;
    }
#else
    /* Set the sub-clock to stopped. */
    SYSTEM.SOSCCR.BYTE = 0x01;
#endif

#if (BSP_CFG_CLOCK_SOURCE == 4)
    /* PLL is chosen. Start it operating. Must start main clock as well since PLL uses it. */

    /* If the main oscillator is >10MHz then the main clock oscillator forced oscillation control register (MOFCR) must
       be changed. */
    if (BSP_CFG_XTAL_HZ > 10000000)
    {
        /* 10 - 20MHz. */
        SYSTEM.MOFCR.BIT.MODRV21 = 1;
    }
    else
    {
        /* 1 - 10MHz. */
        SYSTEM.MOFCR.BIT.MODRV21 = 0;
    }

    /* Wait 65,536 cycles * 16 MHz = 4.096 ms */
    SYSTEM.MOSCWTCR.BYTE = 0x07;

    /* Set the main clock to operating. */
    SYSTEM.MOSCCR.BYTE = 0x00;

    /* Set PLL Input Divisor. */
    SYSTEM.PLLCR.BIT.PLIDIV = BSP_CFG_PLL_DIV >> 1;

    /* Set PLL Multiplier. */
    SYSTEM.PLLCR.BIT.STC = (BSP_CFG_PLL_MUL * 2) - 1;

    /* Set the PLL to operating. */
    SYSTEM.PLLCR2.BYTE = 0x00;

    /* NOTE: This will be updated for correct time when value is known. See comment at top of this function for info.
             Currently using info from RX63N where needed.
       Using 12ms for now. Need to update when more info on delays is available.
       12ms / 2.75us (per iteration) = 437 iterations */
    for(i = 0; i < 437; i++)
    {
        /* Wait 12ms. See comment above for why. */
        nop() ;
    }
#else
    /* Set the PLL to stopped. */
    SYSTEM.PLLCR2.BYTE = 0x01;
#endif

    /* LOCO is saved for last since it is what is running by default out of reset. This means you do not want to turn
       it off until another clock has been enabled and is ready to use. */
#if (BSP_CFG_CLOCK_SOURCE == 0)
    /* LOCO is chosen. This is the default out of reset. */
    SYSTEM.LOCOCR.BYTE = 0x00;
#else
    /* LOCO is not chosen and another clock has already been setup. Turn off the LOCO. */
    SYSTEM.LOCOCR.BYTE = 0x01;
#endif

    /* Make sure a valid clock was chosen. */
#if (BSP_CFG_CLOCK_SOURCE > 4) || (BSP_CFG_CLOCK_SOURCE < 0)
    #error "ERROR - Valid clock source must be chosen in r_bsp_config.h using BSP_CFG_CLOCK_SOURCE macro."
#endif
}

