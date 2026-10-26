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
/* Copyright (C) 2012 Renesas Electronics Corporation. All rights reserved.   */
/*******************************************************************************
* File Name     : switch.c
* Version       : 1.00
* Device        : R5F52206
* Tool-Chain    : RX Family C Compiler 1.2.1.0
* H/W Platform  : RSKRX220
* Description   : Defines interrupt service routine functions for the switch
*                 interrupts. De-bounces switch and sets a key press flag.
*******************************************************************************/
/*******************************************************************************
* History       : 14.12.2012  Ver. 1.00 First Release
*******************************************************************************/

/*******************************************************************************
* System Includes
*******************************************************************************/

/* Following header file provides standard integer type definitions. */
#include <stdint.h>

/* Following header file provides string type definitions. */
#include <string.h>

/*******************************************************************************
* User Includes (Project Level Includes)
*******************************************************************************/

/* Defines RX220 LEDs and switches */
#include "rskrx220def.h"

/* Defines RX220 port registers */
#include <iorx220.h>

/* Switch handler function definitions */
#include "switch.h"

/*******************************************************************************
* Global Variables
*******************************************************************************/

/* Switch flag global variable */
volatile uint8_t g_switch_flag = 0x00;

/* Switch standby ready global variable */
volatile bool g_switch_standby_ready = true;

/* Detected switch faults counter variable */
volatile uint16_t g_switch_faults_detected = 0x0000;

/* Switch press callback pointer declaration */
void (*g_switch_press_callback_func)(void) = NULL;

/* Switch release callback pointer declaration */
void (*g_switch_release_callback_func)(void) = NULL;

/*******************************************************************************
* Local Function Prototypes
*******************************************************************************/

/* Start debounce timer function prototype */
static void start_debounce_timer (const uint16_t compare_match);

/* Switch debounce timer callback function prototype */
static void switch_debounce_cb (void);

/*******************************************************************************
* Function Name: initialise_switch_interrupts
* Description  : Configures the IRQ pins connected to switches SW1-SW3 to
*                 detect switch presses, and invoke an ISR.
* Arguments    : none
* Return Value : none
*******************************************************************************/
void initialise_switch_interrupts (void)
{
    /* Reset the switch flag */
    g_switch_flag = 0x00;

    /* Reset the switch standby ready flag */
    g_switch_standby_ready = 0x1u;

    /* Enable write to PFSWE bit */
    MPC.PWPR.BYTE = 0x00;

    /* Disable write protection to PFS registers */
    MPC.PWPR.BYTE = PFSPROTECTOFF;

    /* Set P3_1 (SW1) as an IRQ pin */
    MPC.P31PFS.BIT.ISEL = 0x1u;

    /* Select the pin function for the lower 5 bits */
    MPC.P31PFS.BIT.PSEL = 0x0;

    /* Set P3_3 (SW2) as an IRQ pin */
    MPC.P33PFS.BIT.ISEL = 0x1u;

    /* Select the pin function for the lower 5 bits */
    MPC.P33PFS.BIT.PSEL = 0x0;

    /* Set P3_4 (SW3) as an IRQ pin */
    MPC.P34PFS.BIT.ISEL = 0x1u;

    /* Select the pin function for the lower 5 bits */
    MPC.P34PFS.BIT.PSEL = 0x0;

    /* Enable write protection to PFS registers */
    MPC.PWPR.BYTE = PFSPROTECTON;

    /* Disable digital pin filtering for all IRQ pins */
    ICU.IRQFLTE0.BYTE = 0;

    /* Configure switch SW1 (IRQ1) */
    /* Clear SW1 interrupt requests */
    IEN(ICU, IRQ1) = 0x0;

    /* Set P3_1 (SW2) as an input */
    PORT3.PDR.BIT.B1 = 0;

    /* Set detection direction as falling edge */
    ICU.IRQCR[SW1_IRQ1].BIT.IRQMD = 0x1u;

    /* Clear IR flag */
    IR(ICU, IRQ1) = 0x0;

    /* Set interrupt priority level to 7 */
    IPR(ICU, IRQ1) = SWITCH_IPL;

    /* Enable SW1 interrupt requests */
    IEN(ICU, IRQ1) = 0x1u;

    /* Configure IRQ3 interrupt (connected to SW2) */
    /* Clear SW2 interrupt requests */
    IEN(ICU, IRQ3) = 0x0;

    /* Set P3_2 (SW2) as an input */
    PORT3.PDR.BIT.B3 = 0x0;

    /* Set detection direction as falling edge */
    ICU.IRQCR[SW2_IRQ3].BIT.IRQMD = 0x1u;

    /* Clear IR flag */
    IR(ICU, IRQ3) = 0x0;

    /* Set interrupt priority level to 7 */
    IPR(ICU, IRQ3) = SWITCH_IPL;

    /* Enable SW2 interrupt requests */
    IEN(ICU, IRQ3) = 0x1u;

    /* Configure IRQ4 interrupt (connected to SW3) */
    /* Clear SW3 interrupt requests */
    IEN(ICU, IRQ4) = 0x0;

    /* Set P3_4 (SW3) as an input */
    PORT3.PDR.BIT.B4 = 0x0;

    /* Set detection direction as falling edge */
    ICU.IRQCR[SW3_IRQ4].BIT.IRQMD = 0x1u;

    /* Clear IR flag */
    IR(ICU, IRQ4) = 0x0;

    /* Set interrupt priority level to 7 */
    IPR(ICU, IRQ4) = SWITCH_IPL;

    /* Enable SW3 interrupt requests */
    IEN(ICU, IRQ4) = 0x1u;
}
/******************************************************************************
* End of function initialise_switch_interrupts
******************************************************************************/

/******************************************************************************
* Function Name: set_switch_press_callback
* Description  : Takes a pointer to a function, and sets it as the callback
*                 function for the switch interrupts. The passed function is
*                 executed every time any switch is pressed down.
* Arguments    : pointer to callback function (set to NULL to disable)
* Return Value : none
******************************************************************************/
void set_switch_press_callback (void(*const callback)(void))
{
    /* Store the callback function pointer into the global variable */
    g_switch_press_callback_func = callback;
}
/******************************************************************************
* End of function set_switch_press_callback
******************************************************************************/

/******************************************************************************
* Function Name: set_switch_release_callback
* Description  : Takes a pointer to a function, and sets it as the callback
*                 function for the switch interrupts. The passed function is
*                 executed every time any switch is released.
* Arguments    : pointer to callback function (set to NULL to disable)
* Return Value : none
******************************************************************************/
void set_switch_release_callback (void(*const callback)(void))
{
    /* Store the callback function pointer into the global variable */
    g_switch_release_callback_func = callback;
}
/******************************************************************************
* End of function set_switch_release_callback
******************************************************************************/

/******************************************************************************
* Function Name: control_switch_interrupts
* Description  : Enables or disables the switch IRQ interrupts, based on the
*                 input variable, control.
* Arguments    : uint8_t - 0x1  : Enables switch interrupts.
*                           0x0  : Disables switch interrupts.
* Return Value : none
******************************************************************************/
void control_switch_interrupts (const uint8_t control)
{
    /* Check if control input is 0x1 */
    if (control)
    {
        /* Enable SW1 interrupt requests */
        IEN(ICU, IRQ1) = 0x1u;

        /* Enable SW2 interrupt requests */
        IEN(ICU, IRQ3) = 0x1u;

        /* Enable SW3 interrupt requests */
        IEN(ICU, IRQ4) = 0x1u;
    }

    /* Control input is 0x0 */
    else
    {
        /* Disable SW1 interrupts */
        IEN(ICU, IRQ1) = 0x0;

        /* Disable SW2 interrupts */
        IEN(ICU, IRQ3) = 0x0;

        /* Disable SW3 interrupts */
        IEN(ICU, IRQ4) = 0x0;
    }
}

/******************************************************************************
* End of function control_switch_interrupts
******************************************************************************/

/******************************************************************************
* Function Name: Excep_IRQ1_isr
* Description  : Switch 1 callback ISR function. The function disables switch
*                 interrupts, then uses the debounce timer to re-enable them
*                 after the debounce period finishes.
* Arguments    : none
* Return Value : none
******************************************************************************/
#pragma vector=VECT_ICU_IRQ1
__interrupt  void Excep_IRQ1_isr(void)
{
    /* Disable switch 1 interrupts */
    IEN(ICU, IRQ1) = 0x0;

    /* Set standby ready flag as false */
    g_switch_standby_ready = false;

    /* Check if interrupt was generated by falling edge */
    if (0x1 == ICU.IRQCR[SW1_IRQ1].BIT.IRQMD)
    {
        /* Start debounce timer */
        start_debounce_timer(DEBOUNCE_SHORT);

        /* Set detection direction as rising edge */
        ICU.IRQCR[SW1_IRQ1].BIT.IRQMD = DETECT_RISING;

        /* Set global switch flag to indicate SW1 is held down */
        g_switch_flag |= SWITCHHOLD_1;
    }
    else
    {
        /* Start debounce timer */
        start_debounce_timer(DEBOUNCE_LONG);

        /* Set detection direction to falling edge */
        ICU.IRQCR[SW1_IRQ1].BIT.IRQMD = DETECT_FALLING;

        /* Clear SW1 held-down flag bit in switch flag */
        g_switch_flag &= (SWITCHPRESS_ALL | ~SWITCHHOLD_1);

        /* Set global switch flag to indicate SW1 press complete */
        g_switch_flag |= SWITCHPRESS_1;

        /* Check if switch release callback function is not NULL */
        if (g_switch_release_callback_func)
        {
            /* Execute user callback function */
            g_switch_release_callback_func();
        }
    }
}
/******************************************************************************
* End of function Excep_IRQ1_isr
******************************************************************************/

/******************************************************************************
* Function Name: Excep_IRQ3_isr
* Description  : Switch 2 callback ISR function. The function disables switch
*                 interrupts, then uses the debounce timer to re-enable them
*                 after the debounce period finishes.
* Arguments    : none
* Return Value : none
******************************************************************************/
#pragma vector=VECT_ICU_IRQ3
__interrupt  void Excep_IRQ3_isr(void)
{
    /* Disable switch 2 interrupts */
    IEN(ICU, IRQ3) = 0x0;

    /* Set standby ready flag as false */
    g_switch_standby_ready = false;

    /* Check if interrupt triggered from falling edge */
    if (0x1 == ICU.IRQCR[SW2_IRQ3].BIT.IRQMD)
    {
        /* Start debounce timer */
        start_debounce_timer(DEBOUNCE_SHORT);

        /* Set detection direction as rising edge */
        ICU.IRQCR[SW2_IRQ3].BIT.IRQMD = DETECT_RISING;

        /* Set global switch flag to indicate SW2 is held down */
        g_switch_flag |= SWITCHHOLD_2;
    }
    else
    {
        /* Start debounce timer */
        start_debounce_timer(DEBOUNCE_LONG);

        /* Set detection direction to falling edge */
        ICU.IRQCR[SW2_IRQ3].BIT.IRQMD = DETECT_FALLING;

        /* Clear SW2 held-down flag bit in switch flag */
        g_switch_flag &= (SWITCHPRESS_ALL | ~SWITCHHOLD_2);

        /* Set global switch flag to indicate SW2 press complete */
        g_switch_flag |= SWITCHPRESS_2;

        /* Check if switch release callback function is not NULL */
        if (g_switch_release_callback_func)
        {
            /* Execute user callback function */
            g_switch_release_callback_func();
        }
    }
}
/******************************************************************************
* End of function Excep_IRQ3_isr
******************************************************************************/

/******************************************************************************
* Function Name: Excep_IRQ4_isr
* Description  : Switch 3 callback ISR function. The function disables switch
*                 interrupts, then uses the debounce timer to re-enable them
*                 after the debounce period finishes.
* Arguments    : none
* Return Value : none
******************************************************************************/
#pragma vector=VECT_ICU_IRQ4
__interrupt  void Excep_IRQ4_isr(void)
{
    /* Disable switch 3 interrupts */
    IEN(ICU, IRQ4) = 0x0;

    /* Set standby ready flag as false */
    g_switch_standby_ready = false;

    /* Check if detection direction is set to falling edge */
    if (0x1 == ICU.IRQCR[SW3_IRQ4].BIT.IRQMD)
    {
        /* Start debounce timer */
        start_debounce_timer(DEBOUNCE_SHORT);

        /* Set detection direction as rising edge */
        ICU.IRQCR[SW3_IRQ4].BIT.IRQMD = DETECT_RISING;

        /* Set global switch flag to indicate SW3 is held down */
        g_switch_flag |= SWITCHHOLD_3;
    }
    else
    {
        /* Start debounce timer */
        start_debounce_timer(DEBOUNCE_LONG);

        /* Set detection direction to falling edge */
        ICU.IRQCR[SW3_IRQ4].BIT.IRQMD = DETECT_FALLING;

        /* Clear SW3 held-down flag bit in switch flag */
        g_switch_flag &= (SWITCHPRESS_ALL | ~SWITCHHOLD_3);

        /* Set global switch flag to indicate SW1 press complete */
        g_switch_flag |= SWITCHPRESS_3;

        /* Check if switch release callback function is not NULL */
        if (g_switch_release_callback_func)
        {
            /* Execute user callback function */
            g_switch_release_callback_func();
        }
    }
}
/******************************************************************************
* End of function Excep_IRQ4_isr
******************************************************************************/

/******************************************************************************
* Function Name: switch_debounce_cb
* Description  : Switch debounce timer callback function. Function is executed
*                 by the CMT channel ISR. Function re-enables all switch all
*                 switch interrupts; then verifies a correct switch press was
*                 detected, and calls the user switch press callback function.
* Arguments    : none
* Return Value : none
******************************************************************************/
static void switch_debounce_cb (void)
{
    /* Clear IRQ1 interrupt flag */
    IR(ICU, IRQ1) = 0x0;

    /* Re-enable switch 1 interrupts */
    IEN(ICU, IRQ1) = 0x1u;

    /* Clear IRQ3 interrupt flag */
    IR(ICU, IRQ3) = 0x0;

    /* Re-enable switch 2 interrupts */
    IEN(ICU, IRQ3) = 0x1u;

    /* Clear IRQ4 interrupt flag */
    IR(ICU, IRQ4) = 0x0;

    /* Re-enable switch 3 interrupts */
    IEN(ICU, IRQ4) = 0x1u;

    /* Check if switch 1 hold flag is set */
    if ((g_switch_flag & SWITCHHOLD_ALL) & SWITCHHOLD_1)
    {
        /* Check if switch 1 pin level is high (switch press duration
           less than debounce, invalid switch press) */
        if (PORT3.PIDR.BIT.B1)
        {
            /* Reset detection direction to falling edge */
            ICU.IRQCR[SW1_IRQ1].BIT.IRQMD = DETECT_FALLING;

            /* Clear switch press flag */
            g_switch_flag &= (SWITCHPRESS_ALL | ~SWITCHHOLD_1);

            /* Increment detected switch faults counter */
            g_switch_faults_detected++;
        }

        /* Switch 1 pin level is low (valid switch press) */
        else
        {
            /* Check if switch press callback function is not NULL */
            if (g_switch_press_callback_func)
            {
                /* Execute user callback function */
                g_switch_press_callback_func();
            }
        }
    }

    /* Check if switch 2 hold flag is set */
    if ((g_switch_flag & SWITCHHOLD_ALL) & SWITCHHOLD_2)
    {
        /* Check if switch 2 pin level is high (switch press duration
           less than debounce, invalid switch press) */
        if (PORT3.PIDR.BIT.B3)
        {
            /* Reset detection direction to falling edge */
            ICU.IRQCR[SW2_IRQ3].BIT.IRQMD = DETECT_FALLING;

            /* Clear switch press flag */
            g_switch_flag &= (SWITCHPRESS_ALL | ~SWITCHHOLD_2);

            /* Increment detected switch faults counter */
            g_switch_faults_detected++;
        }

        /* Switch 2 pin level is low (valid switch press) */
        else
        {
            /* Check if switch press callback function is not NULL */
            if (g_switch_press_callback_func)
            {
                /* Execute user callback function */
                g_switch_press_callback_func();
            }
        }
    }

    /* Check if switch 3 hold flag is set */
    if ((g_switch_flag & SWITCHHOLD_ALL) & SWITCHHOLD_3)
    {
        /* Check if switch 3 pin level is high (switch press duration
           less than debounce, invalid switch press) */
        if (PORT3.PIDR.BIT.B4)
        {
            /* Reset detection direction to falling edge */
            ICU.IRQCR[SW3_IRQ4].BIT.IRQMD = DETECT_FALLING;

            /* Clear switch press flag */
            g_switch_flag &= (SWITCHPRESS_ALL | ~SWITCHHOLD_3);

            /* Increment detected switch faults counter */
            g_switch_faults_detected++;
        }

        /* Switch 3 pin level is low (valid switch press) */
        else
        {
            /* Check if switch press callback function is not NULL */
            if (g_switch_press_callback_func)
            {
                /* Execute user callback function */
                g_switch_press_callback_func();
            }
        }
    }

    /* Check if any switches are held down */
    if (0x00 == (g_switch_flag & SWITCHHOLD_ALL))
    {
        g_switch_standby_ready = true;
    }

}
/******************************************************************************
* End of function switch_debounce_cb
******************************************************************************/

/******************************************************************************
* Function Name: start_debounce_timer
* Description  : Function initialises the CMT timer (the first time the
*                 function is called), and starts the CMT timer to perform
*                 generate the switch debounce interrupt.
* Arguments    : uint16_t compare_match    : compare match value to trigger
*                                             *interrupt at.
* Return Value : none
******************************************************************************/
static void start_debounce_timer (const uint16_t compare_match)
{
    /* Declare local static variable to track if the CMT timer has been
        initialised yet */
    static bool timer_initialised = false;

    /* Check if the CMT timer is not initialised (first time function has been
       called). */
    if (!timer_initialised)
    {
        /* Disable register protection */
        SYSTEM.PRCR.WORD = PROTECTOFF;

        /* Enable the CMT0 module */
        MSTP_CMT0 = 0;

        /* Re-enable register protection */
        SYSTEM.PRCR.WORD = PROTECTON;

        /* Set CMT0 clock source as PLCK/512 */
        CMT0.CMCR.BIT.CKS = 0x2u;

        /* Enable compare match interrupt */
        CMT0.CMCR.BIT.CMIE = 0x1u;

        /* Enable CMT0 interrupt request */
        IEN(CMT0, CMI0) = 0x1u;

        /* Set interrupt priority to 12 */
        IPR(CMT0, CMI0) = 0xCu;

        /* Set the timer inititalised flag */
        timer_initialised = true;
    }

    /* Set compare match to to generate debounce period */
    CMT0.CMCOR = compare_match;

    /* Reset count to zero */
    CMT0.CMCNT = 0x0000;

    /* Start timer */
    CMT.CMSTR0.BIT.STR0 = 0x1u;
}
/******************************************************************************
* End of function start_debounce_timer
******************************************************************************/

/******************************************************************************
* Function Name: Excep_CMTU0_CMT0_isr
* Description  : Interupt service routine for the CMT channel 0 compare
*                 interrupt.
* Arguments    : uint16_t compare_match    : compare match value to trigger
*                                             *interrupt at.
* Return Value : none
******************************************************************************/
#pragma vector=VECT_CMT0_CMI0
__interrupt void Excep_CMTU0_CMT0_isr (void)
{
    /* Stop the CMT0 timer running */
    CMT.CMSTR0.BIT.STR0 = 0x0;

    /* Call the switch debounce callback function */
    switch_debounce_cb();
}
/******************************************************************************
* End of function Excep_CMTU0_CMT0_isr
******************************************************************************/

