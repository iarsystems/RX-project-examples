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
* File Name    : r_cg_main.c
* Version      : Applilet4 for RX113 V1.01.00.02 [31 Jul 2014]
* Device(s)    : R5F51138AxFP
* Tool-Chain   : IAR Systems ICCRX
* Description  : This file implements main function.
* Creation Date: 2015-01-21
***********************************************************************************************************************/

/***********************************************************************************************************************
Pragma directive
***********************************************************************************************************************/
/* Start user code for pragma. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */

/***********************************************************************************************************************
Includes
***********************************************************************************************************************/
#include "r_cg_macrodriver.h"
#include "r_cg_cgc.h"
#include "r_cg_port.h"
#include "r_cg_cmt.h"
#include "r_cg_iwdt.h"
#include "r_cg_sci.h"
#include "r_cg_crc.h"
/* Start user code for include. Do not edit comment generated here */
#include <string.h>
#include "crc.h"
#include "srec.h"
#include "hex.h"
#include "iorx113.h"
#include "r_rsk_utility.h"
#include "buffer.h"
#include "code_flash.h"
#include "xmodem.h"
#include "rskrx113def.h"
/* End user code. Do not edit comment generated here */
#include "r_cg_userdefine.h"

/***********************************************************************************************************************
Global variables and functions
***********************************************************************************************************************/
/* Start user code for global. Do not edit comment generated here */

#define IMMEDIATE_JUMP (0)	/* Used to determine behaviour of user application jump */
#define SW_WAIT_JUMP   (1)  /* Used to determine behaviour of user application jump */

static void jump_to_application (uint8_t jumptype);
static uint8_t poll_switch_press (uint8_t portValue);
static void report_reset_reason (void);
static uint8_t update_from_data_source (void);
static void exit_reset (void);
static void reset_watchdog (void);
static void prep_for_user_jmp (void);

static uint8_t switch_pressed = UNDECIDED;
static uint8_t appvalid = FALSE;
static uint8_t file_format = FILE_FORMAT_UNKNOWN;

/* this set of commands clears the screen on a terminal */
static const uint8_t cmd_clr_scr[] =
{ 27, 91, 50, 74, 0, 27, 91, 72, 0 };
static char str[5];

uint8_t g_user_appvects_valid = FALSE;

/* function pointers for decoding the data file being received */
uint8_t (*import_file_ptr) (const uint8_t);
uint8_t (*is_error_ptr) (void);
char * (*get_error_text_ptr) (void);
uint8_t (*handle_record_line_ptr) (void);
char * (*get_data_Ptr) (void);
uint32_t (*get_address_ptr) (void);
uint32_t (*get_start_address_ptr) (void);
uint8_t (*get_num_data_bytes_ptr) (void);

/* function pointer for sending a message to the terminal device */
void (*R_SCI1_SerialTxString_Ptr) (const char *);

/* Used for storing and calling the user's application initial function */
void (*start_address) (void) = NULL;

//#pragma section

/* End user code. Do not edit comment generated here */


/* Set option bytes */
#pragma public_equ="__OFS0",0xFFFFFFFF
#pragma public_equ="__OFS1",0xFFFFFF7F
//#pragma location = "OPTBYTE"
//__root const uint32_t OFS0_location = 0xFFFFFFFFUL;
//#pragma location = "OPTBYTE"
//__root const uint32_t OFS1_location = 0xFFFFFF7FUL;


void R_MAIN_UserInit(void);
/***********************************************************************************************************************
* Function Name: main
* Description  : This function implements main function.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void main(void)
{
    R_MAIN_UserInit();
    /* Start user code. Do not edit comment generated here */

    /* send character combination to clear screen */
    R_SCI1_SerialTxString((const char *) cmd_clr_scr);
    R_SCI1_SerialTxString("\r\nSystem BootLoader\r\n");
    R_SCI1_SerialTxString("\r\nReset reason...");
    report_reset_reason();

    /* Turn LED0 On to Indicate boot loader has started */
    LED0 = LED_ON;
    LED1 = LED_OFF;

    /* De-bounce SW1 to see if button pressed */
    while (UNDECIDED == switch_pressed)
    {
        switch_pressed = poll_switch_press((uint8_t) SW1);
        reset_watchdog();
    }

    /* If no switch has been pressed then Application area should be validated
     * by calculating the CRC checksum for the area and comparing to a value
     * stored in a special location in flash, which was programmed when the
     * application code was last programmed.
     * If the 2 codes match then jump to the application otherwise the application
     * is invalid, so begin the update process
     */
    if (FALSE == switch_pressed)
    {
        uint16_t chksm;

        /* inform that SW1 was not held down at start */
        R_SCI1_SerialTxString("\r\nDevice Firmware Update : SW1 not pressed.");

        /* calculate CRC and compare to value stored in App area to see if App area is valid */
        chksm = calc_app_crc_value();
        appvalid = is_application_crc_ok(chksm);

        /* The switch SW1 is not being held down, so user is not requesting update */
        /* So now the application code area needs to be verified to see that it is valid */
        R_SCI1_SerialTxString("\r\nDevice Firmware Update : Verifying Application area with CRC.");

        if (appvalid)
        {

            /* create function pointer to start of application (first entry of the
             * User's Fixed Vector Table) */
            start_address = ((void (*)(void))*((uint32_t *)APP_VECTOR_START));
            //(void*) start_address = (void *) ((void *) *((uint32_t *) APP_VECTOR_START));
            
            /* verify that the start address is located in the Application area */
            if ((uint32_t) start_address > (uint32_t) APP_VECTOR_END)
            {

                /* address out of bounds */
                R_SCI1_SerialTxString("\r\nInvalid Start Address");

                /* LED0 being off indicates update failure */
                LED0 = LED_OFF;
                LED1 = LED_OFF;
                appvalid = FALSE;
            }
        }

        /* inform checksum results */
        R_SCI1_SerialTxString("\r\nDevice Firmware : Calculated Checksum Value : 0x");

        /* convert calculated 16 bit calculated CRC number to string */
        uint16_to_string(str, (uint8_t) 0, (uint16_t) chksm);
        R_SCI1_SerialTxString(str);

        R_SCI1_SerialTxString("\r\nDevice Firmware : Read Checksum Value       : 0x");

        /* convert calculated 16 bit read CRC number to string */
        uint16_to_string(str, 0, read_app_crc_value());
        R_SCI1_SerialTxString(str);

        /* inform whether Application is valid or not */
        R_SCI1_SerialTxString("\r\nDevice Firmware : ");
        if (!appvalid)
        {
            R_SCI1_SerialTxString("Application Flash not valid");
        }
        else
        {
            R_SCI1_SerialTxString("Application Flash valid");
        }
    }
    else
    {

        /* inform that SW1 was held down at start */
        R_SCI1_SerialTxString("\r\nDevice Firmware : SW1 pressed.");
    }

    /* if SW1 not being pressed and application CRC is verified, then jump to application code */
    if ((!switch_pressed) && appvalid)
    {

        /* Jump to Application */
        jump_to_application(IMMEDIATE_JUMP);
    }

    /* User requested application code update or application area is invalid */
    /* so get update from UART */
    while (1U)
    {
        uint16_t checksum;
        uint8_t result;

        /* reset watchdog timer */
        reset_watchdog();

        /* wipe application area */
        R_SCI1_SerialTxString("\r\nErasing Application Area...");
        result = UNDECIDED;
        while (UNDECIDED == result)
        {

            /* erase the application space */
            result = erase_application_code();

            /* blink LED0 to show update in progress */
            blink_led0();

            /* reset watch dog timer */
            reset_watchdog();
        }

        /* handle erase failure */
        if (FALSE == result)
        {
            R_SCI1_SerialTxString("\r\nError erasing Application space");

            /* LEDs being off indicates update failure */
            LED0 = LED_OFF;
            LED1 = LED_OFF;
            exit_reset();
        }

        R_SCI1_SerialTxString("\r\nDone.");
        R_SCI1_SerialTxString("\r\nStarting update from UART.");

        /* handle the application code update process */
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

        /* update failed. Wait for user to press reset button to retry */
        if (TRUE != result)
        {
            xmodem_cancel(); /* Abort any transfer in progress */

            R_SCI1_SerialTxString("\r\nError re-programming Application space");

            /* LED0 being off indicates update failure */
            LED0 = LED_OFF;
            exit_reset();
        }

        /* LEDs 0 and 1 being on indicates update success */
        LED0 = LED_ON;
        LED1 = LED_ON;

        /* Flush the X-Modem protocol through. As the end of the S-Record has been reached
         * there should be no further packets, just the EOT condition to handle
         */
        xmodem_frame_rx();

        /* Program any final bytes outstanding in to the flash */
        flash_flush_buffer();

        /* Flash program complete - read CRC and program to location in application area */
        R_SCI1_SerialTxString("\r\nApplication Programming Successful.");

        /* Calculate the CRC value for the memory block and write it to the Application CRC location */
        checksum = calc_app_crc_value();

        /* Write CRC value to Flash */
        write_app_crc_value(checksum);

        /* Display CRC value */
        R_SCI1_SerialTxString("\r\nStoring CRC value: 0x");
        memset(str, '\0', 5);
        uint16_to_string(str, 0, checksum);
        R_SCI1_SerialTxString(str);
        R_SCI1_SerialTxString(" - done.");

        /* CRC value updated, now jump to application         */
        jump_to_application(SW_WAIT_JUMP);
    }

    /* End user code. Do not edit comment generated here */
}
/***********************************************************************************************************************
* Function Name: R_MAIN_UserInit
* Description  : This function adds user code before implementing main function.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_MAIN_UserInit(void)
{
    EI();
    /* Start user code. Do not edit comment generated here */
    R_SCI1_Start(); /* Start the serial port */
    R_SCI1_SerialRxInit(); /* Setup the serial port for single character receive */

    import_file_ptr = NULL;

    /* set R_SCI1_SerialTxString function pointer to point to UART1 send message function */
    R_SCI1_SerialTxString_Ptr = R_SCI1_SerialTxString;

    /* initialise receive buffer */
    init_buffer();

    /* initialise Flash API */
    flash_write_init();

    /* Turn off all the LEDs */
    LED0 = LED_OFF;
    LED1 = LED_OFF;
    LED2 = LED_OFF;
    LED3 = LED_OFF;

    R_CRC_SetCRC16(CRC_LSB); /* Setup the CRC block */

    /* End user code. Do not edit comment generated here */
}

/* Start user code for adding. Do not edit comment generated here */

/***********************************************************************************************************************
 * Function Name: jump_to_application
 * Description  : This function jumps to the application code entry point
 * Arguments    : uint8_t jumptype
 * 				:             IMMEDIATE_JUMP - Jumps directly in to the user application.
 * 				:			  SW_WAIT_JUMP 	 - Requests the user to press SW2 before jumping.
 * Return Value : None
 ***********************************************************************************************************************/
static void jump_to_application (uint8_t jumptype)
{

    /* Update LEDs to indicate application is about to be run */
    LED0 = LED_ON;
    LED1 = (LED2 = (LED3 = LED_OFF));

    switch_pressed = UNDECIDED;

    /* if this function has been called with SW_WAIT_JUMP, wait for
     * SW2 to be pressed before launching user application
     */
    if (SW_WAIT_JUMP == jumptype)
    {
        /* Ask user to press button before jumping to application */
        R_SCI1_SerialTxString("\r\nPress SW2 to jump to Application...");

    	/* wait for SW2 button to be pressed */
		while ( TRUE != switch_pressed )
		{

			/* test switch1, with de-bounce included */
			switch_pressed = poll_switch_press(SW2);

			/* reset watch dog timer */
			reset_watchdog();
		}
	}

    R_SCI1_SerialTxString("\r\nJumping to Application.");

    /* reset watch dog timer */
    reset_watchdog();

    /* turn off LEDs */
    LED0 = LED_OFF;
    LED1 = LED_OFF;

    /* Set flag to indicate the Fixed Exception vectors should
     * jump to user's application fixed vectors.
     */
    g_user_appvects_valid = TRUE;

    /* Fetch the start address from the vector table */
    start_address = ((void (*)(void))*((uint32_t *)APP_VECTOR_START));
    //(void*) start_address = (void *) ((void *) *((uint32_t *) APP_VECTOR_START));

    prep_for_user_jmp();

    /* jump to the application */
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

    /* update via serial process is controlled with this switch statement */
    /* Important that each case statement does not block or watchdog reset could occur */
    switch (step)
    {
        case 0 :

            /* send message via UART to inform ready to receive data */
            R_SCI1_SerialTxString("\r\nPlease start file send using XModem protocol....\r\n");

            lineNumber = 0;
            step = 1;
        break;

        case 1 :

            /*handle ongoing data reception */
            if (poll_buffer_for_new_data())
            {

                /* byte available - read from UART receive register */
                byte = get_buffer_char();
                next_buffer_char();

                /* now awaiting first byte from srec or hex format file containing code. */
                /* The first byte in an srec format file is an 'S'. */
                /* The first byte in a hex format file is an ':'. */
                /* other inputs are wrong */
                if (TRUE == is_file_srec(byte))
                {

                    /* an srec file is being received */
                    /* set function pointers to the srec import functions */
                    import_file_ptr = import_srec_file;
                    get_error_text_ptr = get_srec_error_text;
                    is_error_ptr = is_srec_file_error;
                    handle_record_line_ptr = get_srec_type;
                    get_data_Ptr = get_srec_data;
                    get_address_ptr = get_srec_address;
                    get_start_address_ptr = get_srec_start_address;
                    get_num_data_bytes_ptr = get_srec_num_data_bytes;

                    /* show that an SREC file is being received */
                    file_format = FILE_FORMAT_SREC;

                    /* process first byte */
                    import_file_ptr(byte);
                    step = 2;
                }
                else if (TRUE == is_file_hex(byte))
                {

                    /* a hex file is being received */
                    /* set function pointers to the hex import functions */
                    import_file_ptr = import_hex_file;
                    get_error_text_ptr = get_hex_error_text;
                    is_error_ptr = is_hex_file_error;
                    handle_record_line_ptr = get_hex_type;
                    get_data_Ptr = get_hex_data;
                    get_address_ptr = get_hex_address;
                    get_start_address_ptr = get_hex_start_address;
                    get_num_data_bytes_ptr = get_hex_num_data_bytes;

                    /* show that a HEX file is being received */
                    file_format = FILE_FORMAT_HEX;

                    /* process first byte */
                    import_file_ptr(byte);
                    step = 2;
                }
                else
                {

                    /* if the first byte of the file is not a 'S' or a ':' then it is invalid */
                    /* also a single newline control character is acceptable (but not more) */
                    if ('\n' != byte)
                    {

                        /* indicate error in received file */
                        xmodem_cancel(); /* Cancel the XModem transfer in progress */
                        R_SCI1_SerialTxString("\r\n\r\nError : File not SREC or HEX format.");
                        step = 0;
                        first_data_line = TRUE;
                        return (FALSE);
                    }
                }
            }
        break;

            /* an S-Record or hex file is being received */
        case 2 :

            /*handle ongoing data reception */
            /* poll_buffer_for_new_data returns TRUE if a new character received on the buffer */
            if (TRUE == poll_buffer_for_new_data())
            {

                /* get next byte from the buffer */
                byte = get_buffer_char();

                /* update the buffer to point to the next available byte in the future */
                next_buffer_char();

                /* paranoid check to ensure pointer is set */
                if (NULL != import_file_ptr)
                {

                    /* process the received byte as part of the import of the Srecord or hex file */
                    const uint8_t result = import_file_ptr(byte);

                    /* check if line is complete */
                    if (TRUE == result)
                    {

                        /* handle error */
                        if (is_error_ptr())
                        {

                            /* inform of error and describe reason */
                            xmodem_cancel(); /* Cancel the XModem transfer in progress */
                            R_SCI1_SerialTxString("\r\n\r\nFile error: ");
                            R_SCI1_SerialTxString(get_error_text_ptr());
                            step = 0;
                            return (FALSE);
                        }

                        /* handle the complete line, add to flash program buffer */
                        else
                        {

                            /* valid line of record received. Now decide what action to take */
                            uint8_t line_result;
                            line_result = handle_record_line_ptr();

                            /* handle end of import */
                            if (RECORD_FINISHED == line_result)
                            {

                                /* end of file received                  */
                                /* Make sure that any data in the buffer */
                                /* is flushed to the code flash          */
                                const uint8_t res = flash_flush_buffer();

                                /* handle an error */
                                if (TRUE != res)
                                {
                                    xmodem_cancel(); /* Cancel the XModem transfer in progress */
                                    R_SCI1_SerialTxString("\r\n\r\nFlash Write Error: ");
                                    step = 0;
                                    return (FALSE);
                                }

                                /* report success of import */
                                R_SCI1_SerialTxString("\r\nEnd of File.");

                                /* get start address of data */
                                start_address = (void (*)(void))get_start_address_ptr();
                                //(void*) start_address = (void*) get_start_address_ptr();

                                /* reset processing variables */
                                step = 0;
                                first_data_line = TRUE;
                                return (TRUE);
                            }

                            /* Header received - we now know the file format */
                            else if (RECORD_HEADER == line_result)
                            {

                                /* Information stub: Srec file detected here  */
                                if (FILE_FORMAT_SREC == file_format)
                                {
                                    /* Perform any special actions here for SREC file format */
                                }

                                /* Information stub: Hex file detected here  */
                                if (FILE_FORMAT_HEX == file_format)
                                {
                                    /* Perform any special actions here for HEX file format */
                                }

                                /* increment line counter */
                                lineNumber++;
                            }

                            /* Ready to flash a block of code */
                            else if (RECORD_FLASH == line_result)
                            {

                                /* next step is to program line to flash */
                                step = 3;

                                /* if first line of code to be flashed, then provide address and data headings */
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

                                /* nothing - line_result = RECORD_NEXT */
                            }
                        }
                    }
                    else if (FALSE == result)
                    {
                        /* report error, type and line number */
                        char lineNumText[6];
                        xmodem_cancel(); /* Cancel the XModem transfer in progress */
                        memset(lineNumText, '\0', 6);
                        R_SCI1_SerialTxString("\r\nFile Reading error:\r\n");
                        R_SCI1_SerialTxString(get_error_text_ptr());
                        R_SCI1_SerialTxString("\r\nAt line: ");

                        /* Convert line Number to bcd string */
                        uint16_to_bcd_string(lineNumText, 0, lineNumber);
                        R_SCI1_SerialTxString(lineNumText);
                        return (FALSE);
                    }
                    else
                    {
                        /* Undefined result */
                    }
                }
            }
        break;

        case 3 :

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
        break;

        default :
            step = 0;
            first_data_line = TRUE;
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
    R_SCI1_SerialTxString("\r\n");

    /* the RESF register holds the reset reason */
    if (1 == SYSTEM.RSTSR1.BIT.CWSF)
    {
        R_SCI1_SerialTxString("Warm Reset.\r\n");
    }

    if (0 == SYSTEM.RSTSR1.BIT.CWSF)
    {
        R_SCI1_SerialTxString("Cold Reset.\r\n");
    }

    if (SYSTEM.RSTSR0.BIT.PORF)
    {
        R_SCI1_SerialTxString("Power-on Reset.\r\n");
    }

    if (SYSTEM.RSTSR0.BIT.LVD1RF)
    {
        R_SCI1_SerialTxString("Voltage Mon. 1 Reset.\r\n");
    }

    if (SYSTEM.RSTSR0.BIT.LVD2RF)
    {
        R_SCI1_SerialTxString("Voltage Mon. 2 Reset.\r\n");
    }

    if (SYSTEM.RSTSR2.BIT.IWDTRF)
    {
        R_SCI1_SerialTxString("Watchdog Reset.\r\n");
    }

    if (SYSTEM.RSTSR2.BIT.SWRF)
    {
        R_SCI1_SerialTxString("Software Reset.\r\n");
    }

    R_SCI1_SerialTxString("Power On Reset / reset button.\r\n");
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

    R_SCI1_SerialTxString("\r\nPress reset button to retry....");

    while (1)
    {

        /* Reset watch dog timer */
        R_IWDT_Restart();
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

    /* sum the SW1 port value into a variable for a defined period of time */
    if ((delay++) < (SWITCH_PRESS_DELAY_COUNTS >> 8))
    {
        if (port_value)
        {
            reading_count++;
        }

        return (UNDECIDED);
    }

    delay = 0;

    /* if the count is greater than half the number of readings then it is mostly high */
    if (reading_count > (SWITCH_PRESS_DELAY_COUNTS >> 9))
    {

        /* a logic high indicates SW1 is not being pressed */
        reading_count = 0;
        return (FALSE);
    }
    reading_count = 0;

    /* a logic low indicates SW1 is being pressed */
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
    R_IWDT_Restart();
}

/***********************************************************************************************************************
 * End of function reset_watchdog
 ***********************************************************************************************************************/

/***********************************************************************************************************************
 * Function Name: prep_for_user_jmp
 * Description  : Perform actions required to leave the micro controller in a state
 * 				  for the user code to start running.
 * Arguments    : none
 * Return Value : none
 ***********************************************************************************************************************/
static void prep_for_user_jmp (void)
{
    /* Turn off all LEDs */
    LED0 = LED_OFF;
    LED1 = LED_OFF;
    LED2 = LED_OFF;
    LED3 = LED_OFF;

    /* Stop all peripherals */
    R_CMT0_Stop();
    R_CMT3_Stop();
    R_SCI1_Stop();

    set_psw(0); /* Disable all interrupts */
}

/***********************************************************************************************************************
 * End of function prep_for_user_jmp
 ***********************************************************************************************************************/

/* End user code. Do not edit comment generated here */
