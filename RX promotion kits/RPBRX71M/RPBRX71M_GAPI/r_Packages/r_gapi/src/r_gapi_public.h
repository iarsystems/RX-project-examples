/******************************************************************************
* DISCLAIMER
* Please refer to http://www.renesas.com/disclaimer
******************************************************************************/

#ifndef R_GAPI_H
#define R_GAPI_H
/***********************************************************************************
FILE NAME    : r_GAPI.h

DESCRIPTION  : Structure definitions and function prototypes for Renesas Graphics API.

Copyright   : 2010 Renesas Electric America
Copyright   : 2010 Renesas Electric Corporation.
          All Rights Reserved
***********************************************************************************/

/* Typedef fundemental data types that may change between compilers/families */
#include <inttypes.h>
#include <stdbool.h>
#include <stddef.h>

typedef int16_t GAPI_Resp_t;
typedef struct GFont_struct GAPI_Font_t;
typedef struct BMP_struct BMP_t;

#include "r_gapiu_utility.h"
#include "r_gapic_core.h"
#include "r_gapiw_wrapper.h"

#endif
