/***********************************************************************************************************************
* DISCLAIMER
* This software is supplied by Renesas Electronics Corporation and is only intended for use with Renesas products. No 
* other uses are authorized. This software is owned by Renesas Electronics Corporation and is protected under all 
* applicable laws, including copyright laws. 
* THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES REGARDING
* THIS SOFTWARE, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING BUT NOT LIMITED TO WARRANTIES OF MERCHANTABILITY, 
* FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT. ALL SUCH WARRANTIES ARE EXPRESSLY DISCLAIMED. TO THE MAXIMUM 
* EXTENT PERMITTED NOT PROHIBITED BY LAW, NEITHER RENESAS ELECTRONICS CORPORATION NOR ANY OF ITS AFFILIATED COMPANIES 
* SHALL BE LIABLE FOR ANY DIRECT, INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES FOR ANY REASON RELATED TO THIS 
* SOFTWARE, EVEN IF RENESAS OR ITS AFFILIATES HAVE BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
* Renesas reserves the right, without notice, to make changes to this software and to discontinue the availability of 
* this software. By using this software, you agree to the additional terms and conditions found by accessing the 
* following link:
* http://www.renesas.com/disclaimer 
*
* Copyright (C) 2012 Renesas Electronics Corporation. All rights reserved.    
***********************************************************************************************************************/
/***********************************************************************************************************************
* File Name    : r_example_config.h
* Description  : Configures this code
************************************************************************************************************************
* History : DD.MM.YYYY Version Description           
*         : 18.01.2012 1.00    First Release     
*         : 29.02.2012 1.10    Removed references to 'grab' and replaced with 'lock'. This is clearer for non-English
*                              speakers. Also changed 'release' to 'unlock' to go along with previous statement.       
*         : 25.06.2012 1.20    Updated to match v0.7 Spec. Added use of BSP lock functions. Added interface header file
*                              and renamed r_example.h to r_example_private.h to show use of private header files. 
*                              Added examples of using BSP lock functions when module state is needed and when it is 
*                              not. Added R_EXAMPLE_GetVersion() macro in r_example_if.h.
*         : 18.09.2012 1.30    Updating for v1.00 spec. Renamed r_example_private.h to r_example.h since interface file
*                              is now the public one. Removed includes for stdint.h and stdbool.h since these are now
*                              included through platform.h. Added R_EXAMPLE_Close() function. Now uses global lock. 
*                              Moved R_EXAMPLE_GetVersion() to r_example.c since it can be inlined instead of using 
*                              function-like macro. Added R_EXAMPLE_ReadState() to show a 'getter' function that reads 
*                              the a static variable inside of a FIT module. Moved example_states_t typedef to 
*                              r_example_if.h.
*         : 27.09.2012 1.40    Added examples of using hardware and software locks.
***********************************************************************************************************************/
#ifndef EXAMPLE_CONFIG_HEADER_FILE
#define EXAMPLE_CONFIG_HEADER_FILE

/***********************************************************************************************************************
Configuration Options
***********************************************************************************************************************/
/* This defintion does this, that, and everything else. Give a good description of what will happen when this macro is
   used. Example, what implications are there for uncommenting the macro, commenting the macro, or changing the 
   definition of the macro.   
*/
//#define EXAMPLE_MACRO      (42)

#endif /* EXAMPLE_CONFIG_HEADER_FILE */


