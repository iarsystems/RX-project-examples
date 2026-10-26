/*******************************************************************************
 * DISCLAIMER
 * This software is supplied by Renesas Electronics Corporation and is only
 * intended for use with Renesas products. No other uses are authorized. This
 * software is owned by Renesas Electronics Corporation and is protected under
 * all applicable laws, including copyright laws.
 * THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES REGARDING
 * THIS SOFTWARE, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING BUT NOT
 * LIMITED TO WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE
 * AND NON-INFRINGEMENT. ALL SUCH WARRANTIES ARE EXPRESSLY DISCLAIMED.
 * TO THE MAXIMUM EXTENT PERMITTED NOT PROHIBITED BY LAW, NEITHER RENESAS
 * ELECTRONICS CORPORATION NOR ANY OF ITS AFFILIATED COMPANIES SHALL BE LIABLE
 * FOR ANY DIRECT, INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES FOR
 * ANY REASON RELATED TO THIS SOFTWARE, EVEN IF RENESAS OR ITS AFFILIATES HAVE
 * BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
 * Renesas reserves the right, without notice, to make changes to this software
 * and to discontinue the availability of this software. By using this software,
 * you agree to the additional terms and conditions found by accessing the
 * following link:
 * http://www.renesas.com/disclaimer
 *
 * Copyright (C) 2016 Renesas Electronics Corporation. All rights reserved.
 *******************************************************************************/
/*******************************************************************************
 * System Name  : RX65N group, RX651 group GLCDC sample program.
 * File Name    : main.c
 * Version      : 0.50
 * Device       : R5F565NEDDFC(RX65N Group)
 * Abstract     : Example of image display on TFT-LCD panel using graphic LCD controller
 * Tool-Chain   : e2 studio Version 5.2.0.020
 *              : C/C++ Compiler Package for RX Family (V.2.05.00)
 * OS           : not use
 * H/W Platform : Renesas Starter Kit+ for RX65N
 * Description  : After initializing GLCDC, display picture is changed corresponding to pressed witch.
 * Limitation   : none
 *******************************************************************************/
/*******************************************************************************
 * History      : DD.MM.YYYY Version  Description
 *              : 23.12.2016 0.50     First Release
 *******************************************************************************/

/*******************************************************************************
 Includes <System Includes> , "Project Includes"
 *******************************************************************************/
#include <machine.h>
#include <stdint.h>
#include <string.h>

#include "platform.h"
//#include "r_cg_mtu3.h"

#include "r_glcdc_rx_if.h"
#include "r_pinset.h"

/*******************************************************************************
 Macro definitions
 *******************************************************************************/
/* Switch */
#define SW1_PIDR    (PORT0.PIDR.BIT.B3)
#define SW2_PIDR    (PORT0.PIDR.BIT.B5)

#define PUSH_SW1  (1)    /* Next image */
#define PUSH_SW2  (2)    /* Back image */
#define PUSH_NONE (-1)   /* No Push */

/* Image info */
#define BMP_CLUT_TABLE_OFFSET (54)
#define BMP_IMAGE_DATA_OFFSET (1078)

#define BMP_IMAGE_DATA_SIZE   (97152)
#define BMP_IMAGE_WIDTH       (384)

#define IMAGE_RELOCATION_ADDR (0x00800000)
#define IMAGE_BASE_ADDR       (IMAGE_RELOCATION_ADDR + (BMP_IMAGE_DATA_SIZE - BMP_IMAGE_WIDTH))


/*******************************************************************************
 Imported global variables and functions (from other files)
 *******************************************************************************/
/* Bitmap image access pointer */
extern uint8_t g_pict_data1[];

/*******************************************************************************
 Exported global variables and functions (to be accessed by other files)
 *******************************************************************************/
glcdc_cfg_t        glcdc_init_cfg;        /* Arguments of R_GLCDC_Open function */
uint32_t           gr_clut_table[256];    /* Color palette */
uint8_t * gp_pict_table[1] = {g_pict_data1};

static volatile bool first_interrupt_flag;

const gamma_correction_t g_gamma_table =
{
  /* gain (r = 1.00) */
  { 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024 },

  /* threshold */
  {   64,  128,  192,  256,  320,  384,  448,  512,  576,  640,  704,  768,  832,  896,  960 }
};

unsigned char table_count;

void lcd_main(void);

/*******************************************************************************
 Private variables and functions
 *******************************************************************************/
static void glcdc_initialize(void);
static void glcdc_port_setting(void);
static void bmp_image_load(uint8_t * p_image_data, uint32_t dest_addr, uint32_t * p_clut_data);
static void glcdc_callback(void *pdata);

/*******************************************************************************
 * Outline      : Main processing routine
 * Header       : none
 * Function Name: main
 * Description  : After initializing GLCDC, display picture is changed corresponding to pressed witch.
 * Arguments    : none
 * Return Value : none
 *******************************************************************************/
void lcd_main(void)
{

    /* ---- Initialization of the GLCDC , and start display ---- */
    glcdc_initialize ();

} /* End of function main() */


/*******************************************************************************
 * Outline      : GLCDC Initial Setting and Operation Start
 * Header       : none
 * Function Name: glcdc_initialize
 * Description  : Prepare image data, set GLDCD ports, initialize GLCDC and start GLCDC operation
 * Arguments    : none
 * Return Value : none
 *******************************************************************************/
static void glcdc_initialize (void)
{
    glcdc_err_t ret;

    /* Graphics data load */
    bmp_image_load (gp_pict_table[0], (uint32_t) IMAGE_RELOCATION_ADDR, gr_clut_table);

    /*-- Open parameter configure --*/
    /* Graphic 2 setting */
    glcdc_init_cfg.input[GLCDC_FRAME_LAYER_2].p_base = (uint32_t *) IMAGE_BASE_ADDR;
    glcdc_init_cfg.input[GLCDC_FRAME_LAYER_2].hsize = BMP_IMAGE_WIDTH;
    glcdc_init_cfg.input[GLCDC_FRAME_LAYER_2].vsize = 253;
    glcdc_init_cfg.input[GLCDC_FRAME_LAYER_2].offset = (BMP_IMAGE_WIDTH * -1);
    glcdc_init_cfg.input[GLCDC_FRAME_LAYER_2].format = GLCDC_IN_FORMAT_CLUT8;
    glcdc_init_cfg.input[GLCDC_FRAME_LAYER_2].frame_edge = false;
    glcdc_init_cfg.input[GLCDC_FRAME_LAYER_2].coordinate.x = 16+32;
    glcdc_init_cfg.input[GLCDC_FRAME_LAYER_2].coordinate.y = 9;
    glcdc_init_cfg.input[GLCDC_FRAME_LAYER_2].bg_color.argb = 0x00CCCCCC;

    glcdc_init_cfg.blend[GLCDC_FRAME_LAYER_2].visible = true;
    glcdc_init_cfg.blend[GLCDC_FRAME_LAYER_2].blend_control = GLCDC_BLEND_CONTROL_NONE;
    glcdc_init_cfg.blend[GLCDC_FRAME_LAYER_2].fixed_blend_value = 0x00;
    glcdc_init_cfg.blend[GLCDC_FRAME_LAYER_2].fade_speed = 0x00;
    glcdc_init_cfg.blend[GLCDC_FRAME_LAYER_2].frame_edge = false;
    glcdc_init_cfg.blend[GLCDC_FRAME_LAYER_2].start_coordinate.x = 0;
    glcdc_init_cfg.blend[GLCDC_FRAME_LAYER_2].start_coordinate.y = 0;
    glcdc_init_cfg.blend[GLCDC_FRAME_LAYER_2].end_coordinate.x = 0;
    glcdc_init_cfg.blend[GLCDC_FRAME_LAYER_2].end_coordinate.y = 0;

    glcdc_init_cfg.chromakey[GLCDC_FRAME_LAYER_2].enable = false;
    glcdc_init_cfg.chromakey[GLCDC_FRAME_LAYER_2].before.byte.g = 0x00;
    glcdc_init_cfg.chromakey[GLCDC_FRAME_LAYER_2].before.byte.b = 0x00;
    glcdc_init_cfg.chromakey[GLCDC_FRAME_LAYER_2].before.byte.r = 0x00;
    glcdc_init_cfg.chromakey[GLCDC_FRAME_LAYER_2].after.byte.a = 0x00;
    glcdc_init_cfg.chromakey[GLCDC_FRAME_LAYER_2].after.byte.g = 0x00;
    glcdc_init_cfg.chromakey[GLCDC_FRAME_LAYER_2].after.byte.b = 0x00;
    glcdc_init_cfg.chromakey[GLCDC_FRAME_LAYER_2].after.byte.r = 0x00;

    glcdc_init_cfg.clut[GLCDC_FRAME_LAYER_2].enable = true;
    glcdc_init_cfg.clut[GLCDC_FRAME_LAYER_2].p_base = (uint32_t *) gr_clut_table;
    glcdc_init_cfg.clut[GLCDC_FRAME_LAYER_2].size = 256;
    glcdc_init_cfg.clut[GLCDC_FRAME_LAYER_2].start = 0;

    /* Graphic 1 setting */
    glcdc_init_cfg.input[GLCDC_FRAME_LAYER_1].p_base = NULL; /* Base address to the frame buffer.
                                                              If p_base is NULL, the setting for graphic 1 is invalid. */


    /* Output timing */
    glcdc_init_cfg.output.htiming.front_porch = 3;
    glcdc_init_cfg.output.htiming.back_porch = 2;
    glcdc_init_cfg.output.htiming.display_cyc = 480;
    glcdc_init_cfg.output.htiming.sync_width = 41;

    glcdc_init_cfg.output.vtiming.front_porch = 2;
    glcdc_init_cfg.output.vtiming.back_porch = 2;
    glcdc_init_cfg.output.vtiming.display_cyc = 272;
    glcdc_init_cfg.output.vtiming.sync_width = 10;

    /* Output format */
    glcdc_init_cfg.output.format = GLCDC_OUT_FORMAT_16BITS_RGB565;
    glcdc_init_cfg.output.endian = GLCDC_ENDIAN_LITTLE;
    glcdc_init_cfg.output.color_order = GLCDC_COLOR_ORDER_BGR;
    glcdc_init_cfg.output.data_enable_polarity = GLCDC_SIGNAL_POLARITY_HIACTIVE;
    glcdc_init_cfg.output.hsync_polarity = GLCDC_SIGNAL_POLARITY_LOACTIVE;
    glcdc_init_cfg.output.vsync_polarity = GLCDC_SIGNAL_POLARITY_LOACTIVE;
    glcdc_init_cfg.output.sync_edge = GLCDC_SIGNAL_SYNC_EDGE_RISING;
    glcdc_init_cfg.output.bg_color.argb = 0x00CCCCCC;

    /* Output pin */
    glcdc_init_cfg.output.tcon_hsync = GLCDC_TCON_PIN_2;
    glcdc_init_cfg.output.tcon_vsync = GLCDC_TCON_PIN_0;
    glcdc_init_cfg.output.tcon_de = GLCDC_TCON_PIN_3;

    /* Output clock */
    glcdc_init_cfg.output.clksrc = GLCDC_CLK_SRC_INTERNAL;
    glcdc_init_cfg.output.clock_div_ratio = GLCDC_PANEL_CLK_DIVISOR_24;

    /* Correction circuit sequence */
    glcdc_init_cfg.output.correction_proc_order = GLCDC_BRIGHTNESS_CONTRAST_TO_GAMMA;

    /* Brightness */
    glcdc_init_cfg.output.brightness.enable = true;

    glcdc_init_cfg.output.brightness.r = 0x200;
    glcdc_init_cfg.output.brightness.g = 0x200;
    glcdc_init_cfg.output.brightness.b = 0x200;

    /* Contrast */
    glcdc_init_cfg.output.contrast.enable = true;

    glcdc_init_cfg.output.contrast.r = 0x80;
    glcdc_init_cfg.output.contrast.g = 0x80;
    glcdc_init_cfg.output.contrast.b = 0x80;

    /* Gamma */
    glcdc_init_cfg.output.gamma.enable = true;

    glcdc_init_cfg.output.gamma.p_r = (gamma_correction_t *) &g_gamma_table;
    glcdc_init_cfg.output.gamma.p_g = (gamma_correction_t *) &g_gamma_table;
    glcdc_init_cfg.output.gamma.p_b = (gamma_correction_t *) &g_gamma_table;

    /* Dethering */
    glcdc_init_cfg.output.dithering.dithering_on = true;
    glcdc_init_cfg.output.dithering.dithering_mode = GLCDC_DITHERING_MODE_2X2PATTERN;
    glcdc_init_cfg.output.dithering.dithering_pattern_a = GLCDC_DITHERING_PATTERN_11;
    glcdc_init_cfg.output.dithering.dithering_pattern_b = GLCDC_DITHERING_PATTERN_00;
    glcdc_init_cfg.output.dithering.dithering_pattern_c = GLCDC_DITHERING_PATTERN_10;
    glcdc_init_cfg.output.dithering.dithering_pattern_d = GLCDC_DITHERING_PATTERN_01;

    /* Detection */
    glcdc_init_cfg.detection.vpos_detect = true;
    glcdc_init_cfg.detection.gr1uf_detect = true;
    glcdc_init_cfg.detection.gr2uf_detect = true;

    /* Interrupt */
    glcdc_init_cfg.interrupt.vpos_enable = true;
    glcdc_init_cfg.interrupt.gr1uf_enable = true;
    glcdc_init_cfg.interrupt.gr2uf_enable = true;

    glcdc_init_cfg.p_callback = (void (*) (void *)) glcdc_callback;


    /* Initialize a first time interrupt flag */
    /* Unintended specified line notification from graphic 2 and graphic 1,2 underflow is detected only
    for first time after release GLCDC software reset.
    This variable is a flag to skip the first time interrupt processing.
    Refer to Graphic LCD Controller (GLCDC) section of User's Manual: Hardware for details. */
    first_interrupt_flag = false;


    /* Initialize */
    ret = R_GLCDC_Open(&glcdc_init_cfg);
    if (GLCDC_SUCCESS != ret)
    {
        while (1)
        {
            nop();
        }
    }

    /* GLCDC port setting */
    glcdc_port_setting();

    /* Start displaying graphics */
    ret = R_GLCDC_Control(GLCDC_CMD_START_DISPLAY, NULL);
    if (GLCDC_SUCCESS != ret)
    {
        while (1)
        {
            nop();
        }
    }

} /* End of function glcdc_initialize() */

/*******************************************************************************
 * Outline      : GLCDC Port Setting
 * Header       : none
 * Function Name: glcdc_port_setting
 * Description  : Set GLCDC ports
 * Arguments    : none
 * Return Value : none
 *******************************************************************************/
static void glcdc_port_setting (void)
{
    /* ---- Port setting ---- */

    /* LCD back-light and display-on */
    PORTB.PODR.BIT.B7 = 0;
    PORT9.PODR.BIT.B7 = 0;
    PORTB.PDR.BIT.B7 = 1;
    PORT9.PDR.BIT.B7 = 1;

    /* GLCDC */
    R_GLCDC_PinSet();

    /* LCD back light and display on */
    PORTB.PODR.BIT.B7 = 1;      /* Back light */
    PORT9.PODR.BIT.B7 = 1;      /* Display */

} /* End of function glcdc_port_setting() */

/*******************************************************************************
 * Outline      : Read image data
 * Header       : none
 * Function Name: bmp_image_load
 * Description  : Extract color palette data and image data from the specified bitmap image (256 colors)
 *                and copy it to the specified address.
 * Arguments    : p_image_data -
 *                  Start address of bitmap image.
 *                dest_addr -
 *                  Destination address of image data.
 *                p_clut_data -
 *                  Destination address of color palette.
 * Return Value : none
 *******************************************************************************/
static void bmp_image_load(uint8_t * p_image_data, uint32_t dest_addr, uint32_t * p_clut_data)
{
    uint32_t i;
    uint32_t * p_local_clut_data;
    uint8_t * p_local_image_data;
    uint8_t * p_image_data_base;

    /* Set color palette table data address of bitmap image */
    p_local_clut_data = (uint32_t *) (p_image_data + BMP_CLUT_TABLE_OFFSET);

    /* Copy color palette table data */
    for (i = 0; i < 256; i++)
    {
        p_clut_data[i] = *p_local_clut_data;
        p_local_clut_data++;
    }

    /* Set image data address of bitmap image */
    p_local_image_data = (uint8_t *) (p_image_data + BMP_IMAGE_DATA_OFFSET);
    p_image_data_base = (uint8_t*) dest_addr;

    /* Copy image data to ram area */
    memcpy(p_image_data_base, p_local_image_data, (size_t)BMP_IMAGE_DATA_SIZE);

} /* End of function bmp_image_load() */

/*******************************************************************************/


/*******************************************************************************
 * Outline      : GLCDC callback function
 * Header       : none
 * Function Name: glcdc_callback
 * Description  : Be called from within GLCDC interrupt handling.
 *                Registering callback function with API function.
 * Arguments    : callback_event -
 *                  Generated interrupt event
 *                    GLCDC_EVENT_GR1_UNDERFLOW: Graphic 1 Underflow
 *                    GLCDC_EVENT_GR2_UNDERFLOW: Graphic 2 Underflow
 *                    GLCDC_EVENT_LINE_DETECTION: Graphic 2 Line Detection
 * Return Value : none
 * Note         : Unintended specified line notification from graphic 2 and graphic 1,2 underflow is detected
 *                only for first time after release GLCDC software reset.
 *                Therefore, do nothing with first VPOS interrupt processing after execution of R_GLCDC_Open function,
 *                execute user process from next interrupt.
 *                In this sample program, processing depend on detection is not done.
 *                When using on the user system, please incorporate the above processing.
 *******************************************************************************/
static void glcdc_callback (void * pdata)
{

    if (false == first_interrupt_flag)
    {
        first_interrupt_flag = true;
        /* do nothing */
    }
    else
    {
        glcdc_callback_args_t * pdecode;
        pdecode = (glcdc_callback_args_t *) pdata; /* cast pointer to glcdc_callback_args_t */

        if (GLCDC_EVENT_GR1_UNDERFLOW == pdecode->event)
        {
            nop(); /* GR1 underflow , if necessary */
        }
        else if (GLCDC_EVENT_GR2_UNDERFLOW == pdecode->event)
        {
            nop(); /* GR2 underflow , if necessary */
        }
        else /* GLCDC_EVENT_LINE_DETECTION */
        {
            nop(); /* GR2 line detection , if necessary */
        }
    }

} /* End of function glcdc_callback() */



/* End of File */
