// Owner: TerminalAppearance — named look presets. Registry, lookup, apply, draw.
#include "terminal/terminal_appearance.h"
#include "determinant/support.h"

int main(void) {
    assert(TerminalAppearance_presetCount() >= 6);
    assert(strcmp(TerminalAppearance_presetName(0), "ascii-solid") == 0);
    assert(strcmp(TerminalAppearance_presetName(1), "ascii-striped") == 0);
    assert(TerminalAppearance_presetName(999) == nullptr);

    // Default is the striped connector.
    TerminalAppearance def = TerminalAppearance_0();
    TerminalRule rule = TerminalRule_0();
    TerminalAppearance_getRule(&def, &rule);
    assert(rule.horizontal == '-' && rule.vertical == '|' && rule.junction == '+');
    assert(rule.stripe == '.' && rule.stripeEvery == 4);

    // Named lookup; unknown falls back to the default.
    TerminalAppearance solid = TerminalAppearance_named("ascii-solid");
    TerminalAppearance_getRule(&solid, &rule);
    assert(rule.stripe == 0 && rule.stripeEvery == 0);
    TerminalAppearance unknown = TerminalAppearance_named("no-such-look");
    TerminalAppearance_getRule(&unknown, &rule);
    assert(rule.stripe == '.');                       // default

    // A Unicode preset really uses box-drawing codepoints (cells are codepoints).
    TerminalAppearance em = TerminalAppearance_named("unicode-emdash");
    TerminalAppearance_getRule(&em, &rule);
    assert(rule.horizontal == 0x2014u /* — */ && rule.vertical == 0x2502u /* │ */);
    TerminalAppearance light = TerminalAppearance_preset(3);
    TerminalAppearance_getRule(&light, &rule);
    assert(rule.horizontal == 0x2500u && rule.junction == 0x252Cu);

    // Palette.
    uint32_t accent = 0, muted = 0, text = 0, error = 0;
    TerminalAppearance_getColors(&def, &accent, &muted, &text, &error);
    assert(accent != TERMINAL_COLOR_DEFAULT && muted != TERMINAL_COLOR_DEFAULT && text != 0 && error != 0);
    assert(TerminalAppearance_setColors(&def, TERMINAL_RGB(1, 2, 3), TERMINAL_RGB(4, 5, 6),
                                        TERMINAL_RGB(7, 8, 9), TERMINAL_RGB(10, 11, 12)));
    TerminalAppearance_getColors(&def, &accent, &muted, &text, &error);
    assert(accent == TERMINAL_RGB(1, 2, 3) && error == TERMINAL_RGB(10, 11, 12));

    // Custom rule via setter.
    assert(TerminalAppearance_setRule(&def, TerminalRule('=', '!')));
    TerminalAppearance_getRule(&def, &rule);
    assert(rule.horizontal == '=' && rule.vertical == '!');

    // Apply into a frame, and draw a run into a canvas region.
    enum { W = 16, H = 4 };
    static uint32_t cells[W * H];
    TerminalCanvas canvas = TerminalCanvas(cells, nullptr, W, H);
    TerminalFrame frame = TerminalFrame(5, 5);
    TerminalAppearance apply = TerminalAppearance_named("ascii-solid");
    TerminalAppearance_applyTo(&apply, &frame);
    TerminalRule inFrame = TerminalRule_0();
    TerminalFrame_getRule(&frame, &inFrame);
    assert(inFrame.horizontal == '-' && inFrame.stripe == 0);
    TerminalAppearance_drawRule(&apply, &canvas, 0, TerminalPen());
    assert(TerminalCanvas_glyphAt(&canvas, 0, 0) == '-');
    TerminalAppearance striped = TerminalAppearance_0();
    TerminalAppearance_drawRule(&striped, &canvas, 1, TerminalPen());
    assert(TerminalCanvas_glyphAt(&canvas, 3, 1) == '.' && TerminalCanvas_glyphAt(&canvas, 4, 1) == '-');

    // Null safety.
    TerminalAppearance_applyTo(nullptr, &frame);
    TerminalAppearance_applyTo(&apply, nullptr);
    TerminalAppearance_drawRule(nullptr, &canvas, 0, TerminalPen());
    TerminalAppearance_drawRule(&apply, nullptr, 0, TerminalPen());
    TerminalAppearance_getRule(nullptr, &rule);
    TerminalAppearance_getColors(nullptr, nullptr, nullptr, nullptr, nullptr);
    REJECT(TerminalAppearance_setRule(nullptr, TerminalRule_0()), "null self");
    REJECT(TerminalAppearance_setColors(nullptr, 0, 0, 0, 0), "null self");
    assert(TerminalAppearance_named(nullptr).rule.horizontal == '-');

    PROJECT(TerminalAppearance, &def);
    return 0;
}
