# Darling UI battle tests

Run `./tools/b test ui_` from the workspace root. These are actual tree/paint/
capture tests, not gallery buttons that all instantiate the same placeholder.
Detailed API coverage, confirmed defects and next tests are recorded in
`_notes/darling/ui-battle-testing.md` in the umbrella workspace.

Seven passing targets exercise every public Element operation, 162 full-image
anchor/pivot combinations, oversized children clipped by rounded parents on
CPU/GPU, visual paint-state pixels, actual ScrollPanel captured bands, a real
Frame with 50,000 visible children, and a 96-level nested Panel tree.

Four additional regressions expose nested clip intersection, enclosing clip
restoration, clipped-child hit-testing and missing raster borders. Those failing
targets remain uncommitted pending implementation fixes; do not weaken their
oracles or count them as passing coverage.

For PNG evidence, set `UI_BATTLE_ARTIFACT_DIR` to an existing writable directory.
Pixel failures emit capture paths; clipped-hit tests save their image before
checking input targeting. Pixel tolerance is two channel levels; the analytic
rounded-boundary oracle omits only the antialias fringe.

Tests are compiled/discovered by the umbrella build with C23 and warnings as
errors. Added through shell fallback because the authoring session lacked direct
write/edit tools. No library source was modified by this test lane.
