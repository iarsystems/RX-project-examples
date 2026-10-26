/* Adapted for use with IAR Embedded Workbench */
/******************************************************************************
* DISCLAIMER
* Please refer to http://www.renesas.com/disclaimer
******************************************************************************/
/***********************************************************************/
/*                                                                     */
/*  FILE        :hwsetup.h                                             */
/*  DATE        :Mon, Jul 23, 2007                                     */
/*  DESCRIPTION :Main Program                                          */
/*  CPU TYPE    :RX                                                    */
/*                                                                     */
/*                                                                     */
/***********************************************************************/

#ifndef HWSETUP_H_INCLUDED
#define HWSETUP_H_INCLUDED

/***********************************************************************************
Macro Defines
***********************************************************************************/
#define XTAL_FREQUENCY (12000000L)
#define  ICLK_MUL (8)
#define  PCLK_MUL (4)
#define  BCLK_MUL (4)
#define   ICLK_FREQUENCY (XTAL_FREQUENCY * ICLK_MUL)
#define   PCLK_FREQUENCY (XTAL_FREQUENCY * PCLK_MUL)
#define   BCLK_FREQUENCY (XTAL_FREQUENCY * BCLK_MUL)

#define LED_0_PORT 0
#define LED_0_PIN  2
#define LED_1_PORT 0
#define LED_1_PIN  3
#define LED_2_PORT 0
#define LED_2_PIN  5
#define SW_1_PORT  0
#define SW_1_PIN   0
#define SW_2_PORT  0
#define SW_2_PIN   1
#define SW_3_PORT  0
#define SW_3_PIN   7

#define FLASH_SCS_PORT 2
#define FLASH_SCS_PIN  7
#define FLASH_SCK_PORT 1
#define FLASH_SCK_PIN  2
#define FLASH_SCI SCI2
#define FLASH_SCI_ENABLE {;}
#define LOAD_RESOURCES

#define DEBUG_0_PORT 2
#define DEBUG_0_PIN  0
#define DEBUG_1_PORT 2
#define DEBUG_1_PIN  1
#define DEBUG_2_PORT 2
#define DEBUG_2_PIN  2
#define DEBUG_3_PORT 2
#define DEBUG_3_PIN  3
#define DEBUG_OUT(a) PM.DR.BYTE = ((a) & 0xF); 

/* Touchscreen hardware connections */
#define OUT_TS_XL_PORT  8    /* driver port/pin for terminal resistor at left of screen */
#define OUT_TS_XL_PIN   4
#define OUT_TS_XR_PORT  8    /* driver port/pin for terminal resistor at right of screen */
#define OUT_TS_XR_PIN   4
#define OUT_TS_YL_PORT  8    /* driver port/pin for terminal resistor at bottom of screen */
#define OUT_TS_YL_PIN   5
#define OUT_TS_YU_PORT  8    /* driver port/pin for terminal resistor at top of screen */
#define OUT_TS_YU_PIN   5

#define ADC_TS_XL 4     /* channel connected to terminal resistor at left of screen */
#define ADC_TS_XR 6     /* channel connected to terminal resistor at right of screen */
#define ADC_TS_YL 7     /* channel connected to terminal resistor at bottom of screen */
#define ADC_TS_YU 5     /* channel connected to terminal resistor at top of screen */

#define ADC_POT S12AD.ADDRA

#define TOUCH_CALIBRATION_START_PORT SW_1_PORT
#define TOUCH_CALIBRATION_START_PIN  SW_1_PIN

#define RAM_32WIDE

/**********************************************************************************
Global variables
***********************************************************************************/

/**********************************************************************************
Function Prototypes
***********************************************************************************/
void initLEDs(void);
void HardwareSetup(void);

#endif    /* HWSETUP_H_INCLUDED */  
