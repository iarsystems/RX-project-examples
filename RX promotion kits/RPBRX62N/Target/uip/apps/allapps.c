/* Adapted for use with IAR Embedded Workbench */
/*******************************************************************************
* Copyright (C) 2010 Renesas Electronics Corporation. All rights reserved.    */ 
/*******************************************************************************
* File Name    : allapps.c
* Version     : 1.00
* Device     : R5F562N8
* Tool Chain   : IAR Embedded Workbench
* H/W Platform  : RSK+RX62N
* Description   : High level Multi Tasking Framework:
*                 definitions of tasks, initialisation
*                 of tasks, start/suspend/exit tasks,
* Author        : Renesas
******************************************************************************/
/******************************************************************************
* History     : 17.08.2010 Ver. 1.00 First Release
******************************************************************************/


// Renesas ++
#include <string.h>

#include "uip.h"
#include "httpd.h"
#include "httpd-fs.h"
#include "httpd-cgi.h"
#include "http-strings.h"

#include "screen.h"
#include "vnc-server.h"
#include "vnc-out.h"

/*---------------------------------------------------------------------------*/
void allapps_appcall(void)
{
  switch(uip_conn->lport) {    
  case HTONS(5900):
  case HTONS(5901):
  case HTONS(5800):
  case HTONS(5801):
     vnc_server_appcall_vs();
     break;
  case HTONS(80):
     httpd_appcall();
   break;
  }
}
/*---------------------------------------------------------------------------*/
void allapps_init(void)
{
  uip_listen(HTONS(80));
  uip_listen(HTONS(5900));
  uip_listen(HTONS(5800));
}
/*---------------------------------------------------------------------------*/
