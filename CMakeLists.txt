# vexgraph unified test suite — renovation wiring.
#
# Only the graphvex language tests are wired while the consumers migrate. Each
# test target is defined by its owning repo (graphvex/CMakeLists.txt); this file
# aggregates the run target.
cmake_minimum_required(VERSION 3.28)

# Small, explicit diagnostic proof set. This is not a claim that every test
# source in this independent repository is built or executed.
if(TARGET throw_test AND TARGET throwable_test)
    add_test(NAME recoverable_throw_test COMMAND $<TARGET_FILE:throw_test>)
    add_test(NAME fatal_throw_test COMMAND $<TARGET_FILE:throwable_test>)
    set_tests_properties(recoverable_throw_test fatal_throw_test PROPERTIES TIMEOUT 15)
    add_custom_target(diagnostic_smoke_tests
        COMMAND $<TARGET_FILE:throw_test>
        COMMAND $<TARGET_FILE:throwable_test>
        DEPENDS throw_test throwable_test
        COMMENT "Checking recoverable and fatal diagnostics separately"
    )
endif()
foreach(CONTAINER_TEST array collection)
    if(TARGET ${CONTAINER_TEST}_test)
        add_test(NAME ${CONTAINER_TEST}_test COMMAND $<TARGET_FILE:${CONTAINER_TEST}_test>)
        set_tests_properties(${CONTAINER_TEST}_test PROPERTIES TIMEOUT 15)
    endif()
endforeach()

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
