/* Pointwise seam owner for compositor/compositor.{h,c} plus inline constructors.
 * Independent straight-color oracle versus premultiplied implementation; every
 * implemented color form, signed-zero/endpoints, tiny alpha, HDR and saturation,
 * ordered groups, recipe retention, unchanged bounds, rejection/retry/borrowing.
 * Cold CPU, external synchronization only. No GPU, typed-pool migration, OOM
 * injection or thread-safety claim. Existing compositor_test owns other APIs. */
#include "compositor/compositor.h"
#include "compositor/filter_pool.h"
#include "compositor/compositor_scope.h"
#include <assert.h>
#include <float.h>
#include <math.h>
#include <stdio.h>
#include <string.h>

static void closeValue(float actual, double expected) {
    assert(isfinite(actual));
    double tolerance = fmax(1e-44, fabs(expected) * 3e-6);
    assert(fabs(actual - expected) <= tolerance);
}

/* Oracle explicitly works in straight linear color, not the implementation's
 * premultiplied affine formulas. Known coefficient ratios express Rec.709. */
static void oracle(const float *input, FilterToken token, double *expected) {
    double a = input[3];
    double rgb[3] = {0};
    if (a > 0)
        for (unsigned c = 0; c < 3; ++c)
            rgb[c] = input[c] / a;
    uint32_t bits = (uint32_t) Filter_payload(token);
    float scalar;
    memcpy(&scalar, &bits, sizeof scalar);
    uint16_t id = Filter_id(token);
    double gray = (2126 * rgb[0] + 7152 * rgb[1] + 722 * rgb[2]) / 10000;
    if (id == GRAYSCALE_RED_ID) gray = rgb[0];
    if (id == GRAYSCALE_GREEN_ID) gray = rgb[1];
    if (id == GRAYSCALE_BLUE_ID) gray = rgb[2];
    if (id == BLACK_AND_WHITE_ID) gray = gray >= scalar ? 1 : 0;
    for (unsigned c = 0; c < 3; ++c) {
        double value = gray;
        if (id == BRIGHTNESS_ID) value = rgb[c] + scalar;
        if (id == CONTRAST_ID) value = (rgb[c] - .5) * scalar + .5;
        if (id == INVERT_ID) value = 1 - rgb[c];
        if (id == BRIGHTNESS_ID || id == CONTRAST_ID || id == INVERT_ID)
            value = fmin(1, fmax(0, value));
        expected[c] = a > 0 ? fmin(FLT_MAX, value * a) : 0;
    }
    expected[3] = a;
}

static CompositorSurface *source(const float *rgba) {
    CompositorBounds bounds = {-7, 11, 1, 1};
    CompositorSurface *s = CompositorSurface(bounds);
    assert(s);
    memcpy(CompositorSurface_pixels(s), rgba, 4 * sizeof(float));
    return s;
}

static void numericMatrix(void) {
    const FilterToken tokens[] = {
        Filter_brightness(-1), Filter_brightness(-.25f), Filter_brightness(-0.0f),
        Filter_brightness(.25f), Filter_brightness(1), Filter_contrast(-0.0f),
        Filter_contrast(1), Filter_contrast(2), Filter_contrast(FLT_MAX),
        Filter_grayscale(), Filter_grayscaleRed(), Filter_grayscaleGreen(),
        Filter_grayscaleBlue(), Filter_invert(), Filter_blackAndWhite(0),
        Filter_blackAndWhite(.5f), Filter_blackAndWhite(1)
    };
    const float alphas[] = {0, -0.0f, FLT_MIN, .25f, .5f, 1};
    const float colors[][3] = {{0, 0, 0}, {1, 1, 1}, {1, 0, 0},
        {0, 1, 0}, {0, 0, 1}, {.125f, .375f, .875f}, {4, 2, 1}};
    for (size_t i = 0; i < sizeof tokens / sizeof tokens[0]; ++i) {
        for (size_t j = 0; j < sizeof alphas / sizeof alphas[0]; ++j) {
            for (size_t k = 0; k < sizeof colors / sizeof colors[0]; ++k) {
                float rgba[4] = {colors[k][0] * alphas[j], colors[k][1] * alphas[j],
                    colors[k][2] * alphas[j], alphas[j]};
                CompositorSurface *s = source(rgba), *out = NULL;
                const CompositorSurface *inputs[] = {s};
                assert(Compositor_compose(inputs, 1, &tokens[i], 1, &out) == COMPOSITOR_OK);
                double expected[4];
                oracle(rgba, tokens[i], expected);
                const float *p = CompositorSurface_constPixels(out);
                for (unsigned c = 0; c < 4; ++c)
                    closeValue(p[c], expected[c]);
                assert(CompositorSurface_validate(out) == COMPOSITOR_OK);
                CompositorBounds b = CompositorSurface_bounds(out);
                assert(b.x == -7 && b.y == 11 && b.width == 1 && b.height == 1);
                assert(!memcmp(rgba, CompositorSurface_constPixels(s), sizeof rgba));
                CompositorSurface_destroy(out);
                CompositorSurface_destroy(s);
            }
        }
    }
    float tiny[4] = {nextafterf(0, 1), 0, 0, nextafterf(0, 1)};
    CompositorSurface *s = source(tiny), *out = NULL;
    const CompositorSurface *inputs[] = {s};
    FilterToken invert = Filter_invert();
    assert(Compositor_compose(inputs, 1, &invert, 1, &out) == COMPOSITOR_OK);
    const float *p = CompositorSurface_constPixels(out);
    assert(p[0] == 0 && p[1] == tiny[3] && p[2] == tiny[3] && p[3] == tiny[3]);
    CompositorSurface_destroy(out);
    CompositorSurface_destroy(s);
    const float huge[4] = {FLT_MAX, FLT_MAX, FLT_MAX, FLT_MIN};
    s = source(huge); inputs[0] = s;
    FilterToken gray = Filter_grayscale();
    assert(Compositor_compose(inputs, 1, &gray, 1, &out) == COMPOSITOR_OK);
    assert(CompositorSurface_constPixels(out)[0] == FLT_MAX);
    CompositorSurface_destroy(out);
    CompositorSurface_destroy(s);
}

static void rejectionAndRecovery(void) {
    const float rgba[4] = {.25f, .5f, .75f, 1};
    CompositorSurface *s = source(rgba);
    const CompositorSurface *inputs[] = {s};
    const FilterToken bad[] = {
        Filter_brightness(nextafterf(1, INFINITY)), Filter_brightness(nextafterf(-1, -INFINITY)),
        Filter_brightness(NAN), Filter_brightness(INFINITY), Filter_contrast(-1),
        Filter_contrast(NAN), Filter_contrast(INFINITY),
        Filter_blackAndWhite(nextafterf(0, -INFINITY)),
        Filter_blackAndWhite(nextafterf(1, INFINITY)), Filter_blackAndWhite(NAN),
        Filter_brightness(0) | (UINT64_C(1) << 32),
        Filter_contrast(1) | (UINT64_C(1) << 47),
        Filter_blackAndWhite(.5f) | (UINT64_C(1) << 32),
        Filter_grayscale() | 1, Filter_grayscaleRed() | 1,
        Filter_grayscaleGreen() | 1, Filter_grayscaleBlue() | 1, Filter_invert() | 1
    };
    for (size_t i = 0; i < sizeof bad / sizeof bad[0]; ++i) {
        CompositorSurface *out = s;
        FilterToken stack[] = {Filter_brightness(.25f), bad[i]};
        assert(Compositor_compose(inputs, 1, stack, 2, &out) == COMPOSITOR_INVALID);
        assert(out == s && !memcmp(rgba, CompositorSurface_constPixels(s), sizeof rgba));
        CompositorBounds before = {5, 6, 7, 8}, bounds = before;
        assert(Compositor_filterBounds((CompositorBounds) {0}, &bad[i], 1, &bounds) == COMPOSITOR_INVALID);
        assert(!memcmp(&before, &bounds, sizeof bounds));
        FilterToken valid = Filter_contrast(1);
        assert(Compositor_compose(inputs, 1, &valid, 1, &out) == COMPOSITOR_OK);
        assert(!memcmp(rgba, CompositorSurface_constPixels(out), sizeof rgba));
        CompositorSurface_destroy(out);
    }
    CompositorSurface_destroy(s);
}

static void orderAndRecipes(void) {
    const float rgba[4] = {.125f, .125f, .125f, .5f};
    CompositorSurface *s = source(rgba);
    const CompositorSurface *inputs[] = {s};
    FilterToken forward[] = {Filter_brightness(.25f), Filter_contrast(2)};
    FilterToken reverse[] = {forward[1], forward[0]};
    CompositorSurface *a = NULL, *b = NULL;
    assert(Compositor_compose(inputs, 1, forward, 2, &a) == COMPOSITOR_OK);
    assert(Compositor_compose(inputs, 1, reverse, 2, &b) == COMPOSITOR_OK);
    assert(CompositorSurface_constPixels(a)[0] == .25f);
    assert(CompositorSurface_constPixels(b)[0] == .125f);
    FilterPool *pool = NULL;
    FilterPoolConfig config = {1, 2};
    assert(FilterPool(config, &pool) == FILTER_POOL_OK);
    FilterToken reference;
    assert(FilterPool_insert(pool, forward, 2, &reference) == FILTER_POOL_OK);
    CompositorSurface *pooled = NULL;
    assert(FilterPool_compose(pool, inputs, 1, &reference, 1, &pooled) == FILTER_POOL_OK);
    assert(!memcmp(CompositorSurface_constPixels(a), CompositorSurface_constPixels(pooled), 4 * sizeof(float)));
    assert(FilterPool_release(pool, reference) == FILTER_POOL_OK);
    assert(FilterPool_destroy(pool) == FILTER_POOL_OK);
    CompositorSurface_destroy(pooled);
    CompositorSurface_destroy(a);
    CompositorSurface_destroy(b);
    CompositorSurface_destroy(s);
}

static void scopeEquivalence(void) {
    const float rgba[4] = {.125f, .375f, .875f, 1};
    CompositorSurface *s = source(rgba);
    const CompositorSurface *inputs[] = {s};
    const FilterToken tokens[] = {Filter_brightness(.25f), Filter_contrast(2),
        Filter_grayscale(), Filter_grayscaleRed(), Filter_grayscaleGreen(),
        Filter_grayscaleBlue(), Filter_invert(), Filter_blackAndWhite(.5f)};
    for (size_t i = 0; i < sizeof tokens / sizeof tokens[0]; ++i) {
        double expected[4];
        oracle(rgba, tokens[i], expected);
        for (unsigned scope = 0; scope < 3; ++scope) {
            CompositorScopeDesc desc = {.priorScene = s,
                .panelBounds = {-7, 11, 1, 1}};
            if (scope == 0) {
                desc.backdropFilters = &tokens[i];
                desc.backdropFilterCount = 1;
            } else {
                desc.foreground = inputs;
                desc.foregroundCount = 1;
                if (scope == 1) {
                    desc.foregroundFilters = &tokens[i];
                    desc.foregroundFilterCount = 1;
                } else {
                    desc.elementFilters = &tokens[i];
                    desc.elementFilterCount = 1;
                }
            }
            CompositorSurface *out = NULL;
            assert(Compositor_scopedScene(&desc, &out) == COMPOSITOR_OK);
            const float *p = CompositorSurface_constPixels(out);
            for (unsigned c = 0; c < 4; ++c)
                closeValue(p[c], expected[c]);
            CompositorSurface_destroy(out);
        }
    }
    CompositorSurface_destroy(s);
}

int main(void) {
    numericMatrix();
    rejectionAndRecovery();
    orderAndRecipes();
    scopeEquivalence();
    puts("CPU linear pointwise filters: numeric/alpha/HDR/order/rejection/recipe PASS");
    return 0;
}
