/* Adapted for use with IAR Embedded Workbench */
/*""FILE COMMENT""*******************************************************
* System Name  : PDL driver API
* File Name    : r_pdl_definitions.h
* Version    : 0.10
* Contents    : API headers for RX610xV
* Customer    : 
* Model      : 
* Order      : 
* CPU      : RX600
* Compiler    : RXC
* OS      :
* Programmer  : 
* Note      : 
************************************************************************
* Copyright,2009 RENESAS TECHNOLOGY CORPORATION
* AND RENESAS SOLUTIONS CORPORATION
************************************************************************
* History    : 2010.02.17
*        : Ver 0.10
*""FILE COMMENT END""**************************************************/

#ifndef R_PDL_DEFS_RX610xV_H
#define R_PDL_DEFS_RX610xV_H

#define PDL_VERSION  0x0010u  /* v00.10 in BCD format */

#include "r_pdl_user_definitions.h"

/* Power-on Reset function */
extern void PowerON_Reset_PC(void);

#include "r_pdl_adc_10_RX610xV.h"
#include "r_pdl_bsc_RX610xV.h"
#include "r_pdl_cgc_RX610xV.h"
#include "r_pdl_cmt_RX610xV.h"
#include "r_pdl_crc_RX610xV.h"
#include "r_pdl_dac_10_RX610xV.h"
#include "r_pdl_dmac_RX610xV.h"
#include "r_pdl_dtc_RX610xV.h"
#include "r_pdl_iic_RX610xV.h"
#include "r_pdl_intc_RX610xV.h"
#include "r_pdl_io_port_RX610xV.h"
#include "r_pdl_lpc_RX610xV.h"
#include "r_pdl_mcu_RX610xV.h"
#include "r_pdl_pfc_RX610xV.h"
#include "r_pdl_ppg_RX610xV.h"
#include "r_pdl_sci_RX610xV.h"
#include "r_pdl_tmr_RX610xV.h"
#include "r_pdl_tpu_RX610xV.h"
#include "r_pdl_wdt_RX610xV.h"

#endif

/* End of file */
