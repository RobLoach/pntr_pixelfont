/**********************************************************************************************
*
*   pntr_pixelfont - Additional pixel fonts for pntr, with fonts from uGUI.
*
*   Copyright 2023-2026 Rob Loach (@RobLoach)
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
    PNTR_PIXELFONT_10X16,
    PNTR_PIXELFONT_12X16,
    PNTR_PIXELFONT_12X20,
    PNTR_PIXELFONT_16X26,
    PNTR_PIXELFONT_22X36,
    PNTR_PIXELFONT_24X40,
    PNTR_PIXELFONT_32X53,
    PNTR_PIXELFONT_LAST
} pntr_pixelfont;

/**
 * Load a pixel font.
 *
 * The loaded font covers printable ASCII only: 0x20 (space) through 0x7E
 * (tilde), 95 glyphs, which is the same range as pntr's own default font.
 * Any other character is skipped when drawing and measuring.
 *
 * The uGUI tables behind these fonts hold 256 raw single-byte glyphs in a
 * CP437-like arrangement, but a caller can only ask for a glyph through the
 * bytes of the string it draws. Codes above 0x7F would need a UTF-8 encoding
 * that raw byte tables cannot express, so they are not emitted. Keeping to
 * ASCII also makes the font identical whether or not pntr was built with
 * PNTR_ENABLE_UTF8, and keeps the atlas to 95 columns instead of 255.
 *
 * @param font The pixel font to load.
 *
 * @return The loaded font, or NULL on failure, or if the font is not supported.
 */
PNTR_PIXELFONT_API pntr_font* pntr_load_pixelfont(pntr_pixelfont font);

/**
 * Load the default pixel font: PNTR_PIXELFONT_DEFAULT, or 4x6 if not defined.
 *
 * The default font is always compiled in, whether or not its
 * PNTR_PIXELFONT_ENABLE_* macro is defined.
 *
 * Define PNTR_DEFAULT_FONT as pntr_load_pixelfont_default to use this as
 * pntr's default font. Include pntr_pixelfont.h before defining PNTR_IMPLEMENTATION
 * in pntr.h to activate this automatically.
 *
 * @return The loaded default font, or NULL on failure.
 */
PNTR_PIXELFONT_API pntr_font* pntr_load_pixelfont_default(void);

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

// The default font is always compiled in, so pntr_load_pixelfont_default()
// never fails for want of a PNTR_PIXELFONT_ENABLE_* define. The preprocessor
// cannot see the enum, so its names are mapped to their values just for this.
#ifndef PNTR_PIXELFONT_DEFAULT
    #define PNTR_PIXELFONT_DEFAULT PNTR_PIXELFONT_4X6
#endif
#define PNTR_PIXELFONT_4X6 0
#define PNTR_PIXELFONT_5X8 1
#define PNTR_PIXELFONT_5X12 2
#define PNTR_PIXELFONT_6X8 3
#define PNTR_PIXELFONT_6X10 4
#define PNTR_PIXELFONT_7X12 5
#define PNTR_PIXELFONT_8X8 6
#define PNTR_PIXELFONT_8X12 7
#define PNTR_PIXELFONT_8X14 8
#define PNTR_PIXELFONT_10X16 9
#define PNTR_PIXELFONT_12X16 10
#define PNTR_PIXELFONT_12X20 11
#define PNTR_PIXELFONT_16X26 12
#define PNTR_PIXELFONT_22X36 13
#define PNTR_PIXELFONT_24X40 14
#define PNTR_PIXELFONT_32X53 15
#if PNTR_PIXELFONT_DEFAULT == PNTR_PIXELFONT_4X6 && !defined(PNTR_PIXELFONT_ENABLE_4X6)
    #define PNTR_PIXELFONT_ENABLE_4X6
#elif PNTR_PIXELFONT_DEFAULT == PNTR_PIXELFONT_5X8 && !defined(PNTR_PIXELFONT_ENABLE_5X8)
    #define PNTR_PIXELFONT_ENABLE_5X8
#elif PNTR_PIXELFONT_DEFAULT == PNTR_PIXELFONT_5X12 && !defined(PNTR_PIXELFONT_ENABLE_5X12)
    #define PNTR_PIXELFONT_ENABLE_5X12
#elif PNTR_PIXELFONT_DEFAULT == PNTR_PIXELFONT_6X8 && !defined(PNTR_PIXELFONT_ENABLE_6X8)
    #define PNTR_PIXELFONT_ENABLE_6X8
#elif PNTR_PIXELFONT_DEFAULT == PNTR_PIXELFONT_6X10 && !defined(PNTR_PIXELFONT_ENABLE_6X10)
    #define PNTR_PIXELFONT_ENABLE_6X10
#elif PNTR_PIXELFONT_DEFAULT == PNTR_PIXELFONT_7X12 && !defined(PNTR_PIXELFONT_ENABLE_7X12)
    #define PNTR_PIXELFONT_ENABLE_7X12
#elif PNTR_PIXELFONT_DEFAULT == PNTR_PIXELFONT_8X8 && !defined(PNTR_PIXELFONT_ENABLE_8X8)
    #define PNTR_PIXELFONT_ENABLE_8X8
#elif PNTR_PIXELFONT_DEFAULT == PNTR_PIXELFONT_8X12 && !defined(PNTR_PIXELFONT_ENABLE_8X12)
    #define PNTR_PIXELFONT_ENABLE_8X12
#elif PNTR_PIXELFONT_DEFAULT == PNTR_PIXELFONT_8X14 && !defined(PNTR_PIXELFONT_ENABLE_8X14)
    #define PNTR_PIXELFONT_ENABLE_8X14
#elif PNTR_PIXELFONT_DEFAULT == PNTR_PIXELFONT_10X16 && !defined(PNTR_PIXELFONT_ENABLE_10X16)
    #define PNTR_PIXELFONT_ENABLE_10X16
#elif PNTR_PIXELFONT_DEFAULT == PNTR_PIXELFONT_12X16 && !defined(PNTR_PIXELFONT_ENABLE_12X16)
    #define PNTR_PIXELFONT_ENABLE_12X16
#elif PNTR_PIXELFONT_DEFAULT == PNTR_PIXELFONT_12X20 && !defined(PNTR_PIXELFONT_ENABLE_12X20)
    #define PNTR_PIXELFONT_ENABLE_12X20
#elif PNTR_PIXELFONT_DEFAULT == PNTR_PIXELFONT_16X26 && !defined(PNTR_PIXELFONT_ENABLE_16X26)
    #define PNTR_PIXELFONT_ENABLE_16X26
#elif PNTR_PIXELFONT_DEFAULT == PNTR_PIXELFONT_22X36 && !defined(PNTR_PIXELFONT_ENABLE_22X36)
    #define PNTR_PIXELFONT_ENABLE_22X36
#elif PNTR_PIXELFONT_DEFAULT == PNTR_PIXELFONT_24X40 && !defined(PNTR_PIXELFONT_ENABLE_24X40)
    #define PNTR_PIXELFONT_ENABLE_24X40
#elif PNTR_PIXELFONT_DEFAULT == PNTR_PIXELFONT_32X53 && !defined(PNTR_PIXELFONT_ENABLE_32X53)
    #define PNTR_PIXELFONT_ENABLE_32X53
#endif
#undef PNTR_PIXELFONT_4X6
#undef PNTR_PIXELFONT_5X8
#undef PNTR_PIXELFONT_5X12
#undef PNTR_PIXELFONT_6X8
#undef PNTR_PIXELFONT_6X10
#undef PNTR_PIXELFONT_7X12
#undef PNTR_PIXELFONT_8X8
#undef PNTR_PIXELFONT_8X12
#undef PNTR_PIXELFONT_8X14
#undef PNTR_PIXELFONT_10X16
#undef PNTR_PIXELFONT_12X16
#undef PNTR_PIXELFONT_12X20
#undef PNTR_PIXELFONT_16X26
#undef PNTR_PIXELFONT_22X36
#undef PNTR_PIXELFONT_24X40
#undef PNTR_PIXELFONT_32X53

#define PNTR_PIXELFONT_DATA_IMPLEMENTATION
#include "pntr_pixelfont_data.h"

/**
 * The first glyph of a uGUI table that pntr_load_pixelfont() emits: space.
 */
#define PNTR_PIXELFONT_FIRST_CHARACTER 0x20

/**
 * How many glyphs pntr_load_pixelfont() emits: 0x20 through 0x7E inclusive.
 */
#define PNTR_PIXELFONT_CHARACTERS_LEN 95

#ifdef __cplusplus
extern "C" {
#endif

PNTR_PIXELFONT_API pntr_font* pntr_load_pixelfont_default(void) {
    return pntr_load_pixelfont(PNTR_PIXELFONT_DEFAULT);
}

PNTR_PIXELFONT_API pntr_vector pntr_pixelfont_size(pntr_pixelfont font) {
    if (font < PNTR_PIXELFONT_FIRST || font >= PNTR_PIXELFONT_LAST) {
        return PNTR_CLITERAL(pntr_vector) {0, 0};
    }

    const pntr_pixelfont_font* pixelfont = &PNTR_PIXELFONT_FONTS[font];
    return PNTR_CLITERAL(pntr_vector) {
        pixelfont->char_width,
        pixelfont->char_height
    };
}

PNTR_PIXELFONT_API pntr_font* pntr_load_pixelfont(pntr_pixelfont font) {
    if (font < PNTR_PIXELFONT_FIRST || font >= PNTR_PIXELFONT_LAST) {
        return (pntr_font*)pntr_set_error(PNTR_ERROR_INVALID_ARGS);
    }

    // Get the font.
    const pntr_pixelfont_font* pixelfont = &PNTR_PIXELFONT_FONTS[font];
    if (pixelfont->data == NULL) {
        return (pntr_font*)pntr_set_error(PNTR_ERROR_NOT_SUPPORTED);
    }

    // How many bytes one row of one glyph takes up in the font data. The uGUI
    // tables pack each row into ceil(char_width / 8) bytes, so a glyph is
    // ceil(char_width / 8) * char_height bytes long.
    int bytesPerRow = ((int)pixelfont->char_width + 7) / 8;
    if (bytesPerRow <= 0) {
        return (pntr_font*)pntr_set_error(PNTR_ERROR_INVALID_ARGS);
    }

    // Create the font atlas, one column per printable ASCII glyph.
    pntr_image* atlas = pntr_gen_image_color(
        pixelfont->char_width * PNTR_PIXELFONT_CHARACTERS_LEN,
        pixelfont->char_height,
        PNTR_BLANK);
    if (atlas == NULL) {
        return (pntr_font*)pntr_set_error(PNTR_ERROR_NO_MEMORY);
    }

    // Copy glyphs 0x20 through 0x7E of the table into columns 0 through 94.
    for (int column = 0; column < PNTR_PIXELFONT_CHARACTERS_LEN; column++) {
        int glyph = PNTR_PIXELFONT_FIRST_CHARACTER + column;
        int index = glyph * pixelfont->char_height * bytesPerRow;
        for (int j = 0; j < pixelfont->char_height; j++) {
            int c = pixelfont->char_width;
            for (int i = 0; i < bytesPerRow; i++) {
                uint8_t b = pixelfont->data[index++];
                for (int k = 0; (k < 8) && c; k++) {
                    if (b & 0x01) {
                        pntr_draw_point(atlas, (column * pixelfont->char_width) + (i * 8) + k, j, PNTR_WHITE);
                    }

                    b >>= 1;
                    c--;
                }
            }
        }
    }

    // Build the matching character set: one ASCII code per atlas column. Every
    // byte here is below 0x80, so PNTR_STRLEN counts the same 95 characters
    // whether or not pntr redefined it to utf8len via PNTR_ENABLE_UTF8.
    char characters[PNTR_PIXELFONT_CHARACTERS_LEN + 1];
    for (int i = 0; i < PNTR_PIXELFONT_CHARACTERS_LEN; i++) {
        characters[i] = (char)(PNTR_PIXELFONT_FIRST_CHARACTER + i);
    }
    characters[PNTR_PIXELFONT_CHARACTERS_LEN] = '\0'; // Null-terminate the string.

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
