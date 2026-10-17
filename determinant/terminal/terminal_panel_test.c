// Owner: TerminalPanel — the layered panel. Slot ids and their staleness rule,
// total ordered paint, the reserve flow, per-layer background/foreground/style,
// visibility releasing a band, reentrant-paint refusal with a once-per-epoch
// report, cold growth past the named default, the hit-test query, and the
// projections.
//
// The grounding case is semicolon's IDE example (103x23, root pipes at 1/29/103),
// resolved here as a real panel and then as a nested child panel for the centre's
// tab strip / separator / editor — with the child resolving over the sub-canvas
// its parent handed it, which is what makes nesting free of ownership.
//
// Scope note: this owner proves the panel's own contract. It does not prove
// device output, allocation-failure injection (unreachable in-process), or a
// real nested paint through a child panel's draw callback; those stay gaps.
#include "terminal/terminal_panel.h"
#include "terminal/terminal_color.h"
#include "determinant/support.h"

enum { W = 24, H = 8, ORDER_MAX = 32 };

typedef struct LayerProbe {
    size_t tag;            // slot-order tag recorded in the shared order log
    size_t calls;          // draw invocations
    TerminalRect region;   // last region handed to the callback
    TerminalPen pen;       // last pen handed to the callback
} LayerProbe;

static size_t gOrder[ORDER_MAX];
static size_t gOrderCount;
static TerminalPanel *gReenter;
static size_t gReentrantPaints;

static void probeDraw(void *context, TerminalCanvas *region, TerminalRect placement, TerminalPen pen) {
    LayerProbe *probe = context;
    ++(*probe).calls;
    // The sub-canvas and the placement it was cut from must agree, always.
    assert(TerminalCanvas_getWidth(region) == placement.width);
    assert(TerminalCanvas_getHeight(region) == placement.height);
    (*probe).region = placement;
    (*probe).pen = pen;
    if (gOrderCount < ORDER_MAX)
        gOrder[gOrderCount] = (*probe).tag;
    ++gOrderCount;
}

static void reentrantDraw(void *context, TerminalCanvas *region, TerminalRect placement, TerminalPen pen) {
    (void) context;
    (void) placement;
    ++gReentrantPaints;
    TerminalPanel_paint(gReenter, region);   // must be refused as reentrant
    TerminalCanvas_fill(region, 'x', pen);
}

static bool rectEqual(TerminalRect a, TerminalRect b) {
    return a.x == b.x && a.y == b.y && a.width == b.width && a.height == b.height;
}
static TerminalRect rectOf(size_t x, size_t y, size_t w, size_t h) {
    return (TerminalRect) { x, y, w, h };
}

int main(void) {
    uint32_t cells[W * H];
    uint8_t styles[W * H];
    uint32_t fg[W * H];
    uint32_t bg[W * H];
    TerminalCanvas canvas = TerminalCanvas(cells, styles, fg, bg, W, H);

    // --- Constructor and null safety ---
    TerminalPanel *panel = TerminalPanel();
    assert(panel != nullptr);
    assert(TerminalPanel_layerCount(panel) == 0 && TerminalPanel_capacity(panel) == 0);
    assert(TerminalPanel_paintCount(panel) == 0 && TerminalPanel_exhaustionCount(panel) == 0);
    assert(!TerminalPanel_isPainting(panel));
    TerminalPanel *zeroed = TerminalPanel_zero();
    assert(zeroed != nullptr && TerminalPanel_layerCount(zeroed) == 0);
    TerminalPanel_free(zeroed);
    TerminalPanel_free(nullptr);              // harmless
    TerminalPanel_clearExhaustion(nullptr);   // harmless

    assert(TerminalPanel_layerCount(nullptr) == 0);
    assert(TerminalPanel_capacity(nullptr) == 0);
    assert(TerminalPanel_paintCount(nullptr) == 0);
    assert(TerminalPanel_exhaustionCount(nullptr) == 0);
    assert(!TerminalPanel_isPainting(nullptr));
    assert(!TerminalPanel_isValid(nullptr, (TerminalLayerId) { 0, 1 }));
    assert(!TerminalPanel_isLayerId((TerminalLayerId) { 0, 0 }));
    assert(!TerminalPanel_isLayerId((TerminalLayerId) { 3, 0 }));
    assert(TerminalPanel_isLayerId((TerminalLayerId) { 0, 1 }));
    assert(TerminalPanel_getAnchor(nullptr, (TerminalLayerId) { 0, 1 }).kind == TERMINAL_ANCHOR_FILL);
    assert(TerminalPanel_getBackgroundColor(nullptr, (TerminalLayerId) { 0, 1 }) == TERMINAL_COLOR_DEFAULT);
    assert(TerminalPanel_getForegroundColor(nullptr, (TerminalLayerId) { 0, 1 }) == TERMINAL_COLOR_DEFAULT);
    assert(TerminalPanel_getStyle(nullptr, (TerminalLayerId) { 0, 1 }) == TERMINAL_STYLE_NONE);
    assert(!TerminalPanel_isVisible(nullptr, (TerminalLayerId) { 0, 1 }));
    assert(TerminalPanel_getImage(nullptr, (TerminalLayerId) { 0, 1 }) == nullptr);
    assert(TerminalPanel_getDraw(nullptr, (TerminalLayerId) { 0, 1 }) == nullptr);
    assert(TerminalPanel_getContext(nullptr, (TerminalLayerId) { 0, 1 }) == nullptr);
    assert(TerminalPanel_regionOf(nullptr, &canvas, (TerminalLayerId) { 0, 1 }).width == 0);
    assert(TerminalPanel_regionOf(panel, nullptr, (TerminalLayerId) { 0, 1 }).width == 0);
    // The hot entry guard is QUIET and counts nothing (the Hot-Path Minimal Guard Law).
    beginDiagnostics();
    TerminalPanel_paint(nullptr, &canvas);
    TerminalPanel_paint(panel, nullptr);
    TerminalCanvas invalid = TerminalCanvas_0();
    TerminalPanel_paint(panel, &invalid);
    endDiagnostics(0, nullptr);
    assert(TerminalPanel_paintCount(panel) == 0);

    // --- Slots: valid ids, fresh-layer defaults, out-of-range rejection ---
    TerminalLayerId backdrop = TerminalPanel_add(panel, TerminalAnchor());
    TerminalLayerId title = TerminalPanel_add(panel, TerminalAnchor(TERMINAL_ANCHOR_BAND_TOP, 1));
    TerminalLayerId rail = TerminalPanel_add(panel, TerminalAnchor(TERMINAL_ANCHOR_BAND_LEFT, 8));
    TerminalLayerId centre = TerminalPanel_add(panel, TerminalAnchor());
    assert(TerminalPanel_isLayerId(backdrop) && TerminalPanel_isValid(panel, backdrop));
    assert(TerminalPanel_isValid(panel, title) && TerminalPanel_isValid(panel, centre));
    assert(TerminalPanel_layerCount(panel) == 4);
    assert(TerminalPanel_capacity(panel) == TERMINAL_PANEL_LAYERS_DEFAULT);
    assert(TerminalPanel_isVisible(panel, title));
    assert(TerminalPanel_getAnchor(panel, title).kind == TERMINAL_ANCHOR_BAND_TOP);
    assert(TerminalPanel_getAnchor(panel, title).extent == 1);
    assert(TerminalPanel_getBackgroundColor(panel, title) == TERMINAL_COLOR_DEFAULT);
    assert(TerminalPanel_getForegroundColor(panel, title) == TERMINAL_COLOR_DEFAULT);
    assert(TerminalPanel_getStyle(panel, title) == TERMINAL_STYLE_NONE);
    assert(TerminalPanel_getImage(panel, title) == nullptr);
    assert(TerminalPanel_getFit(panel, title) == TERMINAL_LAYER_FIT_STRETCH);
    assert(TerminalPanel_getDraw(panel, title) == nullptr);
    assert(!TerminalPanel_isValid(panel, (TerminalLayerId) { 0, 0 }));      // zero id
    assert(!TerminalPanel_isValid(panel, (TerminalLayerId) { 99, 1 }));     // out of range
    assert(!TerminalPanel_isValid(panel, (TerminalLayerId) { 0, 2 }));      // wrong generation
    REJECT(TerminalPanel_isLayerId(TerminalPanel_add(nullptr, TerminalAnchor())), "add");
    REJECT(TerminalPanel_remove(nullptr, title), "remove");

    // --- Colours and style: round trip, and rejection preserves state ---
    assert(TerminalPanel_setBackgroundColor(panel, rail, TERMINAL_RGB(0x22, 0x22, 0x22)));
    assert(TerminalPanel_getBackgroundColor(panel, rail) == TERMINAL_RGB(0x22, 0x22, 0x22));
    assert(TerminalPanel_setForegroundColor(panel, rail, TERMINAL_RGB(0xEE, 0xEE, 0xEE)));
    assert(TerminalPanel_getForegroundColor(panel, rail) == TERMINAL_RGB(0xEE, 0xEE, 0xEE));
    assert(TerminalPanel_setStyle(panel, rail, TERMINAL_STYLE_MUTED));
    assert(TerminalPanel_getStyle(panel, rail) == TERMINAL_STYLE_MUTED);
    REJECT(TerminalPanel_setBackgroundColor(panel, (TerminalLayerId) { 0, 0 }, 1u), "background");
    REJECT(TerminalPanel_setForegroundColor(panel, (TerminalLayerId) { 5, 9 }, 1u), "foreground");
    REJECT(TerminalPanel_setStyle(panel, (TerminalLayerId) { 5, 9 }, 1u), "style");
    REJECT(TerminalPanel_setAnchor(panel, (TerminalLayerId) { 5, 9 }, TerminalAnchor()), "anchor");
    REJECT(TerminalPanel_setVisible(panel, (TerminalLayerId) { 5, 9 }, false), "visible");
    REJECT(TerminalPanel_setDraw(panel, (TerminalLayerId) { 5, 9 }, probeDraw, nullptr), "draw");
    REJECT(TerminalPanel_setImage(panel, (TerminalLayerId) { 5, 9 }, nullptr, TERMINAL_LAYER_FIT_STRETCH), "image");
    REJECT(TerminalPanel_setBackgroundColor(nullptr, rail, 1u), "background");
    REJECT(TerminalPanel_setForegroundColor(nullptr, rail, 1u), "foreground");
    REJECT(TerminalPanel_setStyle(nullptr, rail, 0u), "style");
    REJECT(TerminalPanel_setAnchor(nullptr, rail, TerminalAnchor()), "anchor");
    REJECT(TerminalPanel_setVisible(nullptr, rail, true), "visible");
    REJECT(TerminalPanel_setDraw(nullptr, rail, probeDraw, nullptr), "draw");
    REJECT(TerminalPanel_setImage(nullptr, rail, nullptr, TERMINAL_LAYER_FIT_STRETCH), "image");
    assert(TerminalPanel_getBackgroundColor(panel, rail) == TERMINAL_RGB(0x22, 0x22, 0x22));
    assert(TerminalPanel_getStyle(panel, rail) == TERMINAL_STYLE_MUTED);

    // --- A bad fit is refused before the layer is touched ---
    REJECT(TerminalPanel_setImage(panel, rail, nullptr, (TerminalLayerFit) 99), "fit");
    assert(TerminalPanel_getFit(panel, rail) == TERMINAL_LAYER_FIT_STRETCH);
    assert(TerminalPanel_setImage(panel, rail, nullptr, TERMINAL_LAYER_FIT_CONTAIN));
    assert(TerminalPanel_getFit(panel, rail) == TERMINAL_LAYER_FIT_CONTAIN);
    assert(TerminalPanel_setImage(panel, rail, nullptr, TERMINAL_LAYER_FIT_STRETCH));
    assert(TerminalPanel_getImage(panel, rail) == nullptr);

    // --- Paint is TOTAL, ordered, and resolves the reserve flow ---
    LayerProbe backdropProbe = { .tag = 0 };
    LayerProbe titleProbe = { .tag = 1 };
    LayerProbe railProbe = { .tag = 2 };
    LayerProbe centreProbe = { .tag = 3 };
    assert(TerminalPanel_setDraw(panel, backdrop, probeDraw, &backdropProbe));
    assert(TerminalPanel_setDraw(panel, title, probeDraw, &titleProbe));
    assert(TerminalPanel_setDraw(panel, rail, probeDraw, &railProbe));
    assert(TerminalPanel_setDraw(panel, centre, probeDraw, &centreProbe));
    assert(TerminalPanel_getDraw(panel, title) == probeDraw);
    assert(TerminalPanel_getContext(panel, title) == &titleProbe);

    gOrderCount = 0;
    beginDiagnostics();
    TerminalPanel_paint(panel, &canvas);
    endDiagnostics(0, nullptr);                                // a clean paint is silent
    assert(TerminalPanel_paintCount(panel) == 1);
    assert(backdropProbe.calls == 1 && titleProbe.calls == 1);
    assert(railProbe.calls == 1 && centreProbe.calls == 1);
    assert(gOrderCount == 4);
    assert(gOrder[0] == 0 && gOrder[1] == 1 && gOrder[2] == 2 && gOrder[3] == 3);   // slot order = z

    // FILL reserves nothing, so the backdrop spans the panel and the bands still flow.
    assert(rectEqual(backdropProbe.region, rectOf(0, 0, W, H)));
    assert(rectEqual(titleProbe.region, rectOf(0, 0, W, 1)));
    assert(rectEqual(railProbe.region, rectOf(0, 1, 8, H - 1)));
    assert(rectEqual(centreProbe.region, rectOf(8, 1, W - 8, H - 1)));
    // The layer's pen reaches its painter.
    assert(railProbe.pen.bg == TERMINAL_RGB(0x22, 0x22, 0x22));
    assert(railProbe.pen.fg == TERMINAL_RGB(0xEE, 0xEE, 0xEE));
    assert(railProbe.pen.style == TERMINAL_STYLE_MUTED);

    // --- It paints EVERY single time: no dirty tracking, no skipping ---
    gOrderCount = 0;
    TerminalPanel_paint(panel, &canvas);
    TerminalPanel_paint(panel, &canvas);
    assert(TerminalPanel_paintCount(panel) == 3);
    assert(titleProbe.calls == 3 && centreProbe.calls == 3 && railProbe.calls == 3);
    assert(gOrderCount == 8);                            // 4 layers x 2 paints, nothing skipped
    assert(gOrder[0] == 0 && gOrder[3] == 3 && gOrder[4] == 0 && gOrder[7] == 3);

    // --- The layer background fills its resolved region ---
    // The swatch is a BOTTOM band added AFTER the centre's FILL. FILL consumes
    // nothing, so the cursor still carries the rail's reservation and the band
    // spans only the unreserved remainder — cols 8..23 of the last row.
    TerminalCanvas_fill(&canvas, ' ', TerminalPen());
    TerminalLayerId swatch = TerminalPanel_add(panel, TerminalAnchor(TERMINAL_ANCHOR_BAND_BOTTOM, 1));
    assert(TerminalPanel_setBackgroundColor(panel, swatch, TERMINAL_RGB(1, 2, 3)));
    TerminalPanel_paint(panel, &canvas);
    assert(TerminalCanvas_bgAt(&canvas, 8, H - 1) == TERMINAL_RGB(1, 2, 3));
    assert(TerminalCanvas_glyphAt(&canvas, 8, H - 1) == ' ');
    assert(TerminalCanvas_bgAt(&canvas, 0, H - 1) == TERMINAL_RGB(0x22, 0x22, 0x22));   // the rail kept its column
    assert(TerminalCanvas_bgAt(&canvas, 0, 0) == TERMINAL_COLOR_DEFAULT);               // the title band is unpainted

    // --- Hidden layers neither draw nor reserve, so the flow closes up ---
    assert(TerminalPanel_setVisible(panel, title, false));
    assert(!TerminalPanel_isVisible(panel, title));
    gOrderCount = 0;
    titleProbe.calls = 0;
    railProbe.calls = 0;
    centreProbe.calls = 0;
    TerminalPanel_paint(panel, &canvas);
    assert(titleProbe.calls == 0);                       // no draw
    assert(railProbe.calls == 1 && centreProbe.calls == 1);
    assert(gOrderCount == 3);                            // backdrop, rail, centre (the swatch has no painter)
    assert(rectEqual(railProbe.region, rectOf(0, 0, 8, H)));      // the band was released
    assert(rectEqual(centreProbe.region, rectOf(8, 0, W - 8, H)));
    assert(TerminalPanel_setVisible(panel, title, true));

    // --- Cold hit-test query agrees with what paint resolved ---
    assert(rectEqual(TerminalPanel_regionOf(panel, &canvas, backdrop), rectOf(0, 0, W, H)));
    assert(rectEqual(TerminalPanel_regionOf(panel, &canvas, title), rectOf(0, 0, W, 1)));
    assert(rectEqual(TerminalPanel_regionOf(panel, &canvas, rail), rectOf(0, 1, 8, H - 1)));
    assert(rectEqual(TerminalPanel_regionOf(panel, &canvas, centre), rectOf(8, 1, W - 8, H - 1)));
    TerminalRect unknown = TerminalPanel_regionOf(panel, &canvas, (TerminalLayerId) { 0, 0 });
    assert(unknown.width == 0 && unknown.height == 0);

    // --- Reentrant paint is refused, counted, and reported once per epoch ---
    TerminalLayerId nested = TerminalPanel_add(panel, TerminalAnchor());
    gReenter = panel;
    gReentrantPaints = 0;
    assert(TerminalPanel_setDraw(panel, nested, reentrantDraw, nullptr));
    beginDiagnostics();
    TerminalPanel_paint(panel, &canvas);
    endDiagnostics(1, "reentrant");
    assert(gReentrantPaints == 1);
    assert(TerminalPanel_exhaustionCount(panel) == 1);
    assert(!TerminalPanel_isPainting(panel));
    beginDiagnostics();
    TerminalPanel_paint(panel, &canvas);                 // same epoch: no second line
    endDiagnostics(0, nullptr);
    assert(TerminalPanel_exhaustionCount(panel) == 2);   // but both are counted
    TerminalPanel_clearExhaustion(panel);
    assert(TerminalPanel_exhaustionCount(panel) == 0);
    beginDiagnostics();
    TerminalPanel_paint(panel, &canvas);                 // a fresh epoch reports again
    endDiagnostics(1, "reentrant");
    assert(TerminalPanel_exhaustionCount(panel) == 1);
    assert(TerminalPanel_setDraw(panel, nested, nullptr, nullptr));
    assert(TerminalPanel_getDraw(panel, nested) == nullptr);
    assert(TerminalPanel_getContext(panel, nested) == nullptr);
    TerminalPanel_clearExhaustion(panel);

    // --- Removal, slot reuse, and the staleness rule ---
    size_t liveBefore = TerminalPanel_layerCount(panel);
    TerminalLayerId doomed = TerminalPanel_add(panel, TerminalAnchor());
    assert(TerminalPanel_isValid(panel, doomed));
    assert(TerminalPanel_layerCount(panel) == liveBefore + 1);
    assert(TerminalPanel_remove(panel, doomed));
    assert(TerminalPanel_layerCount(panel) == liveBefore);
    assert(!TerminalPanel_isValid(panel, doomed));                 // retired
    assert(!TerminalPanel_isVisible(panel, doomed));
    assert(TerminalPanel_getDraw(panel, doomed) == nullptr);
    assert(TerminalPanel_getImage(panel, doomed) == nullptr);
    REJECT(TerminalPanel_remove(panel, doomed), "stale");
    REJECT(TerminalPanel_setAnchor(panel, doomed, TerminalAnchor()), "stale");
    TerminalLayerId reused = TerminalPanel_add(panel, TerminalAnchor());
    assert(TerminalPanel_isValid(panel, reused));
    assert(reused.index == doomed.index);                          // same slot...
    assert(reused.generation != doomed.generation);                // ...new generation
    assert(!TerminalPanel_isValid(panel, doomed));                 // the old id stays stale
    assert(!TerminalLayerId_equal(doomed, reused));
    assert(TerminalLayerId_equal(reused, reused));
    assert(TerminalPanel_remove(panel, reused));

    // --- Cold growth past the named default; every live layer survives ---
    TerminalPanel *grown = TerminalPanel();
    assert(grown != nullptr && TerminalPanel_capacity(grown) == 0);
    for (size_t i = 0; i < TERMINAL_PANEL_LAYERS_DEFAULT * 3; ++i) {
        TerminalLayerId id = TerminalPanel_add(grown, TerminalAnchor());
        assert(TerminalPanel_isLayerId(id));
    }
    assert(TerminalPanel_layerCount(grown) == TERMINAL_PANEL_LAYERS_DEFAULT * 3);
    assert(TerminalPanel_capacity(grown) >= TERMINAL_PANEL_LAYERS_DEFAULT * 3);
    assert(TerminalPanel_exhaustionCount(grown) == 0);
    beginDiagnostics();
    TerminalPanel_paint(grown, &canvas);
    endDiagnostics(0, nullptr);
    assert(TerminalPanel_paintCount(grown) == 1);
    TerminalPanel_free(grown);

    // --- A moving anchor re-flows the layers beneath it ---
    TerminalPanel *mover = TerminalPanel();
    TerminalLayerId band = TerminalPanel_add(mover, TerminalAnchor(TERMINAL_ANCHOR_BAND_TOP, 3));
    TerminalLayerId rest = TerminalPanel_add(mover, TerminalAnchor());
    assert(rectEqual(TerminalPanel_regionOf(mover, &canvas, band), rectOf(0, 0, W, 3)));
    assert(rectEqual(TerminalPanel_regionOf(mover, &canvas, rest), rectOf(0, 3, W, H - 3)));
    assert(TerminalPanel_setAnchor(mover, band, TerminalAnchor(TERMINAL_ANCHOR_BAND_BOTTOM, 2)));
    assert(rectEqual(TerminalPanel_regionOf(mover, &canvas, band), rectOf(0, H - 2, W, 2)));
    assert(rectEqual(TerminalPanel_regionOf(mover, &canvas, rest), rectOf(0, 0, W, H - 2)));
    assert(TerminalPanel_setVisible(mover, band, false));
    assert(rectEqual(TerminalPanel_regionOf(mover, &canvas, rest), rectOf(0, 0, W, H)));
    assert(TerminalPanel_regionOf(mover, &canvas, band).width == 0);
    TerminalPanel_free(mover);

    // --- GROUNDING: semicolon's IDE example as a real panel over 103x23 ---
    static uint32_t ideCells[103 * 23];
    static uint8_t ideStyles[103 * 23];
    TerminalCanvas ide = TerminalCanvas(ideCells, ideStyles, 103, 23);
    TerminalPanel *idePanel = TerminalPanel();
    TerminalLayerId ideTitle = TerminalPanel_add(idePanel, TerminalAnchor(TERMINAL_ANCHOR_BAND_TOP, 1));
    TerminalLayerId ideStatus = TerminalPanel_add(idePanel, TerminalAnchor(TERMINAL_ANCHOR_BAND_BOTTOM, 1));
    TerminalLayerId ideRail = TerminalPanel_add(idePanel, TerminalAnchor(TERMINAL_ANCHOR_BAND_LEFT, 29));
    TerminalLayerId ideCentre = TerminalPanel_add(idePanel, TerminalAnchor());
    assert(rectEqual(TerminalPanel_regionOf(idePanel, &ide, ideTitle), rectOf(0, 0, 103, 1)));
    assert(rectEqual(TerminalPanel_regionOf(idePanel, &ide, ideStatus), rectOf(0, 22, 103, 1)));
    assert(rectEqual(TerminalPanel_regionOf(idePanel, &ide, ideRail), rectOf(0, 1, 29, 21)));
    assert(rectEqual(TerminalPanel_regionOf(idePanel, &ide, ideCentre), rectOf(29, 1, 74, 21)));
    beginDiagnostics();
    TerminalPanel_paint(idePanel, &ide);
    endDiagnostics(0, nullptr);

    // The centre's own nested panel resolves over the sub-canvas its parent handed
    // it — the whole nesting mechanism, with no ownership passing between them.
    TerminalCanvas ideCentreRegion = TerminalCanvas_sub(&ide, 29, 1, 74, 21);
    assert(TerminalCanvas_getWidth(&ideCentreRegion) == 74);
    assert(TerminalCanvas_getHeight(&ideCentreRegion) == 21);
    TerminalPanel *centrePanel = TerminalPanel();
    TerminalLayerId ideTabs = TerminalPanel_add(centrePanel, TerminalAnchor(TERMINAL_ANCHOR_BAND_TOP, 1));
    TerminalLayerId ideSep = TerminalPanel_add(centrePanel, TerminalAnchor(TERMINAL_ANCHOR_BAND_TOP, 1));
    TerminalLayerId ideEditor = TerminalPanel_add(centrePanel, TerminalAnchor());
    assert(rectEqual(TerminalPanel_regionOf(centrePanel, &ideCentreRegion, ideTabs), rectOf(0, 0, 74, 1)));
    assert(rectEqual(TerminalPanel_regionOf(centrePanel, &ideCentreRegion, ideSep), rectOf(0, 1, 74, 1)));
    assert(rectEqual(TerminalPanel_regionOf(centrePanel, &ideCentreRegion, ideEditor), rectOf(0, 2, 74, 19)));
    beginDiagnostics();
    TerminalPanel_paint(centrePanel, &ideCentreRegion);
    endDiagnostics(0, nullptr);
    assert(TerminalPanel_paintCount(centrePanel) == 1);
    TerminalPanel_free(centrePanel);
    TerminalPanel_free(idePanel);

    // --- Projections: value string and a struct dump mirroring STRUCT FIELDS ---
    TERMINAL_PROJECT(TerminalPanel, panel);
    TerminalProjection value = TerminalPanel_toString(panel);
    assert(strstr(value.text, "TerminalPanel{layers=") != nullptr);
    TerminalProjection structured = TerminalPanel_toStringStruct(panel);
    assert(strstr(structured.text, "capacity=") != nullptr);
    assert(strstr(structured.text, "usedCount=") != nullptr);
    assert(strstr(structured.text, "paintCount=") != nullptr);
    assert(strstr(structured.text, "refusalCount=") != nullptr);
    assert(strstr(structured.text, "painting=") != nullptr);

    TerminalPanel_free(panel);
    return 0;
}
