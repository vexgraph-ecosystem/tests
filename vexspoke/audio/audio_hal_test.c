// tests/vexspoke/audio/audio_hal_test.c — owner test for AudioHalConfig.
//
// Proves config constructor arities and setter validation without opening a
// platform audio device or claiming callback delivery.

#include <stdio.h>

#include "audio/audio_hal.h"

static int g_failures = 0;

#define CHECK(cond)                                                        \
    do {                                                                   \
        if (!(cond)) {                                                     \
            printf("FAIL %s:%d %s\n", __FILE__, __LINE__, #cond);          \
            g_failures++;                                                  \
        }                                                                  \
    } while (0)

static void renderSilence(float *output, uint32_t frames, uint32_t channels,
                          void *context) {
    (void) output;
    (void) frames;
    (void) channels;
    (void) context;
}

int main(void) {
    AudioHalConfig defaults = AudioHalConfig();
    CHECK(defaults.sample_rate == 48000.0);
    CHECK(defaults.channels == 2u);
    CHECK(defaults.buffer_frames == 256u);
    CHECK(defaults.callback == nullptr);
    CHECK(defaults.user_data == nullptr);

    int renderState = 7;
    AudioHalConfig configured = AudioHalConfig(renderSilence, &renderState);
    CHECK(configured.sample_rate == defaults.sample_rate);
    CHECK(configured.channels == defaults.channels);
    CHECK(configured.buffer_frames == defaults.buffer_frames);
    CHECK(configured.callback == renderSilence);
    CHECK(configured.user_data == &renderState);

    CHECK(AudioHalConfig_set_sample_rate(&configured, 44100.0));
    CHECK(AudioHalConfig_set_channels(&configured, 1u));
    CHECK(AudioHalConfig_set_buffer_frames(&configured, 128u));
    CHECK(configured.sample_rate == 44100.0);
    CHECK(configured.channels == 1u);
    CHECK(configured.buffer_frames == 128u);
    CHECK(!AudioHalConfig_set_sample_rate(&configured, 0.0));
    CHECK(!AudioHalConfig_set_channels(&configured, 0u));
    CHECK(!AudioHalConfig_set_buffer_frames(&configured, 0u));

    if (g_failures == 0) {
        printf("audio_hal_test: all assertions held\n");
        return 0;
    }
    printf("audio_hal_test: %d FAILURES\n", g_failures);
    return 1;
}
