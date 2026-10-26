/*********************************************************************
*
* Device     : RX/RX200
*
* File Name  : resetprg.c
*
* Abstract   : Reset Program.
*
* History    : 1.00  (2010-12-17)
*            : 1.10  (2011-02-21)
*            : 1.11  (2011-06-20)
*
* NOTE       : THIS IS A TYPICAL EXAMPLE.
*
* Copyright (C) 2010 (2011) Renesas Electronics Corporation.
* and Renesas Solutions Corp.
*
*********************************************************************/

#include    <machine.h>
//#include  <stddef.h>                  // Remove the comment when you use errno
//#include  <stdlib.h>                  // Remove the comment when you use rand()
#include    "typedefine.h"      // Define Types


//#ifdef __cplusplus                // Use SIM I/O
//extern "C" {
//#endif
//extern void _INIT_IOLIB(void);
//extern void _CLOSEALL(void);
//#ifdef __cplusplus
//}
//#endif

#define PSW_init  0x00010000    // PSW bit pattern
#define FPSW_init 0x00000000    // FPSW bit base pattern

//extern void srand(_UINT);     // Remove the comment when you use rand()
//extern _SBYTE *_s1ptr;                // Remove the comment when you use strtok()

//#ifdef __cplusplus                // Use Hardware Setup
//extern "C" {
//#endif
//extern void HardwareSetup(void);
//#ifdef __cplusplus
//}
//#endif

//#ifdef __cplusplus            // Remove the comment when you use global class object
//extern "C" {                  // Sections C$INIT and C$END will be generated
//#endif
//extern void _CALL_INIT(void);
//extern void _CALL_END(void);
//#ifdef __cplusplus
//}
//#endif
extern void usb_cpu_mcu_initialize();

int __low_level_init(void)
{
    /* MCU initialized */
    usb_cpu_mcu_initialize();

//  _INIT_IOLIB();                  // Use SIM I/O

//  errno=0;                        // Remove the comment when you use errno
//  srand((_UINT)1);                    // Remove the comment when you use rand()
//  _s1ptr=NULL;                    // Remove the comment when you use strtok()

//  HardwareSetup();                // Use Hardware Setup

//  _CALL_INIT();                   // Remove the comment when you use global class object

    set_psw(PSW_init);              // Set Ubit & Ibit for PSW

    return 1;
}
