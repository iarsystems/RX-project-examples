/******************************************************************************
* DISCLAIMER
* Please refer to http://www.renesas.com/disclaimer
******************************************************************************/

// use the defintions below to change the MCU ports used for interfacing to the panel 
/* Define which CS is used for the frame buffer */
#define DD_FRAME_CS        3
// CAS Latency if SDRAM usage
#define DD_FRAME_SDRAM_CAS_LATENCY 2
// SDRAM Page Size (in WORDS!)
#define DD_FRAME_SDRAM_PAGE_SIZE 256

/* control vsync */
#define DD_VSYNC_PORT      2
#define DD_VSYNC_PIN       4

/* control hsync */
#define DD_HSYNC_PORT      3
#define DD_HSYNC_PIN       4

/* control dotclock */
#define DD_DOTCLK_PORT     5
#define DD_DOTCLK_PIN      6

/* turn on power to the LCD panel */          
//#define DD_PANEL_POWER_PORT    3
//#define DD_PANEL_POWER_PIN     1

/* turn on power to LCD backlight */
#define DD_BACKLIGHT_PORT 9
#define DD_BACKLIGHT_PIN  3

/* ExDMA Channel used for direct drive */
#define DD_EXDMAC EXDMAC0
#define DD_EXDMAC_REQ_PORT  5
#define DD_EXDMAC_REQ_PIN   5

/* specify timer unit used for direct drive */
#define DD_TMR_MTU
//#define DD_TMR_TPU

/* Timer Channel Used for the Dot Clock Signal and period (must be seperate channel from other timer channels) */
#define DD_DOTCLK_CHANNEL  3
#define DD_DOTCLK_TIOC     C     /* must be A or C because of PWM1 operation */             
#define DD_DOTPER_CHANNEL  3     /* must be same as DD_DOTCLK_CHANNEL */                         
#define DD_DOTPER_TIOC     D     /* must be complementary pin to DD_DOTCLK_TIOC A->B, C->D */ 

/* Timer Channels used for Horizontal Synch Signal */
#define DD_HSYNC_CHANNEL   0
#define DD_HSYNC_TIOC      C     /* must be A or C because of PWM1 operation */             
#define DD_HPER_CHANNEL    0     /* must be same as DD_HSYNC_CHANNEL */                         
#define DD_HPER_TIOC       D     /* must be complementary pin to DD_HSYNC_TIOC A->B, C->D */ 

/* Timer Channels used for Vertical Synch Signal */
#define DD_VSYNC_CHANNEL   4
#define DD_VSYNC_TIOC      A     /* must be A or C because of PWM1 operation */             
#define DD_VPER_CHANNEL    4     /* must be same as DD_VSYNC_CHANNEL */                         
#define DD_VPER_TIOC       B     /* must be complementary pin to DD_VSYNC_TIOC A->B, C->D */ 

/* Timer Channels used for Horizontal Data Enable Signal (ExDMA request) */
#define DD_HDEN_CHANNEL    0
#define DD_HDEN_TIOC       A     /* must be A or C because of PWM1 operation */
#define DD_HDEN2_CHANNEL   0     /* must be same as DD_HDEN_CHANNEL */
#define DD_HDEN2_TIOC      B     /* must be complementary pin to DD_HDEN_TIOC A->B, C->D */

/* (optional) Timer Channels used for Panel Data Enable Signal  */
/* This channel can eliminate need for external multiplexor when using SRAM on panels that require DEN signal + HSYNCH.
   In this case HDEN sources EDREQ and PDEN drives panel DEN  (see app note) */
//#define DD_PDEN_CHANNEL    0
//#define DD_PDEN_TIOC       C     /* must be A or C because of PWM1 operation */
//#define DD_PDEN2_CHANNEL   0     /* must be same as DD_PDEN_CHANNEL */
//#define DD_PDEN2_TIOC      S     /* must be complementary pin to DD_PDEN_TIOC A->B, C->D */


