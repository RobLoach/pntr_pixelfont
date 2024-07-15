/**********************************************************************************************
*
*   pntr_pixelfont - Additional pixel fonts for pntr, with fonts from uGUI.
*
*   Copyright 2024 Rob Loach (@RobLoach)
*   Copyright (C) 2015, Achim Döbler, all rights reserved.
*
*   DEPENDENCIES:
*       pntr https://github.com/robloach/pntr
*
*   LICENSE: zlib/libpng
*
*   pntr_pixelfont is licensed under an unmodified zlib/libpng license, which is an OSI-certified,
*   BSD-like license that allows static linking with closed source software:
*
*   This software is provided "as-is", without any express or implied warranty. In no event
*   will the authors be held liable for any damages arising from the use of this software.
*
*   Permission is granted to anyone to use this software for any purpose, including commercial
*   applications, and to alter it and redistribute it freely, subject to the following restrictions:
*
*     1. The origin of this software must not be misrepresented; you must not claim that you
*     wrote the original software. If you use this software in a product, an acknowledgment
*     in the product documentation would be appreciated but is not required.
*
*     2. Altered source versions must be plainly marked as such, and must not be misrepresented
*     as being the original software.
*
*     3. This notice may not be removed or altered from any source distribution.
*
**********************************************************************************************/

#ifndef PNTR_PIXELFONT_H_
#define PNTR_PIXELFONT_H_

#ifdef __cplusplus
extern "C" {
#endif

#ifndef PNTR_PIXELFONT_API
    #define PNTR_PIXELFONT_API PNTR_API
#endif

typedef enum pntr_pixelfont {
    PNTR_PIXELFONT_FIRST = 0,
    PNTR_PIXELFONT_4X6 = 0,
    PNTR_PIXELFONT_5X8,
    PNTR_PIXELFONT_5X12,
    PNTR_PIXELFONT_6X8,
    PNTR_PIXELFONT_6X10,
    PNTR_PIXELFONT_7X12,
    PNTR_PIXELFONT_8X8,
    PNTR_PIXELFONT_8X12,
    PNTR_PIXELFONT_8X14,

    // TODO: Fix the following fonts.
    // PNTR_PIXELFONT_10X16,
    // PNTR_PIXELFONT_12X16,
    // PNTR_PIXELFONT_12X20,
    // PNTR_PIXELFONT_16X26,
    // PNTR_PIXELFONT_22X36,
    // PNTR_PIXELFONT_24X40,
    // PNTR_PIXELFONT_32X53,
    PNTR_PIXELFONT_LAST
} pntr_pixelfont;

/**
 * Load a pixel font.
 *
 * @param font The pixel font to load.
 *
 * @return The loaded font, or NULL on failure, or if the font is not supported.
 */
PNTR_PIXELFONT_API pntr_font* pntr_load_pixelfont(pntr_pixelfont font);

/**
 * Get the size of one character of the given pixel font.
 *
 * @param font The pixel font to get the character size of.
 *
 * @return A vector representing how large one character in the font is.
 */
PNTR_PIXELFONT_API pntr_vector pntr_pixelfont_size(pntr_pixelfont font);

#ifdef __cplusplus
}
#endif

#endif  // PNTR_PIXELFONT_H_

#ifdef PNTR_PIXELFONT_IMPLEMENTATION
#ifndef PNTR_PIXELFONT_IMPLEMENTATION_ONCE
#define PNTR_PIXELFONT_IMPLEMENTATION_ONCE

#define PNTR_PIXELFONT_DATA_IMPLEMENTATION
#include "pntr_pixelfont_data.h"

#ifdef __cplusplus
extern "C" {
#endif

PNTR_PIXELFONT_API pntr_vector pntr_pixelfont_size(pntr_pixelfont font) {
    if (font < PNTR_PIXELFONT_FIRST || font >= PNTR_PIXELFONT_LAST) {
        return (pntr_vector) {0, 0};
    }

    const pntr_pixelfont_font* pixelfont = &PNTR_PIXELFONT_FONTS[font];
    return (pntr_vector) {
        pixelfont->char_width,
        pixelfont->char_height
    };
}

PNTR_PIXELFONT_API pntr_font* pntr_load_pixelfont(pntr_pixelfont font) {
    if (font < PNTR_PIXELFONT_FIRST || font >= PNTR_PIXELFONT_LAST) {
        return pntr_set_error(PNTR_ERROR_INVALID_ARGS);
    }

    // Get the font.
    const pntr_pixelfont_font* pixelfont = &PNTR_PIXELFONT_FONTS[font];
    if (pixelfont->data == NULL) {
        return pntr_set_error(PNTR_ERROR_NOT_SUPPORTED);
    }

    // Create the font atlas.
    pntr_image* atlas = pntr_gen_image_color(
        pixelfont->char_width * 255,
        pixelfont->char_height,
        PNTR_BLANK);
    if (atlas == NULL) {
        return pntr_set_error(PNTR_ERROR_NO_MEMORY);
    }

    // Iterate through all the characters and draw them manually.
    // const uint8_t* bitmap = data + i;
    // for (int x = 0; x < pixelfont.char_width; x++) {
    //     for (int y = 0; y < pixelfont.char_height; y++) {
    //         if (bitmap[y] & 1 << x) {
    //             pntr_draw_point(atlas, pixelfont.char_width * i + x, y, PNTR_WHITE);
    //         }
    //     }
    // }

    for (int chr = 0; chr < 255; chr++) {
        uint8_t bt = (uint8_t)chr;
        switch ( bt ) {
            case 0xF6: bt = 0x94; break;
            case 0xD6: bt = 0x99; break;
            case 0xFC: bt = 0x81; break;
            case 0xDC: bt = 0x9A; break;
            case 0xE4: bt = 0x84; break;
            case 0xC4: bt = 0x8E; break;
            case 0xB5: bt = 0xE6; break;
            case 0xB0: bt = 0xF8; break;
        }

        uint8_t b;
        int bn = pixelfont->char_width;
        bn >>= 3;
        if ( pixelfont->char_width % 8 ) bn++;

        int index = (bt - 0) * pixelfont->char_height * bn;
        for( int j=0;j<pixelfont->char_height;j++ )
        {
            int c=pixelfont->char_width;
            for(int  i=0;i<bn;i++ )
            {
                b = pixelfont->data[index++];
                for(int k=0;(k<8) && c;k++ )
                {
                    if( b & 0x01 )
                    {
                        //push_pixel(fc);
                        pntr_draw_point(atlas, chr * pixelfont->char_width + k, j, PNTR_WHITE);
                        //pntr_draw_point(atlas, x + pixelfont.char_width - 1, y + pixelfont.char_height - 1, PNTR_WHITE);
                    }
                    else
                    {
                        //push_pixel(bc);
                    }
                    b >>= 1;
                    c--;
                }
            }
        }
    }

    // Build the character set.
    char characters[255];
    characters[0] = (char)255; // First character is blank.
    for (int i = 1; i < 254; i++) {
        characters[i] = (char)(i);
    }
    characters[254] = '\0'; // Null-terminate the string.

    // Use TTY to build the remaining font parameters.
    pntr_font* output = pntr_load_font_tty_from_image(atlas, (int)pixelfont->char_width, (int)pixelfont->char_height, characters);
    if (output == NULL) {
        pntr_unload_image(atlas);
        return NULL;
    }

    return output;
}

#ifdef __cplusplus
}
#endif

#endif  // PNTR_PIXELFONT_IMPLEMENTATION_ONCE
#endif  // PNTR_PIXELFONT_IMPLEMENTATION
