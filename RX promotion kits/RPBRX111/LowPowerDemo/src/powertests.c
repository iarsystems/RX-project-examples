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
*******************************************************************************
* Copyright (C) 2013 Renesas Electronics Corporation. All rights reserved.   */
/*******************************************************************************
* File Name     : PowerTests.c
* Version       : 1.00
* Device        : R5F51115AxFM
* Tool Chain    : CCRX
* H/W Platform  : RPBRX111
* Description   : Low power test routines
*******************************************************************************/
/*******************************************************************************
* History       : 12/08/2013  Ver. 1.00 First Release
*******************************************************************************/

/*******************************************************************************
User Includes (Project Level Includes)
*******************************************************************************/
/* Include machine.h to use the wait() and nop() functions */
#include    <machine.h>

/* Standard integer types with specified widths definitions */
#include    <stdint.h>

/* Register address definitions */
#include <iorx111.h>

/* Hardware platform definitions */
#include    "rsk_defines.h"

/* RAM based CPU exercise function prototypes */
#include    "powertests.h"

/*******************************************************************************
* Global Variables
*******************************************************************************/
/* For use in demonstrating CPU load and operation of DMA transfer */
volatile uint32_t g_product = 0x1234;

/*******************************************************************************
* Function Name : perform_load_test
* Description   : Function to allow user to measure power usage
*                 based on cpu_calc.
*                 Runs in high power mode until SW1 pressed then enters
*                 selected low power mode.
*                 Press RES1 to exit and restart.
* Argument      : uint8_t cpu_calc - Selected CPU Load
* Return value  : Never returns
*******************************************************************************/
static void perform_load_test (const uint8_t cpu_calc)
{
    /* Use cpu_calc to determine whether to perform MUL and DMA
     operations - only do this if set to CPU_MAX */
    switch (cpu_calc)
    {
        /* Treat CPU_MIN and CPU_TYP the same in this code but note there
         is still a difference in which peripherals are enabled */
        case CPU_MIN:
        case CPU_TYP:
        {
            /* Align while loop to 16 byte boundary to minimise prefetch */
            nop();
            nop();
            nop();
            nop();
            nop();
            nop();
            nop();
            nop();
            nop();
            nop();
            nop();
            nop();

            /* Loop forever (until reset pressed) */
            while (1)
            {

                /* Execute nops in this loop until SW1 is pressed.
                 Use many nops to minimise the frequency of looping as this
                 results in a prefetch every time there is a branch.
                 Do not use a function call here. */
                nop();
                nop();
                nop();
                nop();
                nop();
                nop();
                nop();
                nop();
                nop();
                nop();
                nop();
                nop();
                nop();
                nop();
                nop();
                nop();
                nop();
                nop();
                nop();
                nop();
                nop();
                nop();
                nop();
                nop();
                nop();
                nop();
                nop();
                nop();
                nop();
                nop();
                nop();
                nop();
                nop();

                /* Check for SW1 being pressed */
                if (PRESSED == SW1)
                {
                    /* Disable LED2 */
                    LED2_OFF;

                    /* Enter sleep mode */
                    wait();
                }
            }
            /* No break as statement would be unreachable */
        }
        case CPU_MAX:
        {
            /* Align while loop to 16 byte boundary to minimise prefetch */
            nop();
            nop();
            nop();
            nop();
            nop();
            nop();
            nop();
            nop();
            nop();
            nop();

            /* Loop forever (until reset pressed) */
            while (1)
            {
                /* Execute nops in this loop until SW1 is pressed.
                 Use many nops to minimise the frequency of looping as this
                 results in a prefetch every time there is a branch.
                 Do not use a function call here. */
                nop();
                nop();
                nop();
                nop();
                nop();
                nop();
                nop();
                nop();
                nop();
                nop();
                nop();
                nop();
                nop();
                nop();
                nop();
                nop();
                nop();
                nop();
                nop();
                nop();
                nop();
                nop();
                nop();
                nop();
                nop();
                nop();
                nop();
                nop();
                nop();
                nop();

                /* Do an operation */
                g_product = (uint32_t) ((g_product * MTU3.TCNT) + CMT0.CMCNT);

                /* Check for SW1 being pressed */
                if (PRESSED == SW1)
                {
                    /* Disable LED2 */
                    LED2_OFF;

                    /* Disable the interrupt that is triggering the DMAC,
                     otherwise the CPU will be woken from sleep mode
                     on every interrupt. */
                    IEN(MTU0,TGIA0) = 0;
                    MTU0.TIER.BIT.TGIEA = 0;

                    while (0x0 != MTU0.TIER.BIT.TGIEA)
                    {
                        /* Wait for write to complete */
                    }

                    /* Enter sleep mode */
                    wait();
                }
            }
            /* No break as statement would be unreachable */
        }

        /* Default case should never be used */
        default:
        {
            break;
        }
    }
}
/******************************************************************************
* End of function perform_load_test
******************************************************************************/

/*******************************************************************************
* Function Name : perform_power_tests
* Description   : Function to allow configure power consumption tests
*                 based on clock_mode and cpu_calc.
*                 Runs in high power mode until SW1 pressed then enters
*                 selected sleep mode or calls perform_load_test().
*                 Press RES1 to exit and restart.
* Argument      : uint8_t clock_mode - Selected Clock Mode
*               : uint8_t cpu_calc - Selected CPU Load
* Return value  : Never returns
*******************************************************************************/
void perform_power_tests (const uint8_t clock_mode, const uint8_t cpu_calc)
{
    /* For switch debounce delay loop */
    volatile uint32_t debounce;

    /* Light LED to show MCU is awake */
    LED2_ON;

    /* Align while loop to 16 byte boundary to minimise prefetch */
    nop();
    nop();

    /* Handle differently if set to a sleep, or software standby mode */
    if ((SW_SLEEP == clock_mode) || (SW_STBY == clock_mode) ||
        (DP_SLEEP == clock_mode))
    {
        /* Loop forever (until reset pressed) */
        while (1)
        {
            /* Check for SW1 being pressed */
            if (PRESSED == SW1)
            {
                /* SW1 pressed, now debounce the switch press */
                for (debounce = 0; debounce < 5000; debounce++)
                {
                    nop();
                }

                /* Disable LED2 */
                LED2_OFF;

                /* Shut down IDLE_VCC rail */
                IDLE_EN = POWER_OFF;
                IDLE_EN_PDR = 0x0;

                /* Ensure all peripherals are in module stop mode */
                SYSTEM.MSTPCRA.LONG = 0xffffffff;
                SYSTEM.MSTPCRB.LONG = 0xffffffff;

                /* ...but do not turn off the RAM */
                SYSTEM.MSTPCRC.LONG |= 0x7ffffffe;

                /* Use main clock on return from sleep */
                SYSTEM.RSTCKCR.BIT.RSTCKSEL = 0x1;
                SYSTEM.RSTCKCR.BIT.RSTCKEN = 0x0u;

                /* Wait for write to complete */
                while (0x0 != SYSTEM.RSTCKCR.BIT.RSTCKEN)
                {

                }

                /* Enter low power mode */
                wait();
            }

        }
    }

    /* Or if not in SW_SLEEP, SW_STBY or DP_SLEEP mode */
    else
    {
        /* Run load tests */
        perform_load_test(cpu_calc);
    }
}
/******************************************************************************
* End of function perform_power_tests
******************************************************************************/

/* End of section POWERTESTS */





