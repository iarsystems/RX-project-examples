/******************************************************************************
* DISCLAIMER

* This software is supplied by Renesas Electronics Corporation and is only 
* intended for use with Renesas products. No other uses are authorized.

* This software is owned by Renesas Electronics Corporation and is protected under 
* all applicable laws, including copyright laws.

* THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES 
* REGARDING THIS SOFTWARE, WHETHER EXPRESS, IMPLIED OR STATUTORY, 
* INCLUDING BUT NOT LIMITED TO WARRANTIES OF MERCHANTABILITY, FITNESS FOR A 
* PARTICULAR PURPOSE AND NON-INFRINGEMENT.  ALL SUCH WARRANTIES ARE EXPRESSLY 
* DISCLAIMED.

* TO THE MAXIMUM EXTENT PERMITTED NOT PROHIBITED BY LAW, NEITHER RENESAS 
* ELECTRONICS CORPORATION NOR ANY OF ITS AFFILIATED COMPANIES SHALL BE LIABLE 
* FOR ANY DIRECT, INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES 
* FOR ANY REASON RELATED TO THIS SOFTWARE, EVEN IF RENESAS OR ITS 
* AFFILIATES HAVE BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.

* Renesas reserves the right, without notice, to make changes to this 
* software and to discontinue the availability of this software.  
* By using this software, you agree to the additional terms and 
* conditions found by accessing the following link:
* http://www.renesas.com/disclaimer
******************************************************************************/
/* Copyright (C) 2024 Renesas Electronics Corporation. All rights reserved.  */
/******************************************************************************	
* File Name    : 
* Device(s)    : 
* Tool-Chain   : 
* OS           : 
* H/W Platform : 
* Description  : 
* Operation    : 
* Limitations  : 
******************************************************************************
* History : DD.MM.YYYY Version Description
*         : 25.06.2024 1.00    First Release
******************************************************************************/

/******************************************************************************
Includes   <System Includes> , "Project Includes"
******************************************************************************/
#include "R5K574M.h"
#include "r_pz.h"
#include "r_irq.h"
#if defined(__ICCRX__)
#include "intrinsics.h"
#endif

/******************************************************************************
Typedef definitions
******************************************************************************/
/* Defines function pointers to be used with vector table. */
#if 0
#if defined(__GNUC__)
typedef void (* exc_ptr_t)(void);
#endif

/******************************************************************************
Macro definitions
******************************************************************************/
#define BSP_PRV_STACK_LIMIT_SECURE  ((uint32_t) (&g_main_stack[0] + BSP_CFG_STACK_MAIN_BYTES))
#define BSP_PRV_STACK_TOP_SECURE    ((uint32_t) (&g_main_stack[0] + (BSP_CFG_STACK_MAIN_BYTES << 1)))

/******************************************************************************
Imported global variables and functions (from other files)
******************************************************************************/
extern uint8_t g_main_stack[];
#if defined(__GNUC__)
extern const exc_ptr_t __Vectors[];
extern void * g_vector_table[];
#endif

#else //#if 0
#ifdef __ICCRX__
#pragma section="ISTACK_S" __data32
#pragma section=".exvectors" __data32
#pragma section=".rvectors" __data32
uint32_t *istack_secure = __section_end("ISTACK_S");
static uint32_t *exvectors_secure = __section_begin(".exvectors");
static uint32_t *rvectors_secure = __section_begin(".rvectors");

#else
extern uint32_t istack_s;
uint32_t *istack_secure = &istack_s;
extern uint32_t exvectors_start;
uint32_t *exvectors_secure = &exvectors_start;
extern uint32_t rvectors_start;
uint32_t *rvectors_secure = &rvectors_start;
#endif

#endif  //#if 0

extern void SpinProtectZone(void);
extern void R_CPSCU_Swint_Irq0_Set(r_pz_wid_t wid);

//extern icu_instance_ctrl_t g_external_irq0_ctrl;
//extern const external_irq_cfg_t g_external_irq0_cfg;

extern const uint32_t SRAM0_NS_GLB_VAL_START;

/******************************************************************************
Exported global variables and functions (to be accessed by other files)
******************************************************************************/
void pre_main_sec(void);
void main_sec(void);
uint32_t SSCHandler(intptr_t par1, intptr_t par2, intptr_t par3, intptr_t par4, int32_t fncd);

/******************************************************************************
Private global variables and functions
******************************************************************************/
void sec_world_check_error(void);
void irq_IntStatusGet(void);
void r_irq_callback(void);

//Non-Secureプロジェクト構築後に復活 volatile bool *pg_is_irq_int_nonsec = (bool *)0x02008A00;
uint32_t const volatile * const gp_NS_GlbVal_Start = &SRAM0_NS_GLB_VAL_START;
//volatile bool *gp_is_irq_int_nonsec = (bool *)gp_NS_GlbVal_Start;
volatile bool *gp_is_irq_int_nonsec;


/*******************************************************************************************************************//**
 * SSCHandler_0() has no arguments.
 **********************************************************************************************************************/
void irq_IntStatusGet(void)
{
    gp_is_irq_int_nonsec = (bool volatile *)gp_NS_GlbVal_Start;

    *gp_is_irq_int_nonsec = false;
    while(*gp_is_irq_int_nonsec != true)
    {
#if defined(__CCRX__)
        nop();
#elif defined(__GNUC__)
        __asm volatile ("NOP");
#elif defined(__ICCRX__)
    __no_operation();
#endif
    }
    *gp_is_irq_int_nonsec = false;
}

/*******************************************************************************************************************//**
 * SSCHandler_0() has no arguments.
 **********************************************************************************************************************/
uint32_t SSCHandler_0(void) {
    return 1;
}

/*******************************************************************************************************************//**
 * SSCHandler_1() has one argument.
 **********************************************************************************************************************/
uint32_t SSCHandler_1(intptr_t par1) {
    return 5;
}

/*******************************************************************************************************************//**
 * SSCHandler_2() has two argument.
 **********************************************************************************************************************/
uint32_t SSCHandler_2(intptr_t par1, intptr_t par2) {
    uint8_t ret;
//    fsp_err_t status;

    // Enable Interrupt
#if defined(__CCRX__)
    setpsw_i();
#elif defined(__GNUC__)
    __builtin_rx_setpsw('I');
#elif defined(__ICCRX__)
    __enable_interrupt();
#endif

    if(par1 == 1)
    {
        if(par2 == 0)
        {
            R_CPSCU_Swint_Irq0_Set(R_PZ_NONE_SECURE);
            R_IRQ_Close();
        }
        else if(par2 == 1)
        {
            /* Waiting for Non-Secure IRQ0 interrupt */
            irq_IntStatusGet();

            /* Set the interrupt security attribute to Secure. */
            R_CPSCU_Swint_Irq0_Set(R_PZ_SECURE);
            R_IRQ_Open();
            R_IRQ_CallbackSet(r_irq_callback);

            /* Waiting for Secure IRQ0 interrupt */
            irq_IntStatusGet();
            #if defined(__CCRX__)
                nop();
            #elif defined(__GNUC__)
                __asm volatile ("NOP");
            #elif defined(__ICCRX__)
                __no_operation();
            #endif
        }
    }
    return 5;
}

/*******************************************************************************************************************//**
 * SSCHandler_3() has three argument.
 **********************************************************************************************************************/
uint32_t SSCHandler_3(intptr_t par1, intptr_t par2, intptr_t par3) {
    return 5;
}

/*******************************************************************************************************************//**
 * SSCHandler_4() has four argument.
 **********************************************************************************************************************/
uint32_t SSCHandler_4(intptr_t par1, intptr_t par2, intptr_t par3, intptr_t par4) {
    return 5;
}

/*******************************************************************************************************************//**
 * SSCHandler() is a Secure function called from a Non-Secure function.
 **********************************************************************************************************************/
uint32_t SSCHandler(intptr_t par1, intptr_t par2, intptr_t par3, intptr_t par4, int32_t fncd)
{
    switch (fncd) {
      case 1:
        return SSCHandler_0();
        break;                
      case 2:
        return SSCHandler_1(par1);
        break;        
      case 3:
        return SSCHandler_2(par1, par2);
        break;        
      case 4:
        return SSCHandler_3(par1, par2, par3);
        break;        
      case 5:
        return SSCHandler_4(par1, par2, par3, par4);
        break;
      default:        
        return 10;
    }
}

/*******************************************************************************************************************//**
 * pre_main_sec() is a pre-processing before the main function branch.
 **********************************************************************************************************************/
void pre_main_sec(void)
{
    /* Initialize stack. */
#if 0
	__asm volatile ("mvtc\t%0, isp \n" : : "r" (BSP_PRV_STACK_TOP_SECURE) :);
#else
    __asm volatile (
    	"mvtc %0, isp          \n"
    	: : "r" (istack_secure)
    );
#endif

    main_sec();
}

/*******************************************************************************************************************//**
 * main_sec() is the main function of the Secure world.
 **********************************************************************************************************************/
void main_sec(void)
{
    volatile bool b_ret;
    uint8_t ret;
    uint32_t cnt = 0;

    /* Initialize table. */
    /* Secure uses the same tables as Root */
#if defined(__CCRX__)
    __SET_EXTB(&__Vectors);
    __SET_INTB(&g_vector_table);
#elif defined(__GNUC__) || defined(__ICCRX__)
#if 0
    __asm volatile ("mvtc\t%0, extb \n" : : "r" ((uint32_t) &__Vectors) :);
    __asm volatile ("mvtc\t%0, intb \n" : : "r" ((uint32_t) &g_vector_table) :);
#else  //#if 0
    /* setup secure exception vector */
    __asm volatile (
        "mvtc    %0, extb      \n"    /* EXCEPTION VECTOR ADDRESS    */
	    : : "r" (exvectors_secure)
	);
    /* setup secure intb */
	__asm volatile (
	    "mvtc    %0, intb      \n"    /* INTERRUPT VECTOR ADDRESS  definition    */
        : : "r" (rvectors_secure)
    );
#endif  //#if 0
#endif

    /* WID check */
    b_ret = R_ProtectZone_CheckWid(R_PZ_SECURE);
    if (b_ret != true)
    {
        sec_world_check_error();
    }

    // Enable Interrupt
#if defined(__CCRX__)
    setpsw_i();
#elif defined(__GNUC__)
    __builtin_rx_setpsw('I');
#elif defined(__ICCRX__)
    __enable_interrupt();
#endif

    /* Transition to Non-Secure World */
    SpinProtectZone();

    while(1);
}

/*******************************************************************************************************************//**
 * sec_world_check_error() is used when a world check error occurs.
 **********************************************************************************************************************/
void sec_world_check_error(void)
{
    while (1)
    {
#if defined(__CCRX__)
        nop();
#elif defined(__GNUC__)
        __asm("nop");
#elif defined(__ICCRX__)
    __no_operation();
#endif
    }
}

/*******************************************************************************************************************//**
 * r_irq_callback() is the callback function for IRQ0 interrupt.
 **********************************************************************************************************************/
//void r_irq_callback(external_irq_callback_args_t *p_args)
void r_irq_callback(void)
{
    gp_is_irq_int_nonsec = (bool *)gp_NS_GlbVal_Start;

    *gp_is_irq_int_nonsec = true;
#if defined(__CCRX__)
        nop();
#elif defined(__GNUC__)
        __asm("nop");
#elif defined(__ICCRX__)
    __no_operation();
#endif
}
