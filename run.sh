#!/usr/bin/env bash
# tests/run.sh — the one entry point (a script, not a C binary).
#
#   tests/run.sh                 list the friendly names
#   tests/run.sh <name> [args]   run <name> through `b run` (partial names ok)
#
# Presence of a source file is not evidence it was built or ran; this only
# dispatches to the umbrella build system, which compiles on demand.
set -euo pipefail

here="$(CDPATH= cd -- "$(dirname -- "${BASH_SOURCE[0]}")" && pwd)"
root="$(CDPATH= cd -- "$here/.." && pwd)"
b="$root/tools/b"

usage() {
    cat <<'EOF'
tests/run.sh — run anything by name

  tests/run.sh <name> [args...]

  friendly names:
    darling-gallery  -> _main/darling_gallery.c
    hello-window     -> _main/hello_window.c
    gpu              -> tests/graphvex/vulkan/gpu_render_test.c
    vk               -> tests/graphvex/vulkan/vk_renderer_test.c
    capture          -> tests/graphvex/graphics/capture_test.c
    element          -> tests/graphvex/element_test.c

  any other name is handed straight to `b run` (partial names ok), e.g.
    tests/run.sh image      # -> tests/graphvex/image_test
    tests/run.sh frame      # -> tests/darling/frame/frame_test
EOF
}

[ $# -ge 1 ] || { usage; exit 0; }

name="$1"
case "$name" in
    -h|--help|list)  usage; exit 0 ;;
    darling-gallery) name="_main/darling_gallery.c" ;;
    hello-window)    name="_main/hello_window.c" ;;
    gpu)             name="tests/graphvex/vulkan/gpu_render_test.c" ;;
    vk)              name="tests/graphvex/vulkan/vk_renderer_test.c" ;;
    capture)         name="tests/graphvex/graphics/capture_test.c" ;;
    element|panels)  name="tests/graphvex/element_test.c" ;;
esac
shift

exec "$b" run "$name" "$@"
