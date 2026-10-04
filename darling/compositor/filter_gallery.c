#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "drawable/picture.h"
#include "frame/frame.h"
#include "panel/panel.h"
#include "darling/compositor/filter_gallery_fixture.h"


// Interactive gallery, intentionally not an automatically registered _test.
// --smoke performs native captured-pixel checks then closes; it is lab proof,
// never user appearance approval. All filters run in CPU reference preparation.
typedef struct FilterGalleryState {
    Picture *pictures[6];
    Image *images[6];
    Panel *cards[3]; // borrowed from Frame; anchors resolved by the real tree
} FilterGalleryState;
static FilterGalleryState gallery;

static void galleryClosed(Frame *frame, void *userdata) {
    (void) frame;
    FilterGalleryState *state = userdata;
    for (unsigned i = 0; i < 6; ++i) {
        Picture_destroy((*state).pictures[i]);
        (*state).pictures[i] = nullptr;
        Image_destroy((*state).images[i]);
        (*state).images[i] = nullptr;
    }
}

static bool sampleMatches(const Image *capture, const Image *source,
                          unsigned x, unsigned y, unsigned sx, unsigned sy) {
    if (!capture || x >= Image_width(capture) || y >= Image_height(capture))
        return false;
    const uint8_t *a = Image_pixels(capture) + (size_t) y * Image_stride(capture) + (size_t) x * 4;
    const uint8_t *b = Image_pixels(source) + (size_t) sy * Image_stride(source) + (size_t) sx * 4;
    for (unsigned c = 0; c < 4; ++c)
        if (abs((int) a[c] - b[c]) > 1) {
            fprintf(stderr, "sample %u,%u channel %u got %u expected %u\n", x, y, c, a[c], b[c]);
            return false;
        }
    return true;
}

static uint64_t galleryFrozenClock(void *userdata) {
    (void) userdata;
    return 1000000000ULL;
}

// Read the already-published native surface. Unlike Frame_capture, this cannot
// repaint and conceal a deferred resize or a stale anchor position.
static Image *galleryPublished(Frame *frame) {
    void *surface = Window_presentSurfaceContents(Frame_window(frame));
    if (!surface)
        return nullptr;
    unsigned width = Surface_width(Frame_surface(frame));
    unsigned height = Surface_height(Frame_surface(frame));
    Image *image = Image_2(width, height);
    if (!image || !Image_ensureShadow(image, width, height)) {
        Image_destroy(image);
        return nullptr;
    }
    Image_fill(image, COLOR_CLEAR);
    if (!Window_readPresentSurface(Frame_window(frame), surface, Image_pixels(image), Image_stride(image))) {
        Image_destroy(image);
        return nullptr;
    }
    return image;
}

#define DARLING_TEST_HAS_FRAMES
#define DARLING_TEST_WITH_ARGS
#include "darling/test_application.h"

int main(int argc, char **argv) {
    bool smoke = argc > 1 && strcmp(argv[1], "--smoke") == 0;
    Frame *frame = Frame("Graphvex filters | Backdrop / Foreground / Element", 1160, 430);
    if (!frame)
        return 1;
    Frame_setBackground(frame, COLOR_RGBA(11, 15, 24, 255));
    // The gallery rests when clean. Do not impose a 30 Hz content ceiling on
    // native resize; changed geometry is published immediately by Frame_setSize.
    Frame_onClose(frame, galleryClosed, &gallery);
    for (unsigned i = 0; i < 3; ++i) {
        if (FilterGallery_make(i, &gallery.images[i * 2]) != COMPOSITOR_OK) {
            Frame_destroy(frame);
            return 1;
        }
        gallery.images[i * 2 + 1] = FilterGallery_caption(i);
        if (!gallery.images[i * 2 + 1]) {
            Frame_destroy(frame);
            return 1;
        }
        Panel *card = Panel(372, 394);
        if (!card) {
            Frame_destroy(frame);
            return 1;
        }
        static const int anchors[] = {PART_TOP_LEFT, PART_TOP_CENTER, PART_TOP_RIGHT};
        Panel_setAnchor(card, anchors[i]);
        Panel_setPivot(card, anchors[i]);
        Panel_setOffset(card, i == 0 ? 10 : i == 1 ? 0 : -10, 14);
        Panel_setBackground(card, COLOR_RGBA(18, 23, 35, 255));
        Panel_setRadius(card, 10);
        if (!Frame_add_2(frame, card)) {
            Frame_destroy(frame);
            return 1;
        }
        gallery.cards[i] = card;
        for (unsigned j = 0; j < 2; ++j) {
            unsigned index = i * 2 + j;
            Picture *picture = Picture(gallery.images[index]);
            if (!picture) {
                Frame_destroy(frame);
                return 1;
            }
            gallery.pictures[index] = picture;
            Picture_setLocation(picture, 6, j ? 6 : 82);
            Element_add(Panel_graphics(card), Picture_graphics(picture));
        }
    }
    Frame_show(frame);
    if (smoke)
        Surface_setClock(Frame_surface(frame), galleryFrozenClock, nullptr);
    Frame_render(frame);
    if (smoke) {
        static const int widths[] = {1160, 1400, 1240, 1632};
        for (unsigned step = 0; step < sizeof widths / sizeof widths[0]; ++step) {
            uint64_t before = Surface_getPresentCount(Frame_surface(frame));
            if (step)
                Window_setSize(Frame_window(frame), widths[step], 430 + (int) step * 20);
            if (step && Surface_getPresentCount(Frame_surface(frame)) != before + 1) {
                fprintf(stderr, "filter_gallery: geometry publication was deferred\n");
                Frame_destroy(frame);
                return 1;
            }
            Image *capture = galleryPublished(frame);
            if (!capture) {
                Frame_destroy(frame);
                fprintf(stderr, "SKIP: gallery native IOSurface unavailable\n");
                return B_TEST_SKIP;
            }
            for (unsigned i = 0; i < 3; ++i) {
                Rect bound = Element_eventBound(Panel_graphics(gallery.cards[i]), Frame_root(frame));
                float expected = i == 0 ? 10 : i == 1 ? ((float) widths[step] - 372) * 0.5f :
                                                        (float) widths[step] - 382;
                if (bound.x != expected || bound.y != 14 || bound.w != 372 || bound.h != 394 ||
                    !sampleMatches(capture, gallery.images[i * 2], (unsigned) expected + 16, 116, 10, 20) ||
                    !sampleMatches(capture, gallery.images[i * 2], (unsigned) expected + 66, 226, 60, 130)) {
                    fprintf(stderr, "filter_gallery: resized anchor/Picture mismatch in case %u step %u\n", i + 1, step);
                    Image_destroy(capture);
                    Frame_destroy(frame);
                    return 1;
                }
            }
            Image_destroy(capture);
        }
        Frame_destroy(frame);
        puts("filter_gallery: PASS (left/center/right anchors, four extents, frozen-clock native publication)");
    } else {
        puts("Gallery open: backdrop LEFT; child blur CENTER; element blur RIGHT. Drag the window edges to compare anchors.");
        Darling_testKeepOpen();
    }
    return 0;
}
