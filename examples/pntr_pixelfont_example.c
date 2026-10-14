// pntr_pixelfont example: render "Hello, World!" with the 8x12 pixel font and
// save it as a PNG. This is the Usage snippet from README.md, verbatim, so that
// a compiler checks the include order the README documents.

// 1. Optional: make one of the pixel fonts pntr's default font. These two
//    defines have to come before pntr's implementation is compiled in step 4,
//    because that is the code that reads PNTR_DEFAULT_FONT. Without
//    PNTR_PIXELFONT_DEFAULT the default pixel font is PNTR_PIXELFONT_4X6.
#define PNTR_PIXELFONT_DEFAULT PNTR_PIXELFONT_8X8
#define PNTR_DEFAULT_FONT pntr_load_pixelfont_default

// 2. pntr.h first, for the types pntr_pixelfont.h is written against
//    (pntr_font, pntr_image, pntr_vector, PNTR_API).
#include "pntr.h"

// 3. pntr_pixelfont.h next, so that pntr_load_pixelfont_default() is declared
//    before pntr's implementation refers to it through PNTR_DEFAULT_FONT.
//    Pick which fonts get compiled in here.
#define PNTR_PIXELFONT_ENABLE_ALL
#include "pntr_pixelfont.h"

// 4. pntr's implementation, which picks up PNTR_DEFAULT_FONT from step 1.
#define PNTR_IMPLEMENTATION
#include "pntr.h"

// 5. pntr_pixelfont's implementation last, since it calls into pntr.
#define PNTR_PIXELFONT_IMPLEMENTATION
#include "pntr_pixelfont.h"

int main() {
    // Load one of the pixel fonts.
    pntr_font* font = pntr_load_pixelfont(PNTR_PIXELFONT_8X12);

    // Generate an image with the text in it.
    pntr_image* image = pntr_gen_image_text(font, "Hello, World!", PNTR_BLACK, PNTR_RAYWHITE);

    // Save the image locally.
    pntr_save_image(image, "pntr_pixelfont_8x12.png");

    // Unload the font and image.
    pntr_unload_image(image);
    pntr_unload_font(font);

    // Thanks to step 1, pntr's own default font is now PNTR_PIXELFONT_8X8.
    pntr_font* defaultFont = pntr_load_font_default();
    pntr_unload_font(defaultFont);

    return 0;
}
