/******************************************************************************
* File Name     : Platform.h
* Version       : 1.0
* Device(s)     : R5F56108VNFP
* Tool-Chain    : RX Family C Compiler
* OS            : None
* H/W Platform  : YRDKRX62N or RDK RX62N
* Description   : Defines which platform is being built
* Operation     : 
* Limitations   : None
*******************************************************************************
* History : DD.MM.YYYY    Version   Description
*         : 10.Jul.2010   1.00      First release
*******************************************************************************/

/******************************************************************************
 Include files
******************************************************************************/

#ifndef _PLATFORM_H_
#define _PLATFORM_H_

#ifndef PLATFORM_RSK62N
#ifndef PLATFORM_RDK62N
#error NO PLATFORM DEFINED!
#endif
#endif

#ifdef PLATFORM_RSK62N
#include "rskrx62n.h"
#endif

#ifdef PLATFORM_RDK62N
#include "rdkrx62n.h"
#endif

#endif
