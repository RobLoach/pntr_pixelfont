#include <stdio.h> // printf

// Decode UTF-8, so the glyphs above 0x7F become reachable by codepoint.
#define PNTR_ENABLE_UTF8

#define PNTR_IMPLEMENTATION
#include "pntr.h"
#include "pntr_assert.h"

#define PNTR_PIXELFONT_ENABLE_ALL
#define PNTR_PIXELFONT_IMPLEMENTATION
#include "pntr_pixelfont.h"

#define E_ACUTE "\xC3\xA9"          // U+00E9, é, 0x82 in both CP437 and CP850
#define LIGHT_SHADE "\xE2\x96\x91"  // U+2591, ░, 0xB0 in both CP437 and CP850
#define ALPHA "\xCE\xB1"            // U+03B1, α, 0xE0 in CP437 only
#define O_ACUTE "\xC3\x93"          // U+00D3, Ó, 0xE0 in CP850 only
#define DOTLESS_I "\xC4\xB1"        // U+0131, ı, CP850's 0xD5, left out

int main() {
    for (int i = PNTR_PIXELFONT_FIRST; i < PNTR_PIXELFONT_LAST; i++) {
        pntr_font* font = pntr_load_pixelfont((pntr_pixelfont)i);
        pntr_assert(font != NULL);
        pntr_vector size = pntr_pixelfont_size((pntr_pixelfont)i);
        bool cp437 = i == PNTR_PIXELFONT_4X6;

        // The 95 ASCII glyphs, then 0x80 through 0xFF: all 128 for CP437, and
        // 127 for CP850, which leaves out 0xD5.
        int expected = 95 + (cp437 ? 128 : 127);
        printf("%dx%d charactersLen=%d atlas=%dx%d\n", size.x, size.y,
            font->charactersLen, font->atlas->width, font->atlas->height);
        pntr_assert(font->charactersLen == expected);
        pntr_assert(font->atlas->width == size.x * expected);
        pntr_assert(font->atlas->height == size.y);

        // ASCII still measures one character width per character.
        pntr_assert(pntr_measure_text_ex(font, "Hello", 0).x == size.x * 5);

        // Each extra glyph is one character wide, reached by its codepoint.
        pntr_assert(pntr_measure_text_ex(font, E_ACUTE, 0).x == size.x);
        pntr_assert(pntr_measure_text_ex(font, LIGHT_SHADE, 0).x == size.x);
        pntr_assert(pntr_measure_text_ex(font, "a" E_ACUTE LIGHT_SHADE "b", 0).x == size.x * 4);
        pntr_assert(pntr_measure_text_ex(font, ALPHA, 0).x == (cp437 ? size.x : 0));
        pntr_assert(pntr_measure_text_ex(font, O_ACUTE, 0).x == (cp437 ? 0 : size.x));
        pntr_assert(pntr_measure_text_ex(font, DOTLESS_I, 0).x == 0);

        // Drawing the glyph puts pixels on the image.
        pntr_image* image = pntr_gen_image_text(font, LIGHT_SHADE, PNTR_BLACK, PNTR_WHITE);
        pntr_assert(image != NULL);
        int inked = 0;
        for (int y = 0; y < image->height; y++) {
            for (int x = 0; x < image->width; x++) {
                if (pntr_image_get_color(image, x, y).value == PNTR_BLACK.value) {
                    inked++;
                }
            }
        }
        pntr_assert(inked > 0);
        pntr_unload_image(image);

        pntr_unload_font(font);
    }

    return 0;
}
