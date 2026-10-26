/* Adapted for use with IAR Embedded Workbench */
/*******************************************************************************
 *   Copyright (C) 2009. Renesas Technology Corp., All Rights Reserved.
 *""FILE COMMENT""*********** Technical reference data *************************
 *   System Name : RX630 Sample Program
 *   File Name   : main_Test_all.c
 *   Abstract    : Main program
 *   Version     : 0.01.00
 *   Device      : RX630
 *   Tool-Chain  : IAR Embedded Workbench
 *   OS          : None
 *   H/W Platform: RSK+RX630
 *   Description : 
 *******************************************************************************
 *   History     : 11,June,2010 Ver.0.01.00  
 *""FILE COMMENT END""*********************************************************/


/*******************************************************************************
User Includes
*******************************************************************************/
/* The following 'r_pdl' header files define the API functions used in this 
   file */

/* Defines RX630 iodefine */
#include "iorx630.h"
/* Defines main code */
#include "main_test_all.h"
/* Defines LCD display functions used in this file */
#include "lcd.h"

#include <intrinsics.h>

/*******************************************************************************
System Definitions , Global Variables & Function Declarations
*******************************************************************************/

/* Device nickname definition */
#define NICKNAME "RX630   "

/* The global variable KeyPress is used as a flag whenever a switch is 
   pressed */
/* Flag of LED update timing */
volatile unsigned char led_timming = 0;
char LCD_buf[10];

#define END_CODE 0x04

/*******************************************************************************
Initial Setting Memory for RX630
*******************************************************************************/

void HwsetupMainDemo(void);

/*******************************************************************************
Function Name : main
Description	  : This is the main program function
	
Parameters	  : none
Return value  : none
*******************************************************************************/
void main(void)
{
  /* If this included, press SW1 and wait to exit. */
  int Test_number = 0;

  /* Periferal setup */
  HwsetupMainDemo();

  /* Enable interrupt */
  __enable_interrupt();
	
  /* Start RX630 board test function */

  /* Disply menu on PC terminal */
  do {
    switch(Test_number)
    {
      /* for LCD test */
    case 0x00:
      /* for LED test */
      LED_test_func();
      break;
    case 0x01:
      /* for LCD test */
      DisplayString(LCD_LINE1, "Renesas ");
      DisplayString(LCD_LINE2, NICKNAME);
      break;
    case 0x02:
      /* SW test */
      SW_test_func();
      break;
    case 0x03:
      /* AD-POT test
         Press SW1 fo finish. */
      AD_test_func();
      break;
    default:
      break;
    }
    
    /* Wait Press any SW */
    if (Test_number == 0 || 
        Test_number == 2 || 
        Test_number == 3 ){
    }
    else 
      Check_SW_Input();
    
    /* Inclease Test counter */
    Test_number++;
    
    /* clear LCD module display */
    ClearLCD();
    
  } while(Test_number < END_CODE);
  
  DisplayString(LCD_LINE1, "End of");
  DisplayString(LCD_LINE2, "demo");
}

/******************************************************************************
End main function
*******************************************************************************/

/*""FUNC COMMENT""*************************************************************
* Outline		: HwsetupMainDemo
* Description	: Contains all the setup functions called at device restart
* Argument		: none
* Return value	: none
*""FUNC COMMENT END""*********************************************************/
void HwsetupMainDemo(void)
{
// RX630 Setting
// SCKCR setting
// ICLK=96MHz(12MHz * 8), BCLK=24MHz(12MHz * 2), Pphi=48MHz(12MHz * 4)
//	SYSTEM.SCKCR.LONG = 0x00C20100;	
//	ICLK=48MHz(12MHz * 4), BCLK=24MHz(12MHz * 2), Pphi=24MHz(12MHz * 2)
//	SYSTEM.SCKCR.LONG = 0x01C20200;	

// RX630 Setting
  unsigned int i;
  SYSTEM.PRCR.WORD = 0xA503;			/* protect off */
  
  SYSTEM.SOSCCR.BYTE = 0x01;		/* stop sub-clock */
  /* delete when you use sub-clock */
  
  SYSTEM.MOSCWTCR.BYTE = 0x0D;		/* 131072 state */
  /* wait over 10ms  @12.5MHz */
  
  SYSTEM.PLLWTCR.BYTE = 0x0F;		/* 4194304 state (default)*/
  /* wait over 12ms  @PLL=200MHz(12.5MHz*16) */
  
  SYSTEM.PLLCR.WORD = 0x0F00;		/* x16 @PLL */
  SYSTEM.MOSCCR.BYTE = 0x00;		/* EXTAL ON */
  SYSTEM.PLLCR2.BYTE = 0x00;		/* PLL ON */
  
  for(i = 0;i< 0x168;i++){		/* wait over 12ms */
  }
  
  SYSTEM.SCKCR.LONG = 0x21022222;	/* ICK=PLL/2,BCK,FCK,PCK=PLL/4 */
  SYSTEM.SCKCR3.WORD = 0x0400;		/* LOCO -> PLL */
  
  
// RX630 Setting
  
  /* LED PORT setting */
  led_0_port = LED_ON;
  led_1_port = LED_ON;
  led_2_port = LED_ON;
  led_3_port = LED_ON;
  
  led_0_port_dir = 1;
  led_1_port_dir = 1;
  led_2_port_dir = 1;
  led_3_port_dir = 1;

  /* Enable SW port input buffer */
//	PORT3.ICR.BYTE |= 0x04; //P32(SW1-IRQ2)
//	PORT4.ICR.BYTE |= 0x10; //P44(SW2-IRQ12)
//	PORT0.ICR.BYTE |= 0x80; //P07(SW3-IRQ15)
	
  /* MSTPCRA, MSTPCRB and MSTPCRC setting */
  /* S12AD unit and MTU2 UNIT0 in MSTPCRA start. */
  SYSTEM.MSTPCRA.LONG = 0xFFFDFDFF;	
  /* SCI0 modules in MSTPCRB start. */
  SYSTEM.MSTPCRB.LONG = 0x7FFFFFFF;	
  /* All modules in MSTPCRC stop.*/
  //SYSTEM.MSTPCRC.LONG &= 0xFFFF0000;
  
  SYSTEM.PRCR.WORD = 0xA500;			/* protect on */
  
  /* Set LCD module */
  InitialiseDisplay();
  
  /* MTU0 setting */
  /* Clear TCNT when TGRA match, P/64 phi */
  MTU0.TCR.BYTE = 0x23;
  MTU0.TMDR.BYTE = 0;
  MTU0.TCNT = 0;
  MTU0.TGRA = TGRA0_VALUE;
  
  MTU0.TIER.BYTE = 0x01;
//	ICU.IPR[0x51].BYTE = 0x04;
//	ICU.IER[0x0e].BIT.IEN2 = 1;
//	ICU.IR[VECT_MTU0_TGIA0].BIT.IR = 0;
  ICU.IPR[142].BYTE = 0x04;
  ICU.IER[0x11].BIT.IEN6 = 1;
  ICU.IR[VECT_MTU0_TGIA0].BIT.IR = 0;
  
}
/******************************************************************************
End of hwsetup
******************************************************************************/

/*""FUNC COMMENT""*************************************************************
* Outline 		: AD_test_func
* Description 	: Display ADC value on LCD module
* Argument  	: none
* Return value  : none
*""FUNC COMMENT END""*********************************************************/
void AD_test_func(void)
{
  unsigned int ad_data;
  
  DisplayString(LCD_LINE1, "ADC TEST");
  AD_init();
  
  while(1)
  {
    /* start ADC */
    S12AD.ADCSR.BIT.ADST = 1;
    
    /* wait until AD convert end */
    while(S12AD.ADCSR.BIT.ADST);
    
    /* Get Ad data */
    ad_data = S12AD.ADDR0;
    Convert_16BitNumber_ToString(ad_data, LCD_buf);
    DisplayString(LCD_LINE2, LCD_buf);
    
    if(SW_Input() != -1)
    {
      break;
    }
  }
}

/******************************************************************************
End of AD_test_func
******************************************************************************/

/*""FUNC COMMENT""*************************************************************
 * Outline 		: AD_init
 * Description 	: Setup for ADC(
 * Argument  	: none
 * Return value  : none
 *""FUNC COMMENT END""*********************************************************/
void AD_init(void)
{
//	AD0.ADCR.BYTE = 0x00;
//	/* Sible mode, AD clock- PCLK/8, SW torigger */
//	AD0.ADCSR.BYTE |= 0x00;
//	/* Select AN0 for AD unit0  stop AD flag*/
  
  PORT4.PMR.BYTE = 0x01;
  
  /* ADC clock = PCLK/8, 1cycle scan mode */
  S12AD.ADCSR.BYTE |= 0x00;
  /* Selects AN000 */
  S12AD.ADANS0.WORD = 0x0001;
}
/******************************************************************************
End of AD_init
******************************************************************************/

/*""FUNC COMMENT""*************************************************************
 * Outline 		: led_test_func
 * Description 	: Flash LED of LED number
 * Argument  	: LED number
 * Return value  : none
 *""FUNC COMMENT END""*********************************************************/
void LED_test_func(void)
{
  unsigned char led_count = 0;
  
  /* Start MTU0 */
  /* set LED timming flag on MTU handler */ 
  MTU.TSTR.BYTE = 0x01;
  /* Check LED timing. */

  led_0_port = LED_OFF;
  led_1_port = LED_OFF;
  led_2_port = LED_OFF;
  led_3_port = LED_OFF;
  
  while(1){
    if(led_timming == 1){			
      led_timming = 0;
      
      /* update led_pt */
      if(++led_count > 50){
        break;
      }
      /* update LEDs */
      led_fnc();		
    }
  }
  
  led_0_port = LED_OFF;
  led_1_port = LED_OFF;
  led_2_port = LED_OFF;
  led_3_port = LED_OFF;
  
  /* Stop MTU0 */
  MTU.TSTR.BYTE = 0x00;
  
}
/******************************************************************************
End of LED_test_func
******************************************************************************/

/*""FUNC COMMENT""*************************************************************
 * Outline 		: led_func
 * Description 	: Flash LED of LED number
 * Argument  	: LED number
 * Return value  : Error code
 *                 0: none
 *                 1: wrong LED number
 *""FUNC COMMENT END""*********************************************************/
unsigned char led_fnc(void)
{
  led_0_port = ~led_0_port;
  led_1_port = ~led_1_port;
  led_2_port = ~led_2_port;
  led_3_port = ~led_3_port;
  
  return 0;
}

/******************************************************************************
End of led_func
******************************************************************************/

/*""FUNC COMMENT""*************************************************************
 * Outline 		: SW_test_func
 * Description 	: If push some SW2 or SW3, Toggele LED0-2
 *				  If press SW1, exit test function
 * Argument  	: LED number
 * Return value  : none
 *""FUNC COMMENT END""*********************************************************/
void SW_test_func(void)
{
  int end = 0;
  
  DisplayString(LCD_LINE1, "SW Test ");
  DisplayString(LCD_LINE2, "S1 = End");
  
  do {
    /* Read SW port (Port0) */
    switch(Check_SW_Input()){
      /* for pushing SW1 */
    case 0:
      led_0_port = LED_OFF;
      led_1_port = LED_OFF;
      led_2_port = LED_OFF;
      led_3_port = LED_OFF;
      end = 2;
      break;
      /* for pushing SW2 */
    case 1:
      led_0_port = LED_ON;
      led_1_port = LED_OFF;
      led_2_port = LED_ON;
      led_3_port = LED_OFF;
      end = 1;
      break;
      /* for pushing SW3 */
    case 2:
      led_0_port = LED_OFF;
      led_1_port = LED_ON;
      led_2_port = LED_OFF;
      led_3_port = LED_ON;
      end = 1;
      break;
    default:
      break;
    }
  } while(end < 2);
}

/******************************************************************************
End of SW_test_func
******************************************************************************/

/*""FUNC COMMENT""*************************************************************
 * Outline 		: Chgeck_SW_Input
 * Description 	: Check SW input
 * Argument  	: none
 * Return value  : SW code
 *                 0 - Press SW1
 *                 1 - Press SW2
 *                 2 - Press SW3
 *                -1 - other
 *""FUNC COMMENT END""*********************************************************/
int Check_SW_Input(void)
{
  volatile char sw_data;
  int i = 0xF000, code = -1;
  
  while(i--)
  {
    sw_data = PORT3.PIDR.BYTE;
    switch(sw_data & 0x04){
      /* SW1 */
    case 0x00:
      code = 0;
      break;
    default:
      break;
    }
    sw_data = PORT4.PIDR.BYTE;
    switch(sw_data & 0x10){
      /* SW2 */
    case 0x00:
      code = 1;
      break;
    default:
      break;
    }
    sw_data = PORT0.PIDR.BYTE;
    switch(sw_data & 0x80){
      /* SW3 */
    case 0x00:
      code = 2;
      break;
    default:
      break;
    }
    
    if (code != -1)
    {
      /* wait until SW turn off */
      while(1)
      {
        sw_data = PORT3.PIDR.BYTE;
        if ((sw_data & 0x04) >= 0x04){
          break;
        }
        sw_data = PORT4.PIDR.BYTE;
        if ((sw_data & 0x10) >= 0x10){
          break;
        }
        sw_data = PORT0.PIDR.BYTE;
        if ((sw_data & 0x80) >= 0x80){
          break;
        }
      }
      break;
    }
  }
  return(code);
  
}

/******************************************************************************
End of Check_SW_Input
******************************************************************************/

/*""FUNC COMMENT""*************************************************************
 * Outline 		: SW_Input
 * Description 	: Check SW input and If do not input SW, return 
 * Argument  	: none
 * Return value  : SW code
 *                 0 - Press SW1
 *                 1 - Press SW2
 *                 2 - Press SW3
 *                -1 - other
 *""FUNC COMMENT END""*********************************************************/
int SW_Input(void)
{
  volatile char sw_data;
  int code = -1;
  
  sw_data = PORT3.PIDR.BYTE;
  switch(sw_data & 0x04)
  {
    /* SW1 */
  case 0x00:
    code = 0;
    break;
  default:
    break;
  }
  sw_data = PORT4.PIDR.BYTE;
  switch(sw_data & 0x10)
  {
    /* SW2 */
  case 0x00:
    code = 1;
    break;
  default:
    break;
  }
  sw_data = PORT0.PIDR.BYTE;
  switch(sw_data & 0x80)
  {
    /* SW3 */
  case 0x00:
    code = 2;
    break;
  default:
    break;
  }
  
  if (code != -1){
    /* wait until SW turn off */
    while(1)
    {
      sw_data = PORT3.PIDR.BYTE;
      if ((sw_data & 0x04) >= 0x04){
        break;
      }
      sw_data = PORT4.PIDR.BYTE;
      if ((sw_data & 0x10) >= 0x10){
        break;
      }
      sw_data = PORT0.PIDR.BYTE;
      if ((sw_data & 0x80) >= 0x80){
        break;
      }
    }
  }
  return(code);
}

/******************************************************************************
End of Check_SW_Input
******************************************************************************/

/*""FUNC COMMENT""*************************************************************
 * Outline 		: notify_LED_time
 * Description 	: set LED Flash timing and UART data 
 * Argument  	: none
 * Return value  : none
 *""FUNC COMMENT END""*********************************************************/
void notify_LED_time(void)
{
  led_timming = 1;
}
/******************************************************************************
End of notify_LED_time
******************************************************************************/

