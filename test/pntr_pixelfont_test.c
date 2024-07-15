#include <string.h> // sprintf

#define PNTR_IMPLEMENTATION
#include "pntr.h"
#include "pntr_assert.h"

#define PNTR_PIXELFONT_ENABLE_ALL
#define PNTR_PIXELFONT_IMPLEMENTATION
#include "pntr_pixelfont.h"

int main() {
    char name[256];

    // Iterate through all available fonts.
    for (int i = PNTR_PIXELFONT_FIRST; i < PNTR_PIXELFONT_LAST; i++) {
        // Load the font.
        pntr_font* font = pntr_load_pixelfont(i);
        pntr_assert(font != NULL);

        // Generate an image with the text.
        pntr_image* image = pntr_gen_image_text(font, "The strong grey fox jumps over the !", PNTR_BLACK, PNTR_RAYWHITE);
        pntr_assert(image != NULL);

        // Create some padding around the text.
        int padding = 1;
        pntr_image_resize_canvas(image, image->width + padding * 2, image->height + padding * 2, padding, padding, PNTR_RAYWHITE);

        // Scale the image appropriately.
        pntr_image* newImage = pntr_image_scale(image, 4, 4, PNTR_FILTER_NEARESTNEIGHBOR);
        pntr_unload_image(image);
        image = newImage;

        // Save the image.
        pntr_vector size = pntr_pixelfont_size(i);
        sprintf(name, "pntr_pixelfont_test_%dx%d.png", size.x, size.y);
        pntr_save_image(image, name);

        pntr_unload_image(image);
        pntr_unload_font(font);
    }

    return 0;
}
