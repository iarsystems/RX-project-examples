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
#include    "typedefine.h"      // Define Types

/* For Debug Console */
#include    "r_usb_usrconfig.h"

#ifdef __cplusplus
extern "C" {
#endif
void PowerON_Reset_PC(void);
void main(void);
#ifdef __cplusplus
}
#endif

//#ifdef __cplusplus                // Use SIM I/O
//extern "C" {
//#endif
//extern void _CLOSEALL(void);
//#ifdef __cplusplus
//}
//#endif

#define PSW_init  0x00010000    // PSW bit pattern
#define FPSW_init 0x00000000    // FPSW bit base pattern

extern void usb_cpu_mcu_initialize(void);
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
//}rb
//#endif

int __low_level_init(void)
{
    /* MCU initialized */
    usb_cpu_mcu_initialize();

//  errno=0;                        // Remove the comment when you use errno
//  srand((_UINT)1);                // Remove the comment when you use rand()
//  _s1ptr=NULL;                    // Remove the comment when you use strtok()

//  HardwareSetup();                // Use Hardware Setup

//  _CALL_INIT();                   // Remove the comment when you use global class object

    set_psw(PSW_init);              // Set Ubit & Ibit for PSW
//  chg_pmusr();                    // Remove the comment when you need to change PSW PMbit (SuperVisor->User)


    return 1;
}
