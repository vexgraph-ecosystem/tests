# vexgraph unified test suite — renovation wiring.
#
# Only the graphvex language tests are wired while the consumers migrate. Each
# test target is defined by its owning repo (graphvex/CMakeLists.txt); this file
# aggregates the run target.
cmake_minimum_required(VERSION 3.28)

# Target run_all_tests: the renovation matrix — the graphvex language suite.
# Placement (component) → drawing (graphics) → painting (raster row) →
# collage (compositor) → device dialects (device) → filters.
add_custom_target(run_all_tests
    COMMAND $<TARGET_FILE:device_filter_test>
    COMMAND $<TARGET_FILE:device_matrix_test>
    COMMAND $<TARGET_FILE:compositor_test>
    COMMAND $<TARGET_FILE:component_test>
    COMMAND $<TARGET_FILE:graphics_test>
    COMMAND $<TARGET_FILE:element_painter_test>
    COMMAND $<TARGET_FILE:image_fit_test>
    COMMAND $<TARGET_FILE:tostring_test>
    COMMAND $<TARGET_FILE:label_test>
    COMMENT "Executing the graphvex language test suite..."
)
