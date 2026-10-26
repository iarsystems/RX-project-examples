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
/* Copyright (C) 2011 Renesas Electronics Corporation. All rights reserved.   */
/*******************************************************************************
* File Name    : temp_sensor.h
* Version     : 1.00
* Device     : R5F52108DFP
* Tool Chain   : IAR Embedded Workbench
* H/W Platform  : RSKRX210
* Description   : Defines macros & prototypes for functions controlling 
*          temperature sensor & ADC.
********************************************************************************/
/*******************************************************************************
* History     : 05.08.2011  Ver. 1.00 First Release
*******************************************************************************/

/*******************************************************************************
* Macro Definitions
*******************************************************************************/
/* Multiple inclusion prevention macro */
#ifndef TEMP_SENSOR_H
#define TEMP_SENSOR_H

/* Defines the supply voltage as 3.3V */
/* If using supply voltage other than 3.3V please modify this macro
   accordingly  */
#define SUPPLY_V  3.3F

/* Disclaimer: Please note that the values mentioned below are experimental 
   values. Please refer to the RX210 CPU hardware manual sec. 34.3.1 
   (R01UH0037EJ0090 Rev.0.90) for more details on how to calculate/calibrate 
   these values by experiment. */

/* Defines temperature reference 1 */
#define TEMP_T1    5.0F
/* Defines voltage correspond to temperature reference 1 */
#define TEMP_V1    1.383F
/* Defines temperature reference 2 */
#define TEMP_T2    55.0F
/* Defines voltage correspond to temperature reference 2 */
#define TEMP_V2    2.129F
/* Defines calculated value of slope. */
#define  TEMP_SLOPE  ((TEMP_V2 - TEMP_V1)/(TEMP_T2-TEMP_T1))

/*******************************************************************************
* Global Function Prototypes
*******************************************************************************/
/* Temp. sensor & ADC initialisation function prototype declaration */
void Init_TempSensor(void);

/* End of multiple inclusion prevention macro */
#endif
