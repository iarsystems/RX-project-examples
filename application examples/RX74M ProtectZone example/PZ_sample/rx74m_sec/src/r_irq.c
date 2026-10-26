/******************************************************************************
#include <sec.h>
#include <main_sec.h>
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
#include "r_irq.h"

/******************************************************************************
Typedef definitions
******************************************************************************/

/******************************************************************************
Macro definitions
******************************************************************************/

/******************************************************************************
Imported global variables and functions (from other files)
******************************************************************************/

/******************************************************************************
Exported global variables and functions (to be accessed by other files)
******************************************************************************/

/******************************************************************************
Private global variables and functions
******************************************************************************/

typedef void (*irq_callback)(void); /* Callback function type */
volatile irq_callback gp_irq_callbackfunc; /* Callback funcktion */

void R_IRQ_Open(void)
{
    /* Disable Interrupt */
//    R_ICU->IER[8].IER_b.IEN0 = 0U;
	R_ICU->IER[17].IER_b.IEN6 = 0U;  //IRQ30の割り込み禁止

    /* IO port setting */
#if defined(_RX_PZ_SECURE)
	R_PMISC->PWPRS_b.B0WI=0;  //Secure用PWPRレジスタへの書き込み許可
	R_PMISC->PWPRS_b.PFSWE=1; //PmnPFSレジスタへの書き込み許可
#else
	R_PMISC->PWPR_b.B0WI=0;   //Non-Secure用PWPRレジスタへの書き込み許可
	R_PMISC->PWPR_b.PFSWE=1;  //PmnPFSレジスタへの書き込み許可
	__asm volatile (
	        "mov.l #07C00D0Ch, r1  \n"
	        "mov.b [r1],r2 \n"
	    );

#endif
	R_PFS->PORT[4].PIN[14].PmnPFS_b.ISEL=1;  //P4_14はIRQ端子
    __asm volatile (
        "mov.l #87C00938h, r1  \n"
        "mov.l [r1],r2 \n"
    );

	/* select interrupt source */
	R_ICU->SLIXR[14].SLIXR = 0x03FF;  //ベクタ142は外部端子割り込み
    __asm volatile (
        "mov.l #8700691Ch, r1  \n"
        "mov.w [r1],r2 \n"
    );

    /* IR141 clear */
    R_ICU->IR[142].IR_b.IR=0;        //IRQ30のIRクリア

    /* Set Interrupt Priority */
//    R_ICU->IPR[64].IPR_b.IPR = 0x0FU;
    R_ICU->IPR[142].IPR_b.IPR = 0x0FU;  //IRQ30のIPRを15
    __asm volatile (
        "mov.l #8700638Eh, r1  \n"
        "mov.b [r1],r2 \n"
    );

    /* Enable Interrupt */
//    R_ICU->IER[8].IER_b.IEN0 = 1U;
    R_ICU->IER[17].IER_b.IEN6 = 1U;    //IRQ30の割り込み有効

    __asm volatile (
        "mov.l #87006211h, r1  \n"
        "mov.b [r1],r2 \n"
    );
}

void R_IRQ_IRQ30_Handler(void)
{
    if(gp_irq_callbackfunc != NULL)
    {
        gp_irq_callbackfunc();
    }
}


void R_IRQ_CallbackSet(void (* p_callback)(void))
{
    /* Store callback */
    gp_irq_callbackfunc = p_callback;
}

void R_IRQ_Close(void)
{
    /* Disable Interrupt */
//    R_ICU->IER[8].IER_b.IEN0 = 0U;
    R_ICU->IER[17].IER_b.IEN6 = 0U;

    /* Set Interrupt Priority */
//    R_ICU->IPR[64].IPR_b.IPR = 0x00U;
    R_ICU->IPR[142].IPR_b.IPR = 0x00U;
}
