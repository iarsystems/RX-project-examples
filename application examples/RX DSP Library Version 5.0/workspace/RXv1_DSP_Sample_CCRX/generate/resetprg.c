/* Adapted to IAR Embedded Workbench */
                                                                          
                                                                           
                                                                           
                                                                           
                                                                           
                                                                           
                                                                           
                                                                           
                                                                           
                                                                           
                                                                           
                                                                           
                                                                           
                                                                          
/*********************************************************************
*
* Device     : RX/RX600
*
* File Name  : resetprg.c
*
* Abstract   : Reset Program.
*
* History    : 1.00  (2009-08-07)
*            : 1.10  (2011-02-21)
*            : 1.11  (2011-06-20)
*            : 1.12  (2014-02-20)
*            : 1.20  (2014-10-22)
*            : 1.30  (2018-03-26)
*
* NOTE       : THIS IS A TYPICAL EXAMPLE.
*
* Copyright (C) 2009 (2011-2018) Renesas Electronics Corporation.
* All rights reserved.
*
*********************************************************************/

#include	<machine.h>

//#include	<stddef.h>					// Remove the comment when you use errno
//#include 	<stdlib.h>					// Remove the comment when you use rand()
#include	"typedefine.h"		// Define Types
//#include	"stacksct.h"		// Stack Sizes (Interrupt and User)

#ifdef __cplusplus
extern "C" {
#endif
int __low_level_init ( void );
#ifdef __cplusplus
}
#endif
void main(void);

//#ifdef __cplusplus				// Use SIM I/O
//extern "C" {
//#endif
//extern void _INIT_IOLIB(void);
//extern void _CLOSEALL(void);
//#ifdef __cplusplus
//}
//#endif

#define PSW_init  0x00010000	// PSW bit pattern
#define FPSW_init 0x00000000	// FPSW bit base pattern

//extern void srand(_UINT);		// Remove the comment when you use rand()
//extern _SBYTE *_s1ptr;				// Remove the comment when you use strtok()
		
//#ifdef __cplusplus				// Use Hardware Setup
//extern "C" {
//#endif
//extern void HardwareSetup(void);
//#ifdef __cplusplus
//}
//#endif
	
//#ifdef __cplusplus			// Remove the comment when you use global class object
//extern "C" {					// Sections C$INIT and C$END will be generated
//#endif
//extern void _CALL_INIT(void);
//extern void _CALL_END(void);
//#ifdef __cplusplus
//}
//#endif

		// output PowerON_Reset to PResetPRG section



int __low_level_init ( void )
{ 
#if (__RX_ISA_VERSION__ >= 2) || defined(__RXV2)

#endif


#ifdef __FPU
#ifdef __ROZ					// Initialize FPSW
#define _ROUND 0x00000001			// Let FPSW RMbits=01 (round to zero)
#else
#define _ROUND 0x00000000			// Let FPSW RMbits=00 (round to nearest)
#endif
#ifdef __DOFF
#define _DENOM 0x00000100			// Let FPSW DNbit=1 (denormal as zero)
#else
#define _DENOM 0x00000000			// Let FPSW DNbit=0 (denormal as is)
#endif
	set_fpsw(FPSW_init | _ROUND | _DENOM);
#endif



//	_INIT_IOLIB();					// Use SIM I/O

//	errno=0;						// Remove the comment when you use errno
//	srand((_UINT)1);					// Remove the comment when you use rand()
//	_s1ptr=NULL;					// Remove the comment when you use strtok()
		
//	HardwareSetup();				// Use Hardware Setup

//	_CALL_INIT();					// Remove the comment when you use global class object

	asm("POP R15");
	set_psw(PSW_init);
	asm("PUSH.L R15");
//	chg_pmusr();					// Remove the comment when you need to change PSW PMbit (SuperVisor->User)

	return (int)(1U);

//	_CLOSEALL();					// Use SIM I/O
	
//	_CALL_END();					// Remove the comment when you use global class object

	brk();
}

