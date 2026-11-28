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
 * The loaded font always covers printable ASCII: 0x20 (space) through 0x7E
 * (tilde), 95 glyphs, which is the same range as pntr's own default font.
 *
 * When pntr is built with PNTR_ENABLE_UTF8, the font also carries the upper
 * half of its uGUI table, glyphs 0x80 through 0xFF, each mapped to its Unicode
 * codepoint, so "é", "░" or "£" draw as expected. 4x6 is laid out as CP437, and
 * every other font as CP850. CP850's 0xD5 (dotless i) is left out, as several
 * fonts hold a broken glyph there. Without PNTR_ENABLE_UTF8, the font is
 * ASCII only, since raw bytes above 0x7F cannot name these glyphs.
 *
 * Any other character is skipped when drawing and measuring.
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
#if PNTR_PIXELFONT_DEFAULT == PNTR_PIXELFONT_4X6
    #ifndef PNTR_PIXELFONT_ENABLE_4X6
        #define PNTR_PIXELFONT_ENABLE_4X6
    #endif
#elif PNTR_PIXELFONT_DEFAULT == PNTR_PIXELFONT_5X8
    #ifndef PNTR_PIXELFONT_ENABLE_5X8
        #define PNTR_PIXELFONT_ENABLE_5X8
    #endif
#elif PNTR_PIXELFONT_DEFAULT == PNTR_PIXELFONT_5X12
    #ifndef PNTR_PIXELFONT_ENABLE_5X12
        #define PNTR_PIXELFONT_ENABLE_5X12
    #endif
#elif PNTR_PIXELFONT_DEFAULT == PNTR_PIXELFONT_6X8
    #ifndef PNTR_PIXELFONT_ENABLE_6X8
        #define PNTR_PIXELFONT_ENABLE_6X8
    #endif
#elif PNTR_PIXELFONT_DEFAULT == PNTR_PIXELFONT_6X10
    #ifndef PNTR_PIXELFONT_ENABLE_6X10
        #define PNTR_PIXELFONT_ENABLE_6X10
    #endif
#elif PNTR_PIXELFONT_DEFAULT == PNTR_PIXELFONT_7X12
    #ifndef PNTR_PIXELFONT_ENABLE_7X12
        #define PNTR_PIXELFONT_ENABLE_7X12
    #endif
#elif PNTR_PIXELFONT_DEFAULT == PNTR_PIXELFONT_8X8
    #ifndef PNTR_PIXELFONT_ENABLE_8X8
        #define PNTR_PIXELFONT_ENABLE_8X8
    #endif
#elif PNTR_PIXELFONT_DEFAULT == PNTR_PIXELFONT_8X12
    #ifndef PNTR_PIXELFONT_ENABLE_8X12
        #define PNTR_PIXELFONT_ENABLE_8X12
    #endif
#elif PNTR_PIXELFONT_DEFAULT == PNTR_PIXELFONT_8X14
    #ifndef PNTR_PIXELFONT_ENABLE_8X14
        #define PNTR_PIXELFONT_ENABLE_8X14
    #endif
#elif PNTR_PIXELFONT_DEFAULT == PNTR_PIXELFONT_10X16
    #ifndef PNTR_PIXELFONT_ENABLE_10X16
        #define PNTR_PIXELFONT_ENABLE_10X16
    #endif
#elif PNTR_PIXELFONT_DEFAULT == PNTR_PIXELFONT_12X16
    #ifndef PNTR_PIXELFONT_ENABLE_12X16
        #define PNTR_PIXELFONT_ENABLE_12X16
    #endif
#elif PNTR_PIXELFONT_DEFAULT == PNTR_PIXELFONT_12X20
    #ifndef PNTR_PIXELFONT_ENABLE_12X20
        #define PNTR_PIXELFONT_ENABLE_12X20
    #endif
#elif PNTR_PIXELFONT_DEFAULT == PNTR_PIXELFONT_16X26
    #ifndef PNTR_PIXELFONT_ENABLE_16X26
        #define PNTR_PIXELFONT_ENABLE_16X26
    #endif
#elif PNTR_PIXELFONT_DEFAULT == PNTR_PIXELFONT_22X36
    #ifndef PNTR_PIXELFONT_ENABLE_22X36
        #define PNTR_PIXELFONT_ENABLE_22X36
    #endif
#elif PNTR_PIXELFONT_DEFAULT == PNTR_PIXELFONT_24X40
    #ifndef PNTR_PIXELFONT_ENABLE_24X40
        #define PNTR_PIXELFONT_ENABLE_24X40
    #endif
#elif PNTR_PIXELFONT_DEFAULT == PNTR_PIXELFONT_32X53
    #ifndef PNTR_PIXELFONT_ENABLE_32X53
        #define PNTR_PIXELFONT_ENABLE_32X53
    #endif
#else
    #error "PNTR_PIXELFONT_DEFAULT must be one of the PNTR_PIXELFONT_* fonts, such as PNTR_PIXELFONT_8X8"
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

#ifdef PNTR_ENABLE_UTF8
/**
 * The Unicode codepoints of glyphs 0x80 through 0xFF, for PNTR_ENABLE_UTF8.
 * PNTR_PIXELFONT_4X6 is CP437. The other fonts are CP850. A zero skips that
 * glyph: CP850's 0xD5 (U+0131) is broken or truncated in several fonts.
 */
static const uint16_t PNTR_PIXELFONT_CP437[128] = {
    0x00C7, 0x00FC, 0x00E9, 0x00E2, 0x00E4, 0x00E0, 0x00E5, 0x00E7, // 0x80
    0x00EA, 0x00EB, 0x00E8, 0x00EF, 0x00EE, 0x00EC, 0x00C4, 0x00C5, // 0x88
    0x00C9, 0x00E6, 0x00C6, 0x00F4, 0x00F6, 0x00F2, 0x00FB, 0x00F9, // 0x90
    0x00FF, 0x00D6, 0x00DC, 0x00A2, 0x00A3, 0x00A5, 0x20A7, 0x0192, // 0x98
    0x00E1, 0x00ED, 0x00F3, 0x00FA, 0x00F1, 0x00D1, 0x00AA, 0x00BA, // 0xA0
    0x00BF, 0x2310, 0x00AC, 0x00BD, 0x00BC, 0x00A1, 0x00AB, 0x00BB, // 0xA8
    0x2591, 0x2592, 0x2593, 0x2502, 0x2524, 0x2561, 0x2562, 0x2556, // 0xB0
    0x2555, 0x2563, 0x2551, 0x2557, 0x255D, 0x255C, 0x255B, 0x2510, // 0xB8
    0x2514, 0x2534, 0x252C, 0x251C, 0x2500, 0x253C, 0x255E, 0x255F, // 0xC0
    0x255A, 0x2554, 0x2569, 0x2566, 0x2560, 0x2550, 0x256C, 0x2567, // 0xC8
    0x2568, 0x2564, 0x2565, 0x2559, 0x2558, 0x2552, 0x2553, 0x256B, // 0xD0
    0x256A, 0x2518, 0x250C, 0x2588, 0x2584, 0x258C, 0x2590, 0x2580, // 0xD8
    0x03B1, 0x00DF, 0x0393, 0x03C0, 0x03A3, 0x03C3, 0x00B5, 0x03C4, // 0xE0
    0x03A6, 0x0398, 0x03A9, 0x03B4, 0x221E, 0x03C6, 0x03B5, 0x2229, // 0xE8
    0x2261, 0x00B1, 0x2265, 0x2264, 0x2320, 0x2321, 0x00F7, 0x2248, // 0xF0
    0x00B0, 0x2219, 0x00B7, 0x221A, 0x207F, 0x00B2, 0x25A0, 0x00A0, // 0xF8
};

static const uint16_t PNTR_PIXELFONT_CP850[128] = {
    0x00C7, 0x00FC, 0x00E9, 0x00E2, 0x00E4, 0x00E0, 0x00E5, 0x00E7, // 0x80
    0x00EA, 0x00EB, 0x00E8, 0x00EF, 0x00EE, 0x00EC, 0x00C4, 0x00C5, // 0x88
    0x00C9, 0x00E6, 0x00C6, 0x00F4, 0x00F6, 0x00F2, 0x00FB, 0x00F9, // 0x90
    0x00FF, 0x00D6, 0x00DC, 0x00F8, 0x00A3, 0x00D8, 0x00D7, 0x0192, // 0x98
    0x00E1, 0x00ED, 0x00F3, 0x00FA, 0x00F1, 0x00D1, 0x00AA, 0x00BA, // 0xA0
    0x00BF, 0x00AE, 0x00AC, 0x00BD, 0x00BC, 0x00A1, 0x00AB, 0x00BB, // 0xA8
    0x2591, 0x2592, 0x2593, 0x2502, 0x2524, 0x00C1, 0x00C2, 0x00C0, // 0xB0
    0x00A9, 0x2563, 0x2551, 0x2557, 0x255D, 0x00A2, 0x00A5, 0x2510, // 0xB8
    0x2514, 0x2534, 0x252C, 0x251C, 0x2500, 0x253C, 0x00E3, 0x00C3, // 0xC0
    0x255A, 0x2554, 0x2569, 0x2566, 0x2560, 0x2550, 0x256C, 0x00A4, // 0xC8
    0x00F0, 0x00D0, 0x00CA, 0x00CB, 0x00C8, 0x0000, 0x00CD, 0x00CE, // 0xD0
    0x00CF, 0x2518, 0x250C, 0x2588, 0x2584, 0x00A6, 0x00CC, 0x2580, // 0xD8
    0x00D3, 0x00DF, 0x00D4, 0x00D2, 0x00F5, 0x00D5, 0x00B5, 0x00FE, // 0xE0
    0x00DE, 0x00DA, 0x00DB, 0x00D9, 0x00FD, 0x00DD, 0x00AF, 0x00B4, // 0xE8
    0x00AD, 0x00B1, 0x2017, 0x00BE, 0x00B6, 0x00A7, 0x00F7, 0x00B8, // 0xF0
    0x00B0, 0x00A8, 0x00B7, 0x00B9, 0x00B3, 0x00B2, 0x25A0, 0x00A0, // 0xF8
};
#endif

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Copy one glyph of a uGUI table into the given column of the atlas.
 */
static void pntr_pixelfont_draw_glyph(pntr_image* atlas, const pntr_pixelfont_font* pixelfont, int bytesPerRow, int glyph, int column) {
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

    // One atlas column per printable ASCII glyph, plus, with PNTR_ENABLE_UTF8,
    // one per glyph from 0x80 through 0xFF that has a Unicode codepoint.
    int columns = PNTR_PIXELFONT_CHARACTERS_LEN;
#ifdef PNTR_ENABLE_UTF8
    const uint16_t* codepoints = (font == PNTR_PIXELFONT_4X6) ? PNTR_PIXELFONT_CP437 : PNTR_PIXELFONT_CP850;
    for (int i = 0; i < 128; i++) {
        if (codepoints[i] != 0) {
            columns++;
        }
    }
#endif

    // Create the font atlas.
    pntr_image* atlas = pntr_gen_image_color(
        pixelfont->char_width * columns,
        pixelfont->char_height,
        PNTR_BLANK);
    if (atlas == NULL) {
        return (pntr_font*)pntr_set_error(PNTR_ERROR_NO_MEMORY);
    }

    // Copy glyphs 0x20 through 0x7E of the table into columns 0 through 94,
    // with the matching character set of one ASCII code per atlas column.
    // Room for up to 128 extra codepoints of up to three UTF-8 bytes each.
    char characters[PNTR_PIXELFONT_CHARACTERS_LEN + 128 * 3 + 1];
    int length = 0;
    for (int column = 0; column < PNTR_PIXELFONT_CHARACTERS_LEN; column++) {
        pntr_pixelfont_draw_glyph(atlas, pixelfont, bytesPerRow, PNTR_PIXELFONT_FIRST_CHARACTER + column, column);
        characters[length++] = (char)(PNTR_PIXELFONT_FIRST_CHARACTER + column);
    }

#ifdef PNTR_ENABLE_UTF8
    // Append glyphs 0x80 through 0xFF, each UTF-8 encoded by its codepoint, so
    // pntr finds them through PNTR_STRCHR and counts them with PNTR_STRLEN.
    int column = PNTR_PIXELFONT_CHARACTERS_LEN;
    for (int i = 0; i < 128; i++) {
        uint16_t codepoint = codepoints[i];
        if (codepoint == 0) {
            continue;
        }

        pntr_pixelfont_draw_glyph(atlas, pixelfont, bytesPerRow, 0x80 + i, column++);
        if (codepoint < 0x800) {
            characters[length++] = (char)(0xC0 | (codepoint >> 6));
        }
        else {
            characters[length++] = (char)(0xE0 | (codepoint >> 12));
            characters[length++] = (char)(0x80 | ((codepoint >> 6) & 0x3F));
        }
        characters[length++] = (char)(0x80 | (codepoint & 0x3F));
    }
#endif
    characters[length] = '\0'; // Null-terminate the string.

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
