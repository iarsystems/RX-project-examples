/* Adapted for use with IAR Embedded Workbench */
/******************************************************************************
* DISCLAIMER:
* The software supplied by Renesas Technology Europe Ltd is
* intended and supplied for use on Renesas Technology products.
* This software is owned by Renesas Technology Europe, Ltd. Or
* Renesas Technology Corporation and is protected under applicable
* copyright laws. All rights are reserved.
*
* THIS SOFTWARE IS PROVIDED "AS IS". NO WARRANTIES, WHETHER EXPRESS,
* IMPLIED OR STATUTORY, INCLUDING BUT NOT LIMITED TO IMPLIED
* WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
* APPLY TO THIS SOFTWARE. RENESAS TECHNOLOGY AMERICA, INC. AND
* AND RENESAS TECHNOLOGY CORPORATION RESERVE THE RIGHT, WITHOUT
* NOTICE, TO MAKE CHANGES TO THIS SOFTWARE. NEITHER RENESAS
* TECHNOLOGY AMERICA, INC. NOR RENESAS TECHNOLOGY CORPORATION SHALL,
* IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL, OR
* CONSEQUENTIAL DAMAGES FOR ANY REASON WHATSOEVER ARISING OUT OF THE
* USE OR APPLICATION OF THIS SOFTWARE.
******************************************************************************/
/***********************************************************************************
FILE NAME    : main.h

DESCRIPTION  : Main header file

Copyright   : 2008 Renesas Technology Europe Ltd.
        All Rights Reserved
***********************************************************************************/

/***********************************************************************************
Revision History
DD.MM.YYYY OSO-UID Description
19.06.2008 RTE-DPR First Release.
***********************************************************************************/

#ifndef MAIN_H_INCLUDED
#define MAIN_H_INCLUDED

/***********************************************************************************
Defines
***********************************************************************************/
#define DEMO_BALLS      1
#define DEMO_GAME      2
#define DEMO_MANDELBROT    4

/***********************************************************************************
Globals
***********************************************************************************/
extern unsigned char g_ucDryFlag;
extern unsigned char g_ucRunHid;
extern unsigned char g_ucDemoSel;
extern unsigned char g_ucVncLastDemo;

/***********************************************************************************
Function Prototypes
***********************************************************************************/
void main(void);   

void ResumeMandelbrot(void);
void SuspendMandelbrot(void);
void ResumeBalls(void);
void SuspendBalls(void);
void ResumePong(void);
void SuspendPong(void);
void SuspendAllDemos(void);

void SetLed0(int _iOn);
void SetLed1(int _iOn);
void SetLed2(int _iOn);


#endif  /* MAIN_H_INCLUDED */
