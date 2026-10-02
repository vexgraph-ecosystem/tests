// tests/graphvex/element_property_test.c — mirrors src/ui/element.c
//
// Element's rectangle + style live in a pooled, SHAREABLE Property. Two Elements
// may point at one bound (a write is seen by both); owning detaches; revalidate
// clears only the dirty branches; a radius>0 parent clips its children.

#include <stdio.h>

#include "graphics/graphics.h"
#include "image.h"
#include "nio/property_pool.h"
#include "ui/element.h"
#include "ui/property.h"

static int g_fail = 0;
#define CHECK(cond)                                                        \
    do {                                                                   \
        if (!(cond)) {                                                     \
            printf("FAIL %s:%d %s\n", __FILE__, __LINE__, #cond);          \
            g_fail++;                                                      \
        }                                                                  \
    } while (0)

static int alpha_at(const Image *img, int x, int y) {
    return Image_pixels(img)[(size_t)y * Image_stride(img) + (size_t)x * 4 + 3];
}

int main(void) {
    // 1. sharing a bound: two Elements, one Property
    Element *a = Element();
    Element_setSize(a, 100, 50);
    Element_setBackground(a, COLOR_RGBA(10, 20, 30, 255));

    Element *b = Element();
    Element_setProperty(b, Element_property(a));         // bind (borrow)
    CHECK(Element_property(b) == Element_property(a));
    CHECK(Element_property(b)->background == COLOR_RGBA(10, 20, 30, 255));

    Element_setBackground(a, COLOR_RGBA(200, 100, 100, 255));
    CHECK(Element_property(b)->background == COLOR_RGBA(200, 100, 100, 255));  // shared write

    Element_ownProperty(b);                              // detach into a private copy
    CHECK(Element_property(b) != Element_property(a));
    Element_setBackground(a, COLOR_RGBA(1, 2, 3, 255));
    CHECK(Element_property(b)->background == COLOR_RGBA(200, 100, 100, 255));  // no longer follows

    // 2. revalidate + dirty subtree
    Element *root = Element();
    Element_setSize(root, 200, 200);
    Element_add(root, a);
    Element_revalidate(root);                            // settle
    CHECK(!Element_isDirty(root));
    Element_markDirty(a);
    CHECK(Element_isDirty(a) && Element_isDirty(root));  // marked up the chain
    Element_revalidate(root);
    CHECK(!Element_isDirty(root) && !Element_isDirty(a));

    // 3. a rounded parent clips its children (rectangular clip this slice)
    CHECK(Graphics_use(BACKEND_RASTER));
    CHECK(Graphics_resize(100, 100));
    Element *clipParent = Element();
    Element_setSize(clipParent, 60, 60);
    Element_setRadius(clipParent, 30);                   // also the clip trigger
    Element_setBackground(clipParent, COLOR_CLEAR);      // transparent body
    Element *big = Element();
    Element_setSize(big, 200, 200);
    Element_setBackground(big, COLOR_RGBA(255, 0, 0, 255));
    Element_add(clipParent, big);

    DisplayList *dl = DisplayList_0();
    Element_paint(clipParent, (Rect){0, 0, 60, 60}, dl);
    CHECK(Graphics_begin());
    CHECK(Graphics_clear(COLOR_CLEAR));
    CHECK(Graphics_submit(dl));
    CHECK(Graphics_end());
    Image *img = Image_0();
    CHECK(Graphics_capture(img));
    CHECK(alpha_at(img, 50, 50) == 255);                 // inside the viewport: child shows
    CHECK(alpha_at(img, 80, 80) == 0);                   // beyond it: clipped away
    Image_destroy(img);
    DisplayList_free(dl);

    Element_destroy(b);
    Element_destroy(clipParent);   // frees big
    Element_destroy(root);         // frees a

    if (g_fail == 0) printf("element_property_test: ALL PASS\n");
    return g_fail == 0 ? 0 : 1;
}
