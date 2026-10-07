#!/usr/bin/env python3
"""Documentation contract checks only; no GPU, visual or runtime claims."""
from pathlib import Path
import json
import re
import subprocess
import unittest

ROOT = Path(__file__).resolve().parents[2]
GRAPHVEX = ROOT / "ecosystem/repos/graphvex"


class CompositorContractTest(unittest.TestCase):
    def test_picture_promotion_and_gallery_contract(self):
        darling = ROOT / "ecosystem/interface/darling-framework"
        manifest = json.loads((darling / "scaffolds.json").read_text())
        self.assertEqual(len(manifest["entries"]), 125)
        self.assertNotIn("Picture", [row["name"] for row in manifest["entries"]])
        self.assertIn("Picture", [row["name"] for row in manifest["existing"]])
        self.assertIn("125 draft pairs", (darling / "STATUS.md").read_text())
        self.assertIn("90 class drafts", (darling / "SCAFFOLDS.md").read_text())
        readiness = (ROOT / "repos/.ecosystem/darling.md").read_text()
        self.assertIn("125 draft pairs", readiness)
        self.assertIn("native `filter_gallery --smoke` pass", readiness)
        law = (ROOT / "tests/test-preferences.md").read_text()
        self.assertIn("hiding a window is not closing it", law)
        self.assertIn("Application with an attached Darling", law)
        gallery = (ROOT / "tests/darling/compositor/filter_gallery.c").read_text()
        self.assertIn("DARLING_TEST_HAS_FRAMES", gallery)
        self.assertIn("Darling_testKeepOpen", gallery)
        self.assertIn("Frame_onClose(frame, galleryClosed", gallery)
        self.assertIn("PART_TOP_LEFT, PART_TOP_CENTER, PART_TOP_RIGHT", gallery)
        self.assertIn("galleryPublished", gallery)
        self.assertNotIn("Frame_capture(frame)", gallery)
        documentation = (GRAPHVEX / "COMPOSITOR.md").read_text()
        self.assertIn("not automatic", documentation)
        self.assertIn("GPU scope", documentation)
        self.assertIn("backdrop left, foreground centered, element right", documentation)
        self.assertIn("bypasses the content/focus FPS cap", documentation)

    def test_universal_ownership_is_reconciled(self):
        for path in (ROOT / "preferences.md", ROOT / "ecosystem/vexspoke/preferences.md"):
            text = path.read_text()
            with self.subTest(path=str(path)):
                self.assertIn("graphical element trees and the widget/image compositor", text)
                self.assertIn("Darling owns no competing graphics compositor", text)
                self.assertNotIn("no window, no UI tree, no services", text)
                self.assertNotIn("widgets/compositor/", text)
                self.assertIn("R3 `graphvex`: includes `vexspoke` only", text)

    def test_graphvex_bounds_scopes_and_backend_contract(self):
        text = (GRAPHVEX / "graphvex-preferences.md").read_text()
        for title in ("Absolute Rendering & Event Bound Law",
                      "Ordered Filter & Scatter Composition Law",
                      "Independent Scene Cadence Law"):
            self.assertIn(f"### {title}", text)
            self.assertIn(f"**{title}**", text)
        for clause in ("Filters never alter it", "before external ancestor composition clips",
                       "Allocation edges are not semantic clips", "ID16 | payload48",
                       "whole", "left-to-right", "compute stores",
                       "Scene", "linear-light premultiplied RGBA"):
            self.assertIn(clause, text)
        self.assertNotIn("UI compositor** (widget-tree paint) stays", text)
        self.assertNotIn("widget composition, and application behavior remain in Darling", text)

    def test_interface_ownership_and_relative_link(self):
        path = ROOT / "ecosystem/interface/darling-framework/README.md"
        text = path.read_text()
        self.assertIn("Graphvex owns graphical element", text)
        self.assertIn("automatic widget-stack", text)
        self.assertTrue((path.parent / "../../drivers/graphvex/graphvex-preferences.md").is_file())

    def test_documented_reference_client_compiles(self):
        documentation = (GRAPHVEX / "COMPOSITOR.md").read_text()
        example = re.search(r"```c\n(.*?)\n```", documentation, re.S)
        self.assertIsNotNone(example)
        command = ["cc", "-std=gnu23", "-Wall", "-Wextra", "-Werror",
                   "-fsyntax-only", "-x", "c", "-", "-I", str(GRAPHVEX / "src")]
        result = subprocess.run(command, input=example.group(1), text=True,
                                capture_output=True, timeout=30)
        self.assertEqual(result.returncode, 0, result.stderr)

    def test_graphvex_readme_and_compositor_links(self):
        readme = (GRAPHVEX / "README.md").read_text()
        self.assertIn("Graphvex owns widget/element composition", readme)
        self.assertIn("compositor/filter_pool", readme)
        for path in (GRAPHVEX / "README.md", GRAPHVEX / "COMPOSITOR.md"):
            for target in re.findall(r"\]\(([^)]+)\)", path.read_text()):
                with self.subTest(path=str(path), target=target):
                    self.assertTrue((path.parent / target).is_file())

    def test_filter_vocabulary_documents_encoding_not_effect_support(self):
        documentation = (GRAPHVEX / "FILTERS.md").read_text()
        for clause in ("Color effects execute in Vulkan shaders",
                       "No typed parameter pool exists yet", "vertex → fragment",
                       "Graphics blend state does not protect compute writes",
                       "Automatic Element attachment APIs", "HSL and HSV remain distinct"):
            self.assertIn(clause, documentation)
        for name in ("filter_type.h", "filter_functions.h"):
            self.assertTrue((GRAPHVEX / "src/filter" / name).is_file())
        self.assertIn("FILTERS.md", (GRAPHVEX / "COMPOSITOR.md").read_text())
        readiness = (ROOT / "repos/.ecosystem/graphvex.md").read_text()
        self.assertIn("### Filter vocabulary (`src/filter/`)", readiness)
        self.assertIn("typed parameter allocation/COW/migration", readiness)
        self.assertIn("color_pass_test.c", readiness)
        for clause in ("linear Rec.709", "preserve alpha and extents",
                       "nonfinite/out-of-range", "Never sample the current destination"):
            self.assertIn(clause, documentation)
        registry = (GRAPHVEX / "src/filter/filter_type.h").read_text()
        ids = re.findall(r"^#define ([A-Z_]+_ID) (0x[0-9a-f]+u)$", registry, re.M)
        self.assertEqual(len(ids), 53)
        self.assertEqual(len({int(value.rstrip("u"), 0) for _, value in ids}), 53)
        self.assertIn("#define GAUSSIAN_BLUR_ID 0x0014u", registry)
        self.assertIn("#define FILTER_GAIN GAIN_ID", registry)
        law = (GRAPHVEX / "graphvex-preferences.md").read_text()
        self.assertIn("`FILTERNAME_ID`", law)
        self.assertIn("`filter/filter_type.h`", law)
        owner = (ROOT / "tests/graphvex/filter/filter_type_test.c").read_text()
        for name, _ in ids:
            self.assertIn(name, owner)
        functions = (GRAPHVEX / "src/filter/filter_functions.h").read_text()
        owner = (ROOT / "tests/graphvex/filter/filter_functions_test.c").read_text()
        references = re.findall(r"GRAPHVEX_FILTER_REFERENCE\((\w+), [A-Z_]+_ID\)", functions)
        self.assertEqual(len(references), 40)
        for name in references:
            self.assertIn(f"CHECK_REFERENCE({name},", owner)
        shim = (GRAPHVEX / "src/lang/filter.h").read_text()
        self.assertIn('#include "filter/filter_functions.h"', shim)
        self.assertNotIn("#define FILTER_", shim)

    def test_filter_constructor_arity_is_rejected_for_intended_reason(self):
        prefix = '#include "filter/filter_functions.h"\n'
        for call in ("Filter_dithering()", "Filter_hsl(1)", "Filter_hsv(1, 2, 3)",
                     "Filter_grayscale(1)", "Filter_brightness()"):
            with self.subTest(call=call):
                result = subprocess.run(
                    ["cc", "-std=gnu23", "-Wall", "-Wextra", "-Werror",
                     "-fsyntax-only", "-x", "c", "-", "-I", str(GRAPHVEX / "src")],
                    input=prefix + f"FilterToken probe(void) {{ return {call}; }}\n",
                    text=True, capture_output=True, timeout=30)
                self.assertNotEqual(result.returncode, 0)
                self.assertRegex(result.stderr, r"too (few|many) arguments")

    def test_gpu_color_pass_public_arity_and_no_cpu_extension(self):
        command = ["cc", "-std=gnu23", "-Wall", "-Wextra", "-Werror",
                   "-fsyntax-only", "-x", "c", "-", "-I", str(GRAPHVEX / "src")]
        prefix = '#include "compositor/color_pass.h"\n'
        positive = prefix + 'ColorPass *empty(void) { return ColorPass(); }\n'
        positive += 'ColorPass *six(Device *d, void *p, const uint32_t *v, size_t n) { return ColorPass(d,p,v,n,v,n); }\n'
        result = subprocess.run(command, input=positive, text=True, capture_output=True, timeout=30)
        self.assertEqual(result.returncode, 0, result.stderr)
        for call in ("ColorPass(1)", "ColorPass(1,2,3,4,5)"):
            result = subprocess.run(command, input=prefix + f'ColorPass *bad(void) {{ return {call}; }}\n',
                                    text=True, capture_output=True, timeout=30)
            self.assertNotEqual(result.returncode, 0)
            self.assertIn("ColorPass_invalidArity", result.stderr)
        cpu = (GRAPHVEX / "src/compositor/compositor.c").read_text()
        self.assertNotIn("case BRIGHTNESS_ID", cpu)
        self.assertNotIn("static void pointwise", cpu)
        gpu = (GRAPHVEX / "src/compositor/color_pass.c").read_text()
        self.assertIn("vkCreateGraphicsPipelines", gpu)
        self.assertIn("vkCmdDraw", gpu)
        self.assertNotIn("vkCmdCopyImageToBuffer", gpu)
        shader = (GRAPHVEX / "src/shaders/compositor/color.frag").read_text()
        self.assertIn('#include "filter/filter_type.h"', shader)
        self.assertIn("texelFetch(sourceColor", shader)

    def test_gallery_uses_gpu_scope_not_cpu_fixture(self):
        gallery = (ROOT / "tests/darling/compositor/filter_gallery.c").read_text()
        fixture = (ROOT / "tests/darling/compositor/filter_gallery_fixture.h").read_text()
        self.assertNotIn("FilterGallery_make", gallery + fixture)
        self.assertNotIn("Compositor_scopedScene", fixture)
        self.assertNotIn("CompositorSurface_", fixture)
        self.assertIn("FilterGallery_render(gpu", gallery)
        self.assertIn("GpuScope_render", fixture)
        gpu = (GRAPHVEX / "src/compositor/gpu_scope.c").read_text()
        for clause in ("VK_BLEND_FACTOR_ONE", "vkCmdDraw", "GPU_WAIT_NS", "(*self).pending=true"):
            self.assertIn(clause, gpu)
        self.assertNotIn("vkDeviceWaitIdle", gpu)
        self.assertNotIn("vkQueueWaitIdle", gpu)
        doc = (GRAPHVEX / "COMPOSITOR.md").read_text()
        self.assertIn("GPU-resident filter-to-Picture bridge", doc)
        self.assertIn("GpuScope_renderSampled", gallery + fixture)
        renderer = (GRAPHVEX / "src/vulkan/vk_renderer.c").read_text()
        self.assertNotIn("ImageRuns_visit", renderer)
        self.assertIn("SampledImage_retain", renderer)
        self.assertIn("VulkanBackend_device()", gallery)
        self.assertIn("Previous `--smoke` evidence", doc)
        law = (GRAPHVEX / "graphvex-preferences.md").read_text()
        self.assertIn("GPU execution truth", law)
        self.assertIn("never CPU filtering/composition", law)
        command = ["cc", "-std=gnu23", "-Wall", "-Wextra", "-Werror",
                   "-fsyntax-only", "-x", "c", "-", "-I", str(GRAPHVEX / "src")]
        prefix = '#include "compositor/gpu_scope.h"\n'
        result = subprocess.run(command, input=prefix + 'GpuScope *zero(void) { return GpuScope(); }\n',
                                text=True, capture_output=True, timeout=30)
        self.assertEqual(result.returncode, 0, result.stderr)
        result = subprocess.run(command, input=prefix + 'GpuScope *wrong(void) { return GpuScope(1); }\n',
                                text=True, capture_output=True, timeout=30)
        self.assertNotEqual(result.returncode, 0)
        self.assertIn("GpuScope_invalidArity", result.stderr)

    def test_sampled_image_constructor_dispatch(self):
        command = ["cc", "-std=gnu23", "-Wall", "-Wextra", "-Werror",
                   "-fsyntax-only", "-x", "c", "-", "-I", str(GRAPHVEX / "src")]
        prefix = '#include "vulkan/sampled_image.h"\n'
        positive = prefix + ('SampledImage *zero(void) { return SampledImage(); }\n'
                             'SampledImage *upload(Device *d,Image *i) { return SampledImage(d,i); }\n'
                             'SampledImage *adopt(Device *d,void *i,void *m) { return SampledImage(d,i,m,1,1); }\n')
        result = subprocess.run(command, input=positive, text=True, capture_output=True, timeout=30)
        self.assertEqual(result.returncode, 0, result.stderr)
        for call in ("SampledImage(1)", "SampledImage(1,2,3)", "SampledImage(1,2,3,4)", "SampledImage(1,2,3,4,5,6)"):
            result = subprocess.run(command, input=prefix + f'SampledImage *bad(void) {{ return {call}; }}\n',
                                    text=True, capture_output=True, timeout=30)
            self.assertNotEqual(result.returncode, 0)
            self.assertIn("SampledImage_invalidArity", result.stderr)


if __name__ == "__main__":
    unittest.main()
