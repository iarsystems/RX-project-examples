/***********************************************************************************************************************
 * Copyright [2020-2022] Renesas Electronics Corporation and/or its affiliates.  All Rights Reserved.
 *
 * This software and documentation are supplied by Renesas Electronics Corporation and/or its affiliates and may only
 * be used with products of Renesas Electronics Corp. and its affiliates ("Renesas").  No other uses are authorized.
 * Renesas products are sold pursuant to Renesas terms and conditions of sale.  Purchasers are solely responsible for
 * the selection and use of Renesas products and Renesas assumes no liability.  No license, express or implied, to any
 * intellectual property right is granted by Renesas.  This software is protected under all applicable laws, including
 * copyright laws. Renesas reserves the right to change or discontinue this software and/or this documentation.
 * THE SOFTWARE AND DOCUMENTATION IS DELIVERED TO YOU "AS IS," AND RENESAS MAKES NO REPRESENTATIONS OR WARRANTIES, AND
 * TO THE FULLEST EXTENT PERMISSIBLE UNDER APPLICABLE LAW, DISCLAIMS ALL WARRANTIES, WHETHER EXPLICITLY OR IMPLICITLY,
 * INCLUDING WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE, AND NONINFRINGEMENT, WITH RESPECT TO THE
 * SOFTWARE OR DOCUMENTATION.  RENESAS SHALL HAVE NO LIABILITY ARISING OUT OF ANY SECURITY VULNERABILITY OR BREACH.
 * TO THE MAXIMUM EXTENT PERMITTED BY LAW, IN NO EVENT WILL RENESAS BE LIABLE TO YOU IN CONNECTION WITH THE SOFTWARE OR
 * DOCUMENTATION (OR ANY PERSON OR ENTITY CLAIMING RIGHTS DERIVED FROM YOU) FOR ANY LOSS, DAMAGES, OR CLAIMS WHATSOEVER,
 * INCLUDING, WITHOUT LIMITATION, ANY DIRECT, CONSEQUENTIAL, SPECIAL, INDIRECT, PUNITIVE, OR INCIDENTAL DAMAGES; ANY
 * LOST PROFITS, OTHER ECONOMIC DAMAGE, PROPERTY DAMAGE, OR PERSONAL INJURY; AND EVEN IF RENESAS HAS BEEN ADVISED OF THE
 * POSSIBILITY OF SUCH LOSS, DAMAGES, CLAIMS OR COSTS.
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * Includes
 **********************************************************************************************************************/
#include "R5K574M.h"
#include "r_pz.h"
#if defined(__ICCRX__)
#include "intrinsics.h"
#endif
/***********************************************************************************************************************
 * Typedef definitions
 **********************************************************************************************************************/
/* Defines function pointers to be used with vector table. */
typedef void (* exc_ptr_t)(void);

/******************************************************************************
Macro definitions
******************************************************************************/
#define BSP_ALIGN_VARIABLE(x)       __attribute__((aligned(x)))

/******************************************************************************
Imported global variables and functions (from other files)
******************************************************************************/
extern void pre_main_sec(void);
extern void sv_trans_sec(void);
extern uint32_t SSCHandler(intptr_t par1, intptr_t par2, intptr_t par3, intptr_t par4, int32_t fncd);

extern CPU_CONTEXT SContext;
extern CPU_CONTEXT NSContext;

extern const uint32_t PRE_INT_FUNC_NS;

/******************************************************************************
Exported global variables and functions (to be accessed by other files)
******************************************************************************/
void sv_trans_sec_root(void);
void sv_trans_nonsec_root(void);
void sv_trans_sec(void);
void sv_trans_nonsec(void);

void init_TRCTB_WINT(void);
void set_icv(uint32_t data);

void StartSecure(uint32_t pc_adrs, uint32_t psw_data);
void SpinProtectZone(void);

void trans_sec_root_func(void);
void trans_nonsec_root_func(void);
void trans_sec_nonsec_trcall(void);
void call_ssc_trcall(void);
void return_ssc_trcall(void);
void intrpt_rtn_sec(void);

void wint_prg_root(void);
void wint_prg_sec(void);
void wint_prg_ns(void);

const exc_ptr_t trctb_table[];
const exc_ptr_t wintb_table[];

uint32_t const * const gp_preInt_NS_Start = &PRE_INT_FUNC_NS;

/*******************************************************************************************************************//**
 * sv_trans_sec_root() is used when an error occurs in the transition from the Secure world to the Root world.
 **********************************************************************************************************************/
void sv_trans_sec_root(void)
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
 * sv_trans_nonsec_root() is used when an error occurs in the transition from the Non-Secure world to the Root world.
 **********************************************************************************************************************/
void sv_trans_nonsec_root(void)
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
 * sv_trans_sec() is used when an error occurs in the transition to the Secure world.
 **********************************************************************************************************************/
void sv_trans_sec(void)
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
 * sv_trans_nonsec() is used if an error occurs during the transition to the Non-Secure world.
 **********************************************************************************************************************/
void sv_trans_nonsec(void)
{
    while(1)
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

/******************************************************************************
 * init_TRCTB_WINT() Initialize the TRCTB and WINTB registers.
******************************************************************************/
void init_TRCTB_WINT(void)
{
#if defined(__GNUC__) || defined(__ICCRX__)
    /* This is a naked/stackless function. Do not pass arguments to the inline assembly when the GCC compiler is
     * used. */
    __asm volatile (
        "mov.l  %0, r1       \n"
        "trmvtc r1, TRCTB    \n"
        "mov.l  %1, r1       \n"
        "trmvtc r1, WINTB    \n"
        : : "i" (trctb_table), "i" (wintb_table)
    );
#endif
}

/******************************************************************************
 * set_ivc() set the IVC register.
 *****************************************************************************/
void set_ivc(uint32_t data)
{
#if defined(__GNUC__) || defined(__ICCRX__)
    /* This is a naked/stackless function. Do not pass arguments to the inline assembly when the GCC compiler is
     * used. */
    __asm volatile (
        "mvtc   %0, ivc    \n"
        ::"r" (data)
    );
#endif
}

/******************************************************************************
 * StartSecure() is Root process for transitioning
 * from Root to Secure.
 *****************************************************************************/
void StartSecure(uint32_t pc_adrs, uint32_t psw_data)
{
#if defined(__GNUC__) || defined(__ICCRX__)
    /* This is a naked/stackless function. Do not pass arguments to the inline assembly when the GCC compiler is
     * used. */
    __asm volatile (
        "mov.l  #0100h, r5    \n"    /* Set WID.BWID = 1(Secure) */
        "mvtc  r5,wid         \n"
        "push.l %0            \n"    /* Push PSW to stack */
        "push.l %1            \n"    /* Push Secure Entry to stack */
        "trrte                \n"    /* Call pre_main_sec */
        "nop                  \n"    /* Dummy Nop */
        "StartSecure_1:       \n"
        "bra.b  StartSecure_1 \n"    /* Never reach here */
        : : "r" (psw_data), "r" (pc_adrs)
    );
#endif
}

/******************************************************************************
 * SpinProtectZone() processes the following.
 * -Calling NonSecure from Secure
 * -Calling a secure service call
 *****************************************************************************/
void SpinProtectZone(void)
{
#if defined(__GNUC__) || defined(__ICCRX__)
    /* This is a naked/stackless function. Do not pass arguments to the inline assembly when the GCC compiler is
     * used. */
    __asm volatile (
       "_SpinProtectZone_0:         \n"
        "mov.l  #0, r5              \n"  /* Set r5 to 0 to determine if SSC is called */
        "trcall #2                  \n"  /* Call trans_sec_nonsec_trcall() */

        /* Transition from NonSecure */
        /* r5 == 0: Transition due to Secure interrupt */
        /* r5 != 0: Transition due to SSC call  */
        "_SpinProtectZone_1:        \n"
        "cmp    #0, r5              \n"  /* If r5 is not 0, call SSCHandler. */
        "beq.b  _SpinProtectZone_0  \n"
        /* Call SSCHandler() */
        "_SpinProtectZone_2:        \n"
        "push.l r5                  \n"  /* Push the 5th argument */
//        "mov.l  #_SSCHandler, r14   \n"
        "mov.l  %0, r14             \n"
        "jsr    r14                 \n"
        "add    #4, r0              \n"
        "mov.l  #0, r5              \n"  /* Set to 0 to determine if SSC is called */
        "trcall #4                  \n"  /* Call return_ssc_trcall() */
        "bra.b  _SpinProtectZone_1  \n"
        : : "i" (SSCHandler)
    );
#endif
}

/******************************************************************************
 * [TRCALL 0]
 *  trans_sec_root_func() is Root processing when transitioning
 *  from Secure to Root
 *****************************************************************************/
void trans_sec_root_func(void)
{
#if defined(__GNUC__) || defined(__ICCRX__)
    /* This is a naked/stackless function. Do not pass arguments to the inline assembly when the GCC compiler is
     * used. */
    __asm volatile (
        "trans_sec_root_func_0:        \n"
        "bra.b trans_sec_root_func_0   \n"
    );
#endif
}

/******************************************************************************
 * [TRCALL 1]
 *  trans_nonsec_root_func() is Root processing when transitioning
 *  from Non-Secure to Root
 *****************************************************************************/
void trans_nonsec_root_func(void)
{
#if defined(__GNUC__) || defined(__ICCRX__)
    /* This is a naked/stackless function. Do not pass arguments to the inline assembly when the GCC compiler is
     * used. */
    __asm volatile (
        "trans_nonsec_root_func_0:     \n"
        "bra.b trans_nonsec_root_func_0\n"
    );
#endif
}

/******************************************************************************
 * [TRCALL 2]
 *  trans_sec_nonsec_trcall() is Root processing when transitioning
 *  from Secure to Non-Secure
 *****************************************************************************/
void trans_sec_nonsec_trcall(void)
{
#if defined(__GNUC__) || defined(__ICCRX__)
    /* This is a naked/stackless function. Do not pass arguments to the inline assembly when the GCC compiler is
     * used. */
    __asm volatile (
        /* Error check */
        /* If the call is not from a Secure, an error occurs. */
        "pushm  r5-r6                     \n"
        "mvfc   wid, r5                   \n"
        "mov.l  r5,r6                     \n"
        "and    #00000300h,r6             \n"
        "cmp    #00000100h,r6             \n"  /* check BWID */
//        "bne   _sv_trans_nonsec            \n"
        "beq.b  trans_sec_nonsec_trcall_1 \n"
        "popm   r5-r6                     \n"
//        "mov.l #_sv_trans_nonsec,r1      \n"
        "mov.l  %0,r1                     \n"
        "jsr    r1                        \n"

        "trans_sec_nonsec_trcall_1:       \n"
        "popm   r5-r6                     \n"

        /* Save the PC and PSW from ISP_TR in SContext. */
        "popm   r1-r2                     \n"  /* r1:PC r2:PSW */
//        "mov.l  #_SContext, r3            \n"
        "mov.l  %1, r3                    \n"
        "mov.l  r2, [r3]                  \n"  /* PSW */
        "mov.l  r1, 4[r3]                 \n"  /* PC */

        /* Save Secure Context */
        "save   #1                        \n"

        /* Call NonSecure */
        "mov.l  #00000200h,r6             \n"  /* Set WID.BWID = 2 (NonSecure) */
        "mvtc   r6,wid                    \n"

//        "mov.l  #_NSContext, r1           \n"
        "mov.l  %2, r1                    \n"
        "mov.l  [r1], r3                  \n"  /* PSW */
        "mov.l  4[r1], r4                 \n"  /* PC */
        "push.l r3                        \n"
        "push.l r4                        \n"

        /* Restore Non-Secure Context */
        "rstr   #2                        \n"

        "nop                              \n"  /* Dummy Nop */
        "trrte                            \n"
        "nop                              \n"  /* Dummy Nop */

        /* Never reach here */
        "trans_sec_nonsec_trcall_2:       \n"
        "bra.b trans_sec_nonsec_trcall_2  \n"
        : : "i" (sv_trans_nonsec), "i" (&SContext), "i" (&NSContext)
    );
#endif
}

/******************************************************************************
 * [TRCALL 3]
 *  call_ssc_trcall() is Root processing for a secure service call.
 *  r1 - r4      : argument
 *  r5           : Function Code
 *  r1           : Return Value
 *  r14,r15      : scratch
 *  *(SP_TR + 0) : PC + 4 (Return Address)
 *  *(SP_TR + 4) : PSW
 *****************************************************************************/
void call_ssc_trcall(void)
{
#if defined(__GNUC__) || defined(__ICCRX__)
    /* This is a naked/stackless function. Do not pass arguments to the inline assembly when the GCC compiler is
     * used. */
    __asm volatile (
        /* Error check */
        /* If the call is not from a Non-Secure, an error occurs. */
        "pushm r5-r6                      \n"
        "mvfc  wid, r5                    \n"
        "mov.l r5,r6                      \n"
        "and   #00000300h,r6              \n"
        "cmp   #00000200h,r6              \n"  /* check BWID */
//        "bne   _sv_trans_sec              \n"
        "beq.b call_ssc_trcall_1          \n"
        "popm  r5-r6                      \n"
//        "mov.l #_sv_trans_sec,r1          \n"
        "mov.l %0,r1                      \n"
        "jsr   r1                         \n"

        "call_ssc_trcall_1:               \n"
        "popm  r5-r6                      \n"

        /* Save NonSecure Context */
        "save  #2                         \n"

        /* Preparing for a Secure call */
        "mov.l #00000100h,r6              \n"  /* Set WID.BWID = 1 (Secure) */
        "mvtc  r6,wid                     \n"

        /* To return to the instruction following TRCALL in the */
        /* SpinProtectZone() function, push the PC and PSW into ISP_TR. */
//        "mov.l  #_SContext, r6            \n"
        "mov.l %1, r6                       \n"
        "mov.l [r6], r15                    \n"  /* PSW */
        "mov.l 4[r6], r14                   \n"  /* PC */
        "pushm r14-r15                      \n"  /* R14:PC R15:PSW */

        /* A secure function called from a non-secure function is called        */
        /* via the secure function's SpinProtectZone() function.                */
        /* Therefore, it branches to SpinProtectZone() without corrupting r1-r5 */
        /* passed as arguments from the non-secure function.                    */

        /* Temporary save of r1-r5 passed as arguments from non-secure functions */
        "pushm r1-r5                        \n"

        /* Restore the secure context */
        "rstr  #1                           \n"

        /* Restoration of r1-r5 passed as arguments from Non-Secure */
        "popm  r1-r5                        \n"

        "nop                                \n"  /* Dummy Nop */
        "trrte                              \n"
        "nop                                \n"  /* Dummy Nop */

        /* Never reach here */
        "call_ssc_trcall_2:                 \n"
        "bra.b call_ssc_trcall_2            \n"
        : : "i" (sv_trans_sec), "i" (&SContext)
    );
#endif
}

/******************************************************************************
 * [TRCALL 4]
 * return_ssc_trcall() is Root processing is a root process to return
 * from Secure to Non-Secure after executing a secure service call.
 *
 *  r1 : Return value
 *****************************************************************************/
void return_ssc_trcall(void)
{
#if defined(__GNUC__) || defined(__ICCRX__)
    /* This is a naked/stackless function. Do not pass arguments to the inline assembly when the GCC compiler is
     * used. */
    __asm volatile (
        /* Error check */
        /* If the call is not from a Secure, an error occurs. */
        "pushm  r5-r6                     \n"
        "mvfc   wid, r5                   \n"
        "mov.l  r5,r6                     \n"
        "and    #00000300h,r6             \n"
        "cmp    #00000100h,r6             \n"  /* check BWID */
//        "bne   _sv_trans_nonsec           \n"
        "beq.b  return_ssc_trcall_1       \n"
        "popm   r5-r6                     \n"
//        "mov.l #_sv_trans_nonsec,r1       \n"
        "mov.l  %0,r1                     \n"
        "jsr    r1                        \n"

        "return_ssc_trcall_1:             \n"
        "popm   r5-r6                     \n"

        /* Save PC and PSW from ISP_TR to SContext. */
        "popm   r2-r3                     \n"  /* R2:PC R3:PSW */
//        "mov.l #_SContext, r4             \n"
        "mov.l  %1, r4                    \n"
        "mov.l  r3, [r4]                  \n"  /* PSW */
        "mov.l  r2, 4[r4]                 \n"  /* PC */

        /* save Secure Cotext */
        "save   #1                        \n"

        /* Preparing for a Non-Secure call */
        "mov.l  #00000200h,r6             \n"  /* Set WID.BWID = 2 (NonSecure) */
        "mvtc   r6,wid                    \n"

        /* Restore NonSecure context */
        /* r1 is the return value, so use the Secure(SpinProtectZone()) value. */
        "push.l r1                        \n"
        "rstr   #2                        \n"
        "pop    r1                        \n"

        "nop                              \n"  /* Dummy Nop */
        "trrte                            \n"
        "nop                              \n"  /* Dummy Nop */

        /* Never reach here */
        "return_ssc_trcall_2:             \n"
        "bra.b return_ssc_trcall_2        \n"
        : : "i" (sv_trans_nonsec), "i" (&SContext)
    );
#endif
}

/******************************************************************************
 * [TRCALL 5]
 *  intrpt_rtn_sec() is Root process that returns to Secure from Non-Secure
 *  interrupt processing involving a world transition.
 *****************************************************************************/
void intrpt_rtn_sec(void)
{
#if defined(__GNUC__) || defined(__ICCRX__)
    /* This is a naked/stackless function. Do not pass arguments to the inline assembly when the GCC compiler is
     * used. */
    __asm volatile (
        /* Error check */
        /* If the call is not from a Secure, an error occurs. */
        "mvfc  wid, r1                    \n"
        "mov.l r1,r2                      \n"
        "and   #00000300h,r2              \n"
        "cmp   #00000200h,r2              \n"  /* check BWID */
//        "bne _sv_trans_sec                \n"
        "beq.b   intrpt_rtn_sec_1         \n"
//        "mov.l #_sv_trans_sec,r1          \n"
        "mov.l %0,r1                      \n"
        "jsr   r1                         \n"

        "intrpt_rtn_sec_1:                \n"

        /* Drops from the stack the Non-secure PSW that was pushed onto the stack by the "TRCALL #5" instruction. */
        "pop   r1                         \n"

        /* Drops from the stack the Non-secure PC that was pushed onto the stack by the "TRCALL #5" instruction. */
        "pop   r1                         \n"

        /* Pops the WID(WID.BWID=1) pushed onto the stack by the wint_prg_ns function. */
        "pop   r1                         \n"
        "mvtc  r1,wid                     \n"  /* Restore BWID */
        "rstr  #1                         \n"
        "trrte                            \n"

        /* Never reach here */
        "intrpt_rtn_sec_2:                \n"
        "bra.b intrpt_rtn_sec_2           \n"
        : : "i" (sv_trans_sec)

    );
#endif
}

/******************************************************************************
 * wint_prg_root() is executed when a Root world interrupt occurs
 * while executing in the Non-Secure or Secure world.
 *****************************************************************************/
void wint_prg_root(void)
{
#if defined(__GNUC__) || defined(__ICCRX__)
    /* This is a naked/stackless function. Do not pass arguments to the inline assembly when the GCC compiler is
     * used. */
    __asm volatile (
        "wint_prg_root_1:                \n"
        "bra.b wint_prg_root_1           \n"
    );
#endif
}

/******************************************************************************
 * wint_prg_sec() is executed when a Secure world interrupt occurs
 * while executing in the Non-Secure world.
 *****************************************************************************/
void wint_prg_sec(void)
{
#if defined(__GNUC__) || defined(__ICCRX__)
    /* This is a naked/stackless function. Do not pass arguments to the inline assembly when the GCC compiler is
     * used. */
    __asm volatile (
        /* Save Non-Secure Context */
        "save   #2                       \n"

        /* Save the PC and PSW from ISP_TR in NSContext. */
        "popm  r1-r2                     \n"  /* r1:PC r2:PSW */
//        "mov.l #_NSContext, r3           \n"
        "mov.l  %0, r3                   \n"
        "mov.l  r2, [r3]                 \n"  /* PSW */
        "mov.l  r1, 4[r3]                \n"  /* PC */

        "mvfc   wid, r1                  \n"
        "mov.l  #00000100h,r2            \n"  /* WID.BWID = Secure */
        "mvtc   r2,wid                   \n"  /* set WID */

        /* Save the PSW and PC from SContext to ISP_S so that the return    */
        /* destination from a Secure interrupt involving a world transition */
        /* is the Secure SpinProtectZone() function.                        */
//        "mov.l  #_SContext, r3           \n"
        "mov.l  %1, r3                   \n"
        "mov.l  [r3], r2                 \n"  /* PSW */
        "mov.l  4[r3], r1                \n"  /* PC */
        "mov.l  r0, r3                   \n"  /* ISP_TR temporary storage */
        "trmvfc isp, r0                  \n"  /* Get ISP_S */
        "pushm  r1-r2                    \n"  /* r1:PC r2:PSW */
        "trmvtc r0, isp                  \n"  /* Update ISP_S */
        "mov.l  r3, r0                   \n"  /* Restore ISP_TR */

        /* Read the interrupt handler address */
        "mvfc   ivc, r1                  \n"
        "and    #000000ffh, r1           \n"  /* r1 = interrupt cause number */
        "trmvfc intb, r2                 \n"
        "shll   #2, r1                   \n"
        "add    r1, r2                   \n"
        "mov.l  [r2], r3                 \n"  /* r3:interrupt handler address */

        /* Setting PSW for calling the interrupt handler */
        "mvfc   psw, r1                  \n"
        "push.l r1                       \n"
        "push.l r3                       \n"

        /* Restore Secure Context */
        "rstr   #1                       \n"

        "nop                             \n"  /* Dummy Nop */
        "trrte                           \n"
        "nop                             \n"  /* Dummy Nop */

        /* Never reach here */
        "wint_prg_sec_1:                 \n"
        "bra.b wint_prg_sec_1            \n"
        : : "i" (&NSContext), "i" (&SContext)
    );
#endif
}

/******************************************************************************
 * wint_prg_ns() is executed when a Non-Secure world interrupt occurs
 * while executing in the Secure world.
 *****************************************************************************/
void wint_prg_ns(void)
{
#if defined(__GNUC__) || defined(__ICCRX__)
    /* This is a naked/stackless function. Do not pass arguments to the inline assembly when the GCC compiler is
     * used. */
    __asm volatile (
        /* Save Secure Context */
        "save   #1                       \n"

        "mvfc   wid, r1                  \n"
        "push.l r1                       \n"  /* Backup WID */

        "mov.l  #00000200h,r2            \n"  /* WID.BWID = Non-Secure */
        "mvtc   r2,wid                   \n"  /* set WID */

        "mvfc   psw, r1                  \n"
        "push.l r1                       \n"

//        "mov.l #NS_PRG_TABLE,r1          \n"
        "mov.l  %0,r1                    \n"
        "push.l r1                       \n"
        "trrte                           \n"

        /* Never reach here */
        "wint_prg_ns_1:                  \n"
        "bra.b wint_prg_ns_1             \n"
#ifdef __ICCRX__
        : : "i" (&PRE_INT_FUNC_NS)
#else
        : : "i" (gp_preInt_NS_Start)
#endif
    );
#endif
}




/* TRCTB Table */
const exc_ptr_t trctb_table[] BSP_ALIGN_VARIABLE(4) =
{
    trans_sec_root_func,     /* Root Call Vecter Table 0 */
    trans_nonsec_root_func,  /* Root Call Vecter Table 1 */
    trans_sec_nonsec_trcall, /* Root Call Vecter Table 2 */
    call_ssc_trcall,         /* Root Call Vecter Table 3 */
    return_ssc_trcall,       /* Root Call Vecter Table 4 */
    intrpt_rtn_sec           /* Root Call Vecter Table 5 */
};

/* WINTB Table */
const exc_ptr_t wintb_table[] BSP_ALIGN_VARIABLE(4) =
{
    wint_prg_root,           /* World transition interrupt vector table0 */
    wint_prg_sec,            /* World transition interrupt vector table1 */
    wint_prg_ns              /* World transition interrupt vector table2 */
};
