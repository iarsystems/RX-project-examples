/**
 * \file  bsp.h
 * \brief Header file defining vibcom board specific values
 *
 * This file defines vibcom specific configuration values
 *
 * \internal Author, version, and date are provided by SVN.  Do not edit \endinternal
 * $Author$
 *          
 * $Date$
 *
 * (C)2011 INOVA Geophysical Equipment
 * All Rights Reserved
 *
 */

#ifndef  BSP_H_
#define  BSP_H_

#include <stdint.h>
#include <stdbool.h>
#include "iorx62n.h"
#include "io_rx62n_port_define.h"


typedef void (*Function_ptr)(void);

// ---------------------------------------------------------------------------------------------------------
//                                            DEFINES
// ---------------------------------------------------------------------------------------------------------
#define  BSP_CPU_EXT_CLK_FREQ    12000000uL

// ---------------------------------------------------------------------------------------------------------
//                                            FUNCTION PROTOTYPES
// ---------------------------------------------------------------------------------------------------------
void        BSP_Init( void );
void        BSP_Board_init( void );
void        BSP_UART_init( void );
uint32_t    BSP_CPU_PerClkFreq(void);

// ---------------------------------------------------------------------------------------------------------
//                                            BSP SERVICES
// ---------------------------------------------------------------------------------------------------------
void BSP_Cfg_Clocks( void );
void BSP_Cfg_SDRAM( void );
void BSP_Cfg_Port_Function( void );

#endif //BSP_H_
