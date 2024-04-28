#include <string.h>

#define PNTR_IMPLEMENTATION
#include "pntr.h"

#define PNTR_PIXELFONTS_ENABLE_ALL
#define PNTR_PIXELFONTS_IMPLEMENTATION
#include "pntr_pixelfonts.h"

// Assertion library
#include "pntr_assert.h"

int main() {
    char name[256];

    for (int i = PNTR_PIXELFONT_FIRST; i < PNTR_PIXELFONT_LAST; i++) {
        pntr_font* font = pntr_load_pixelfont(i);
        pntr_assert(font != NULL);

        pntr_image* image = pntr_gen_image_text(font, "Hello, World!", PNTR_RED);
        pntr_assert(image != NULL);

        pntr_image* newImage = pntr_image_scale(image, 2, 2, PNTR_FILTER_BILINEAR);
        pntr_unload_image(image);
        image = newImage;

        // Save the image.
        pntr_vector size = pntr_pixelfonts_size(i);
        sprintf(name, "pntr_pixelfonts_test_%dx%d.png", size.x, size.y);
        pntr_save_image(image, name);

        pntr_unload_image(image);
        pntr_unload_font(font);
    }

    return 0;
}
