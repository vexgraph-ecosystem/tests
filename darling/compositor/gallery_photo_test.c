// Gallery photograph bridge owner: decode the tracked photo reproducibly,
// preserve destination on rejection, and resolve an app-bundled copy without
// source-tree access. No window/appearance or production decoder claim.
#include "gallery_photo.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <CoreFoundation/CoreFoundation.h>
#include <CoreGraphics/CoreGraphics.h>
#include <ImageIO/ImageIO.h>
#include <unistd.h>

int main(int argc, char **argv) {
    const char *path = FILTER_GALLERY_SOURCE_RESOURCE;
    if (argc == 2 && !strcmp(argv[1], "--bundle"))
        path = nullptr;
    enum { WIDTH = 360, HEIGHT = 300, BYTES = WIDTH * HEIGHT * 4 };
    uint8_t *pixels = malloc(BYTES), *again = malloc(BYTES);
    assert(pixels && again);
    memset(pixels, 0x5a, BYTES);
    memcpy(again, pixels, BYTES);
    assert(!GalleryPhoto_decode(path, WIDTH, HEIGHT, nullptr, WIDTH * 4));
    assert(!GalleryPhoto_decode(path, 0, HEIGHT, pixels, WIDTH * 4));
    assert(!GalleryPhoto_decode(path, WIDTH, 0, pixels, WIDTH * 4));
    assert(!GalleryPhoto_decode(path, UINT32_MAX, UINT32_MAX, pixels, WIDTH * 4));
    assert(!GalleryPhoto_decode(path, WIDTH, HEIGHT, pixels, WIDTH * 4 - 1));
    assert(!GalleryPhoto_decode(path, WIDTH, HEIGHT, pixels, SIZE_MAX));
    assert(!GalleryPhoto_decode("/nonexistent-gallery-photo.png", WIDTH, HEIGHT, pixels, WIDTH * 4));
    assert(!memcmp(pixels, again, BYTES));

    // Encode an asymmetric 2x2 PNG to prove top-left orientation/channel order.
    uint8_t corners[] = {255,0,0,255, 0,255,0,255, 0,0,255,255, 255,255,255,255};
    char temporary[] = "gallery-photo-XXXXXX";
    int fd = mkstemp(temporary);
    assert(fd >= 0);
    close(fd);
    CFURLRef url = CFURLCreateFromFileSystemRepresentation(nullptr,
        (const UInt8*) temporary, (CFIndex) strlen(temporary), false);
    CGColorSpaceRef space = CGColorSpaceCreateWithName(kCGColorSpaceSRGB);
    CGDataProviderRef provider = CGDataProviderCreateWithData(nullptr, corners, sizeof corners, nullptr);
    CGImageRef image = CGImageCreate(2, 2, 8, 32, 8, space,
        kCGImageAlphaLast | kCGBitmapByteOrder32Big, provider, nullptr, false, kCGRenderingIntentDefault);
    CGImageDestinationRef destination = CGImageDestinationCreateWithURL(url, CFSTR("public.png"), 1, nullptr);
    assert(destination && image);
    CGImageDestinationAddImage(destination, image, nullptr);
    assert(CGImageDestinationFinalize(destination));
    CFRelease(destination);
    CGImageRelease(image);
    CGDataProviderRelease(provider);
    CGColorSpaceRelease(space);
    CFRelease(url);
    uint8_t decoded[16];
    assert(GalleryPhoto_decode(temporary, 2, 2, decoded, 8));
    assert(!memcmp(decoded, corners, sizeof corners));
    assert(!GalleryPhoto_decode(temporary, 1, GALLERY_PHOTO_OUTPUT_PIXEL_LIMIT + 1, decoded, 4));
    // Malformed data fails without touching the output, then valid input recovers.
    FILE *file = fopen(temporary, "wb");
    assert(file);
    assert(fwrite("bad PNG", 1, 7, file) == 7);
    assert(!fclose(file));
    assert(!GalleryPhoto_decode(temporary, 2, 2, decoded, 8));
    assert(!memcmp(decoded, corners, sizeof corners));
    assert(!unlink(temporary));
    assert(GalleryPhoto_decode(path, WIDTH, HEIGHT, pixels, WIDTH * 4));
    assert(GalleryPhoto_decode(path, WIDTH, HEIGHT, again, WIDTH * 4));
    assert(!memcmp(pixels, again, BYTES));
    unsigned differing = 0;
    for (size_t i = 0; i < BYTES; i += 4) {
        assert(pixels[i + 3] == 255);
        differing += pixels[i] != pixels[0] || pixels[i + 1] != pixels[1] || pixels[i + 2] != pixels[2];
    }
    assert(differing > WIDTH * HEIGHT / 2);
    free(again);
    free(pixels);
    puts("gallery_photo_test: PASS bounded photo decoding, orientation, rejection and recovery");
    return 0;
}
