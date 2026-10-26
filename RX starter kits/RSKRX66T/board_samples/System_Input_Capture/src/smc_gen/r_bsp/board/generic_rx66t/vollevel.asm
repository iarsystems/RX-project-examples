;;/*********************************************************************************************************************
;;* DISCLAIMER
;;* This software is supplied by Renesas Electronics Corporation and is only intended for use with Renesas products.
;;* No other uses are authorized. This software is owned by Renesas Electronics Corporation and is protected under all
;;* applicable laws, including copyright laws.
;;* THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIESREGARDING THIS SOFTWARE, WHETHER EXPRESS, IMPLIED
;;* OR STATUTORY, INCLUDING BUT NOT LIMITED TO WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
;;* NON-INFRINGEMENT.  ALL SUCH WARRANTIES ARE EXPRESSLY DISCLAIMED.TO THE MAXIMUM EXTENT PERMITTED NOT PROHIBITED BY
;;* LAW, NEITHER RENESAS ELECTRONICS CORPORATION NOR ANY OF ITS AFFILIATED COMPANIES SHALL BE LIABLE FOR ANY DIRECT,
;;* INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES FOR ANY REASON RELATED TO THIS SOFTWARE, EVEN IF RENESAS OR
;;* ITS AFFILIATES HAVE BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
;;* Renesas reserves the right, without notice, to make changes to this software and to discontinue the availability
;;* of this software. By using this software, you agree to the additional terms and conditions found by accessing the
;;* following link:
;;* http://www.renesas.com/disclaimer
;;*
;;* Copyright (C) 2013, 2018 Renesas Electronics Corporation. All rights reserved.
;;*********************************************************************************************************************/
;;/*
;;**********************************************************************************************************************
;;* File Name    : vollevel.asm
;;* Version      : 1.0B (2018-06-05)
;;* Device(s)    : RX66T
;;* Tool-Chain   : ICCRX
;;* Description  : This is voltage level setting file.
;;**********************************************************************************************************************
;;*/

;;vollevel.asm
    SECTION .text:CODE
    PUBLIC _R_BSP_VoltageLevelSetting

_R_BSP_VoltageLevelSetting:
    PUSHM R4-R5
    PUSHM R14-R15
#if (BSP_CFG_PARAM_CHECKING_ENABLE == 1)
    MOV.L R1, R14            ;arguments set to R14
    AND #03H, R14
    CMP #03H, R14            ;Check whether the setting of USBVON bit conflicts
    BEQ LABEL_RT_FALSE

    MOV.L R1, R15            ;arguments set to R15
    AND #0CH, R15
    CMP #0CH, R15            ;Check whether the setting of PGAVLS bit conflicts
    BEQ LABEL_RT_FALSE

    MOV.L R1, R5             ;arguments set to R5
    AND #30H, R5
    CMP #30H, R5             ;Check whether the setting of RICVLS bit conflicts
    BEQ LABEL_RT_FALSE
#endif /* BSP_CFG_PARAM_CHECKING_ENABLE == 1 */

LABEL_CHECK_USB_MODULE_STOP:
    CMP #00H, R14            ;Check whether the setting of USBVON bit inputs
    BEQ LABEL_CHECK_AD_MODULE_STOP

    MOV.L #00080014H, R14    ;MSTPCRB address set to R14
    MOV.L [R14], R4          ;MSTPCRB set to R4
    BTST #13H, R4            ;Check MSTPB19 bit(USB0 module stop setting bit)
    BEQ LABEL_RT_FALSE

LABEL_CHECK_AD_MODULE_STOP:
    CMP #00H, R15            ;Check whether the setting of PGAVLS bit inputs
    BEQ LABEL_CHECK_RIIC_MODULE_STOP

    MOV.L #00080010H, R15    ;MSTPCRA address set to R15
    MOV.L [R15], R4          ;MSTPCRA set to R4
    BTST #11H, R4            ;Check MSTPB17 bit(S12AD module stop setting bit)
    BEQ LABEL_RT_FALSE
    MOV.L [R15], R15         ;MSTPCRA set to R15
    BTST #10H, R15           ;Check MSTPB16 bit(S12AD1 module stop setting bit)
    BEQ LABEL_RT_FALSE

LABEL_CHECK_RIIC_MODULE_STOP:
    CMP #00H, R5             ;Check whether the setting of RICVLS bit inputs
    BEQ LABEL_UPDATE_VOLSR

    MOV.L #00080014H, R14    ;MSTPCRB address set to R14
    MOV.L [R14], R14         ;MSTPCRB set to R14
    BTST #15H, R14           ;Check MSTPB21 bit(RIIC0 module stop setting bit)
    BEQ LABEL_RT_FALSE

LABEL_UPDATE_VOLSR:
    MOV.L #000803FEH, R14    ;PRCR address set to R14
    MOV.L #0008C295H, R15    ;VOLSR address set to R15
    MOV.W #0A502H, [R14]     ;Protect off

LABEL_RICVLS_TO_1:
    BTST #05H, R1
    BEQ LABEL_RICVLS_TO_0
    BSET #07H, [R15].B       ;RICVLS bit set to 1

LABEL_RICVLS_TO_0:
    BTST #04H, R1
    BEQ LABEL_PGAVLS_TO_1
    BCLR #07H, [R15].B       ;RICVLS bit set to 0

LABEL_PGAVLS_TO_1:
    BTST #03H, R1
    BEQ LABEL_PGAVLS_TO_0
    BSET #06H, [R15].B       ;PGAVLS bit set to 1

LABEL_PGAVLS_TO_0:
    BTST #02H, R1
    BEQ LABEL_USBVON_TO_1
    BCLR #06H, [R15].B       ;PGAVLS bit set to 0

LABEL_USBVON_TO_1:
    BTST #01H, R1
    BEQ LABEL_USBVON_TO_0
    BSET #02H, [R15].B       ;USBVON bit set to 1

LABEL_USBVON_TO_0:
    BTST #00H, R1
    BEQ LABEL_RT_TRUE
    BCLR #02H, [R15].B       ;USBVON bit set to 0

LABEL_RT_TRUE:
    MOV.W #0A500H, [R14]     ;Protect on
    MOV.L #00000001H, R1     ;true set to R1
    POPM R14-R15
    POPM R4-R5
    RTS

LABEL_RT_FALSE:
    MOV.L #00000000H, R1     ;false set to R1
    POPM R14-R15
    POPM R4-R5

    END
