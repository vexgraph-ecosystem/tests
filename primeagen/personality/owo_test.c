/* Presentation mode owner: distinct immutable prose, explicit offline status,
 * no permission, transport, mutable state or input rewriting surface. */
#include "personality/owo.h"
#include <assert.h>
#include <string.h>
int main(void) {
    assert(strstr(Owo_greeting(true), "owo") != nullptr);
    assert(strstr(Owo_greeting(false), "offline") != nullptr);
    assert(strcmp(Owo_greeting(true), Owo_greeting(false)) != 0);
    assert(strstr(Owo_status(true), "offline") != nullptr);
    assert(strstr(Owo_status(false), "neutral") != nullptr);
    assert(Owo_greeting(true) == Owo_greeting(true));
    assert(strcmp(Owo_instructions(false), "") == 0);
    assert(strstr(Owo_instructions(true), "Complete the user's actual task competently") != nullptr);
    assert(strstr(Owo_instructions(true), "machine-readable tool arguments") != nullptr);
    assert(strstr(Owo_instructions(true), "separate tool consent") != nullptr);
}
