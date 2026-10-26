/* Adapted for use with IAR Embedded Workbench */
/*""FILE COMMENT""*******************************************************
* System Name  : PDL driver API
* File Name    : r_pdl_definitions.h
* Version    : 1.01
* Contents    : API headers for RX62N (176-pin package)
* Customer    : 
* Model      : 
* Order      : 
* CPU      : RX600
* Compiler    : RXC
* OS      :
* Programmer  : 
* Note      : 
************************************************************************
* Copyright, 2011. Renesas Electronics Corporation
* and Renesas Solutions Corporation
************************************************************************
* History    : 2011.02.18
*        : Ver 1.01
*        : CS-4 release.
*""FILE COMMENT END""**************************************************/

#ifndef R_PDL_DEFS_H
#define R_PDL_DEFS_H

#define PDL_VERSION  0x0101u  /* v01.01 in BCD format */
#define DEVICE_PACKAGE_LFBGA_176

/* Peripiherals that are common to all device packages */
#include "r_pdl_adc_10_rx62nxx.h"
#include "r_pdl_adc_12_rx62nxx.h"
#include "r_pdl_bsc_rx62nxx.h"
#include "r_pdl_cgc_rx62nxx.h"
#include "r_pdl_cmt_rx62nxx.h"
#include "r_pdl_crc_rx62nxx.h"
#include "r_pdl_dac_10_rx62nxx.h"
#include "r_pdl_dmac_rx62nxx.h"
#include "r_pdl_dtc_rx62nxx.h"
#include "r_pdl_exdmac_rx62nxx.h"
#include "r_pdl_intc_rx62nxx.h"
#include "r_pdl_iwdt_RX62Nxx.h"
#include "r_pdl_lpc_rx62nxx.h"
#include "r_pdl_lvd_RX62Nxx.h"
#include "r_pdl_mcu_rx62nxx.h"
#include "r_pdl_mtu2_rx62nxx.h"
#include "r_pdl_pfc_rx62nxx.h"
#include "r_pdl_poe_rx62nxx.h"
#include "r_pdl_ppg_rx62nxx.h"
#include "r_pdl_rtc_RX62Nxx.h"
#include "r_pdl_sci_rx62nxx.h"
#include "r_pdl_spi_rx62nxx.h"
#include "r_pdl_tmr_rx62nxx.h"
#include "r_pdl_wdt_rx62nxx.h"

/* Peripiherals that are device package-dependent */
#include "r_pdl_iic_rx62nxnfp.h"
#include "r_pdl_io_port_rx62nxbg.h"

#endif
/* End of file */
