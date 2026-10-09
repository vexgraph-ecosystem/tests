#include <assert.h>
#include <math.h>
#include <stdio.h>
#include <string.h>

#include "input/piano_key.h"
#include "input/turntable.h"
#include "input/gesture.h"
#include "input/gamepad.h"
#include "input/hardware_event.h"

// Checks note conversion and piano note-on/note-off state transitions.
static void test_piano_keys(void) {
    // A4 (note 69) is 440.0 Hz
    float a4_freq = PianoKey_frequency(PIANO_A4_NOTE);
    assert(fabsf(a4_freq - 440.0f) < 0.001f);

    // Middle C (note 60) is ~261.63 Hz
    float c4_freq = PianoKey_frequency(60);
    assert(fabsf(c4_freq - 261.6256f) < 0.01f);

    // A5 (note 81) is one octave higher = 880.0 Hz
    float a5_freq = PianoKey_frequency(81);
    assert(fabsf(a5_freq - 880.0f) < 0.001f);

    assert(PianoKey_octave(60) == 4);
    assert(PianoKey_pitchClass(60) == PITCH_C);
    assert(PianoKey_pitchClass(61) == PITCH_CSHARP);

    char name[16];
    PianoKey_noteName(60, name, sizeof(name));
    assert(strcmp(name, "C4") == 0);

    PianoState state;
    PianoState_init(&state);
    assert(!PianoState_isNoteOn(&state, 60));

    PianoKeyEvent evt;
    memset(&evt, 0, sizeof(evt));
    evt.note = 60;
    evt.isDown = true;
    evt.velocity = 0.8f;
    evt.timestampUs = 1000;

    PianoState_processEvent(&state, evt);
    assert(PianoState_isNoteOn(&state, 60));
    assert(fabsf(PianoState_noteVelocity(&state, 60) - 0.8f) < 0.001f);

    evt.isDown = false;
    PianoState_processEvent(&state, evt);
    assert(!PianoState_isNoteOn(&state, 60));
}

// Checks turntable event processing and crossfader curve endpoints.
static void test_turntable(void) {
    TurntableState tt;
    TurntableState_init(&tt, 0);
    assert(tt.deckIndex == 0);
    assert(!tt.platterTouched);

    TurntableEvent evt;
    memset(&evt, 0, sizeof(evt));
    evt.deckIndex = 0;
    evt.platterTouched = true;
    evt.angularDeltaRad = 0.5f;

    TurntableState_processEvent(&tt, evt);
    assert(tt.platterTouched == true);

    TurntableState_tick(&tt, 0.016f);

    // Crossfader calculation: linear, sharp cut, smooth dip
    float left = 0.0f, right = 0.0f;
    Turntable_calculateCrossfade(0.0f, CROSSFADER_LINEAR, &left, &right);
    assert(fabsf(left - 0.5f) < 0.001f);
    assert(fabsf(right - 0.5f) < 0.001f);

    Turntable_calculateCrossfade(-1.0f, CROSSFADER_LINEAR, &left, &right);
    assert(fabsf(left - 1.0f) < 0.001f);
    assert(fabsf(right - 0.0f) < 0.001f);
}

// Checks gesture magnification and rotation accumulation.
static void test_gesture(void) {
    GestureState state;
    GestureState_init(&state);
    assert(fabsf(state.currentScale - 1.0f) < 0.001f);

    GestureEvent evt;
    memset(&evt, 0, sizeof(evt));
    evt.phase = GESTURE_PHASE_CHANGED;
    evt.magnificationDelta = 0.25f;
    evt.rotationDeltaRad = 0.1f;

    GestureState_processEvent(&state, evt);
    assert(fabsf(state.currentScale - 1.25f) < 0.001f);
    assert(fabsf(state.currentRotationRad - 0.1f) < 0.001f);

    GestureState_tick(&state, 0.016f);
}

// Checks button state and circular deadzone filtering.
static void test_gamepad(void) {
    GamepadState pad;
    GamepadState_init(&pad);
    assert(pad.buttons == 0);

    pad.buttons |= GAMEPAD_BUTTON_A;
    assert(GamepadState_isDown(&pad, GAMEPAD_BUTTON_A));
    assert(!GamepadState_isDown(&pad, GAMEPAD_BUTTON_B));

    // Circular deadzone test
    Vec2 raw;
    raw.x = 0.05f;
    raw.y = 0.05f;
    Vec2 filtered;
    Gamepad_applyCircularDeadzone(raw, GAMEPAD_DEFAULT_DEADZONE, &filtered);
    assert(filtered.x == 0.0f);
    assert(filtered.y == 0.0f);

    raw.x = 0.8f;
    raw.y = 0.0f;
    Gamepad_applyCircularDeadzone(raw, GAMEPAD_DEFAULT_DEADZONE, &filtered);
    assert(filtered.x > 0.0f);
}

// Checks tagged hardware-event constructors preserve device and payload data.
static void test_hardware_event(void) {
    PianoKeyEvent pke;
    memset(&pke, 0, sizeof(pke));
    pke.note = 72;
    pke.isDown = true;
    pke.velocity = 0.9f;

    HardwareEvent he1 = HardwareEvent_makePiano(1, pke);
    assert(he1.type == HW_EVENT_PIANO);
    assert(he1.deviceId == 1);
    assert(he1.data.piano.note == 72);

    GamepadState pad;
    GamepadState_init(&pad);
    HardwareEvent he2 = HardwareEvent_makeGamepad(2, pad);
    assert(he2.type == HW_EVENT_GAMEPAD);
    assert(he2.deviceId == 2);
}

// Runs headless owner checks for piano, turntable, gesture, gamepad, and events.
int main(void) {
    printf("[Hardware Input Test] Starting test suite...\n");
    test_piano_keys();
    test_turntable();
    test_gesture();
    test_gamepad();
    test_hardware_event();
    printf("[Hardware Input Test] ALL TESTS PASSED\n");
    return 0;
}
