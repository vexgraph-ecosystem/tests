# Gallery resources

`other-sunflower.png` is the user-supplied 700 × 448 sunflower photograph,
copied unchanged from Downloads. Its external source/license is not established;
this records a local test fixture, not permission for redistribution.

`tools/b build filter_gallery` copies it into the application's
`Contents/Resources` before signing. The gallery resolves it through its bundle,
with no runtime dependency on Downloads or the source checkout. Owner tests use
an explicitly configured source-fixture path. ImageIO decodes and center-crops
the photo to the gallery viewport; Vulkan executes the three filter scopes.

The GPU-resident Picture path now removes color-run geometry memory overhead:
filter outputs remain textures and each Picture draws one quad. The photograph
is still decoded on CPU once as input; this is not an all-GPU decoding claim.
The existing filter shaders still resolve through the build-state directory.
Apple ImageIO decoding is currently macOS-only; other hosts remain unproved.
