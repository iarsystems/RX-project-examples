/***********************************************************************************************************************
 * DISCLAIMER
 * This software is supplied by Renesas Electronics Corporation and is only intended for use with Renesas products.
 * No other uses are authorized. This software is owned by Renesas Electronics Corporation and is protected under all
 * applicable laws, including copyright laws.
 * THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIESREGARDING THIS SOFTWARE, WHETHER EXPRESS, IMPLIED
 * OR STATUTORY, INCLUDING BUT NOT LIMITED TO WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NON-INFRINGEMENT.  ALL SUCH WARRANTIES ARE EXPRESSLY DISCLAIMED.TO THE MAXIMUM EXTENT PERMITTED NOT PROHIBITED BY
 * LAW, NEITHER RENESAS ELECTRONICS CORPORATION NOR ANY OF ITS AFFILIATED COMPANIES SHALL BE LIABLE FOR ANY DIRECT,
 * INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES FOR ANY REASON RELATED TO THIS SOFTWARE, EVEN IF RENESAS OR
 * ITS AFFILIATES HAVE BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
 * Renesas reserves the right, without notice, to make changes to this software and to discontinue the availability
 * of this software. By using this software, you agree to the additional terms and conditions found by accessing the
 * following link:
 * http://www.renesas.com/disclaimer
 *
 * Copyright (C) 2014 Renesas Electronics Corporation. All rights reserved.
 ***********************************************************************************************************************/

/***********************************************************************************************************************
 * File Name    : r_lcd_appv2.c
 * Version      : 1.00
 * Device(s)    : R5F51138AxFP
 * Tool-Chain   : IAR Systems ICCRX
 * Description  : RSK LCD APP V2 handing routines
 * Creation Date: 28/08/2014
 ***********************************************************************************************************************/

/***********************************************************************************************************************
 Includes
 ***********************************************************************************************************************/
/* Contains string functions used in this file */
#include <string.h>

#include "r_cg_macrodriver.h"

/* Header file contains declarations of functions defined in this file */
#include "r_lcd_appv2.h"

/* Following header file provides a structure to access on-chip I/O
 registers. */
#include "iorx113.h"

/******************************************************************************
 Global Variables & Defined Constants
 ******************************************************************************/

/* Declare Array Maps */
lcdmap_t g_sect_a_map[20];
lcdmap_t g_sect_b_map[20];
lcdmap_t g_sect_c_map[41];

/******************************************************************************
 User Program Code
 ******************************************************************************/

/***********************************************************************************************************************
 * Function Name: validate_input
 * Description  : Takes any char and validates the input for the Update_Display
 * 				 function. Capitalises all letters, and remaps symbols. Outputs
 * 				 to a variable specified by the pointer parameter passed.
 * Arguments    : int8_t to be validated, pointer to output buffer
 * Return Value : 0: Unsupported Character
 * 				 1: Number (or space)
 * 				 2: Letter A-F
 * 				 3: Letter G-Z
 * 				 4: Supported Symbol
 ***********************************************************************************************************************/
static int16_t validate_input (uint8_t input, uint8_t * output)
{
    /* Is space or Carriage Return? */
    if ((' ' == input) || (0x0D == input))
    {
        (*output) = 0x29;
        return 1;
    }

    /* Is Number? */
    else if ((input < 0x3A) && (input > 0x2F))
    {
        /* Convert from ASCII */
        (*output) = (input - 0x30);
        return 1;
    }

    /* Is Uppercase Alpha A-F ? */
    else if ((input < 0x47) && (input > 0x40))
    {
        /* Convert from ASCII */
        (*output) = (input - 0x33);
        return 2;
    }

    /* Is Lowercase Alpha A-F */
    else if ((input < 0x67) && (input > 0x60))
    {
        /* Shift Case & Convert from ASCII */
        (*output) = (input - 0x53);
        return 2;
    }

    /* Is Uppercase Alpha G-Z ? */
    else if ((input > 0x46) && (input < 0x5B))
    {
        /* Convert from ASCII */
        (*output) = (input - 0x33);
        return 3;
    }

    /* Is Lowercase Alpha G-Z ? */
    else if ((input > 0x66) && (input < 0x7B))
    {
        /* Shift Case & Convert from ASCII */
        (*output) = (input - 0x53);
        return 3;
    }

    /* Is Symbol? */
    else
    {
        /* Check input against supported symbols */
        switch (input)
        {

            /* Is Plus */
            case 0x2B :
            {
                (*output) = 0x0A;
            }
            break;

                /* Is Minus */
            case 0x2D :
            {
                (*output) = 0x0B;
            }
            break;

                /* Is Forward Slash */
            case 0x2F :
            {
                (*output) = 0x0C;
            }
            break;

                /* Is Back Slash */
            case 0x5C :
            {
                (*output) = 0x0D;
            }
            break;

                /* Is full stop */
            case 0x2E :
            {
                (*output) = 0x2E;
            }
            break;

                /* Is colon */
            case 0x3A :
            {
                (*output) = 0x3A;
            }
            break;

                /* Is asterisk */
            case 0x2A :
            {
                (*output) = 0x28;
            }
            break;

            default :

                /* Unsupported Character */
                return 0;
        }
        return 4;
    }
}

/******************************************************************************
 End of Function validate_input
 ******************************************************************************/

/***********************************************************************************************************************
 * Function Name: sect_a_glyph_map
 * Description  : Takes a validated char input and maps the character to a
 * 				 segment pattern, to be displayed on Section A of the LCD panel.
 * Arguments    : character to be displayed, digit position of character
 * Return Value : None
 ***********************************************************************************************************************/
static void sect_a_glyph_map (uint8_t glyph, int16_t digit)
{
    uint8_t *pinptr1 = PDL_NO_PTR;
    uint8_t *pinptr2 = PDL_NO_PTR;

    /* Set Pointers to Correct Segments */
    switch (digit)
    {

        /* First Digit */
        case 0 :
            pinptr1 = (uint8_t*) (&LCDC.SEG11);
            pinptr2 = (uint8_t*) (&LCDC.SEG10);
        break;

            /* Second Digit */
        case 1 :
            pinptr1 = (uint8_t*) (&LCDC.SEG13);
            pinptr2 = (uint8_t*) (&LCDC.SEG12);
        break;

            /* Decimal Point */
        case 2 :
            pinptr1 = (uint8_t*) (&LCDC.SEG14);
            pinptr2 = PDL_NO_PTR;
        break;

            /* Third Digit */
        case 3 :
            pinptr1 = (uint8_t*) (&LCDC.SEG15);
            pinptr2 = (uint8_t*) (&LCDC.SEG14);
        break;

        default :

            /* nothing */
        break;
    }

    /* Bit Mask Segments */
    (*pinptr1) &= (uint8_t) 0xF0;
    (*pinptr2) &= (uint8_t) 0xF8;

    /* decimal point? */
    if (2 == digit)
    {
        if ('.' == glyph)
        {
            /* Turn on Decimal Point */
            (*pinptr1) |= (uint8_t) 0x08;
        }
        else
        {
            /* Turn off Decimal Point */
            (*pinptr1) &= (uint8_t) 0xF7;
        }
    }
    else
    {
        if (0x29 != glyph)
        {
            /* Digit-Segment Mapping */
            (*pinptr1) |= g_sect_a_map[glyph].BYTE.TWO;
            (*pinptr2) |= g_sect_a_map[glyph].BYTE.ONE;
        }
    }
}

/******************************************************************************
 End of Function sect_a_glyph_map
 ******************************************************************************/

/***********************************************************************************************************************
 * Function Name : sect_b_glyph_map
 * Description   : Takes a validated char input and maps the character to a
 *                 segment pattern, to be displayed on Section B of the LCD panel.
 * Arguments     : character to be displayed, digit position of character
 * Return Value  : none
 ***********************************************************************************************************************/
static void sect_b_glyph_map (const uint8_t glyph, const uint16_t digit)
{
    /* Declare Pointers */
    volatile uint8_t * pinptr1 = PDL_NO_PTR;
    volatile uint8_t * pinptr2 = PDL_NO_PTR;

    /* Set Pointers to Current Digit Pins */
    switch (digit)
    {
        /* First Digit */
        case 0 :
        {
            pinptr1 = (uint8_t*) (&LCDC.SEG28);
            pinptr2 = (uint8_t*) (&LCDC.SEG29);
        }
        break;

            /* Second Digit */
        case 1 :
        {
            pinptr1 = (uint8_t*) (&LCDC.SEG26);
            pinptr2 = (uint8_t*) (&LCDC.SEG27);
        }
        break;

            /* Third Digit */
        case 2 :
        {
            pinptr1 = (uint8_t*) (&LCDC.SEG24);
            pinptr2 = (uint8_t*) (&LCDC.SEG25);
        }
        break;

            /* Colon/Decimal Point */
        case 3 :
        {
            pinptr1 = (uint8_t*) (&LCDC.SEG23);
            pinptr2 = PDL_NO_PTR;
        }
        break;

            /* Fourth Digit */
        case 4 :
        {
            pinptr1 = (uint8_t*) (&LCDC.SEG21);
            pinptr2 = (uint8_t*) (&LCDC.SEG22);
        }
        break;

            /* Fifth Digit */
        case 5 :
        {
            pinptr1 = (uint8_t*) (&LCDC.SEG19);
            pinptr2 = (uint8_t*) (&LCDC.SEG20);
        }
        break;

            /* AM/PM Indicator */
        case 6 :
        {
            /* Nullify Pointers */
            pinptr1 = PDL_NO_PTR;
            pinptr2 = PDL_NO_PTR;

            /* If last digit is A */
            if (0x0E == glyph)
            {
                /* AM Indicator On */
                LCDC.SEG18 |= 0x01U;

                /* PM Indicator Off */
                LCDC.SEG16 &= 0xFDU;
            }

            /* If last digit is P */
            else if (0x1DU == glyph)
            {
                /* AM Indicator Off */
                LCDC.SEG18 &= 0xFEU;

                /* PM Indicator On */
                LCDC.SEG16 |= 0x02U;
            }

            /* If last digit is other */
            else
            {
                /* AM Indicator Off */
                LCDC.SEG18 &= 0xFEU;

                /* PM Indicator Off */
                LCDC.SEG16 &= 0xFDU;
            }
        }
        break;

            /* Handles undefined cases */
        default :
        {
            /* Do nothing */
        }
        break;
    }

    /* Bit Mask Segments */
    (*pinptr1) &= 0xF0U;
    (*pinptr2) &= 0xF1U;

    /* Is colon/decimal point ? */
    if (3 == digit)
    {
        /* Is Colon? */
        if (':' == glyph)
        {
            /* Turn on Colon */
            (*pinptr1) |= 0x02U;
        }

        /* Is Decimal Point? */
        else if ('.' == glyph)
        {
            /* Turn off Colon */
            (*pinptr1) &= 0xFDU;
        }

        /* Is neither? */
        else
        {
            /* Turn off Decimal Point & Colon */
            (*pinptr1) &= 0xFCU;
        }
    }
    else if (0x29 != glyph)
    {
        /* Map Glyph to Segments */
        (*pinptr1) |= (uint8_t) g_sect_b_map[glyph].BYTE.TWO;
        (*pinptr2) |= (uint8_t) g_sect_b_map[glyph].BYTE.ONE;
    }
    else
    {
        /* Do nothing */
    }
}

/***********************************************************************************************************************
 End of function sect_b_glyph_map
 ***********************************************************************************************************************/

/***********************************************************************************************************************
 * Function Name: sect_c_glyph_map
 * Description  : Takes a validated char input and maps the character to a
 * 				 segment pattern, to be displayed on Section c of the LCD panel.
 * Arguments    : character to be displayed, digit position of character
 * Return Value : None
 ***********************************************************************************************************************/
static void sect_c_glyph_map (uint8_t glyph, int16_t digit)
{
    /* Declare Pin Address Pointers */
    uint8_t *pinptr1 = (PDL_NO_PTR);
    uint8_t *pinptr2 = (PDL_NO_PTR);
    uint8_t *pinptr3 = (PDL_NO_PTR);
    uint8_t *pinptr4 = (PDL_NO_PTR);

    /* Set Pointers to Current Digit Pin Addresses */
    switch (digit)
    {

        /* First Digit (Specify Aux pins in order to use)
         * Note - The first digit is not available in the RSKRX113
         * Hardware design.
         */

        /* Second Digit */
        case 1 :
            pinptr1 = (uint8_t*) (&LCDC.SEG00);
            pinptr2 = (uint8_t*) (&LCDC.SEG01);
            pinptr3 = (uint8_t*) (&LCDC.SEG38);
            pinptr4 = (uint8_t*) (&LCDC.SEG39);
        break;

            /* Third Digit */
        case 2 :
            pinptr1 = (uint8_t*) (&LCDC.SEG02);
            pinptr2 = (uint8_t*) (&LCDC.SEG03);
            pinptr3 = (uint8_t*) (&LCDC.SEG36);
            pinptr4 = (uint8_t*) (&LCDC.SEG37);
        break;

            /* Four Digit */
        case 3 :
            pinptr1 = (uint8_t*) (&LCDC.SEG04);
            pinptr2 = (uint8_t*) (&LCDC.SEG05);
            pinptr3 = (uint8_t*) (&LCDC.SEG34);
            pinptr4 = (uint8_t*) (&LCDC.SEG35);
        break;

            /* Fifth Digit */
        case 4 :
            pinptr1 = (uint8_t*) (&LCDC.SEG06);
            pinptr2 = (uint8_t*) (&LCDC.SEG07);
            pinptr3 = (uint8_t*) (&LCDC.SEG32);
            pinptr4 = (uint8_t*) (&LCDC.SEG33);
        break;

            /* Sixth Digit */
        case 5 :
            pinptr1 = (uint8_t*) (&LCDC.SEG08);
            pinptr2 = (uint8_t*) (&LCDC.SEG09);
            pinptr3 = (uint8_t*) (&LCDC.SEG30);
            pinptr4 = (uint8_t*) (&LCDC.SEG31);
        break;

        default :

            /* Invalid Digit Selection */
        break;
    }

    /* Bit Mask All Segments */
    (*pinptr1) &= (uint8_t) 0xF0;
    (*pinptr2) &= (uint8_t) 0xF0;
    (*pinptr3) &= (uint8_t) 0xF1;
    (*pinptr4) &= (uint8_t) 0xF1;

    if (0x29 != glyph)
    {

        /* Map Glyph To Segments */
        (*pinptr4) |= g_sect_c_map[glyph].BYTE.ONE;
        (*pinptr3) |= g_sect_c_map[glyph].BYTE.TWO;
        (*pinptr2) |= g_sect_c_map[glyph].BYTE.THREE;
        (*pinptr1) |= g_sect_c_map[glyph].BYTE.FOUR;

    }
}

/******************************************************************************
 End of Function sect_c_glyph_map
 ******************************************************************************/

/* sect_d_glyph_map is used for the manipulation of the battery indicator symbol
 not required in this sample set and removed to reduce code size.
 */

#ifdef SECT_D_GLYPH_MAP
/***********************************************************************************************************************
 Function Name : sect_d_glyph_map
 Description   : Takes a number input, and sets the level of the battery indicator (section D)
 Argument      : uint8_t level
 *                  - Indicator Level (char)
 Return Values : None
 ***********************************************************************************************************************/
static void sect_d_glyph_map (const uint8_t level)
{
    /* Switch to segment option */
    switch (level)
    {
        /* Battery Outline On */
        case 0:
        {
            LCDC.SEG17 &= 0xF0U;
            LCDC.SEG16 |= 0x01U;
        }
        break;

        /* Battery Outline & 1 Level */
        case 1:
        {
            LCDC.SEG17 &= 0xF0U;
            LCDC.SEG17 |= 0x08U;
            LCDC.SEG16 |= 0x01U;
        }
        break;

        /* Battery Outline & 2 Levels */
        case 2:
        {
            LCDC.SEG17 &= 0xF0U;
            LCDC.SEG17 |= 0x0CU;
            LCDC.SEG16 |= 0x01U;
        }
        break;

        /* Battery Outline & 3 Levels */
        case 3:
        {
            LCDC.SEG17 &= 0xF0U;
            LCDC.SEG17 |= 0x0EU;
            LCDC.SEG16 |= 0x01U;
        }
        break;

        /* Battery Outline & All Levels */
        case 4:
        {
            LCDC.SEG17 |= 0x0FU;
            LCDC.SEG16 |= 0x01U;
        }
        break;

        default:
        {
            /* Bad Selection */
        }
        break;
    }
}

/***********************************************************************************************************************
 End of function sect_d_glyph_map
 ***********************************************************************************************************************/
#endif /* SECT_D_GLYPH_MAP */

/***********************************************************************************************************************
 * Function Name : sect_f_glyph_map
 * Description   : Sets the week calendar (Section F) based on a number input
 *                 0: OFF            4: Thursday
 *                 1: Monday         5: Friday
 *                 2: Tuesday        6: Saturday
 *                 3: Wednesday      7: Sunday
 * Arguments     : uint8_t day - character to be displayed, digit position of character
 * Return Value  : None
 ***********************************************************************************************************************/
static void sect_f_glyph_map (const uint8_t day)
{
    /* Switch to segment option */
    switch (day)
    {
        /* All Days Off */
        case 0 :
        {
            LCDC.SEG25 &= 0x0EU;
            LCDC.SEG23 &= 0x03U;
            LCDC.SEG22 &= 0x0EU;
            LCDC.SEG20 &= 0x0EU;
            LCDC.SEG18 &= 0x03U;
        }
        break;

            /* Monday Lit */
        case 1 :
        {
            LCDC.SEG25 &= 0x0EU;
            LCDC.SEG23 &= 0x03U;
            LCDC.SEG23 |= 0x08U;
            LCDC.SEG22 &= 0x0EU;
            LCDC.SEG20 &= 0x0EU;
            LCDC.SEG18 &= 0x03U;
        }
        break;

            /* Tuesday Lit */
        case 2 :
        {
            LCDC.SEG25 &= 0x0EU;
            LCDC.SEG23 &= 0x03U;
            LCDC.SEG23 |= 0x04U;
            LCDC.SEG22 &= 0x0EU;
            LCDC.SEG20 &= 0x0EU;
            LCDC.SEG18 &= 0x03U;
        }
        break;

            /* Wednesday Lit */
        case 3 :
        {
            LCDC.SEG25 &= 0x0EU;
            LCDC.SEG31 &= 0x03U;
            LCDC.SEG22 |= 0x01U;
            LCDC.SEG20 &= 0x0EU;
            LCDC.SEG18 &= 0x03U;
        }
        break;

            /* Thursday Lit */
        case 4 :
        {
            LCDC.SEG25 &= 0x0EU;
            LCDC.SEG23 &= 0x03U;
            LCDC.SEG22 &= 0x0EU;
            LCDC.SEG20 |= 0x01U;
            LCDC.SEG18 &= 0x03U;
        }
        break;

            /* Friday Lit */
        case 5 :
        {
            LCDC.SEG25 &= 0x0EU;
            LCDC.SEG23 &= 0x03U;
            LCDC.SEG22 &= 0x0EU;
            LCDC.SEG20 &= 0x0EU;
            LCDC.SEG18 &= 0x03U;
            LCDC.SEG18 |= 0x08U;
        }
        break;

            /* Saturday Lit */
        case 6 :
        {
            LCDC.SEG25 &= 0x0EU;
            LCDC.SEG23 &= 0x03U;
            LCDC.SEG22 &= 0x0EU;
            LCDC.SEG20 &= 0x0EU;
            LCDC.SEG18 &= 0x03U;
            LCDC.SEG18 |= 0x04U;
        }
        break;

            /* Sunday Lit */
        case 7 :
        {
            LCDC.SEG25 |= 0x01U;
            LCDC.SEG23 &= 0x03U;
            LCDC.SEG22 &= 0x0EU;
            LCDC.SEG20 &= 0x0EU;
            LCDC.SEG18 &= 0x03U;
        }
        break;

        default :
        {
            /* Bad Selection */
        }
        break;
    }
}

/***********************************************************************************************************************
 End of function sect_f_glyph_map
 ***********************************************************************************************************************/

/***********************************************************************************************************************
 * Function Name: init_maps
 * Description  : Initialises the glyph-segment maps used to display letters,
 * 				  symbols and numbers
 * Arguments    : None
 * Return Value : None
 ***********************************************************************************************************************/
static void init_maps (void)
{

    /* Section A maps START */
    g_sect_a_map[0x0].WORD = 0x00F5u; /* 0 */
    g_sect_a_map[0x1].WORD = 0x0060u; /* 1 */
    g_sect_a_map[0x2].WORD = 0x00B6u; /* 2 */
    g_sect_a_map[0x3].WORD = 0x00F2u; /* 3 */
    g_sect_a_map[0x4].WORD = 0x0063u; /* 4 */
    g_sect_a_map[0x5].WORD = 0x00D3u; /* 5 */
    g_sect_a_map[0x6].WORD = 0x00D7u; /* 6 */
    g_sect_a_map[0x7].WORD = 0x0070u; /* 7 */
    g_sect_a_map[0x8].WORD = 0x00F7u; /* 8 */
    g_sect_a_map[0x9].WORD = 0x00F3u; /* 9 */
    g_sect_b_map[0xA].WORD = 0x0000u; /* dummy */
    g_sect_b_map[0xB].WORD = 0x0000u; /* dummy */
    g_sect_b_map[0xC].WORD = 0x0000u; /* dummy */
    g_sect_b_map[0xD].WORD = 0x0000u; /* dummy */
    g_sect_a_map[0xE].WORD = 0x0077u; /* A */
    g_sect_a_map[0xF].WORD = 0x00C7u; /* B */
    g_sect_a_map[0x10].WORD = 0x0095u; /* C */
    g_sect_a_map[0x11].WORD = 0x00E6u; /* D */
    g_sect_a_map[0x12].WORD = 0x0097u; /* E */
    g_sect_a_map[0x13].WORD = 0x0017u; /* F */

    /* Section A maps END */

    /* Section B maps START */
    g_sect_b_map[0x0].WORD = 0x00FAu; /* 0 */
    g_sect_b_map[0x1].WORD = 0x0060u; /* 1 */
    g_sect_b_map[0x2].WORD = 0x00D6u; /* 2 */
    g_sect_b_map[0x3].WORD = 0x00F4u; /* 3 */
    g_sect_b_map[0x4].WORD = 0x006Cu; /* 4 */
    g_sect_b_map[0x5].WORD = 0x00BCu; /* 5 */
    g_sect_b_map[0x6].WORD = 0x00BEu; /* 6 */
    g_sect_b_map[0x7].WORD = 0x00E0u; /* 7 */
    g_sect_b_map[0x8].WORD = 0x00FEu; /* 8 */
    g_sect_b_map[0x9].WORD = 0x00FCu; /* 9 */
    g_sect_b_map[0xA].WORD = 0x0000u; /* dummy */
    g_sect_b_map[0xB].WORD = 0x0000u; /* dummy */
    g_sect_b_map[0xC].WORD = 0x0000u; /* dummy */
    g_sect_b_map[0xD].WORD = 0x0000u; /* dummy */
    g_sect_b_map[0xE].WORD = 0x00EEu; /* A */
    g_sect_b_map[0xF].WORD = 0x003Eu; /* B */
    g_sect_b_map[0x10].WORD = 0x009Au; /* C */
    g_sect_b_map[0x11].WORD = 0x0076u; /* D */
    g_sect_b_map[0x12].WORD = 0x009Eu; /* E */
    g_sect_b_map[0x13].WORD = 0x008Eu; /* F */

    /* Section B maps END */

    /* Section C maps START */
    /* Numbers */
    g_sect_c_map[0].WORD = 0xC3AAu; /* 0 */
    g_sect_c_map[1].WORD = 0x00A0u; /* 1 */
    g_sect_c_map[2].WORD = 0x816Cu; /* 2 */
    g_sect_c_map[3].WORD = 0x81E4u; /* 3 */
    g_sect_c_map[4].WORD = 0x00E6u; /* 4 */
    g_sect_c_map[5].WORD = 0x81C6u; /* 5 */
    g_sect_c_map[6].WORD = 0x81CEu; /* 6 */
    g_sect_c_map[7].WORD = 0x01A0u; /* 7 */
    g_sect_c_map[8].WORD = 0x81EEu; /* 8 */
    g_sect_c_map[9].WORD = 0x81E6u; /* 9 */

    /* Symbols */
    g_sect_c_map[10].WORD = 0x2444u; /* + */
    g_sect_c_map[11].WORD = 0x0044u; /* - */
    g_sect_c_map[12].WORD = 0x4200u; /* / */
    g_sect_c_map[13].WORD = 0x1800u; /* \ */
    g_sect_c_map[40].WORD = 0x7e44u; /* * */

    /* Letters */
    g_sect_c_map[14].WORD = 0x01EEu; /* A */
    g_sect_c_map[15].WORD = 0xA5E0u; /* B */
    g_sect_c_map[16].WORD = 0x810Au; /* C */
    g_sect_c_map[17].WORD = 0xA5A0u; /* D */
    g_sect_c_map[18].WORD = 0x814Eu; /* E */
    g_sect_c_map[19].WORD = 0x014Eu; /* F */
    g_sect_c_map[20].WORD = 0x81CAu; /* G */
    g_sect_c_map[21].WORD = 0x00EEu; /* H */
    g_sect_c_map[22].WORD = 0xA500u; /* I */
    g_sect_c_map[23].WORD = 0x80A0u; /* J */
    g_sect_c_map[24].WORD = 0x0A0Eu; /* K */
    g_sect_c_map[25].WORD = 0x800Au; /* L */
    g_sect_c_map[26].WORD = 0x12AAu; /* M */
    g_sect_c_map[27].WORD = 0x18AAu; /* N */
    g_sect_c_map[28].WORD = 0x81AAu; /* O */
    g_sect_c_map[29].WORD = 0x016Eu; /* P */
    g_sect_c_map[30].WORD = 0x89AAu; /* Q */
    g_sect_c_map[31].WORD = 0x096Eu; /* R */
    g_sect_c_map[32].WORD = 0x81C6u; /* S */
    g_sect_c_map[33].WORD = 0x2500u; /* T */
    g_sect_c_map[34].WORD = 0x80AAu; /* U */
    g_sect_c_map[35].WORD = 0x420Au; /* V */
    g_sect_c_map[36].WORD = 0x48AAu; /* W */
    g_sect_c_map[37].WORD = 0x5A00u; /* X */
    g_sect_c_map[38].WORD = 0x1600u; /* Y */
    g_sect_c_map[39].WORD = 0xC300u; /* Z */

    /* Section C maps END */
}

/******************************************************************************
 End of Function init_maps
 ******************************************************************************/

/***********************************************************************************************************************
 * Function Name : R_LCD_DisplaySymbol
 * Description   : Takes a symbol code and turns on/off LCD Panel Symbols. The
 *                 first digit should be which symbol to control (see symbols
 *                 listed below)
 *                 The second should be either a 1 or a 0.
 *                 (1 - Turn on, 0 - Turn Off)
 *                 1 - Heat         5 - mmHg             9 - Degrees F
 *                 2 - Fan          6 - Volts            A - Alarm
 *                 3 - Zone         7 - Heart            B - AM
 *                 4 - mg/ml        8 - Degrees C        C - PM
 *                 For example '61' would turn on the Volts symbol and
 *                 C0 would turn off the PM symbol.
 * Arguments     : int16_t input
 *                           0 - valid input
 *                          -1 - invalid input
 * Return Value  : int8_t
 ***********************************************************************************************************************/
int8_t R_LCD_DisplaySymbol (const uint16_t input)
{
    /* Declare a status variable */
    int8_t status = 0;

    switch (input)
    {
        /* Heat Symbol On/Off */
        case LCD_HEAT_ON :
        {
            LCDC.SEG27 |= 0x01U;
        }
        break;

        case LCD_HEAT_OFF :
        {
            LCDC.SEG27 &= 0xFEU;
        }
        break;

            /* Fan Symbol On/Off */
        case LCD_FAN_ON :
        {
            LCDC.SEG33 |= 0x01U;
        }
        break;

        case LCD_FAN_OFF :
        {
            LCDC.SEG33 &= 0xFEU;
        }
        break;

            /* Zone Symbol On/Off */
        case LCD_ZONE_ON :
        {
            LCDC.SEG32 |= 0x01U;
        }
        break;

        case LCD_ZONE_OFF :
        {
            LCDC.SEG32 &= 0xFEU;
        }
        break;

            /* MGML Symbol On/Off */
        case LCD_MGML_ON :
        {
            LCDC.SEG34 |= 0x01U;
        }
        break;

        case LCD_MGML_OFF :
        {
            LCDC.SEG34 &= 0xFEU;
        }
        break;

            /* MMHG Symbol On/Off */
        case LCD_MMHG_ON :
        {
            LCDC.SEG35 |= 0x01U;
        }
        break;

        case LCD_MMHG_OFF :
        {
            LCDC.SEG35 &= 0xFEU;
        }
        break;

            /* Volts Symbol On/Off */
        case LCD_VOLTS_ON :
        {
            LCDC.SEG31 |= 0x01U;
        }
        break;

        case LCD_VOLTS_OFF :
        {
            LCDC.SEG31 &= 0xFEU;
        }
        break;

            /* Heart Symbol On/Off */
        case LCD_HEART_ON :
        {
            LCDC.SEG30 |= 0x01U;
        }
        break;

        case LCD_HEART_OFF :
        {
            LCDC.SEG30 &= 0xFEU;
        }
        break;

            /* Degrees C Symbol On/Off */
        case LCD_DEGREESC_ON :
            LCDC.SEG16 |= 0x04U;
        break;

        case LCD_DEGREESC_OFF :
        {
            LCDC.SEG16 &= 0xFBU;
        }
        break;

            /* Degrees F Symbol On/Off */
        case LCD_DEGREESF_ON :
        {
            LCDC.SEG16 |= 0x08U;
        }
        break;

        case LCD_DEGREESF_OFF :
        {
            LCDC.SEG16 &= 0xF7U;
        }
        break;

            /* Alarm Symbol On/Off */
        case LCD_ALARM_ON :
        {
            LCDC.SEG18 |= 0x02U;
        }
        break;

        case LCD_ALARM_OFF :
        {
            LCDC.SEG18 &= 0xFDU;
        }
        break;

            /* Alarm Symbol On/Off */
        case LCD_ALARM_ON_1 :
            LCDC.SEG18 |= 0x02U;
        break;

        case LCD_ALARM_OFF_1 :
        {
            LCDC.SEG18 &= 0xFDU;
        }
        break;

            /* AM Symbol On/Off */
        case LCD_AM_ON :
        {
            LCDC.SEG18 |= 0x01U;
        }
        break;

        case LCD_AM_OFF :
        {
            LCDC.SEG18 &= 0xFEU;
        }
        break;

            /* AM Symbol On/Off */
        case LCD_AM_ON_1 :
        {
            LCDC.SEG18 |= 0x01U;
        }
        break;

        case LCD_AM_OFF_1 :
        {
            LCDC.SEG27 &= 0xFEU;
        }
        break;

            /* PM Symbol On/Off */
        case LCD_PM_ON :
        {
            LCDC.SEG16 |= 0x02U;
        }
        break;

        case LCD_PM_OFF :
        {
            LCDC.SEG16 &= 0xFDU;
        }
        break;

            /* PM Symbol On/Off */
        case LCD_PM_ON_1 :
        {
            LCDC.SEG16 |= 0x02U;
        }
        break;

        case LCD_PM_OFF_1 :
        {
            LCDC.SEG16 &= 0xFDU;
        }
        break;

            /* Renesas Logo On/Off */
        case LCD_R_LOGO_ON :
        {
            LCDC.SEG12 |= 0x08U;
        }
        break;

        case LCD_R_LOGO_OFF :
        {
            LCDC.SEG12 &= 0xF7U;
        }
        break;

            /* Renesas Logo On/Off */
        case LCD_R_LOGO_ON_1 :
        {
            LCDC.SEG12 |= 0x08U;
        }
        break;

        case LCD_R_LOGO_OFF_1 :
        {
            LCDC.SEG12 &= 0xF7U;
        }
        break;

        default :
        {
            /* Bad Selection */
            status = 1;
        }
        break;
    }
    return status;
}

/***********************************************************************************************************************
 End of function R_LCD_DisplaySymbol
 ***********************************************************************************************************************/

/***********************************************************************************************************************
 * Function Name: R_LCD_DisplayPanelString
 * Description  : This function writes to line 1, line 2 or 3 of the LCD.
 * 				 You need to use the defines LCD_LINE1, LCD_LINE2 and LCD_LINE3
 * 				 in order to specify the starting position.
 * Arguments    : (uint8_t) position - LCD row (symbol) number.
 * 				 (uint8_t*) string - pointer to data to be written to display
 * 				 (up to 8 chars).
 * Return Value : None
 ***********************************************************************************************************************/
void R_LCD_DisplayPanelString (uint8_t position, uint8_t* string)
{

    /* Declare variable to hold the output data */
    uint8_t output_buf = 0;

    /* Declare a variable flag to hold the input validation result */
    int16_t flag = 0;

    /* Declare a loop count variable */
    uint16_t i = 0;

    /* Declare a symbol select variable */
    uint16_t symbsel = 0;

    /* Display input onto LCD */
    switch (position)
    {
        /* Start Section 1, Numerical */
        case PANEL_LCD_LINE2 :
        {
            /* Cycle Through Each Digit */
            for (i = 0; i < 6; i++)
            {
                /* Validate Input */
                flag = validate_input(string[i], &output_buf);

                /* Is Hex Number? */
                if ((((1 == flag) || (2 == flag)) || ((3 == i) && (4 == flag))) || ((6 == i) && (3 == flag)))
                {
                    sect_b_glyph_map(output_buf, i);
                }
            }
        }
        break;

            /* End Section 1 */

            /* Start Section C, Alphanumeric */
        case PANEL_LCD_LINE1 :
        {
            /* Cycle through Each Section */
            for (i = 0; i < 5; i++)
            {
                flag = validate_input(string[i], &output_buf);

                /* Is Alphanumeric or Supported Symbol? */
                if (flag)
                {
                    /* Map Glyph to Segment Pattern */
                    sect_c_glyph_map(output_buf, i + 1);
                }
            }
        }
        break;

            /* Start Section 2, Signed Numerical */
        case PANEL_LCD_LINE3 :
        {
            /* Turn off minus symbol */
            LCDC.SEG10 &= 0xF7;

            /* Cycle through each digit */
            for (i = 0; i < 4; i++)
            {
                /* Validate Input */
                flag = validate_input(string[i], &output_buf);

                /* Is Number? */
                if (((1 == flag) || (2 == flag)) || (4 == flag))
                {
                    if ((i > 1) && ('.' != string[2]))
                    {
                        /* Map Glyph to Segment Pattern */
                        sect_a_glyph_map(output_buf, i + 1);
                    }
                    else
                    {
                        /* Map Glyph to Segment Pattern */
                        sect_a_glyph_map(output_buf, i);
                    }
                }
            }
        }
        break;

            /* End Section 2 */

            /* Start Symbol Section */
        case PANEL_LCD_SYMBOL :
        {
            if (('F' == string[0]) || ('f' == string[0]))
            {
                /* Validate Input */
                flag = validate_input(string[1], &output_buf);

                /* Is Number ? */
                if (1 == flag)
                {
                    /* Map Glyph to Segment Pattern */
                    sect_f_glyph_map(output_buf);
                }
            }
            else if ('-' == string[0])
            {
                if (string[1])
                {
                    /* Turn off minus symbol */
                    LCDC.SEG10 |= 0x08U;
                }
                else
                {
                    /* Turn off minus symbol */
                    LCDC.SEG10 &= 0xF7;
                }
            }
            else
            {
                /* Convert To Symbol Code */
                symbsel = (uint8_t) string[0];
                symbsel = (symbsel << 8);
                symbsel |= (uint8_t) string[1];

                /* Map Symbol to Segment */
                R_LCD_DisplaySymbol(symbsel);
            }
        }
        break;

            /* End Symbol Section */

            /* Do nothing for undefined selections */
        default :
        {
            /* Bad Selection */
        }
        break;
    }

}

/*****************************************************************************
 End of Function R_LCD_DisplayPanelString
 ******************************************************************************/

/***********************************************************************************************************************
 * Function Name: R_LCD_Create
 * Description  : This function initializes the LCDC channel.
 * Arguments    : None
 * Return Value : None
 ***********************************************************************************************************************/
void R_LCD_Create (void)
{
    uint32_t i = 0;

    /* Enable writing to registers related to operating modes, LPC, CGC and software reset */
    SYSTEM.PRCR.WORD = 0xA507U;

    /* Cancel LCDC stop state in LPC */
    MSTP(LCDC) = 0U;

    /* Enable protection */
    SYSTEM.PRCR.WORD = 0xA500U;

    LCDC.LCDM1.BIT.LCDVLM = 1; /* Voltage Booting Pin Initial Control VCC <= 3.8v */

    LCDC.LCDM0.BIT.LBAS = 0x01; /* LCD Bias = 1/3 Bias Method */
    LCDC.LCDM0.BIT.LDTY = 0x03; /* Time Slice = 4-time Slice */
    LCDC.LCDM0.BIT.LWAVE = 0x00; /* LCD Display Waveform = Waveform A */
    LCDC.LCDM0.BIT.MDSET = 0x01; /* LCD Drive Voltage = External Boosting Method */

    MPC.PWPR.BYTE = 0x00; /* PWPR.PFSWE write protect off */
    MPC.PWPR.BYTE = 0x40; /* PFS register write protect off */

    PORT1.PMR.BYTE |= 0x0F; /* P10-P13 */
    PORT5.PMR.BYTE |= 0x7F; /* P50-P56 */
    PORTA.PMR.BYTE |= 0xBF; /* PA0-PA5,PA7 */
    PORTB.PMR.BYTE |= 0xFE; /* PB1-PB7 */
    PORTC.PMR.BYTE |= 0xFF; /* PC0-PC7 */
    PORTD.PMR.BYTE |= 0x1F; /* PD0-PD4 */
    PORTE.PMR.BYTE |= 0xFF; /* PE0-PE7 */
    PORTF.PMR.BYTE |= 0xC0; /* PF6-PF7 */

    MPC.P55PFS.BYTE = 0x18; /* P55:VL1 */
    MPC.P54PFS.BYTE = 0x18; /* P54:VL2 */
    MPC.PC7PFS.BYTE = 0x18; /* PC7:VL3 */
    MPC.PC6PFS.BYTE = 0x18; /* PC6:VL4 */

    MPC.PC5PFS.BYTE = 0x18; /* PC5:COM0 */
    MPC.PC4PFS.BYTE = 0x18; /* PC4:COM1 */
    MPC.PC3PFS.BYTE = 0x18; /* PC3:COM2 */
    MPC.PC2PFS.BYTE = 0x18; /* PC2:COM3 */

    MPC.P13PFS.BYTE = 0x18; /* P13:SEG0 */
    MPC.P12PFS.BYTE = 0x18; /* P12:SEG1 */
    MPC.P11PFS.BYTE = 0x18; /* P11:SEG2 */
    MPC.P10PFS.BYTE = 0x18; /* P10:SEG3 */

    MPC.P56PFS.BYTE = 0x18; /* P56:SEG4 */
    MPC.P53PFS.BYTE = 0x18; /* P53:SEG5 */
    MPC.P52PFS.BYTE = 0x18; /* P52:SEG6 */
    MPC.P51PFS.BYTE = 0x18; /* P51:SEG7 */
    MPC.P50PFS.BYTE = 0x18; /* P50:SEG8 */

    MPC.PC1PFS.BYTE = 0x18; /* PC1:SEG9 */
    MPC.PC0PFS.BYTE = 0x18; /* PC0:SEG10 */

    MPC.PB7PFS.BYTE = 0x18; /* PB7:SEG11 */
    MPC.PB6PFS.BYTE = 0x18; /* PB6:SEG12 */
    MPC.PB5PFS.BYTE = 0x18; /* PB5:SEG13 */
    MPC.PB4PFS.BYTE = 0x18; /* PB4:SEG14 */
    MPC.PB3PFS.BYTE = 0x18; /* PB3:SEG15 */
    MPC.PB2PFS.BYTE = 0x18; /* PB2:SEG16 */
    MPC.PB1PFS.BYTE = 0x18; /* PB1:SEG17 */

    MPC.PA7PFS.BYTE = 0x18; /* PA7:SEG18 */
    MPC.PA5PFS.BYTE = 0x18; /* PA5:SEG19 */
    MPC.PA4PFS.BYTE = 0x18; /* PA4:SEG20 */
    MPC.PA3PFS.BYTE = 0x18; /* PA3:SEG21 */
    MPC.PA2PFS.BYTE = 0x18; /* PA2:SEG22 */
    MPC.PA1PFS.BYTE = 0x18; /* PA1:SEG23 */
    MPC.PA0PFS.BYTE = 0x18; /* PA0:SEG24 */

    MPC.PF7PFS.BYTE = 0x18; /* PF7:SEG25 */
    MPC.PF6PFS.BYTE = 0x18; /* PF6:SEG26 */

    /* PExPFS�́A���̐ݒ�l���Ⴄ�̂Œ��� */
    MPC.PE5PFS.BYTE = 0x18; /* PE5:SEG27 */
    MPC.PE4PFS.BYTE = 0x18; /* PE4:SEG28 */
    MPC.PE3PFS.BYTE = 0x18; /* PE3:SEG29 */
    MPC.PE2PFS.BYTE = 0x18; /* PE2:SEG30 */
    MPC.PE1PFS.BYTE = 0x18; /* PE1:SEG31 */
    MPC.PE0PFS.BYTE = 0x18; /* PE0:SEG32 */
    MPC.PE7PFS.BYTE = 0x18; /* PE7:SEG33 */
    MPC.PE6PFS.BYTE = 0x18; /* PE6:SEG34 */

    MPC.PD4PFS.BYTE = 0x18; /* PD4:SEG35 */
    MPC.PD3PFS.BYTE = 0x18; /* PD3:SEG36 */
    MPC.PD2PFS.BYTE = 0x18; /* PD2:SEG37 */
    MPC.PD1PFS.BYTE = 0x18; /* PD1:SEG38 */
    MPC.PD0PFS.BYTE = 0x18; /* PD0:SEG39 */

    MPC.PWPR.BYTE = 0x80; /* PFS register write protect on */

    /* Reset all LCD Display Data Registers */
    LCDC.SEG00 = 0x00;
    LCDC.SEG01 = 0x00;
    LCDC.SEG02 = 0x00;
    LCDC.SEG03 = 0x00;
    LCDC.SEG04 = 0x00;
    LCDC.SEG05 = 0x00;
    LCDC.SEG06 = 0x00;
    LCDC.SEG07 = 0x00;
    LCDC.SEG08 = 0x00;
    LCDC.SEG09 = 0x00;
    LCDC.SEG10 = 0x00;
    LCDC.SEG11 = 0x00;
    LCDC.SEG12 = 0x00;
    LCDC.SEG13 = 0x00;
    LCDC.SEG14 = 0x00;
    LCDC.SEG15 = 0x00;
    LCDC.SEG16 = 0x00;
    LCDC.SEG17 = 0x00;
    LCDC.SEG18 = 0x00;
    LCDC.SEG19 = 0x00;
    LCDC.SEG20 = 0x00;
    LCDC.SEG21 = 0x00;
    LCDC.SEG22 = 0x00;
    LCDC.SEG23 = 0x00;
    LCDC.SEG24 = 0x00;
    LCDC.SEG25 = 0x00;
    LCDC.SEG26 = 0x00;
    LCDC.SEG27 = 0x00;
    LCDC.SEG28 = 0x00;
    LCDC.SEG29 = 0x00;
    LCDC.SEG30 = 0x00;
    LCDC.SEG31 = 0x00;
    LCDC.SEG32 = 0x00;
    LCDC.SEG33 = 0x00;
    LCDC.SEG34 = 0x00;
    LCDC.SEG35 = 0x00;
    LCDC.SEG36 = 0x00;
    LCDC.SEG37 = 0x00;
    LCDC.SEG38 = 0x00;
    LCDC.SEG39 = 0x00;

    LCDC.LCDM1.BIT.LCDSEL = 0;
    LCDC.LCDM1.BIT.BLON = 0; /* Display Area Control - Displaying an A-pattern */

    LCDC.LCDC0.BYTE = 0x05; /* LCD Clock = 1/64 of Sub-Clock */

    LCDC.VLCD.BYTE = 0x0C; /* Reference Voltage VL4 = 4.20V(1/3 bias) */

    LCDC.LCDM1.BIT.VLCON = 1; /* Enable voltage boot circuit */

    for (i = 0; i < 100000; i++)
    {
        ; /* Delay loop to allow voltage to settle */
    }

    init_maps(); /* Initialises the glyph-segment maps */
}

/*****************************************************************************
 End of Function R_LCD_Create
 ******************************************************************************/

/***********************************************************************************************************************
 * Function Name: R_LCD_Start
 * Description  : This function starts the LCDC channel.
 * Arguments    : None
 * Return Value : None
 ***********************************************************************************************************************/
void R_LCD_Start (void)
{
    LCDC.LCDM1.BIT.SCOC = 1;
    LCDC.LCDM1.BIT.LCDON = 1; /* LCD Display Enable */
}

/*****************************************************************************
 End of Function R_LCD_Start
 ******************************************************************************/

/***********************************************************************************************************************
 * Function Name: R_LCD_Stop
 * Description  : This function stops the LCDC channel.
 * Arguments    : None
 * Return Value : None
 ***********************************************************************************************************************/
void R_LCD_Stop (void)
{
    LCDC.LCDM1.BIT.SCOC = 0;
    LCDC.LCDM1.BIT.LCDON = 0; /* LCD Display Disable */
}

/*****************************************************************************
 End of Function R_LCD_Stop
 ******************************************************************************/

/***********************************************************************************************************************
 * Function Name: R_LCD_ClearDisplay
 * Description  : Clears all the segment data registers, thereby clearing the
 *  				 screen by the next LCD frame duration.
 * Arguments    : None
 * Return Value : None
 ***********************************************************************************************************************/
void R_LCD_ClearDisplay (void)
{
    uint8_t i;
    uint8_t* pregister;

    /* Initialise Pointer to Start of Registers */
    pregister = (uint8_t*) (&LCDC.SEG00);

    for (i = 0; i < 40; i++)
    {
        (*pregister) = 0;
        pregister++;
    }
}

/******************************************************************************
 End of Function R_LCD_ClearDisplay
 ******************************************************************************/

