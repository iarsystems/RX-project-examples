/*""FILE COMMENT""*******************************************************************
* File Name     : r_target_io.h
* Version       : 1.00
* Device(s)     : SH7216
* Tool-Chain    : SuperH Family C/C++ Compiler Package V.9.04 Release 01
* H/W Platform  : SH7216 CPU Board
* Description   : TFAT FS define i/o header file
*************************************************************************************
* History       : DD.MM.YYYY Version Description
*""FILE COMMENT END""****************************************************************/
#ifndef _R_TARGET_IO_H_
#define _R_TARGET_IO_H_

/******************************************************************************
Includes <System Includes> , "Project Includes"
******************************************************************************/
#include <machine.h>
/******************************************************************************
Macro definitions
******************************************************************************/
#define ENABLE_IRQ(value)	do{\
	set_imask(value);			\
}while(0)

#define DISABLE_IRQ(value)	do{	\
	value = get_imask();		\
	set_imask(15);				\
}while(0)

#define LED_INIT()	do{\
	PE.DR.BIT.B9 = 0;	\
	PFC.PEIORL.BIT.B9 = 1;	\
	PE.DR.BIT.B11 = 0;	\
	PFC.PEIORL.BIT.B11 = 1;	\
	PE.DR.BIT.B12 = 0;	\
	PFC.PEIORL.BIT.B12 = 1;	\
	PE.DR.BIT.B13 = 0;	\
	PFC.PEIORL.BIT.B13 = 1;	\
}while(0)

#define	LED0_ON()	(PE.DR.BIT.B9 = 1)
#define	LED0_OFF()	(PE.DR.BIT.B9 = 0)
#define	LED1_ON()	(PE.DR.BIT.B11 = 1)
#define	LED1_OFF()	(PE.DR.BIT.B11 = 0)
#define	LED2_ON()	(PE.DR.BIT.B12 = 1)
#define	LED2_OFF()	(PE.DR.BIT.B12 = 0)
#define	LED3_ON()	(PE.DR.BIT.B13 = 1)
#define	LED3_OFF()	(PE.DR.BIT.B13 = 0)

#endif	/*	#ifndef _R_TARGET_IO_H_	*/
