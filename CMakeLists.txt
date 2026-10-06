cmake_minimum_required(VERSION 3.24)

# Open tests/ independently in CLion; reuse the owning workspace's b adapter.
# This does not define another source/include/link graph or run any tests.
project(vexgraph_test_workspace LANGUAGES NONE)
enable_testing()
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
get_filename_component(default_workspace "${CMAKE_CURRENT_SOURCE_DIR}/.." ABSOLUTE)
set(VEXGRAPH_WORKSPACE_ROOT "${default_workspace}" CACHE PATH "Workspace containing tools/b and the root IDE adapter")
if(NOT EXISTS "${VEXGRAPH_WORKSPACE_ROOT}/tools/b" OR
   NOT EXISTS "${VEXGRAPH_WORKSPACE_ROOT}/CMakeLists.txt")
    message(FATAL_ERROR "Set VEXGRAPH_WORKSPACE_ROOT to the Vexgraph checkout; tests borrow its build graph.")
endif()
add_subdirectory("${VEXGRAPH_WORKSPACE_ROOT}" "${CMAKE_BINARY_DIR}/workspace")
