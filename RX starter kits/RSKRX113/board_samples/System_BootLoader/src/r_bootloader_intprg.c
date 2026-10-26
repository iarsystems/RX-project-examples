/***********************************************************************************************************************
 * DISCLAIMER
 * This software is supplied by Renesas Electronics Corporation and is only intended for use with Renesas products.
 * No other uses are authorized. This software is owned by Renesas Electronics Corporation and is protected under all
 * applicable laws, including copyright laws.
 * THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIESREGARDING THIS SOFTWARE, WHETHER EXPRESS, IMPLIED
 * OR STATUTORY, INCLUDING BUT NOT LIMITED TO WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NON-INFRINGEMENT.  ALL SUCH WARRANTIES ARE EXPRESSLY DISCLAIMED.TO THE MAXIMUM EXTENT PERMITTED NOT PROHIBITED BY
 * LAW, NEITHER RENESAS ELECTRONICS CORPORATION NOR ANY OF ITS AFFILIATED COMPANIES SHALL BE LIABLE FOR ANY DIRECT,
 * INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES FOR ANY REASON RELATED TO THIS SOFTWARE, EVEN IF RENESAS OR
 * ITS AFFILIATES HAVE BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
 * Renesas reserves the right, without notice, to make changes to this software and to discontinue the availability
 * of this software. By using this software, you agree to the additional terms and conditions found by accessing the
 * following link:
 * http://www.renesas.com/disclaimer
 *
 * Copyright (C) 2014 Renesas Electronics Corporation. All rights reserved.
 ***********************************************************************************************************************/

/***********************************************************************************************************************
 * File Name    : r_bootloader_intprg.c
 * Version      : RX113 V1.00.00.01 [30 May 2014]
 * Device(s)    : R5F51138AxFP
 * Tool-Chain   : IAR Systems ICCRX
 * Description  : This file replaces the excluded 'r_cg_intprog.c' file.
 *                Contained within this file is the fixed vector handling routines which
 *                includes a feature to perform a redirection jump in to the user's 'pseudo-fixed' vector
 *                table.
 *
 *                Note: Please ensure 'r_cg_intprog.c' is *EXCLUDED* from the build, to avoid duplication.
 *
 * Creation Date: 30/10/2014
 ***********************************************************************************************************************/

/***********************************************************************************************************************
 Pragma directive
 ***********************************************************************************************************************/
/* Start user code for pragma. Do not edit comment generated here */

/* End user code. Do not edit comment generated here */

/***********************************************************************************************************************
 Includes
 ***********************************************************************************************************************/
#include <machine.h>
#include "r_cg_macrodriver.h"
//#include "r_cg_vect.h"
#include "r_cg_iwdt.h"
#include "r_cg_userdefine.h"

/***********************************************************************************************************************
 Global variables and functions
 ***********************************************************************************************************************/
/* Defines for the locations of user's vector table netries */
#define USER_RESET_VECTOR 	  ((void*)0xFFFFFF70)
#define USER_NMI_VECTOR 	  ((void*)0xFFFFFF6C)
#define USER_RESERVED1_VECTOR ((void*)0xFFFFFF68)
#define USER_RESERVED2_VECTOR ((void*)0xFFFFFF64)
#define USER_RESERVED3_VECTOR ((void*)0xFFFFFF60)
#define USER_RESERVED4_VECTOR ((void*)0xFFFFFF5C)
#define USER_RESERVED5_VECTOR ((void*)0xFFFFFF58)
#define USER_RESERVED6_VECTOR ((void*)0xFFFFFF54)
#define USER_UNDEFINED_VECTOR ((void*)0xFFFFFF50)
#define USER_RESERVED7_VECTOR ((void*)0xFFFFFF4C)
#define USER_RESERVED8_VECTOR ((void*)0xFFFFFF48)
#define USER_PRIVILEDGED_VECTOR ((void*)0xFFFFFF44)

extern uint8_t g_user_appvects_valid;
void (*jump_address) (void);

/* Located below are the functions used to jump to the user's fixed vector table
 * once g_user_appvects_valid is TRUE, which is set immediately before the jump to
 * user code.
 */
#pragma section = "IntPRG"
#pragma location =  "IntPRG"
/***********************************************************************************************************************
 * Function Name: jump_to_users_vector
 * Description  : Assembly jump routine.
 *
 * This function is invoked as the last step of the bootloader
 * fixed exception service routine.
 *
 * This fetches fetches the address of the user's function from the passed
 * the User's Fixed Exception table.
 *
 * The final instruction is an assembly jump. This is required as the user's
 * function will perform the RTE.
 *
 *
 * Arguments    : None
 * Return Value : None
 ***********************************************************************************************************************/
#pragma inline
void jump_to_users_vector(void *users_vector_address)
{
  asm("MOV.L [R1], R2\n"
      "JMP R2");
}
/***********************************************************************************************************************
 * End of function jump_to_users_vector
 ***********************************************************************************************************************/



/* Undefined exceptions for supervisor instruction, undefined instruction and floating point exceptions */
/***********************************************************************************************************************
 * Function Name: r_undefined_exception
 * Description  : Exception Handler Routine
 *
 * Arguments    : None
 * Return Value : None
 ***********************************************************************************************************************/
//__interrupt void __undefined_hander(void)
//{
//
//    /* Please do not use this function - see the user
//     * generated code and the r_bootloader_vecttbl
//     * files for the modified interrupt operation needed for this boot loader.
//     */
//
//}
/***********************************************************************************************************************
 * End of function r_undefined_exception
 ***********************************************************************************************************************/



/***********************************************************************************************************************
 * Function Name: r_reserved_exception
 * Description  : Exception Handler Routine
 *
 * Arguments    : None
 * Return Value : None
 ***********************************************************************************************************************/

//void r_reserved_exception (void)
//{
//
//    /* Please do not use this function - see the user
//     * generated code and the r_bootloader_vecttbl
//     * files for the modified interrupt operation needed for this boot loader.
//     */
//
//}
/***********************************************************************************************************************
 * End of function r_reserved_exception
 ***********************************************************************************************************************/

#pragma location =  "IntPRG"
/***********************************************************************************************************************
 * Function Name: r_nmi_exception
 * Description  : Exception Handler Routine
 *
 * Arguments    : None
 * Return Value : None
 ***********************************************************************************************************************/
__interrupt void __NMI_handler(void)
{
    if (1U == ICU.NMISR.BIT.IWDTST)
    {
        r_iwdt_nmi_interrupt();
    }
    /* Start user code. Do not edit comment generated here */

    if (FALSE == g_user_appvects_valid)
    {
        /* Place any service routine for the boot loader here */
    }
    else
    {
        /* g_user_appvects_valid is true */
        /* Call to the function in the user's fixed vector interrupt table */
        jump_to_users_vector(USER_NMI_VECTOR);
    }
}
/***********************************************************************************************************************
 * End of function r_nmi_exception
 ***********************************************************************************************************************/

#pragma location =  "IntPRG"
/***********************************************************************************************************************
 * Function Name: r_brk_exception
 * Description  : Exception Handler Routine
 *
 * Arguments    : None
 * Return Value : None
 ***********************************************************************************************************************/
#pragma vector=0
__interrupt void r_brk_exception (void)
{
    /* Start user code. Do not edit comment generated here */
    /* End user code. Do not edit comment generated here */
}
/***********************************************************************************************************************
 * End of function r_brk_exception
 ***********************************************************************************************************************/




/* The functions below are for forwarding on the non-standard exceptions
 * to the user's fixed vector table:
 */
#pragma location =  "IntPRG"
/***********************************************************************************************************************
 * Function Name: ___privileged_handler
 * Description  : Privileged Instruction Exception
 *
 * Arguments    : None
 * Return Value : None
 ***********************************************************************************************************************/
void ___privileged_handler (void)
{
    if (FALSE == g_user_appvects_valid)
    {
        /* Place any service routine for the boot loader here */
    }
    else
    {
        /* g_user_appvects_valid is true */
        /* Call to the function in the user's fixed vector interrupt table */
        jump_to_users_vector(USER_PRIVILEDGED_VECTOR);
    }
}
/***********************************************************************************************************************
 * End of function r_ffffffd0_exception
 ***********************************************************************************************************************/


/***********************************************************************************************************************
 * Function Name: r_ffffffd4_exception
 * Description  : 0xffffffd4 Exception Handler Routine
 *
 * Arguments    : None
 * Return Value : None
 **********************************************************************************************************************/
//reserved vector address
//void r_ffffffd4_exception (void)
//{
//    if (FALSE == g_user_appvects_valid)
//    {
//        /* Place any service routine for the boot loader here */
//    }
//    else
//    {
//        /* g_user_appvects_valid is true */
//        /* Call to the function in the user's fixed vector interrupt table */
//        jump_to_users_vector(USER_RESERVED8_VECTOR);
//    }
//}
/***********************************************************************************************************************
 * End of function r_ffffffd4_exception
 ***********************************************************************************************************************/


/***********************************************************************************************************************
 * Function Name: r_ffffffd8_exception
 * Description  : 0xffffffd8Exception Handler Routine
 *
 * Arguments    : None
 * Return Value : None
 ***********************************************************************************************************************/
//Reserved vector address
//void r_ffffffd8_exception (void)
//{
//    if (FALSE == g_user_appvects_valid)
//    {
//        /* Place any service routine for the boot loader here */
//    }
//    else
//    {
//        /* g_user_appvects_valid is true */
//        /* Call to the function in the user's fixed vector interrupt table */
//        jump_to_users_vector(USER_RESERVED7_VECTOR);
//    }
//}
/***********************************************************************************************************************
 * End of function r_ffffffd8_exception
 ***********************************************************************************************************************/

#pragma location =  "IntPRG"
/***********************************************************************************************************************
 * Function Name: r_ffffffdc_exception
 * Description  : 0xffffffdc Exception Handler Routine (Undefined Instruction
 *
 * Arguments    : None
 * Return Value : None
 ***********************************************************************************************************************/
__interrupt void ___undefined_handler (void)
{
    if (FALSE == g_user_appvects_valid)
    {
        /* Place any service routine for the boot loader here */
    }
    else
    {
        /* g_user_appvects_valid is true */
        /* Call to the function in the user's fixed vector interrupt table */
        jump_to_users_vector(USER_UNDEFINED_VECTOR);
    }
}
/***********************************************************************************************************************
 * End of function r_ffffffdc_exception
 ***********************************************************************************************************************/


/***********************************************************************************************************************
 * Function Name: r_ffffffe0_exception
 * Description  : 0xffffffe0 Exception Handler Routine
 *
 * Arguments    : None
 * Return Value : None
 ***********************************************************************************************************************/
//void r_ffffffe0_exception (void)
//{
//    if (FALSE == g_user_appvects_valid)
//    {
//        /* Place any service routine for the boot loader here */
//    }
//    else
//    {
//        /* g_user_appvects_valid is true */
//        /* Call to the function in the user's fixed vector interrupt table */
//        jump_to_users_vector(USER_RESERVED6_VECTOR);
//    }
//}
/***********************************************************************************************************************
 * End of function r_ffffffe0_exception
 ***********************************************************************************************************************/


/***********************************************************************************************************************
 * Function Name: r_ffffffe4_exception
 * Description  : 0xffffffe4 Exception Handler Routine
 *
 * Arguments    : None
 * Return Value : None
 ***********************************************************************************************************************/
//void r_ffffffe4_exception (void)
//{
//    if (FALSE == g_user_appvects_valid)
//    {
//        /* Place any service routine for the boot loader here */
//    }
//    else
//    {
//        /* g_user_appvects_valid is true */
//        /* Call to the function in the user's fixed vector interrupt table */
//        jump_to_users_vector(USER_RESERVED5_VECTOR);
//    }
//}
/***********************************************************************************************************************
 * End of function r_ffffffe4_exception
 ***********************************************************************************************************************/


/***********************************************************************************************************************
 * Function Name: r_ffffffe8_exception
 * Description  : 0xffffffe8 Exception Handler Routine
 *
 * Arguments    : None
 * Return Value : None
 **********************************************************************************************************************/
//void r_ffffffe8_exception (void)
//{
//    if (FALSE == g_user_appvects_valid)
//    {
//        /* Place any service routine for the boot loader here */
//    }
//    else
//    {
//        /* g_user_appvects_valid is true */
//        /* Call to the function in the user's fixed vector interrupt table */
//        jump_to_users_vector(USER_RESERVED4_VECTOR);
//    }
//}
/***********************************************************************************************************************
 * End of function r_ffffffe8_exception
 ***********************************************************************************************************************/


/***********************************************************************************************************************
 * Function Name: r_ffffffec_exception
 * Description  : 0xffffffec Exception Handler Routine
 *
 * Arguments    : None
 * Return Value : None
 ***********************************************************************************************************************/
//void r_ffffffec_exception (void)
//{
//    if (FALSE == g_user_appvects_valid)
//    {
//        /* Place any service routine for the boot loader here */
//    }
//    else
//    {
//        /* g_user_appvects_valid is true */
//        /* Call to the function in the user's fixed vector interrupt table */
//        jump_to_users_vector(USER_RESERVED3_VECTOR);
//    }
//}
/***********************************************************************************************************************
 * End of function r_ffffffec_exception
 ***********************************************************************************************************************/


/***********************************************************************************************************************
 * Function Name: r_fffffff0_exception
 * Description  : 0xfffffff0 Exception Handler Routine
 *
 * Arguments    : None
 * Return Value : None
 ***********************************************************************************************************************/
//void r_fffffff0_exception (void)
//{
//    if (FALSE == g_user_appvects_valid)
//    {
//        /* Place any service routine for the boot loader here */
//    }
//    else
//    {
//        /* g_user_appvects_valid is true */
//        /* Call to the function in the user's fixed vector interrupt table */
//        jump_to_users_vector(USER_RESERVED2_VECTOR);
//    }
//}
/***********************************************************************************************************************
 * End of function r_fffffff0_exception
 ***********************************************************************************************************************/


/***********************************************************************************************************************
 * Function Name: r_fffffff4_exception
 * Description  : 0xfffffff4 Exception Handler Routine
 *
 * Arguments    : None
 * Return Value : None
 ***********************************************************************************************************************/
//void r_fffffff4_exception (void)
//{
//    if (FALSE == g_user_appvects_valid)
//    {
//        /* Place any service routine for the boot loader here */
//    }
//    else
//    {
//        /* g_user_appvects_valid is true */
//        /* Call to the function in the user's fixed vector interrupt table */
//        jump_to_users_vector(USER_RESERVED1_VECTOR);
//    }
//}
/***********************************************************************************************************************
 * End of function r_fffffff4_exception
 ***********************************************************************************************************************/

/* End user code. Do not edit comment generated here */
