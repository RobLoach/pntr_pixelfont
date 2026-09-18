#include <stdio.h> // sprintf

// Switch the default font.
#define PNTR_DEFAULT_FONT pntr_load_pixelfont_default

#include "pntr.h"
#define PNTR_PIXELFONT_ENABLE_ALL
#include "pntr_pixelfont.h"

#define PNTR_IMPLEMENTATION
#include "pntr.h"
#include "pntr_assert.h"

#define PNTR_PIXELFONT_IMPLEMENTATION
#include "pntr_pixelfont.h"

int main() {
    char text[256];
    pntr_image* rows[PNTR_PIXELFONT_LAST];
    int maxWidth = 0;
    int totalHeight = 0;

    // Verify pntr_load_font_default() returns the 4x6 pixelfont via PNTR_DEFAULT_FONT.
    pntr_font* defaultFont = pntr_load_font_default();
    pntr_assert(defaultFont != NULL);
    pntr_unload_font(defaultFont);

    // Render a row of text for every available font.
    for (int i = PNTR_PIXELFONT_FIRST; i < PNTR_PIXELFONT_LAST; i++) {
        // Load the font.
        pntr_font* font = pntr_load_pixelfont(i);
        pntr_assert(font != NULL);

        // Generate an image with the text, labelled with the font size.
        pntr_vector size = pntr_pixelfont_size(i);
        sprintf(text, "%dx%d  The quick brown fox jumps over the lazy dog!", size.x, size.y);
        rows[i] = pntr_gen_image_text(font, text, PNTR_BLACK, PNTR_RAYWHITE);
        pntr_assert(rows[i] != NULL);

        // Find out how large the combined image has to be.
        if (rows[i]->width > maxWidth) {
            maxWidth = rows[i]->width;
        }
        totalHeight += rows[i]->height;

        pntr_unload_font(font);
    }

    // Create the combined image, with padding around every row.
    int padding = 1;
    pntr_image* combined = pntr_gen_image_color(
        maxWidth + padding * 2,
        totalHeight + padding * (PNTR_PIXELFONT_LAST + 1),
        PNTR_RAYWHITE);
    pntr_assert(combined != NULL);

    // Draw each of the rows into the combined image.
    int y = padding;
    for (int i = PNTR_PIXELFONT_FIRST; i < PNTR_PIXELFONT_LAST; i++) {
        pntr_draw_image(combined, rows[i], padding, y);
        y += rows[i]->height + padding;
        pntr_unload_image(rows[i]);
    }

    // Scale the image appropriately.
    pntr_image* scaled = pntr_image_scale(combined, 2, 2, PNTR_FILTER_NEARESTNEIGHBOR);
    pntr_unload_image(combined);
    pntr_assert(scaled != NULL);

    // Save the image. The call is kept out of the assert so that it still runs
    // when NDEBUG compiles the assertion away.
    bool saved = pntr_save_image(scaled, "pntr_pixelfont_test.png");
    pntr_assert(saved);
    (void)saved;

    pntr_unload_image(scaled);

    return 0;
}
