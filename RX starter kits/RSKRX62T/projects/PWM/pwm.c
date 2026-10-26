/* Adapted for use with IAR Embedded Workbench */
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
/*******************************************************************************
* Copyright (C) 2011 Renesas Electronics Corporation. All rights reserved.    */ 
/*******************************************************************************
* File Name    : pwm.c
* Version     : 2.00
* Device     : R5F562TAADFP
* Tool Chain     : RX Family C Compiler
* H/W Platform  : RSKRX62T
* Description   : File provides PWM timer functions       
*******************************************************************************/
/*******************************************************************************
* History     : 15.10.2010 Ver. 1.00 First Release
              : 21.05.2011 Ver. 2.00 Second Release (Updated to RPDL CS3)
*******************************************************************************/

/*******************************************************************************
* Project Includes
*******************************************************************************/
/* IO Port RPDL function definitions */
#include "r_pdl_io_port.h"
/* GPT RPDL function defintions */
#include "r_pdl_gpt.h"
/* General RPDL function definitions */
#include "r_pdl_definitions.h"
/* Defines switch functions and variables used in this file */
#include "switch.h"
/* Defines debug LCD functions used in this file */
#include "lcd.h"
/* Provides declarations of functions defined in this file */
#include "pwm.h"


/*******************************************************************************
* Global Variables
*******************************************************************************/
/* Duty cycle variable initialised to 10% of the period */
uint16_t gDutyCycle = 0x9C4;
/* PWM varying duty cycle on/off boolean global variable */
bool gVaryDutyCycle = true;

/* Allocate a copy of the structure */
R_GPT_Create_structure ch_parameters;
/* Allocate a copy of the structure for the selected channel */
R_GPT_ControlChannel_structure gpt_0_control_parameters;

/* Declare variables to store the timer's status */
uint16_t GPT_Status;

/*******************************************************************************
* Local Function Prototypes
*******************************************************************************/
/* Switch interrupt callback function prototype delcaration */
void CB_Switch (void);
/* GPT's GTCCRA interrupt callback function prototype declaration */
void CB_GPT_A(void);
/* 8-bit integer to BCD string function prototype delcaration */
void uint16_ToBCDString(uint8_t *, uint8_t, uint16_t);

/******************************************************************************
* Outline     : Init_PWM
* Description   : Configures the compare match pin as an output then initialises 
*          the Genaral PWM Timers to create the PWM output. A switch 
*          callback function is configured for SW3 releases.
* Argument    : none
* Return value  : none
******************************************************************************/
void Init_PWM(void)
{
  /* Declare error flag */
  bool err = true;  
  
  /* Use the GPT–A pins */
  err &= R_GPT_Set
          (
            PDL_GPT_PINS_AB_A
          );
    
  /* Load the defaults */
  R_GPT_Create_load_defaults(&ch_parameters);

  /* Set the non-default options for channel 0 */
  ch_parameters.data2 = PDL_GPT_MODE_SAW | PDL_GPT_CLK_ICLK_DIV_8;
  
  /* Enable GTCCRA and GTCCRB interrupts */
  ch_parameters.data3 = PDL_GPT_IRQ_A_ENABLE |PDL_GPT_IRQ_B_ENABLE; 

  /* Set the GTCCRA value */  
  gpt_0_control_parameters.data5 = 0x9C4;

  /* Set the GTCCRB value */  
  gpt_0_control_parameters.data6 = 0x57E4; 
  
  /* Set the GTPR value */  
  gpt_0_control_parameters.data11 = 0x61A8;
  
  /* Output a low at occurences of a compare match */
  ch_parameters.data6 = PDL_GPT_A_CM_LOW | \
              PDL_GPT_A_CYCLE_HIGH | PDL_GPT_A_RETAIN;
  
  /* Retain output state when the clock is stopped */
  ch_parameters.data7 = PDL_GPT_B_CM_LOW | \
                PDL_GPT_B_CYCLE_HIGH | PDL_GPT_B_RETAIN;
  
  /* Specify the interrupt callback function for GTCCRA compare matches */
  ch_parameters.func1 = CB_GPT_A;
  
  /* Specify the interrupt level for the callback function */
  ch_parameters.data11 = 7;
  
  /* Configure the GPT with the non-default parameters */
  err &= R_GPT_Create
          (
            0,
            &ch_parameters
          );
        
  /* Set the switch callback funtion to call function SW1_callback */
  SetSwitchReleaseCallback(CB_Switch );
  
  /* Load the register values and stop channel 0 */
  err &= R_GPT_ControlChannel
        (
          0,
          PDL_GPT_WRITE_ENABLE | PDL_GPT_START | \
          PDL_GPT_COUNTER_CLEAR,
          PDL_GPT_COUNT_DIRECTION_UP | PDL_GPT_REGISTER_A | \
          PDL_GPT_REGISTER_B | PDL_GPT_REGISTER_CYCLE,
          &gpt_0_control_parameters
        );
               
  /* Halt in while loop when RPDL errors detected */  
  while(!err);
}
/******************************************************************************
* End of function Init_PWM
******************************************************************************/

/******************************************************************************
* Outline     : CB_GPT_A
* Description   : Callback function executed at each compare match of the  
*          counter and GTCCRA. The function stores the timer's status 
*          and count values and updates the duty cycle accordingly 
*          depending on the compare match detected.
* Argument    : none
* Return value  : none
******************************************************************************/
void CB_GPT_A(void)
{  
  /* Declare error flag */
  bool err = true;
  
  /* Check the timer status and read the current duty cycle */
  err &= R_GPT_ReadChannel
          (
            0,
            &GPT_Status,
            &gDutyCycle,
            PDL_NO_PTR,
            PDL_NO_PTR,
            PDL_NO_PTR,
            PDL_NO_PTR,
            PDL_NO_PTR,
            PDL_NO_PTR,
            PDL_NO_PTR,
            PDL_NO_PTR,
            PDL_NO_PTR,
            PDL_NO_PTR,
            PDL_NO_PTR,
            PDL_NO_PTR,
            PDL_NO_PTR,
            PDL_NO_PTR,
            PDL_NO_PTR,
            PDL_NO_PTR,
            PDL_NO_PTR,
            PDL_NO_PTR,
            PDL_NO_PTR
          );
  
  /* Update waveform duty cycle to 10% */
  gpt_0_control_parameters.data5 = gDutyCycle;
  
  /* Vary the duty cycle if the flag is true */
  if (gVaryDutyCycle == true)
  {      
    /* Check if GTCCRA compare match was detected */    
    if(GPT_Status & 0x0001)    
    {
      /* Update waveform duty cycle to 10% */
      gpt_0_control_parameters.data5 = 0x9C4;    
    
      /* Clear the compare match interrupt flag A */
      GPT0.GTST.BIT.TCFA = 0;
    }
  
    /* Check if GTCCRB compare match was detected */
    if(GPT_Status & 0x0002)
    {
      /* Update waveform duty cycle with the count's value */
      gpt_0_control_parameters.data5 = gDutyCycle; 
    
      /* Clear the compare match interrupt flag B */
      GPT0.GTST.BIT.TCFB = 0;
    }
            
    /* Load the GTCRRA value */
    err &= R_GPT_ControlChannel
              (
                0,
                PDL_GPT_START,
                PDL_GPT_REGISTER_A,
                &gpt_0_control_parameters
              );
  }
  
  /* Halt in while loop when RPDL errors detected */  
  while(!err);  
}
/******************************************************************************
* End of function CB_GPT_A
******************************************************************************/

/******************************************************************************
* Outline     : CB_Switch 
* Description   : Function is called after switch 1 key press has been
*          debounced. Function stops the duty cycle update timer, thereby
*          freezing the output waveform duty cycle. The duty cylce is 
*          then displayed on the debug LCD as a percentage.
* Return value  : none
******************************************************************************/
void CB_Switch(void)
{
  /* Declare error flag */
  bool err = true;
  
  /* Declare local duty cycle container variable */
  uint16_t duty_cycle = 0x0;
    
  /* Local LCD buffer string */
  uint8_t lcd_buffer[] = "D.C: XY%";
  
  /* Check if switch SW1 has been pressed */
  if(gSwitchFlag & SWITCHPRESS_1)
  {
    /* Stop the counter */
    err &= R_GPT_ControlChannel
          (
            0,
            PDL_GPT_STOP,
            PDL_NO_DATA,
            &gpt_0_control_parameters
          );
            
    /* Local duty cycle container variable takes the value of 
       gDutyCycle which is then scaled down by a factor of 250 */
    duty_cycle = (uint16_t)((gDutyCycle)/0xFA);
    
    /* Set the vary-duty-cycle flag to false */
    gVaryDutyCycle = false;
    
    /* Convert the duty cycle value into a BCD string, inserted
       into the string lcd_buffer */
    uint16_ToBCDString(lcd_buffer, 2u, (duty_cycle));
    
    /* Display the contents of lcd_buffer onto the debug LCD */
    DisplayLCD(LCD_LINE2, lcd_buffer);
    
    /* Set the non-default options for channel 0 */
    ch_parameters.data2 = PDL_GPT_MODE_SAW | PDL_GPT_CLK_ICLK_DIV_8;
  
    /* Disable GTCCRA and GTCCRB interrupts */
    ch_parameters.data3 = PDL_GPT_IRQ_A_DISABLE |PDL_GPT_IRQ_B_DISABLE; 
    
    /* Restart the timer */
    err &= R_GPT_ControlChannel
          (
            0,
            PDL_GPT_START,
            PDL_NO_DATA,
            &gpt_0_control_parameters
          );
          
    /* Set flag to stop variations in the duty */
    gVaryDutyCycle = false;
  }
  
  /* Clear the switch press flags */
  gSwitchFlag &= 0x0F;
  
  /* Halt in while loop when RPDL errors detected */
  while(!err);
}
/******************************************************************************
* End of function CB_Switch 
******************************************************************************/

/******************************************************************************
* Outline      : uint16_ToBCDString
* Description  : This function converts an 16bit integer into a BCD string,
*         which is inserted into the character array passed as an 
*         argument.
* Argument     : uint8_t * string  - Pointer to array to store BCD string 
*         uint8_t pos    - Array element to start BCD string from.
*         uint16_t number  - 16bit integer to convert.
* Return value : none
* Note       : No input validation is used, so output data can overflow the
*         array passed.
******************************************************************************/
void uint16_ToBCDString(uint8_t * output_string, uint8_t pos, uint16_t number)
{
  /* Local digit position variable */
  uint8_t digit = 5u;
  
  /* Digit is a leading zero indicator variable */
  bool leading_zero = true;
  
  /* Local string buffer variable */
  uint8_t buffer_string[] = "00000";
  
  /* Decrement digit each iteration, until digit is zero */
  while(digit--)
  {
    /* Write BCD value to string element */
    buffer_string[digit] = (uint8_t) (0x30 + (number % 10));
    
    /* Move to the next decimal digit */
    number /= 10u;
  }
  
  /* Set digit position to zero */
  digit = 0u;
  
  /* Loop through each digit, and replace leading zeros with a space
     character */
  while(digit < 5u)
  {
    /* Check if digit is not a zero character, 
       or if leading_zero indicator is false */
    if((buffer_string[digit] != 0x30) | (!leading_zero))
    {
      /* Copy digit from buffer string to output string */
      output_string[digit + pos] = buffer_string[digit];
      
      /* Set the leading zero indicator variable to false */
      leading_zero = false;  
    }
    
    /* Increment the loop counter */
    digit++;
  }
}
/*******************************************************************************
* End of function uint16_ToBCDString
*******************************************************************************/
