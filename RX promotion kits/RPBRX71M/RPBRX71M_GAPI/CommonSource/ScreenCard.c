/* Adapted to IAR Embedded Workbench */
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
 * Copyright (C) 2015 Renesas Electronics Corporation. All rights reserved.
 *******************************************************************************/
/*******************************************************************************
 * File Name    : ScreenCard.c
 * Version      : 1.0 <- Optional as long as history is shown below
 * Description  : Screen handler showing business card demonstration demo
 *    Expected Image Sizes:
 *       Logo & Photo - JPEG  150 x 110 pixels
 *       Banner Logo  - JPEG  320 x 100 pixels
 *******************************************************************************/
/* Screen Business Card Demo */
/************************************************************************
 Revision History
 DD.MM.YYYY OSO-UID Description
 12.04.2016 v1.0    First Release
 ************************************************************************/
#include <string.h>
#include "Global.h"
#include "LCD_demo.h"
#include "r_tfat_lib.h"
#include "r_expand_jpegd.h"
#include "r_config_parse.h"

/* Declarations */
#define TEXT_IMAGE_X (1000)             /* X - Dim. of BMP area for the scrolling text */
#define TEXT_IMAGE_Y (60)               /* Y - Dim. of BMP area for the scrolling text */

#define LOGO_ID ("[logo]\r\n")          /* Customer Logo config identifier */
#define PHOTO_ID ("[photo]\r\n")        /* Photo config identifier */
#define NAME_ID ("[name]\r\n")          /* Person Name config identifier */
#define TITLE_ID ("[title]\r\n")        /* Title and Company Name config identifier */
#define BANNER_ID ("[banner]\r\n")      /* Large banner config identifier */

#define TXT_MAX_CHARS (30)                                      /* Maximum number of chars in scrolling and company text*/
#define TXT_MAX_EXCEEDED_MSG ("Err: Too many characters.. ")    /* Error message to display if char count exceeded */

#define SCROLL_RATE        (15)            /* ms between scroll function is called*/

#define SCROLL_TXT_AREAX   (270)           /* X Size of area on screen of scrolling text */
#define SCROLL_TXT_AREAY   (TEXT_IMAGE_Y)  /* Y Size of area on screen of scrolling text */

#define SCROLL_TXT_LOCX    ((int16_t)((DD_FRAME_WIDTH/2) - (SCROLL_TXT_AREAX/2))) /* X Location on screen of scrolling text */
#define SCROLL_TXT_LOCY    (57)                                                   /* Y Location on screen of scrolling text */

#define COMPANY_TXT_LOCY   (45)       /* Y Location on screen of company name (X-location is calculated on the fly) */

#define RENESAS_LOGO_X (195)         /* Location of small Renesas Logo bmp */
#define RENESAS_LOGO_Y (160)

#define RENESAS_BANNER_X (2)         /* Location of Renesas banner bmp */
#define RENESAS_BANNER_Y (155)

/* Note Logo 1, Logo 2 & Banner are JPEG images
 * JPG coordinate calculations:
 * Origin is top left if source image.
 * Y value: ((Display width * (Y-coordiante-1) )
 * X value: X coordinate on screen.
 */
#define LOGO_1_X (5)                    /* Location of logo 1 */
#define LOGO_1_Y (240)
#define LOGO_1_JPG_COORD (((DD_H_DOT_DISPLAY * (LOGO_1_Y-1)) ) + LOGO_1_X)

#define LOGO_2_X (165)                  /* Location of logo 2 */
#define LOGO_2_Y (240)
#define LOGO_2_JPG_COORD (((DD_H_DOT_DISPLAY * (LOGO_2_Y-1)) ) + LOGO_2_X)

#define BANNER_LGO_X  (0)              /* Location of user's large logo */
#define BANNER_LGO_Y  (240)
#define BANNER_JPG_COORD (((DD_H_DOT_DISPLAY * (BANNER_LGO_Y-1))) + BANNER_LGO_X)

#define SCROLL_TXT_MSG   ("Default Text...   ")       /* Scrolling Text to display when not found in config file */
#define TXT_NO_CONFIG    ("Config File Missing...")   /* Text to display if config file is missing */

#define MAX_LOGO_FILE_SIZE (sizeof(ecc_buffer))       /* Maximum size that the users logo file can be */
#define LOGO_FILE_ERROR  ("CardTooBig")               /* Resource to display if the logo file size exceeds available memory */

/* Prototypes */
static void constructor (Screen_t const *pS);
static void destructor (Screen_t const *pS);
static void bus_card (ScreenObj_t const *pS, EVENT_MSG const *pMsg);
static portTickType card_scroll (void);
static void card_text (ScreenObj_t const *pS, EVENT_MSG const *pMsg);
static int16_t get_from_file_system (const char* file_name, uint8_t* buffer, const uint16_t buffer_size);
static void display_renesas_banner (void);
static void display_renesas_logo (void);
static int16_t display_jpg (char* image_id, uint32_t jpg_coord);
static void setup_user_images (void);
static void setup_scrolling_text (void);
static void setup_static_text (void);

/* Section definition for ECC RAM */

#pragma location=".ECC_RAM"
/* The ECC RAM is used as a single 32k-byte buffer for:
 *   Temporary stores the user's JPEG Images. Intermediary step from the file system to
 *    the display raster. */
__root static uint8_t ecc_buffer[32 * 1024];

static char scroll_txt_buffer[TXT_MAX_CHARS + 1]; /* Stores the scrolling text message after recovery from the file system */
static BMP_t back_frame_backup; /* Stores the backFrameBuf setting for recovery on exit of this page */
static uint8_t text_setup_rqd = 0; /* Flag indicating if the scrolling text BMP needs to be constructed */
static BMP_t const* pbmp_logo;

/* Font format for the scrolling text */
static const GAPI_PUT_t font_scroller =
{ &pfontSqBT_B60,
{ CT_NONE, CT_BLACK }, GAPI_F_SPACE_PROPORTIONAL, GAPI_F_SPACE_PROPORTIONAL,
{ GAPI_F_ALIGN_H_LEFT, GAPI_F_ALIGN_V_CENTER, 0, 0, GAPI_F_BACK_TRANSPARENT, GAPI_F_FORE_SOLID } };

/* Font format for the company name text */
static const GAPI_PUT_t font_company =
{ &pfontSqBT_18,
{ CT_NONE, CT_BLACK }, GAPI_F_SPACE_PROPORTIONAL, GAPI_F_SPACE_PROPORTIONAL,
{ GAPI_F_ALIGN_H_LEFT, GAPI_F_ALIGN_V_CENTER, 0, 0, GAPI_F_BACK_TRANSPARENT, GAPI_F_FORE_SOLID } };

static const uint32_t logo_positions[] =
{ LOGO_1_JPG_COORD, LOGO_2_JPG_COORD };

static const GAPI_Scheme_t t_card_status_scheme[] =
{
/* Button off */
{
{ CT_NONE, CT_WHITE },
{ &pfontSqBT_B12,
{ CT_NONE, CT_BLACK }, GAPI_F_SPACE_PROPORTIONAL, GAPI_F_SPACE_PROPORTIONAL,
{ GAPI_F_ALIGN_H_CENTER, GAPI_F_ALIGN_V_CENTER, 0, 0, GAPI_F_BACK_TRANSPARENT, GAPI_F_FORE_SOLID } },
{ CT_COLOR_NO_CHANGE, CT_TEXT_CENTER, GAPI_T_MULC } },

/* Button on */
{
{ CT_NONE, CT_NONE },
{ &pfontSqBT_B12,
{ CT_NONE, CT_RED }, GAPI_F_SPACE_PROPORTIONAL, GAPI_F_SPACE_PROPORTIONAL,
{ GAPI_F_ALIGN_H_CENTER, GAPI_F_ALIGN_V_CENTER, 0, 0, GAPI_F_BACK_TRANSPARENT, GAPI_F_FORE_SOLID } },
{ CT_COLOR_NO_CHANGE, CT_TEXT_CENTER, GAPI_T_NONE } } };

/* MUST INCLUDE NULL TERMINATED RECORD */
static const ScreenObj_t icons[] =
{
/*  Bit map Address     Color Scheme                 Function call     X Pos       Y Pos  */
/* Home Icon - as the Business Card is not part of the standard menu system this has not
 * been included. Uncomment if the home icon is needed in the future
 { &pBMP_HomeIcon,     t_card_home_scheme,          ButtonBack,         278,        207 }, */
{ &pbmp_logo, t_card_status_scheme, card_text, 111, 6 },
{ NULL, NULL, NULL, 0, 0 }, };

/* Shared event message */
static EVENT_MSG msg;

Screen_t g_screen_card_data =
{ constructor, destructor, icons };

/*****************************************************************************
 Function Name: constructor
 Description:   Clears the backbuffer and invokes the default constructor.
 Parameters:    pS - Screen Pointer (unused)
 Return value:  None
 *****************************************************************************/
static void constructor (Screen_t const *pS)
{
    static const BMP_ColorTable_t bg_color = CT_WHITE;

    /* locate and point to common resources */
    if (NULL == pfontTerminal)
    {
        pfontTerminal = FileFind(pResources, "Square721 BT_Roman_16");
    }

    if (NULL == pBMP_HomeIcon)
    {
        pBMP_HomeIcon = FileFind(pResources, "Home");
    }

    if (NULL == pfontSqBT_B12)
    {
        pfontSqBT_B12 = FileFind(pResources, "Square721 BT_Roman_B12");
    }

    if (NULL == pfontSqBT_18)
    {
        pfontSqBT_18 = FileFind(pResources, "Square721 BT_Roman_18");
    }

    if (NULL == pfontSqBT_B60)
    {
        pfontSqBT_B60 = FileFind(pResources, "Square721 BT_Roman_B60");
    }

    /* Clear the background */
    (void) R_GAPI_Fill(backFrameBuf, 0, 0, DD_FRAME_WIDTH, DD_FRAME_HEIGHT, &bg_color);

    /* Run default behaviour */
    BasicConstructor(pS);
}
/*******************************************************************************
 End of function constructor
 *******************************************************************************/

/**********************************************************************************
 Function Name: destructor
 Description:   Runs the default behaviour
 Parameters:    pS - Screen Pointer (unused)
 Return value:  None
 ***********************************************************************************/
static void destructor (Screen_t const *pS)
{
    static const BMP_ColorTable_t bg_color = CT_WHITE;

    /* Recover back frame buffer settings from backup taking in constructor */
    *backFrameBuf = back_frame_backup;

    /* Clear the background */
    (void) R_GAPI_Fill(backFrameBuf, 0, 0, DD_FRAME_WIDTH, DD_FRAME_HEIGHT, &bg_color);

    /* Redraw scrolling text next time in */
    text_setup_rqd = 0;

    /* Run default behaviour */
    BasicDestructor(pS);
}
/*******************************************************************************
 End of function destructor
 *******************************************************************************/

/*****************************************************************************
 Function Name: card_text
 Description:   System call - called on each event.
 On ME_DRAW event, setups up the demo and starts the text
 scrolling task.
 Parameters:    pS - Screen object (unused)
 pMsg - Message being passed in
 Return value:  None
 *****************************************************************************/
static void card_text (ScreenObj_t const *pS, EVENT_MSG const *pMsg)
{
    (void) pS; /* Acknowledge standard API variable is not used */

    if (pMsg->event == ME_DRAW)
    {
        /* ME_DRAW - initialisation of the screen */
        R_init_jpeg(); /* Initialise the JPED Decode library */

        /* Read the configuration file in to RAM */
        r_config_load_from_filesystem();

        setup_user_images(); /* Setup user images */

        setup_scrolling_text(); /* Setup the scrolling text */

        setup_static_text(); /* Setup the static text */

        /* This demo requires a large buffer to store the scrolling text string as a formed bitmap.
         * To reduce overall memory footprint the backFrameBuffer is re-tasked here to store
         * the scrolling text bitmap.
         */
        /* Take a backup of the frame buffer settings, recovered at exit */
        back_frame_backup = *backFrameBuf;

        /* Start the text scrolling... */
        (void) ScreenTaskStart(card_scroll);
    }

    if (pMsg->event == ME_DRAW_DISPLAY)
    {
        /* On the first re-draw, indicate the scrolling text can be setup */
        /* before this time the backDispBuf is being used for the initial screen
         * setup and risk corruption if we setup the backbuf too early    */
        text_setup_rqd = 1;
    }

}
/*******************************************************************************
 End of function card_text
 *******************************************************************************/

/**********************************************************************************
 Function Name:   bus_card
 Description:  Demo Card on Demo Home screen
 Parameters:   *pS
 *pMsg
 Return value:   None
 ***********************************************************************************/
static void bus_card (ScreenObj_t const *pS, EVENT_MSG const *pMsg)
{
    static int16_t state = 0;

    if (0 != (state = ButtonHandler(pS, pMsg, (uint8_t *) "CARD DEMO", state)))
    {
        if (ME_PRESS == pMsg->event)
        {
            /* send a screen change command to the handler */
            msg.source = MS_DISPLAY;
            msg.event = ME_SCREEN_PUSH;
            msg.param.pS = &g_screen_card_data;
            (void) (int16_t) xQueueSend(EventQueue, (void * )&msg, 0UL); /* RTOS_USAGE */
        }
    }
}
/*******************************************************************************
 End of function bus_card
 *******************************************************************************/

/*****************************************************************************
 Function Name: card_scroll
 Description:   Screen task - scrolls the text previously setup.
 Parameters:    None
 Return value:  TickType: Amount of time to sleep until next wakeup.
 *****************************************************************************/
static portTickType card_scroll (void)
{
    portTickType xdelay = port_mS_to_TICK(SCROLL_RATE); /* RTOS_USAGE */
    static const BMP_ColorTable_t bg_color = CT_WHITE;
    uint32_t text_box_mem;
    BMP_t* ptext_box_bmp = backFrameBuf;

    /* If this is the first time run - setup the scrolling text bitmap */
    if (1 == text_setup_rqd)
    {
        ptext_box_bmp = backFrameBuf;
        text_box_mem = ((uint32_t) ptext_box_bmp + R_BMP_Offset(ptext_box_bmp));

        /* Create and fill a new bitmap for the full line of text */
        R_BMP_Init(ptext_box_bmp, text_box_mem, BMP_DISP_BPP, TEXT_IMAGE_X, TEXT_IMAGE_Y);
        R_GAPI_Fill(ptext_box_bmp, 0, 0, TEXT_IMAGE_X, TEXT_IMAGE_Y, &bg_color);

        /* Write string to the newly created bitmap */
        R_GAPI_PutS((const uint8_t*) scroll_txt_buffer, &font_scroller, ptext_box_bmp, 0, TEXT_IMAGE_Y / 2);

        text_setup_rqd = 0;
    }

    /* Scroll the text bitmap on to itself */
    R_GAPI_Scroll(ptext_box_bmp, ptext_box_bmp, 0, 0, 1, 0);

    /* Only perform the display buffer update in the vertical blank period to
     * avoid tearing. */
    R_DDLCD_DispMemoryAcquire(xTaskGetCurrentTaskHandle());
    {
        /* Copy a section of the scrolling text box to the display */
        R_GAPI_CopySub(ptext_box_bmp, displayFrameBuf, /* From text box to Display */
        SCROLL_TXT_LOCX, SCROLL_TXT_LOCY, /* Destination location     */
        0, 0, /* Origin image start       */
        SCROLL_TXT_AREAX, SCROLL_TXT_AREAY, /* Size of area to copy     */
        NULL, /* No colour table          */
        NO_TRANSPARENCY_COLOR); /* No Transparency          */
    }
    R_DDLCD_DispMemoryRelease(xTaskGetCurrentTaskHandle());

    return (xdelay);
}
/*******************************************************************************
 End of function card_scroll
 *******************************************************************************/

/*****************************************************************************
 Function Name: get_from_file_system
 Description:   Mounts and locates 'file_name' in the file system,
 Verifies file is not larger than buffer_size and then copies
 file contents to 'buffer'
 Parameters:    file_name - file name to copy
 buffer - buffer in which file contents is copied
 buffer_size - size of 'buffer'
 Return value:  FILE_LARGER_THAN_BUFF if file size is > buffer_size or
 Number of bytes copied in to buffer.
 *****************************************************************************/
static int16_t get_from_file_system (const char* file_name, uint8_t* buffer, const uint16_t buffer_size)
{
    FATFS file_system_object;
    FRESULT fresult;
    FILINFO finfo;
    FIL file_object;
    int16_t retval = 0;

    fresult = R_tfat_f_mount(0, &file_system_object);

    if (TFAT_FR_OK == fresult)
    {
        /* Check we have sufficient buffer in which to load the file */
        fresult = R_tfat_f_stat(file_name, &finfo);
        if (finfo.fsize > buffer_size)
        {
            retval = FILE_LARGER_THAN_BUFF;
        }
        else
        {
            fresult = R_tfat_f_open(&file_object, file_name, TFAT_FA_READ);
            if (TFAT_FR_OK == fresult)
            {
                fresult = R_tfat_f_read(&file_object, buffer, buffer_size, (uint16_t*) &retval);
            }
        }
    }

    return (retval);
}
/*****************************************************************************
 End of function  get_from_file_system
 ******************************************************************************/

/*****************************************************************************
 Function Name: display_renesas_banner
 Description:   Displays the Renesas banner BMP from conventional resources.
 Parameters:    None
 Return value:  None
 *****************************************************************************/
static void display_renesas_banner (void)
{
    /* Find and copy large Renesas Banner to screen */
    (void) R_GAPI_Copy(FileFind(pResources, "Renesas_big"), backFrameBuf, RENESAS_BANNER_X, RENESAS_BANNER_Y);
}
/*****************************************************************************
 End of function  display_renesas_banner
 ******************************************************************************/

/*****************************************************************************
 Function Name: display_renesas_logo
 Description:   Displays the Renesas logo BMP from conventional resources.
 Parameters:    None
 Return value:  None
 *****************************************************************************/
static void display_renesas_logo (void)
{
    /* Find and copy small Renesas logo to screen */
    (void) R_GAPI_Copy(FileFind(pResources, "Logo"), backFrameBuf, RENESAS_LOGO_X, RENESAS_LOGO_Y);
}
/*****************************************************************************
 End of function  display_renesas_logo
 ******************************************************************************/

/*****************************************************************************
 Function Name: display_jpg
 Description:   Locates a JPEG image specified in the config file and decodes
 it in to the backFrame buffer at coordinates specified.
 by jpg_coord
 Parameters:    pconfig_file - Pointer to the buffer containing the configuration
 file data.
 image_id -     Config file entry that specifies the image file.
 jpg_coord -    Coordinates at which the JPEG decode library
 will output the decoded image to.
 Return value:  -1 Image load failed
 0 Image load successful
 *****************************************************************************/
static int16_t display_jpg (char* image_id, uint32_t jpg_coord)
{
    int32_t width;
    int16_t retval = 0;
    uint16_t jpg_height;
    uint16_t jpg_width;
    uint8_t* plogo_jpg = ecc_buffer;
    int32_t jpeg_file_size;
    int16_t file_res;
    char string_buffer[TXT_MAX_CHARS + 1];

    /* Try and locate customer logo name */
    r_config_get_entry(image_id, string_buffer, TXT_MAX_CHARS);
    file_res = get_from_file_system(string_buffer, ecc_buffer, sizeof(ecc_buffer)); /* Read in the file to a local buffer */

    if (file_res <= 0) /* If unable to locate banner file */
    {
        retval = -1;
    }
    else
    {
        /* User's logo has been successfully loaded in to RAM buffer */
        /* Copy user logo to screen */
        width = R_BMP_Width(backFrameBuf);
        jpeg_file_size = file_res;

        /* ensure we have valid jpeg file */
        if (R_get_info_jpeg(plogo_jpg, jpeg_file_size, &jpg_width, &jpg_height) == 0)
        {
            R_DDLCD_DispMemoryAcquire(xTaskGetCurrentTaskHandle());

            /* expand the file directly in to the frame buffer (-width to flip orientation to match GAPI convention) */
            if (0
                    != R_expand_jpeg(plogo_jpg, jpeg_file_size,
                            (uint16_t *) (((uint8_t *) backFrameBuf) + R_BMP_Offset(backFrameBuf)) + (jpg_coord),
                            -width))
            {
                /* There was an error in the JPEG Expansion routine */
            }

            R_DDLCD_DispMemoryRelease(xTaskGetCurrentTaskHandle());
        }
        else
        {
            retval = -1;
        }
    }

    return (retval);
}
/*****************************************************************************
 End of function  display_jpg
 ******************************************************************************/

/*****************************************************************************
 Function Name: setup_user_images
 Description:   Coordinates the display of the various user images and/or
 default images.
 Parameters:    None
 Return value:  None
 *****************************************************************************/
static void setup_user_images (void)
{
    int16_t logo_load_res;
    uint8_t current_pos = 0;

    /* Display the user's banner if available */
    logo_load_res = display_jpg(BANNER_ID, BANNER_JPG_COORD);
    if (logo_load_res < 0)
    {
        /* No banner Found - try and display the logos */

        logo_load_res = display_jpg(PHOTO_ID, logo_positions[current_pos]);
        if (logo_load_res >= 0)
        {
            /* Photo found and displayed, move on to next image */
            current_pos++;
        }

        logo_load_res = display_jpg(LOGO_ID, logo_positions[current_pos]);
        if (logo_load_res >= 0)
        {
            /* Logo found and displayed, move on to next image */
            current_pos++;
        }

        if (1 == current_pos)
        {
            /* If only one image in screen - display the default Renesas logo */
            display_renesas_logo();
        }

        if (0 == current_pos) /* If no images displayed failed for any reason - use default logo from standard resources */
        {
            /* No customer logo found */
            display_renesas_banner();
        }
    }
}
/*****************************************************************************
 End of function  setup_user_images
 ******************************************************************************/

/*****************************************************************************
 Function Name: setup_scrolling_text
 Description:   Locates and copies the scrolling 'name' text from the
 configuration file to the global text buffer.
 Size is checked against the destination bitmap size to
 ensure all characters can be displayed.
 Displays error messages if appropriate.
 Parameters:    None
 Return value:  None
 *****************************************************************************/
static void setup_scrolling_text (void)
{
    int8_t result;
    uint32_t string_pixels_tot;

    /* Get Person Name */
    result = r_config_get_entry(NAME_ID, scroll_txt_buffer, TXT_MAX_CHARS);
    if (PINI_INVALID == result)
    {
        strcpy(scroll_txt_buffer, TXT_NO_CONFIG); /* Provide default text */
    }
    else if (PSECT_NOTFOUND == result)
    {
        strcpy(scroll_txt_buffer, SCROLL_TXT_MSG); /* Provide default text */
    }
    else if (PSECT_TOOBIG == result)
    {
        strcpy(scroll_txt_buffer, TXT_MAX_EXCEEDED_MSG); /* Report the problem */
    }
    else
    {
        /* No errors detected - do nothing */
    }

    /* Calculate the number of pixels required */
    string_pixels_tot = (TEXT_IMAGE_X * TEXT_IMAGE_Y);

    /* Check we have sufficient pixels available in the buffer as it stands */
    if (string_pixels_tot > (R_BMP_Height(backFrameBuf) * R_BMP_Width(backFrameBuf)))
    {
        strcpy(scroll_txt_buffer, TXT_MAX_EXCEEDED_MSG);
    }
}
/*****************************************************************************
 End of function  setup_scrolling_text
 ******************************************************************************/

/*****************************************************************************
 Function Name: setup_static_text
 Description:   Locates and displays the static 'company' text from the
 configuration file.
 Displays error messages if appropriate.
 Parameters:    None
 Return value:  None
 *****************************************************************************/
static void setup_static_text (void)
{
    int8_t result;
    int16_t string_pixels_x;
    char string_buffer[TXT_MAX_CHARS + 1];

    /* Locate and copy Company name */
    result = r_config_get_entry(TITLE_ID, string_buffer, TXT_MAX_CHARS);
    if (PINI_INVALID == result)
    {
        strcpy(string_buffer, TXT_NO_CONFIG); /* Provide default text */
    }
    else if (PSECT_NOTFOUND == result)
    {
        strcpy(string_buffer, SCROLL_TXT_MSG); /* Provide default text */
    }
    else if (PSECT_TOOBIG == result)
    {
        strcpy(string_buffer, TXT_MAX_EXCEEDED_MSG); /* Report the problem */
    }
    else
    {
        /* No errors detected - do nothing */
    }

    /* Calculate the number of X pixels required */
    string_pixels_x = R_GAPI_FontWidthString((uint8_t const *) string_buffer, &font_company);

    /* Place centred on the display */
    string_pixels_x = (int16_t) ((DD_FRAME_WIDTH / 2) - (string_pixels_x / 2));
    R_GAPI_PutS((uint8_t*) string_buffer, &font_company, backFrameBuf, string_pixels_x, COMPANY_TXT_LOCY);

}
/*****************************************************************************
 End of function  setup_static_text
 ******************************************************************************/

