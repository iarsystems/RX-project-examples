/* Adapted for use with IAR Embedded Workbench */
/*""FILE COMMENT""*************************************************************
* System Name  : PDL driver API
* File Name    : r_pdl_definitions.h
* Version    : 1.00
* Contents    : API headers for RX62G and RX62T (100-pin package)
* Customer    :
* Model      :
* Order      :
* CPU      : RX
* Compiler    : RXC
* OS      :
* Programmer  :
* Note      :
*******************************************************************************
* Copyright, 2012. Renesas Electronics Corporation and Renesas Solutions Corp.
*******************************************************************************
* History    : 2012.09.11
*        : Ver 1.00
*        : CS release.
*""FILE COMMENT END""*********************************************************/

#ifndef R_PDL_DEFS_H
#define R_PDL_DEFS_H

#define PDL_VERSION  0x0100u  /* v01.00 in BCD format */

#define DEVICE_PACKAGE_100_PIN

/* Check the precision setting */
#if (__DOUBLE__ != 64)
 #error "Double precision is not selected. Please change size of type double from 32 bits to 64 bits."
#endif

/* Peripherals */
#include "r_pdl_adc_10_RX62Gxx.h"
#include "r_pdl_adc_12_RX62Gxx.h"
#include "r_pdl_bsc_RX62Gxx.h"
#include "r_pdl_cgc_RX62Gxx.h"
#include "r_pdl_cmt_RX62Gxx.h"
#include "r_pdl_crc_RX62Gxx.h"
#include "r_pdl_dtc_RX62Gxx.h"
#include "r_pdl_gpt_RX62Gxx.h"
#include "r_pdl_iic_RX62Gxx.h"
#include "r_pdl_intc_RX62Gxx.h"
#include "r_pdl_io_port_RX62Gxx.h"
#include "r_pdl_iwdt_RX62Gxx.h"
#include "r_pdl_lin_RX62Gxx.h"
#include "r_pdl_lpc_RX62Gxx.h"
#include "r_pdl_lvd_RX62Gxx.h"
#include "r_pdl_mcu_RX62Gxx.h"
#include "r_pdl_mtu3_RX62Gxx.h"
#include "r_pdl_pfc_RX62Gxx.h"
#include "r_pdl_poe_RX62Gxx.h"
#include "r_pdl_sci_RX62Gxx.h"
#include "r_pdl_spi_RX62Gxx.h"
#include "r_pdl_wdt_RX62Gxx.h"

#endif
/* End of file */
