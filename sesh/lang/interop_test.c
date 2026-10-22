/* Public header composition only: R5 may include both Graphvex and Sesh. */
#include "graphics/render_loop.h"
#include "lang/sesh.h"
#include <assert.h>
int main(void) {
    Client frameClient = {0};
    SeshClient sessionClient = SeshClient(1);
    assert(frameClient.window == nullptr && SeshClient_getId(&sessionClient) == 1);
    return 0;
}
