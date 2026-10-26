/*
 * low_level_init.c
 *
 * This module contains the function '__low_level_init', a function
 * that is called before the 'main' function of the program.  Normally
 * low-level initializations - such as setting the prefered interrupt
 * level or setting the watchdog - can be performed here.
 *
 * Note that this function is called before the data segments are
 * initialized, this means that this function can't rely on the
 * values of global or static variables.
 *
 * When this function returns zero, the startup code will inhibit the
 * initialization of the data segments.  The result is faster startup,
 * the drawback is that neither global nor static data will be
 * initialized.
 *
 *
 */
/* This macro is here so that the stack will be declared here. This is used to prevent multiplication of stack size. */
#define     BSP_DECLARE_STACK
#undef      BSP_DECLARE_STACK
/* Define the target platform */
#include    "platform.h"

/***********************************************************************************************************************
Macro definitions
***********************************************************************************************************************/
/* If the user chooses only 1 stack then the 'U' bit will not be set and the CPU will always use the interrupt stack. */
#if (BSP_CFG_USER_STACK_ENABLE == 1)
    #define PSW_init  (0x00030000)
#else
    #define PSW_init  (0x00010000)
#endif

#if BSP_CFG_RUN_IN_USER_MODE==1
    #if __RENESAS_VERSION__ < 0x01010000
        /* Declare the contents of the function 'Change_PSW_PM_to_UserMode' as assembler to the compiler */
        #pragma inline_asm Change_PSW_PM_to_UserMode

        /* MCU user mode switcher function declaration */
        static void Change_PSW_PM_to_UserMode(void);
    #endif
#endif

#if (BSP_CFG_USE_CGC_MODULE == 0)
static void operating_frequency_set(void);
static void clock_source_select(void);
static void usb_lcd_clock_source_select(void);
#else
static void cgc_clock_config(void);
#endif
 
#pragma language=extended
__intrinsic
int __low_level_init ( void )
{
     __disable_interrupt();
#if (BSP_CFG_USE_CGC_MODULE == 0)
    /* Switch to high-speed operation */
    operating_frequency_set();
    usb_lcd_clock_source_select();
#else
    cgc_clock_config();
#endif

    /* Initialize MCU interrupt callbacks. */
    bsp_interrupt_open();

    /* Initialize register protection functionality. */
    bsp_register_protect_open();

    /* Configure the MCU and board hardware */
    hardware_setup();

    /* Change the MCU's user mode from supervisor to user */
    nop();
    //set_psw(PSW_init);
#if BSP_CFG_RUN_IN_USER_MODE==1
    /* Use chg_pmusr() intrinsic if possible. */
    #if __RENESAS_VERSION__ >= 0x01010000
    chg_pmusr() ;
    #else
    Change_PSW_PM_to_UserMode();
    #endif
#endif

 /* Enable the bus error interrupt to catch accesses to illegal/reserved areas of memory */
    R_BSP_InterruptControl(BSP_INT_SRC_BUS_ERROR, BSP_INT_CMD_INTERRUPT_ENABLE, FIT_NO_PTR);
    return (1);
}
#pragma language=default


#if (BSP_CFG_USE_CGC_MODULE == 0)

/***********************************************************************************************************************
* Function name: operating_frequency_set
* Description  : Configures the clock settings for each of the device clocks
* Arguments    : none
* Return value : none
***********************************************************************************************************************/
static void operating_frequency_set (void)
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
    SYSTEM.PRCR.WORD = 0xA50F;
    
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

#if ((BSP_CFG_CLOCK_SOURCE != 0) && (BSP_CFG_LCD_CLOCK_SOURCE != 0))
    /* We can now turn LOCO off since it is not going to be used. */
    SYSTEM.LOCOCR.BYTE = 0x01;
#endif

    /* Protect on. */
    SYSTEM.PRCR.WORD = 0xA500;          
}

/***********************************************************************************************************************
* Function name: clock_source_select
* Description  : Enables and disables clocks as chosen by the user. If a clock other than LOCO or HOCO is already
*                running when this function is called then that usually means a bootloader was run beforehand and set
*                up the clocks already.
* Arguments    : none
* Return value : none
***********************************************************************************************************************/
static void clock_source_select (void)
{

#if ((BSP_CFG_CLOCK_SOURCE == 1) || (BSP_CFG_LCD_CLOCK_SOURCE == 1))
    /* HOCO is chosen. Start it operating if it is not already operating. */
    if (1 == SYSTEM.HOCOCR.BIT.HCSTP)
    {
        /* Set HOCO wait control register for 266 cycles. */
        SYSTEM.HOCOWTCR.BYTE = 0x06;

        /* Start HOCO operating. */
        SYSTEM.HOCOCR.BYTE = 0x00;
    }

    while (0 == SYSTEM.OSCOVFSR.BIT.HCOVF)
    {
        /* Make sure clock has stabilized. */
    }
#else
    /* HOCO is not chosen. Stop the HOCO. */
    SYSTEM.HOCOCR.BYTE = 0x01;
#endif

#if ((BSP_CFG_CLOCK_SOURCE == 2) || (BSP_CFG_LCD_CLOCK_SOURCE == 2) || (BSP_CFG_USB_CLOCK_SOURCE == 1))
    /* Main clock oscillator is chosen. Start it operating if it is not already operating. If it was already operating*/
    if (1 == SYSTEM.MOSCCR.BIT.MOSTP)
    {
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

        /* Wait 32768 cycles (8.192ms). Adjust if needed. */
        SYSTEM.MOSCWTCR.BYTE = 0x06;

        /* Set the main clock to operating. */
        SYSTEM.MOSCCR.BYTE = 0x00;
    }

    while (0 == SYSTEM.OSCOVFSR.BIT.MOOVF)
    {
        /* Make sure clock has stabilized. */
    }
#else
    /* Set the main clock to stopped. */
    SYSTEM.MOSCCR.BYTE = 0x01;          
#endif

#if ((BSP_CFG_CLOCK_SOURCE == 3) || (BSP_CFG_LCD_CLOCK_SOURCE == 3))
    /* Sub-clock oscillator is chosen. Start it operating. */

    /* Set the sub-clock to operating. */
    SYSTEM.SOSCCR.BYTE = 0x00;

    /* The delay loops used below have been measured to take 11 cycles per iteration. This has been verified using the
       Renesas RX Toolchain with optimizations set to 2, size priority. The same result was obtained using 2, speed
       priority. At this time the MCU is still running on the 4MHz LOCO so it is approximately 2.75us per iteration. */

    /* There is not an oscillation stabilization flag for the sub-clock so a software delay is used.
       The delay period needed is to make sure that the sub-clock has stabilized. According to Rev.1.00 of the RX111's
       HW manual the delay period is tSUBOSC which is typically 0.5 seconds. */
     R_BSP_SoftwareDelay(500, BSP_DELAY_MILLISECS);
#else
    /* Set the sub-clock to stopped. */
    SYSTEM.SOSCCR.BYTE = 0x01;
#endif

#if (BSP_CFG_CLOCK_SOURCE == 4)
    /* PLL is chosen. Start it operating if it is not already. Must start main clock as well since PLL uses it. */

    if (1 == SYSTEM.PLLCR2.BIT.PLLEN)
    {
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

        /* Wait 32768 cycles (8.192ms). Adjust if needed. */
        SYSTEM.MOSCWTCR.BYTE = 0x06;

        /* Set the main clock to operating. */
        SYSTEM.MOSCCR.BYTE = 0x00;

        /* Set PLL Input Divisor. */
        SYSTEM.PLLCR.BIT.PLIDIV = BSP_CFG_PLL_DIV >> 1;

        /* Set PLL Multiplier. */
        SYSTEM.PLLCR.BIT.STC = (BSP_CFG_PLL_MUL * 2) - 1;

        /* Set the PLL to operating. */
        SYSTEM.PLLCR2.BYTE = 0x00;
    }

    while (0 == SYSTEM.OSCOVFSR.BIT.PLOVF)
    {
        /* Make sure clock has stabilized. */
    }
#else
    /* Set the PLL to stopped. */
    SYSTEM.PLLCR2.BYTE = 0x01;          
#endif

    /* LOCO is saved for last since it is what is running by default out of reset. This means you do not want to turn
       it off until another clock has been enabled and is ready to use. */
#if ((BSP_CFG_CLOCK_SOURCE == 0) || (BSP_CFG_LCD_CLOCK_SOURCE == 0))
    /* LOCO is chosen. This is the default out of reset. */
    SYSTEM.LOCOCR.BYTE = 0x00;
#else
    /* LOCO is not chosen but it cannot be turned off yet since it is still being used. */
#endif

    /* Make sure a valid clock was chosen. */
#if (BSP_CFG_CLOCK_SOURCE > 4) || (BSP_CFG_CLOCK_SOURCE < 0)
    #error "ERROR - Valid clock source must be chosen in r_bsp_config.h using BSP_CFG_CLOCK_SOURCE macro."
#endif
}


/***********************************************************************************************************************
* Function name: usb_lcd_clock_source_select
* Description  : Enables clock sources for the usb and lcd (if not already done) as chosen by the user.
*                This function also implements the software delays needed for the clocks to stabilize.
* Arguments    : none
* Return value : none
***********************************************************************************************************************/
static void usb_lcd_clock_source_select (void)
{
    /* Declared volatile for software delay purposes. */
    volatile uint32_t i;


    /* Protect off. DO NOT USE R_BSP_RegisterProtectDisable()! (not initialized yet) */
    SYSTEM.PRCR.WORD = 0xA50F;


    /* INITIALIZE AND SELECT UCLK SOURCE */

#if (BSP_CFG_USB_CLOCK_SOURCE == 1)
    /* USB PLL is chosen. Start it operating. */
    /* main clock oscillator already initialized in clock_source_select() or cgc_clock_config() */

    /* Set PLL Input Divisor. */
    SYSTEM.UPLLCR.BIT.UPLIDIV = BSP_CFG_UPLL_DIV >> 1;

    /* Set PLL Multiplier. */
    SYSTEM.UPLLCR.BIT.USTC = (BSP_CFG_UPLL_MUL * 2) - 1;

    /* Set USB PLL as selected UCLK */
    SYSTEM.UPLLCR.BIT.UCKUPLLSEL = 1;

    /* Set the PLL to operating. */
    SYSTEM.UPLLCR2.BYTE = 0x00;

    while (0 == SYSTEM.OSCOVFSR.BIT.UPLOVF)
    {
        /* Make sure clock has stabilized. */
    }
#elif (BSP_CFG_USB_CLOCK_SOURCE == 0)
    /* Set system clock as selected UCLK */
    SYSTEM.UPLLCR.BIT.UCKUPLLSEL = 0;           // default from reset
    /* Set the USB PLL to stopped. */
    SYSTEM.UPLLCR2.BYTE = 0x01;                 // default from reset
#else
    #error "ERROR - Valid USB clock source must be chosen in r_bsp_config.h using BSP_CFG_USB_CLOCK_SOURCE macro."
#endif


    /* INITIALIZE AND SELECT LCD CLOCK SOURCE */

    /* clock sources 0-3 initialized in clock_source_select() */

#if (BSP_CFG_LCD_CLOCK_SOURCE == 4)
    /* IWDTCLK is chosen. Start it operating. */
    SYSTEM.ILOCOCR.BYTE = 0x00;
    // User application must ensure that 50us (stabilization time) has elapsed before using LCD
#else
    /* Set the IWDTCLK to stopped */
    SYSTEM.ILOCOCR.BYTE = 0x01;
#endif

    /* Make sure a valid clock was chosen. */
#if (BSP_CFG_LCD_CLOCK_SOURCE > 4) || (BSP_CFG_LCD_CLOCK_SOURCE < 0)
    #error "ERROR - Valid LCD clock source must be chosen in r_bsp_config.h using BSP_CFG_LCD_CLOCK_SOURCE macro."
#endif

    /* Set clock source */
    SYSTEM.LCDSCLKCR.BYTE = BSP_CFG_LCD_CLOCK_SOURCE;

    /* Enable/disable clock source output */
#if (BSP_CFG_LCD_CLOCK_ENABLE == 1)
    SYSTEM.LCDSCLKCR2.BYTE = 0x00;
#else
    SYSTEM.LCDSCLKCR2.BYTE = 0x01;
#endif

    SYSTEM.PRCR.WORD = 0xA500;

    return;
}

#else

/***********************************************************************************************************************
* Function name: cgc_clock_config
* Description  : Configures the clock settings for each of the device clocks. Uses the r_cgc_rx module.
* Arguments    : none
* Return value : none
***********************************************************************************************************************/
static void cgc_clock_config (void)
{
    cgc_clocks_t                clock;
    cgc_clock_config_t          pll_config;
    cgc_system_clock_config_t   sys_config;
    cgc_usb_settings_t          usb_cfg;
    cgc_lcd_settings_t          lcd_cfg;


    /* Figure out setting for FCK bits. */
#if   BSP_CFG_FCK_DIV == 1
    sys_config.fclk_div = CGC_SYS_CLOCK_DIV_1;
#elif BSP_CFG_FCK_DIV == 2
    sys_config.fclk_div = CGC_SYS_CLOCK_DIV_2;
#elif BSP_CFG_FCK_DIV == 4
    sys_config.fclk_div = CGC_SYS_CLOCK_DIV_4;
#elif BSP_CFG_FCK_DIV == 8
    sys_config.fclk_div = CGC_SYS_CLOCK_DIV_8;
#elif BSP_CFG_FCK_DIV == 16
    sys_config.fclk_div = CGC_SYS_CLOCK_DIV_16;
#elif BSP_CFG_FCK_DIV == 32
    sys_config.fclk_div = CGC_SYS_CLOCK_DIV_32;
#elif BSP_CFG_FCK_DIV == 64
    sys_config.fclk_div = CGC_SYS_CLOCK_DIV_64;
#else
    #error "Error! Invalid setting for BSP_CFG_FCK_DIV in r_bsp_config.h"
#endif

    /* Figure out setting for ICK bits. */
#if   BSP_CFG_ICK_DIV == 1
    sys_config.iclk_div = CGC_SYS_CLOCK_DIV_1;
#elif BSP_CFG_ICK_DIV == 2
    sys_config.iclk_div = CGC_SYS_CLOCK_DIV_2;
#elif BSP_CFG_ICK_DIV == 4
    sys_config.iclk_div = CGC_SYS_CLOCK_DIV_4;
#elif BSP_CFG_ICK_DIV == 8
    sys_config.iclk_div = CGC_SYS_CLOCK_DIV_8;
#elif BSP_CFG_ICK_DIV == 16
    sys_config.iclk_div = CGC_SYS_CLOCK_DIV_16;
#elif BSP_CFG_ICK_DIV == 32
    sys_config.iclk_div = CGC_SYS_CLOCK_DIV_32;
#elif BSP_CFG_ICK_DIV == 64
    sys_config.iclk_div = CGC_SYS_CLOCK_DIV_64;
#else
    #error "Error! Invalid setting for BSP_CFG_ICK_DIV in r_bsp_config.h"
#endif

    /* Figure out setting for PCKB bits. */
#if   BSP_CFG_PCKB_DIV == 1
    sys_config.pclkb_div = CGC_SYS_CLOCK_DIV_1;
#elif BSP_CFG_PCKB_DIV == 2
    sys_config.pclkb_div = CGC_SYS_CLOCK_DIV_2;
#elif BSP_CFG_PCKB_DIV == 4
    sys_config.pclkb_div = CGC_SYS_CLOCK_DIV_4;
#elif BSP_CFG_PCKB_DIV == 8
    sys_config.pclkb_div = CGC_SYS_CLOCK_DIV_8;
#elif BSP_CFG_PCKB_DIV == 16
    sys_config.pclkb_div = CGC_SYS_CLOCK_DIV_16;
#elif BSP_CFG_PCKB_DIV == 32
    sys_config.pclkb_div = CGC_SYS_CLOCK_DIV_32;
#elif BSP_CFG_PCKB_DIV == 64
    sys_config.pclkb_div = CGC_SYS_CLOCK_DIV_64;
#else
    #error "Error! Invalid setting for BSP_CFG_PCKB_DIV in r_bsp_config.h"
#endif

    /* Figure out setting for PCKD bits. */
#if   BSP_CFG_PCKD_DIV == 1
    sys_config.pclkd_div = CGC_SYS_CLOCK_DIV_1;
#elif BSP_CFG_PCKD_DIV == 2
    sys_config.pclkd_div = CGC_SYS_CLOCK_DIV_2;
#elif BSP_CFG_PCKD_DIV == 4
    sys_config.pclkd_div = CGC_SYS_CLOCK_DIV_4;
#elif BSP_CFG_PCKD_DIV == 8
    sys_config.pclkd_div = CGC_SYS_CLOCK_DIV_8;
#elif BSP_CFG_PCKD_DIV == 16
    sys_config.pclkd_div = CGC_SYS_CLOCK_DIV_16;
#elif BSP_CFG_PCKD_DIV == 32
    sys_config.pclkd_div = CGC_SYS_CLOCK_DIV_32;
#elif BSP_CFG_PCKD_DIV == 64
    sys_config.pclkd_div = CGC_SYS_CLOCK_DIV_64;
#else
    #error "Error! Invalid setting for BSP_CFG_PCKD_DIV in r_bsp_config.h"
#endif

    /* Initialize CGC. */
    R_CGC_Open();

    /* Make sure a valid clock is chosen. */
#if (BSP_CFG_CLOCK_SOURCE > 4) || (BSP_CFG_CLOCK_SOURCE < 0)
    #error "ERROR - Valid clock source must be chosen in r_bsp_config.h using BSP_CFG_CLOCK_SOURCE macro."
#endif

#if ((BSP_CFG_CLOCK_SOURCE == 1) || (BSP_CFG_LCD_CLOCK_SOURCE == 1))
    /* HOCO is chosen. Start it operating. */
    clock = CGC_HOCO;
    R_CGC_ClockStart(clock, (cgc_clock_config_t *)FIT_NO_PTR);
#else
    /* HOCO is not chosen. Stop the HOCO. */
    R_CGC_ClockStop(CGC_HOCO);
#endif

#if ((BSP_CFG_CLOCK_SOURCE == 2) || (BSP_CFG_LCD_CLOCK_SOURCE == 2) || (BSP_CFG_USB_CLOCK_SOURCE == 1))
    /* Main clock oscillator is chosen. Start it operating. */
    clock = CGC_MAIN_OSC;
    R_CGC_ClockStart(clock, (cgc_clock_config_t *)FIT_NO_PTR);
#else
    /* Set the main clock to stopped. */
    R_CGC_ClockStop(CGC_MAIN_OSC);
#endif

#if ((BSP_CFG_CLOCK_SOURCE == 3) || (BSP_CFG_LCD_CLOCK_SOURCE == 3))
    /* Sub-clock oscillator is chosen. Start it operating. */
    clock = CGC_SUBCLOCK;
    R_CGC_ClockStart(clock, (cgc_clock_config_t *)FIT_NO_PTR);
#else
    /* Set the sub-clock to stopped. */
    R_CGC_ClockStop(CGC_SUBCLOCK);
#endif

#if (BSP_CFG_CLOCK_SOURCE == 4)
    /* PLL is chosen. Start it operating. Must start main clock as well since PLL uses it. */
    clock = CGC_PLL;

    #if   (BSP_CFG_PLL_DIV == 1)
    pll_config.divider = CGC_PLL_DIV_1;
    #elif (BSP_CFG_PLL_DIV == 2)
    pll_config.divider = CGC_PLL_DIV_2;
    #elif (BSP_CFG_PLL_DIV == 4)
    pll_config.divider = CGC_PLL_DIV_4;
    #else
        #error "Error! Invalid setting for BSP_CFG_PLL_DIV in r_bsp_config.h"
    #endif

    #if   (BSP_CFG_PLL_MUL == 6)
    pll_config.multiplier = CGC_PLL_MUL_6;
    #elif (BSP_CFG_PLL_MUL == 8)
    pll_config.multiplier = CGC_PLL_MUL_8;
    #else
        #error "Error! Invalid setting for BSP_CFG_PLL_MUL in r_bsp_config.h"
    #endif

    R_CGC_ClockStart(CGC_MAIN_OSC, (cgc_clock_config_t *)FIT_NO_PTR);
    R_CGC_ClockStart(clock, &pll_config);
#else
    /* Set the PLL to stopped. */
    R_CGC_ClockStop(CGC_PLL);
#endif

#if (BSP_CFG_CLOCK_SOURCE == 0)
    /* LOCO is chosen. This is the default out of reset. */
#else
    /* LOCO is not chosen but it cannot be turned off yet since it is still being used. */
    /* Set system clock. */
    while (R_CGC_ClockCheck(clock) == CGC_ERR_NOT_STABILIZED)
    {
        /* Wait for chosen clock to stabilize. */
    }

    R_CGC_SystemClockSet(clock, &sys_config);
#endif
#if ((BSP_CFG_CLOCK_SOURCE != 0) && (BSP_CFG_LCD_CLOCK_SOURCE != 0))
    /* We can now turn LOCO off since it is not going to be used. */
    R_CGC_ClockStop(CGC_LOCO);
#endif



#if (BSP_CFG_USB_CLOCK_SOURCE == 1)
    /* Enable the USB PLL clock */

    #if (BSP_CFG_UPLL_DIV == 4)
        usb_cfg.divider = CGC_USB_DIV_4;
    #elif (BSP_CFG_UPLL_DIV == 2)
        usb_cfg.divider = CGC_USB_DIV_2;
    #elif (BSP_CFG_UPLL_DIV == 1)
        usb_cfg.divider = CGC_USB_DIV_1;
    #else
         #error "Error! Invalid setting for BSP_CFG_UPLL_DIV in r_bsp_config.h"
    #endif

    #if (BSP_CFG_UPLL_MUL == 6)
        usb_cfg.multiplier = CGC_USB_MUL_6;
    #elif (BSP_CFG_UPLL_MUL == 8)
        usb_cfg.multiplier = CGC_USB_MUL_8;
    #else
        #error "Error! Invalid setting for BSP_CFG_UPLL_MUL in r_bsp_config.h"
    #endif

    usb_cfg.source = CGC_USB_SOURCE_PLL;
    usb_cfg.enable = true;
    R_CGC_Control(CGC_CMD_CLKUSB_CONFIGURE, &usb_cfg);
#else
    /* Set the USB PLL to stopped */
    R_CGC_Control(CGC_CMD_CLKUSB_DISABLE, NULL);
#endif


#if (BSP_CFG_LCD_CLOCK_ENABLE == 1)
    /* Make sure a valid clock was chosen. */
#if (BSP_CFG_LCD_CLOCK_SOURCE > 4) || (BSP_CFG_LCD_CLOCK_SOURCE < 0)
    #error "ERROR - Valid LCD clock source must be chosen in r_bsp_config.h using BSP_CFG_LCD_CLOCK_SOURCE macro."
#endif
    /* Enable LCD clock  */
    #if (BSP_CFG_LCD_CLOCK_SOURCE == 0)
        lcd_cfg.source = CGC_LCD_SOURCE_LOCO;
    #elif (BSP_CFG_LCD_CLOCK_SOURCE == 1)
        lcd_cfg.source = CGC_LCD_SOURCE_HOCO;
    #elif (BSP_CFG_LCD_CLOCK_SOURCE == 2)
        lcd_cfg.source = CGC_LCD_SOURCE_MAIN_OSC;
    #elif (BSP_CFG_LCD_CLOCK_SOURCE == 3)
        lcd_cfg.source = CGC_LCD_SOURCE_SUBCLOCK;
    #elif (BSP_CFG_LCD_CLOCK_SOURCE == 4)
        lcd_cfg.source = CGC_LCD_SOURCE_IWDT;
    #endif
    lcd_cfg.enable = true;
    R_CGC_Control(CGC_CMD_CLKLCD_CONFIGURE, &lcd_cfg);
#else
    /* Set the LCD clock to stopped */
    R_CGC_Control(CGC_CMD_CLKLCD_DISABLE, NULL);
#endif

}
#endif /* BSP_CFG_USE_CGC_MODULE */


/***********************************************************************************************************************
* Function name: Change_PSW_PM_to_UserMode
* Description  : Assembler function, used to change the MCU's user mode from supervisor to user.
* Arguments    : none
* Return value : none
***********************************************************************************************************************/
#if BSP_CFG_RUN_IN_USER_MODE==1
    #if __RENESAS_VERSION__ < 0x01010000
static void Change_PSW_PM_to_UserMode(void)
{
    MVFC   PSW,R1
    OR     #00100000h,R1
    PUSH.L R1
    MVFC   PC,R1
    ADD    #10,R1
    PUSH.L R1
    RTE
    NOP
    NOP
}
    #endif
#endif



