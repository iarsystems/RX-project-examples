/* Adapted to IAR Embedded Workbench */
/***********************************************************************************************************************
* DISCLAIMER
* This software is supplied by Renesas Electronics Corporation and is only intended for use with Renesas products.
* No other uses are authorized. This software is owned by Renesas Electronics Corporation and is protected under all
* applicable laws, including copyright laws. 
* THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES REGARDING THIS SOFTWARE, WHETHER EXPRESS, IMPLIED
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
* Copyright (C) 2018 Renesas Electronics Corporation. All rights reserved.
***********************************************************************************************************************/

/***********************************************************************************************************************
* File Name    : main.c
* Version      : 1.00
* Device(s)    : R5F572TKCxFB
* Tool-Chain   : CCRX
* H/W Platform : RSKRX72T
* Description  : This file implements main function.
***********************************************************************************************************************/
/***********************************************************************************************************************
* History : DD.MM.YYYY Version  Description
*         : 30.11.2018 1.00     First Release
***********************************************************************************************************************/

/***********************************************************************************************************************
Includes   <System Includes> , "Project Includes"
***********************************************************************************************************************/
#include <string.h>
#include "r_smc_entry.h"
#include "r_cg_macrodriver.h"
#include "Config_ICU.h"
#include "Config_PORT.h"
#include "Config_TMR0_TMR1.h"
#include "Config_IWDT.h"
#include "Config_SCI11.h"
#include "Config_CRC.h"
#include "crc.h"
#include "srec.h"
#include "hex.h"
#include "r_rsk_utility.h"
#include "buffer.h"
#include "code_flash.h"
#include "xmodem.h"
#include "rskrx72tdef.h"

#include "r_cg_userdefine.h"

/***********************************************************************************************************************
Macro definitions
***********************************************************************************************************************/
#define IMMEDIATE_JUMP (0)  /* Used to determine behaviour of user application jump */
#define SW_WAIT_JUMP   (1)  /* Used to determine behaviour of user application jump */
#define SIZE_VECT_TABLE (256) /* vector table size */

/***********************************************************************************************************************
Typedef definitions
***********************************************************************************************************************/

/***********************************************************************************************************************
Exported global variables (to be accessed by other files)
***********************************************************************************************************************/

/***********************************************************************************************************************
Private global variables and functions
***********************************************************************************************************************/


static void r_main_userinit(void);
static void jump_to_application (uint8_t jumptype);
static uint8_t poll_switch_press (uint8_t portValue);
static void report_reset_reason (void);
static uint8_t update_from_data_source (void);
static void exit_reset (void);
static void reset_watchdog (void);
static void prep_for_user_jmp (void);

static uint8_t gs_switch_pressed = UNDECIDED;
static uint8_t gs_appvalid = FALSE;
static uint8_t gs_file_format = FILE_FORMAT_UNKNOWN;

/* This set of commands clears the screen on a terminal */
static const uint8_t gs_cmd_clr_scr[] =
{ 27, 91, 50, 74, 27, 91, 72, 0 };
static char gs_str[5];

static volatile uint32_t gs_ram_vector_table[SIZE_VECT_TABLE];      /* RAM space to hold the vector table */
static volatile void flash_copy_vector_table(void);

uint8_t g_user_appvects_valid = FALSE;

/* Function pointers for decoding the data file being received */
uint8_t (*import_file_ptr) (const uint8_t);
uint8_t (*is_error_ptr) (void);
char * (*get_error_text_ptr) (void);
uint8_t (*handle_record_line_ptr) (void);
char * (*get_data_Ptr) (void);
uint32_t (*get_address_ptr) (void);
uint32_t (*get_start_address_ptr) (void);
uint8_t (*get_num_data_bytes_ptr) (void);

/* Function pointer for sending a message to the terminal device */
void (*R_SCI11_SerialTxString_Ptr) (const char *);

/* Used for storing and calling the user's application initial function */
void (*start_address) (void) = NULL;



/***********************************************************************************************************************
* Function Name: main
* Description  : This function implements main function.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void main(void)
{
    r_main_userinit();

    /* Send character combination to clear screen */
    R_SCI11_SerialTxString((const char *) gs_cmd_clr_scr);
    R_SCI11_SerialTxString("\r\nSystem BootLoader\r\n");
    R_SCI11_SerialTxString("\r\nReset reason...");
    report_reset_reason();

    /* Turn on LED0 to Indicate boot loader has started */
    LED0 = LED_ON;

    /* Turn off LED1 */
    LED1 = LED_OFF;

    /* De-bounce SW1 to see if button pressed */
    while (UNDECIDED == gs_switch_pressed)
    {
        /* Casting to correctly use MCU registers(SW1) */
        gs_switch_pressed = poll_switch_press((uint8_t) SW1);
        reset_watchdog();
    }

    /* If no switch has been pressed then Application area should be validated
     * by calculating the CRC checksum for the area and comparing to a value
     * stored in a special location in flash, which was programmed when the
     * application code was last programmed.
     * If the 2 codes match then jump to the application otherwise the application
     * is invalid, so begin the update process
     */
    if (FALSE == gs_switch_pressed)
    {
        uint16_t chksm;

        /* Inform that SW1 was not held down at start */
        R_SCI11_SerialTxString("\r\nDevice Firmware Update : SW1 not pressed.");

        /* Calculate CRC and compare to value stored in App area to see if App area is valid */
        chksm = calc_app_crc_value();
        gs_appvalid = is_application_crc_ok(chksm);

        /* The switch SW1 is not being held down, so user is not requesting update */
        /* So now the application code area needs to be verified to see that it is valid */
        R_SCI11_SerialTxString("\r\nDevice Firmware Update : Verifying Application area with CRC.");

        if (gs_appvalid)
        {

            /* Create function pointer to start of application (first entry of the
             * User's Fixed Vector Table) */
            void (*start_address)(void) = (void (*)(void)) *((uint32_t *) APP_VECTOR_START);

            /* Verify that the start address is located in the Application area */
            if ((uint32_t) start_address > (uint32_t) APP_VECTOR_END)
            {

                /* Address out of bounds */
                R_SCI11_SerialTxString("\r\nInvalid Start Address");

                /* LED0 being off indicates update failure.
                   Turn on LED0 */
                LED0 = LED_OFF;

                /* Turn off LED1 */
                LED1 = LED_OFF;
                gs_appvalid = FALSE;
            }
        }

        /* Inform checksum results */
        R_SCI11_SerialTxString("\r\nDevice Firmware : Calculated Checksum Value : 0x");

        /* Convert calculated 16 bit calculated CRC number to string */
        uint16_to_string(gs_str, (uint8_t) 0, (uint16_t) chksm);
        R_SCI11_SerialTxString(gs_str);

        R_SCI11_SerialTxString("\r\nDevice Firmware : Read Checksum Value       : 0x");

        /* Convert calculated 16 bit read CRC number to string */
        uint16_to_string(gs_str, 0, read_app_crc_value());
        R_SCI11_SerialTxString(gs_str);

        /* Inform whether Application is valid or not */
        R_SCI11_SerialTxString("\r\nDevice Firmware : ");
        if (!gs_appvalid)
        {
            R_SCI11_SerialTxString("Application Flash not valid");
        }
        else
        {
            R_SCI11_SerialTxString("Application Flash valid");
        }
    }
    else
    {

        /* Inform that SW1 was held down at start */
        R_SCI11_SerialTxString("\r\nDevice Firmware : SW1 pressed.");
    }

    /* If SW1 not being pressed and application CRC is verified, then jump to application code */
    if ((!gs_switch_pressed) && gs_appvalid)
    {

        /* Jump to Application */
        jump_to_application(IMMEDIATE_JUMP);
    }

    /* Copy vector table to RAM if interrupts possible while erasing/writing ROM */
    flash_copy_vector_table();
    
    /* User requested application code update or application area is invalid */
    /* so get update from UART */
    while (1U)
    {
        uint16_t checksum;
        uint8_t result;

        /* Reset watchdog timer */
        reset_watchdog();

        /* Wipe application area */
        R_SCI11_SerialTxString("\r\nErasing Application Area...");
        result = UNDECIDED;
        while (UNDECIDED == result)
        {

            /* Erase the application space */
            result = erase_application_code();

            /* Blink LED0 to show update in progress */
            blink_led0();

            /* Reset watch dog timer */
            reset_watchdog();
        }

        /* Handle erase failure */
        if (FALSE == result)
        {
            R_SCI11_SerialTxString("\r\nError erasing Application space");

            /* LEDs being off indicates update failure.
               Turn off LED0 */
            LED0 = LED_OFF;

            /* Turn off LED0 */
            LED1 = LED_OFF;
            exit_reset();
        }

        R_SCI11_SerialTxString("\r\nDone.");
        R_SCI11_SerialTxString("\r\nStarting update from UART.");

        /* Handle the application code update process */
        result = UNDECIDED;
        while (UNDECIDED == result)
        {
            /* Reset watchdog timer */
            reset_watchdog();

            /* Handle UART update process */
            result = update_from_data_source();

            /* Blink LED0 to show update in progress */
            blink_led0();
        }

        /* Update failed. Wait for user to press reset button to retry */
        if (TRUE != result)
        {
            xmodem_cancel(); /* Abort any transfer in progress */

            R_SCI11_SerialTxString("\r\nError re-programming Application space");

            /* LED0 being off indicates update failure */
            LED0 = LED_OFF;
            exit_reset();
        }

        /* LEDs 0 and 1 being on indicates update success.
           Turn on LED0 */
        LED0 = LED_ON;

        /* Turn on LED1 */
        LED1 = LED_ON;

        /* Flush the X-Modem protocol through. As the end of the S-Record has been reached
         * there should be no further packets, just the EOT condition to handle
         */
        xmodem_frame_rx();

        /* Program any final bytes outstanding in to the flash */
        flash_flush_buffer();

        /* Flash program complete - read CRC and program to location in application area */
        R_SCI11_SerialTxString("\r\nApplication Programming Successful.");

        /* Calculate the CRC value for the memory block and write it to the Application CRC location */
        checksum = calc_app_crc_value();

        /* Write CRC value to Flash */
        write_app_crc_value(checksum);

        /* Display CRC value */
        R_SCI11_SerialTxString("\r\nStoring CRC value: 0x");
        memset(gs_str, '\0', 5);
        uint16_to_string(gs_str, 0, checksum);
        R_SCI11_SerialTxString(gs_str);
        R_SCI11_SerialTxString(" - done.");

        /* CRC value updated, now jump to application         */
        jump_to_application(SW_WAIT_JUMP);
    }
}

/***********************************************************************************************************************
* End of function main
***********************************************************************************************************************/

/***********************************************************************************************************************
* Function Name: r_main_userinit
* Description  : This function adds user code before implementing main function.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
static void r_main_userinit(void)
{
    R_Config_SCI11_Start(); /* Start the serial port */
    R_SCI11_SerialRxInit(); /* Setup the serial port for single character receive */

    import_file_ptr = NULL;

    /* Set R_SCI11_SerialTxString function pointer to point to UART1 send message function */
    R_SCI11_SerialTxString_Ptr = R_SCI11_SerialTxString;

    /* Initialise receive buffer */
    init_buffer();

    /* Initialise Flash API */
    flash_write_init();

    /* Turn off LED0 */
    LED0 = LED_OFF;

    /* Turn off LED1 */
    LED1 = LED_OFF;

    /* Turn off LED2 */
    LED2 = LED_OFF;

    /* Turn off LED3 */
    LED3 = LED_OFF;

    R_Config_CRC_SetCRC16(); /* Setup the CRC block */
}

/***********************************************************************************************************************
* End of function r_main_userinit
***********************************************************************************************************************/

/***********************************************************************************************************************
* Function Name: jump_to_application
* Description  : This function jumps to the application code entry point
* Arguments    : uint8_t jumptype
*              :             IMMEDIATE_JUMP - Jumps directly in to the user application.
*              :             SW_WAIT_JUMP   - Requests the user to press SW2 before jumping.
* Return Value : None
***********************************************************************************************************************/
static void jump_to_application (uint8_t jumptype)
{
    /* Update LEDs to indicate application is about to be run.
       Turn on LED0 */
    LED0 = LED_ON;

    /* turn off LED1, LED2 and LED3 */
    LED1 = (LED2 = (LED3 = LED_OFF));

    gs_switch_pressed = UNDECIDED;

    /* If this function has been called with SW_WAIT_JUMP, wait for
       SW2 to be pressed before launching user application */
    if (SW_WAIT_JUMP == jumptype)
    {
        /* Ask user to press button before jumping to application */
        R_SCI11_SerialTxString("\r\nPress SW2 to jump to Application...");

        /* Wait for SW2 button to be pressed */
        while ( TRUE != gs_switch_pressed )
        {

            /* Test switch1, with de-bounce included */
            gs_switch_pressed = poll_switch_press(SW2);

            /* Reset watch dog timer */
            reset_watchdog();
        }
    }

    R_SCI11_SerialTxString("\r\nJumping to Application.");

    /* Reset watch dog timer */
    reset_watchdog();

    /* Turn off LED0 */
    LED0 = LED_OFF;

    /* Turn off LED1 */
    LED1 = LED_OFF;

    /* Set flag to indicate the Fixed Exception vectors should
     * jump to user's application fixed vectors.
     */
    g_user_appvects_valid = TRUE;

    /* Fetch the start address from the vector table */
    void (*start_address)(void) = (void (*)(void)) *((uint32_t *) APP_VECTOR_START);

    prep_for_user_jmp();

    set_psw(0);

    /* Jump to the application */
    start_address();

    while (1)
    {
        /* Should never get there, unless user application returns so enter endless loop */
    }
}

/***********************************************************************************************************************
* End of function jump_to_application
***********************************************************************************************************************/

/***********************************************************************************************************************
* Function Name: update_from_data_source
* Description  : This function controls the application code update process
* Arguments    : None
* Return Value : uint8_t UNDECIDED - in progress
*                        TRUE      - completed successfully
*                        FALSE     - error
***********************************************************************************************************************/
static uint8_t update_from_data_source (void)
{
    static uint8_t step = 0;
    static uint8_t first_data_line = TRUE;
    static uint16_t lineNumber = 0;
    uint8_t byte;

    /* Update via serial process is controlled with this switch statement */
    /* Important that each case statement does not block or watchdog reset could occur */
    switch (step)
    {
        case 0 :
            {
                /* Send message via UART to inform ready to receive data */
                R_SCI11_SerialTxString("\r\nPlease start file send using XModem protocol....\r\n");

                lineNumber = 0;
                step = 1;
            }
            break;

        case 1 :
            {
                /* Handle ongoing data reception */
                if (poll_buffer_for_new_data())
                {

                    /* Byte available - read from UART receive register */
                    byte = get_buffer_char();
                    next_buffer_char();

                    /* Now awaiting first byte from srec or hex format file containing code. */
                    /* The first byte in an srec format file is an 'S'. */
                    /* The first byte in a hex format file is an ':'. */
                    /* Other inputs are wrong */
                    if (TRUE == is_file_srec(byte))
                    {

                        /* An srec file is being received */
                        /* Set function pointers to the srec import functions */
                        import_file_ptr = import_srec_file;
                        get_error_text_ptr = get_srec_error_text;
                        is_error_ptr = is_srec_file_error;
                        handle_record_line_ptr = get_srec_type;
                        get_data_Ptr = get_srec_data;
                        get_address_ptr = get_srec_address;
                        get_start_address_ptr = get_srec_start_address;
                        get_num_data_bytes_ptr = get_srec_num_data_bytes;

                        /* Show that an SREC file is being received */
                        gs_file_format = FILE_FORMAT_SREC;

                        /* Process first byte */
                        import_file_ptr(byte);
                        step = 2;
                    }
                    else if (TRUE == is_file_hex(byte))
                    {

                        /* A hex file is being received */
                        /* Set function pointers to the hex import functions */
                        import_file_ptr = import_hex_file;
                        get_error_text_ptr = get_hex_error_text;
                        is_error_ptr = is_hex_file_error;
                        handle_record_line_ptr = get_hex_type;
                        get_data_Ptr = get_hex_data;
                        get_address_ptr = get_hex_address;
                        get_start_address_ptr = get_hex_start_address;
                        get_num_data_bytes_ptr = get_hex_num_data_bytes;

                        /* Show that a HEX file is being received */
                        gs_file_format = FILE_FORMAT_HEX;

                        /* Process first byte */
                        import_file_ptr(byte);
                        step = 2;
                    }
                    else
                    {

                        /* If the first byte of the file is not a 'S' or a ':' then it is invalid */
                        /* also a single newline control character is acceptable (but not more) */
                        if ('\n' != byte)
                        {

                            /* Indicate error in received file */
                            xmodem_cancel(); /* Cancel the XModem transfer in progress */
                            R_SCI11_SerialTxString("\r\n\r\nError : File not SREC or HEX format.");
                            step = 0;
                            first_data_line = TRUE;
                            return (FALSE);
                        }
                    }
                }
            }
            break;

        /* An S-Record or hex file is being received */
        case 2 :
            {
                /* Handle ongoing data reception */
                /* Poll_buffer_for_new_data returns TRUE if a new character received on the buffer */
                if (TRUE == poll_buffer_for_new_data())
                {

                    /* Get next byte from the buffer */
                    byte = get_buffer_char();

                    /* Update the buffer to point to the next available byte in the future */
                    next_buffer_char();

                    /* Paranoid check to ensure pointer is set */
                    if (NULL != import_file_ptr)
                    {

                        /* Process the received byte as part of the import of the Srecord or hex file */
                        const uint8_t result = import_file_ptr(byte);

                        /* Check if line is complete */
                        if (TRUE == result)
                        {

                            /* Handle error */
                            if (is_error_ptr())
                            {

                                /* Inform of error and describe reason */
                                xmodem_cancel(); /* Cancel the XModem transfer in progress */
                                R_SCI11_SerialTxString("\r\n\r\nFile error: ");
                                R_SCI11_SerialTxString(get_error_text_ptr());
                                step = 0;
                                return (FALSE);
                            }

                            /* Handle the complete line, add to flash program buffer */
                            else
                            {

                                /* Valid line of record received. Now decide what action to take */
                                uint8_t line_result;
                                line_result = handle_record_line_ptr();

                                /* Handle end of import */
                                if (RECORD_FINISHED == line_result)
                                {

                                    /* End of file received                  */
                                    /* Make sure that any data in the buffer is flushed to the code flash */
                                    const uint8_t res = flash_flush_buffer();

                                    /* Handle an error */
                                    if (TRUE != res)
                                    {
                                        xmodem_cancel(); /* Cancel the XModem transfer in progress */
                                        R_SCI11_SerialTxString("\r\n\r\nFlash Write Error: ");
                                        step = 0;
                                        return (FALSE);
                                    }

                                    /* Report success of import */
                                    R_SCI11_SerialTxString("\r\nEnd of File.");

                                    /* Get start address of data */
                                    void (*start_address)(void) = (void (*)(void)) get_start_address_ptr();;

                                    /* Reset processing variables */
                                    step = 0;
                                    first_data_line = TRUE;
                                    return (TRUE);
                                }

                                /* Header received - we now know the file format */
                                else if (RECORD_HEADER == line_result)
                                {

                                    /* Information stub: Srec file detected here  */
                                    if (FILE_FORMAT_SREC == gs_file_format)
                                    {
                                        /* Perform any special actions here for SREC file format */
                                    }

                                    /* Information stub: Hex file detected here  */
                                    if (FILE_FORMAT_HEX == gs_file_format)
                                    {
                                        /* Perform any special actions here for HEX file format */
                                    }

                                    /* Increment line counter */
                                    lineNumber++;
                                }

                                /* Ready to flash a block of code */
                                else if (RECORD_FLASH == line_result)
                                {

                                    /* Next step is to program line to flash */
                                    step = 3;

                                    /* If first line of code to be flashed, then provide address and data headings */
                                    if (first_data_line)
                                    {
                                        /* Insert any debug output here - unable to write to terminal
                                         * at this point, XModem transfer is in progress
                                         */
                                        first_data_line = FALSE;
                                    }
                                }
                                else
                                {

                                    /* Nothing - line_result = RECORD_NEXT */
                                }
                            }
                        }
                        else if (FALSE == result)
                        {
                            /* Report error, type and line number */
                            char lineNumText[6];
                            xmodem_cancel(); /* Cancel the XModem transfer in progress */
                            memset(lineNumText, '\0', 6);
                            R_SCI11_SerialTxString("\r\nFile Reading error:\r\n");
                            R_SCI11_SerialTxString(get_error_text_ptr());
                            R_SCI11_SerialTxString("\r\nAt line: ");

                            /* Convert line Number to bcd string */
                            uint16_to_bcd_string(lineNumText, 0, lineNumber);
                            R_SCI11_SerialTxString(lineNumText);
                            return (FALSE);
                        }
                        else
                        {
                            /* Undefined result */
                        }
                    }
                }
            }
            break;

        case 3 :
            {
                /* Program flash block */

                /* Turn LED0 On to Indicate self-program has started */
                LED1 = LED_ON;

                /* Write line to Flash, or store it in RAM buffer, ready for writing */
                /* Data stored in buffer until block is full */
                byte = flash_block_write(get_address_ptr(), get_data_Ptr(), get_num_data_bytes_ptr());

                /* Check for errors */
                if (TRUE != byte)
                {
                    step = 0;
                    return (FALSE);
                }

                /* Finished line, import next one */
                LED1 = LED_OFF;

                /* Increment lineNumber */
                lineNumber++;
                step = 1;
            }
            break;

        default :
            {
                step = 0;
                first_data_line = TRUE;
            }
            break;
    }

    return (UNDECIDED);
}

/***********************************************************************************************************************
* End of function update_from_data_source
***********************************************************************************************************************/

/***********************************************************************************************************************
* Function Name: report_reset_reason
* Description  : This function returns a string describing the reason for the last reset.
* Arguments    : void
* Return Value : char * pointer to reset reason string.
***********************************************************************************************************************/
static void report_reset_reason (void)
{
    R_SCI11_SerialTxString("\r\n");

    /* The RESF register holds the reset reason.
       Casting to correctly use MCU registers(SYSTEM) */
    if (1 == SYSTEM.RSTSR1.BIT.CWSF)
    {
        R_SCI11_SerialTxString("Warm Reset.\r\n");
    }

    /* Casting to correctly use MCU registers(SYSTEM) */
    if (0 == SYSTEM.RSTSR1.BIT.CWSF)
    {
        R_SCI11_SerialTxString("Cold Reset.\r\n");
    }

    /* Casting to correctly use MCU registers(SYSTEM) */
    if (SYSTEM.RSTSR0.BIT.PORF)
    {
        R_SCI11_SerialTxString("Power-on Reset.\r\n");
    }

    /* Casting to correctly use MCU registers(SYSTEM) */
    if (SYSTEM.RSTSR0.BIT.LVD1RF)
    {
        R_SCI11_SerialTxString("Voltage Mon. 1 Reset.\r\n");
    }

    /* Casting to correctly use MCU registers(SYSTEM) */
    if (SYSTEM.RSTSR0.BIT.LVD2RF)
    {
        R_SCI11_SerialTxString("Voltage Mon. 2 Reset.\r\n");
    }

    /* Casting to correctly use MCU registers(SYSTEM) */
    if (SYSTEM.RSTSR2.BIT.IWDTRF)
    {
        R_SCI11_SerialTxString("Watchdog Reset.\r\n");
    }

    /* Casting to correctly use MCU registers(SYSTEM) */
    if (SYSTEM.RSTSR2.BIT.SWRF)
    {
        R_SCI11_SerialTxString("Software Reset.\r\n");
    }

    R_SCI11_SerialTxString("Power On Reset / reset button.\r\n");
}

/***********************************************************************************************************************
* End of function report_reset_reason
***********************************************************************************************************************/

/***********************************************************************************************************************
* Function Name: exit_reset
* Description  : This function waits forever, until the reset button is pressed
* Arguments    : none
* Return Value : none
***********************************************************************************************************************/
static void exit_reset (void)
{

    /* Turn all LEDs off */
    LED0 = (LED1 = (LED2 = (LED3 = LED_OFF)));

    R_SCI11_SerialTxString("\r\nPress reset button to retry....");

    while (1)
    {

        /* Reset watch dog timer */
        R_Config_IWDT_Restart();
    }
}

/***********************************************************************************************************************
* End of function exit_reset
***********************************************************************************************************************/

/***********************************************************************************************************************
* Function Name: poll_switch_press
* Description  : This function reads the value of a switch on the port portValue. Function does not block whilst
*                debouncing switch.
* Arguments    : uint8_t portvalue - The switch port anded with the GPIO bit number.
* Return Value : TRUE      - Switch pressed
*                FALSE     - Switch not pressed
*                UNDECIDED - still debouncing switch before making a decision
***********************************************************************************************************************/
static uint8_t poll_switch_press (const uint8_t port_value)
{
    static uint32_t delay = 0;
    static uint32_t reading_count = 0;

    /* Sum the SW1 port value into a variable for a defined period of time */
    if ((delay++) < (SWITCH_PRESS_DELAY_COUNTS >> 8))
    {
        if (port_value)
        {
            reading_count++;
        }

        return (UNDECIDED);
    }

    delay = 0;

    /* If the count is greater than half the number of readings then it is mostly high */
    if (reading_count > (SWITCH_PRESS_DELAY_COUNTS >> 9))
    {

        /* A logic high indicates SW1 is not being pressed */
        reading_count = 0;
        return (FALSE);
    }
    reading_count = 0;

    /* A logic low indicates SW1 is being pressed */
    return (TRUE);
}

/***********************************************************************************************************************
* End of function poll_switch_press
***********************************************************************************************************************/

/***********************************************************************************************************************
* Function Name: reset_watchdog
* Description  : This function resets the Watchdog
* Arguments    : none
* Return Value : none
***********************************************************************************************************************/
static void reset_watchdog (void)
{
    R_Config_IWDT_Restart();
}

/***********************************************************************************************************************
* End of function reset_watchdog
***********************************************************************************************************************/

/***********************************************************************************************************************
* Function Name: prep_for_user_jmp
* Description  : Perform actions required to leave the micro controller in a state
*                for the user code to start running.
* Arguments    : none
* Return Value : none
***********************************************************************************************************************/
static void prep_for_user_jmp (void)
{
    uint32_t sckcr_dummy;

    /* Turn off LED0 */
    LED0 = LED_OFF;

    /* Turn off LED1 */
    LED1 = LED_OFF;

    /* Turn off LED2 */
    LED2 = LED_OFF;

    /* Turn off LED3 */
    LED3 = LED_OFF;

    /* Stop all peripherals */
    R_Config_TMR0_TMR1_Stop();
    R_Config_SCI11_Stop();

    /* Enable writing to registers related to operating modes, LPC, CGC and software reset */
    SYSTEM.PRCR.WORD = 0xA50BU; 

    /* Set system clock to reset value */
    sckcr_dummy = 0x33833333U;

    /* Casting to correctly use MCU registers(SYSTEM). */
    SYSTEM.SCKCR.LONG = sckcr_dummy;

    /* Casting to correctly use MCU registers(SYSTEM). */
    while (SYSTEM.SCKCR.LONG != sckcr_dummy)
    {
        ;
    }

    /* Set clock source */
    SYSTEM.SCKCR3.WORD = 0x0200;

    /* Enable protection */
    SYSTEM.PRCR.WORD = 0xA500U;
}

/***********************************************************************************************************************
* End of function prep_for_user_jmp
***********************************************************************************************************************/

/***********************************************************************************************************************
* Function Name: flash_copy_vector_table
* Description  : Moves the relocatable vector table to RAM. This is only needed if ROM operations are performed.
*                ROM cannot be accessed during operations. The vector table is located in ROM and will be accessed
*                if an interrupt occurs.
* Arguments    : none
* Return Value : none
***********************************************************************************************************************/

#pragma section = ".inttable"

static volatile void flash_copy_vector_table(void)
{
    uint32_t *pvect_table;
    uint16_t i;

    /* Casting to ensure use of correct data type. */
    pvect_table = (uint32_t *) __section_begin(".inttable");
    for(i=0; i<SIZE_VECT_TABLE; i++)
    {
        /* Casting to ensure use of correct data type. */
        gs_ram_vector_table[i] = (uint32_t)pvect_table[i];
    }

    /* Casting to ensure use of correct data type. */
    set_intb((unsigned long)gs_ram_vector_table);
}

/***********************************************************************************************************************
* End of function flash_copy_vector_table
***********************************************************************************************************************/

