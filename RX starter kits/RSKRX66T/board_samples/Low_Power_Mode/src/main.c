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
* Copyright (C) 2016, 2017 Renesas Electronics Corporation. All rights reserved.
***********************************************************************************************************************/
/***********************************************************************************************************************
* File Name    : main.c
* Version      : 1.00
* Device(s)    : R5F566TEAxFP
* Tool-Chain   : CCRX
* H/W Platform : RSKRX66T
* Description  : This file implements main function.
***********************************************************************************************************************/
/***********************************************************************************************************************
* History : DD.MM.YYYY Version  Description
*         : 30.06.2018 1.00     First Release
***********************************************************************************************************************/

/***********************************************************************************************************************
Includes   <System Includes> , "Project Includes"
***********************************************************************************************************************/
#include "r_smc_entry.h"
#include "r_okaya_lcd.h"

#include "Config_ICU.h"
#include "Config_CMT0.h"
#include "Config_LPC.h"

#include "r_cg_userdefine.h"

/***********************************************************************************************************************
Macro definitions
***********************************************************************************************************************/

/***********************************************************************************************************************
Typedef definitions
***********************************************************************************************************************/
/* Operation mode definition */
typedef enum
{
    MODE_NORMAL1,
    MODE_SLEEP,
    MODE_NORMAL2,
    MODE_SW_STANDBY,
    MODE_NORMAL3,
    MODE_DEEP_SW_STANDBY
} e_op_mode_t;

/***********************************************************************************************************************
Exported global variables (to be accessed by other files)
***********************************************************************************************************************/

/***********************************************************************************************************************
Private global variables and functions
***********************************************************************************************************************/

/* Local functions set the various low power modes */
static void normal_mode (void);
static void sleep_mode (void);
static void software_standby_mode (void);
static void deep_software_standby_mode (void);

void main(void);

/***********************************************************************************************************************
* Function Name: main
* Description  : This function implements main function.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void main(void)
{
    /* Initialise the debug LCD */
    R_LCD_Init();

    /* Displays the application name on the debug LCD */
    R_LCD_Display(0, (uint8_t *)" RSKR66T ");
    R_LCD_Display(1, (uint8_t *)" Low_Power_Mode ");

    /* start the interrupt for SW1 */
    R_Config_ICU_IRQ0_Start();

    while (1U)
    {
        /* Operating mode variable */
        static volatile e_op_mode_t op_mode = MODE_NORMAL1;

        switch (op_mode)
        {
            case MODE_NORMAL1:
            case MODE_NORMAL2:
            case MODE_NORMAL3:
            {
                normal_mode();
                op_mode++;
            }
            break;

            case MODE_SLEEP:
            {
                sleep_mode();
                op_mode++;
            }
            break;

            case MODE_SW_STANDBY:
            {
                software_standby_mode();
                op_mode++;
            }
            break;

            case MODE_DEEP_SW_STANDBY:
            {
                deep_software_standby_mode();
            }
            break;

            default:
            {
                /* Do nothing */
            }
            break;
        }
    }
}
/***********************************************************************************************************************
* End of function main
***********************************************************************************************************************/

/***********************************************************************************************************************
* Function Name : normal_mode
* Description   : Displays the current mode on the LCD with instructions on how
*                 to exit
* Argument      : none
* Return value  : none
***********************************************************************************************************************/
static void normal_mode (void)
{
    R_LCD_Display(3, (uint8_t *)" Normal Mode         ");
    R_LCD_Display(4, (uint8_t *)" Press SW1       ");

    /* clear the switch flag */
    g_switch_flag = 0;

    /* Wait for switch SW1 to be pressed */
    while (1 != g_switch_flag)
    {
        /* Do Nothing */
    }

    /* Debounce timer delay */
    R_CMT_MsDelay(500);
}
/***********************************************************************************************************************
* End of function normal_mode
***********************************************************************************************************************/

/***********************************************************************************************************************
* Function Name : sleep_mode
* Description   : Displays the current mode on the LCD with instructions on how
*                 to exit, then eneters sleep mode using the LPC API
* Argument      : none
* Return value  : none
***********************************************************************************************************************/
static void sleep_mode (void)
{
    R_LCD_Display(3, (uint8_t *)" Sleep Mode     ");
    R_LCD_Display(4, (uint8_t *)" SW1 to exit");

    R_Config_LPC_Sleep();

    /* Debounce timer delay */
    R_CMT_MsDelay(500);
}
/***********************************************************************************************************************
* End of function sleep_mode
***********************************************************************************************************************/

/***********************************************************************************************************************
* Function Name : software_standby_mode
* Description   : Displays the current mode on the LCD with instructions on how
*                 to exit, then eneters sleep mode using the LPC API
* Argument      : none
* Return value  : none
***********************************************************************************************************************/
static void software_standby_mode (void)
{
    R_LCD_Display(3, (uint8_t *)" SW Standby Mode");
    R_LCD_Display(4, (uint8_t *)" SW1 to exit");

    R_Config_LPC_SoftwareStandby();

    /* Debounce timer delay */
    R_CMT_MsDelay(500);
}
/***********************************************************************************************************************
* End of function software_standby_mode
***********************************************************************************************************************/

/***********************************************************************************************************************
* Function Name : deep_software_standby_mode
* Description   : Displays the current mode on the LCD with instructions on how
*                 to exit, then enters sleep mode using the LPC API
* Argument      : none
* Return value  : none
***********************************************************************************************************************/
static void deep_software_standby_mode (void)
{
    R_LCD_Display(3, (uint8_t *)" Deep SW Standby Mode");
    R_LCD_Display(4, (uint8_t *)" RESET to exit");

    R_Config_LPC_DeepSoftwareStandby();

}
/***********************************************************************************************************************
* End of function deep_software_standby_mode
***********************************************************************************************************************/

