/**
 * This file configures the Renesas Package Libraries.
 *  Definitions Global to all libraries are defined here.
 */
#ifndef CONFIG_R_PACKAGES_H
#define CONFIG_R_PACKAGES_H

#define XTAL_FREQUENCY (12000000L)
#define ICLK_MUL (8)
#define PCLK_MUL (4)
#define BCLK_MUL (4)
#define ICLK_FREQUENCY (XTAL_FREQUENCY * ICLK_MUL)
#define PCLK_FREQUENCY (XTAL_FREQUENCY * PCLK_MUL)
#define BCLK_FREQUENCY (XTAL_FREQUENCY * BCLK_MUL)

#endif
